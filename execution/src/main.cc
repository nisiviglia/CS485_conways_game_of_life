#include <atomic>
#include <cstring>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <functional>

#include "execution/impl/dispatch.h"
#include "execution/impl/pipe.h"

using namespace execution;

using std::atomic;
using std::lock_guard;
using std::mutex;
using std::ofstream;
using std::string;
using std::stringbuf;
using std::thread;

// Reads the pipe until an entire {} balanced JSON object has been read in.
// If additional bytes were read after the json message returned, they are saved
// statically and included in the next return value.
// Returns result is valid iff the first char returned is a {, otherwise an
// error occurred.
string ReadJsonObject(const pipe::Handle &handle) {
  // Static buffer and related book-keeping.
  const int kMaxBufLen = 1000;
  static char buf[kMaxBufLen + 1] = {0};
  static char *pos = buf;  // Where we left off in buf.
  static char *end = buf;  // Where the end of the valid content in buf is.

  // Enumerates necessary states for parsing JSON.
  // kDefault: The state the parse is usally in.
  // kString: The state of the parser after an opening ".
  // kEscape: The state of the parser after a \ while in state kString.
  enum JsonState { kNormal, kString, kEscape };

  // Ignore input until we get to a opening {.
  while (true) {
    if (pos >= end) {
      // We've exhausted all input in buf, so get more.
      // Reset pos to beginning.
      pos = buf;
      // Read into buf, and set end to buf + (bytes read).
      end = buf + pipe::Read(handle, buf, kMaxBufLen);
      if (end <= pos) {  // 0 bytes read or error.
        return "";
      }
    }

    // Break when we find our opening {.
    if (*pos++ == '{') {
      break;
    }
  }

  // Parser State.
  JsonState state = kNormal;
  int brace_count = 1;

  // StringBuffer for building the json string (including the opening brace).
  stringbuf output("{", std::ios_base::out | std::ios_base::ate);

  // Parse until we read an entire object.
  while (brace_count > 0) {
    // Buffer is empty, same as above.
    if (pos >= end) {
      pos = buf;
      end = buf + pipe::Read(handle, buf, kMaxBufLen);
      if (end <= pos) {  // 0 bytes read or error.
        return "";
      }
    }

    // Process next character.
    char c = *pos++;
    switch (state) {
      case kNormal:
        if (c == '{')      ++brace_count;
        else if (c == '}') --brace_count;
        else if (c == '"') state = kString;
        break;

      case kString:
        if (c == '"')       state = kNormal;
        else if (c == '\\') state = kEscape;
        break;

      case kEscape:
        // For now, we considering any character to be a valid escape character.
        // Invalid JSON strings should be handled later in the pipeline.
        state = kString;
        break;
    }
    // Store character into buffer.
    output.sputc(c);
  }

  return output.str();
}

// Quick thread managing class.
// To avoid storing memory for each and every thread that was ever active,
// and then calling join on all of them at the end, we will just implement
// a thread-safe counter for number of alive-threads managed by this instance
// and block on them all returning in the destructor.
// TODO: Implement some sort of timeout feature in the destructor in case of
//       an ignored reponse so the program doesn't hang on quit.
class ThreadManager {
 public:
  // Default constructor, initializes count to 0.
  ThreadManager() : active_count_(0) {}

  // Disallow Copy and Assign.
  ThreadManager(const ThreadManager &) = delete;
  ThreadManager &operator=(const ThreadManager &) = delete;
  // Disallow move operators (keep it simple).
  ThreadManager(ThreadManager &&) = delete;
  ThreadManager &operator=(ThreadManager &&) = delete;

  // Block on all threads finishing.
  ~ThreadManager() {
    // Simple spinlock
    while (active_count_) {
      std::this_thread::yield();
    }
  }

  // Spawns a managed thread.
  // NOTE: the function below must NOT be a reference, because the closure
  //       of a lambda does not extend the lifetime of references like local
  //       const &'s do.
  void Spawn(std::function<void()> fn) {
    ++active_count_;
    thread([fn, this] {
      // Need this to ensure active_count_ is decremented regardless of how
      // fn exits (save for an abort call ofc).
      struct Decr { 
        explicit Decr(atomic<int> *active_count) : active_count(active_count) {} 
        ~Decr() { --(*active_count); }

        atomic<int> *active_count;
      } decr(&active_count_);

      fn();
    }).detach();
  }

 private:
  atomic<int> active_count_;
};

int main(int argc, char *argv[]) {  
  ofstream err("err.log");
  if (!err) {
    return -5;
  }

  if(argc != 2) {
    err << "Usage: " << argv[0] << " path/to/input_pipe\n";
    return -1;
  }

  // For managing threads that write json responses.
  ThreadManager thread_manager;
  // Ensures error logs are not interleaved.
  mutex err_mutex;  // GUARDS err.
  // Whether or not to try opening the read pipe if closed.
  bool keep_open = true;
  while (keep_open) {
    auto read_handle = pipe::Open(argv[1], O_RDONLY);
    if (read_handle.descriptor() == -1) {
      err << "Read pipe '" << argv[1] << "' not available.\n";
      return -2;
    }

    // Run forever.
    while (true) {
      auto json_str = ReadJsonObject(read_handle);
      if (json_str.empty()) {
        // Error reading or file was closed, try re-opening.
        break;
      }
      if (json_str[0] != '{') {
        lock_guard<mutex> lock(err_mutex);
        err << "Failed to read a valid JSON object\n";
        return -3;
      }

      auto result = Dispatch(json_str);
      string return_pipe = result.first;
      string json_result = result.second;

      // Check if we're done.
      if (json_result == "quit") {
        keep_open = false;
        break;
      }

      if (return_pipe.empty()) {
        lock_guard<mutex> lock(err_mutex);
        err << "Failed to parse the return pipe from the JSON command object\n";
        return -4;
      }

      // Writing in a new thread to avoid deadlocks.
      thread_manager.Spawn([return_pipe, json_result, &err_mutex, &err] {
        auto write_handle = pipe::Open(return_pipe.c_str(), O_WRONLY);
        if (write_handle.descriptor() == -1) {
          lock_guard<mutex> lock(err_mutex);
          err << "Write pipe '" << return_pipe << "' not available.\n";
          return;
        }

        int bytes_written = pipe::Write(write_handle, json_result);

        if (bytes_written < json_result.size()) {
          lock_guard<mutex> lock(err_mutex);
          err << "Failed to write to pipe '" << return_pipe << "'\n";
          return;
        }
      });
    }
  }

  return 0;
}
