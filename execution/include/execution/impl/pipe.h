#ifndef EXECUTION_IMPL_PIPE_H_
#define EXECUTION_IMPL_PIPE_H_

#include <fcntl.h>
#include <string>

namespace execution {
namespace pipe {
// A pipe handle stores the information needed to specify an open pipe.
// Closes the pipe on destruction.
class Handle {
 public:
  Handle() : descriptor_(-1) {}

  // Disallow Copy and Assign.
  Handle(const Handle &) = delete;
  Handle &operator=(const Handle &) = delete;
  // Moves are okay.
  Handle(Handle &&) = default;
  Handle &operator=(Handle &&) = default;

  ~Handle() { Close(*this); }

  int descriptor() const { return descriptor_; }

 private:
  int descriptor_;

  // Friend Methods
  friend Handle Open(const char *pipeName, int mode);
  friend int Close(Handle &handle);
};

//Creates the inital pipe with the provided name and mode
//Mode is in the same format that chmod uses (e.g. 0666).
//Returns 0 on success, otherwise -1
int Create(const char *pipeName, int mode);

//Deletes the pipe
//Upon successful completion, 0 is returned, otherwise -1 is returned.
int Delete(const char *pipeName);

//Opens pipe for reading or writing, takes following modes
//O_WRONLY : write only
//O_RDONLY : read only
//O_RDWR : read/write
//Returns a handle used for reading/writing. On a failure returns -1
Handle Open(const char *pipeName, int mode);

// Closes the pipe and returns the descriptor to the system pool.
int Close(Handle &handle);

//This will block until another process opens the pipe for reading
//Does NOT write the terminating null byte, but requires it exists.
//Returns the number of bytes actually written.
int Write(const Handle &handle, const char *message);
int Write(const Handle &handle, const char *message, size_t length);
int Write(const Handle &handle, const std::string &message);

//This will block until another process writes to the pipe
//Returns the number of bytes actually read.
int Read(const Handle &handle, char *buffer, int bufferLength);
}  // namespace pipe
}  // namespace execution

#endif  // EXECUTION_IMPL_PIPE_H_
