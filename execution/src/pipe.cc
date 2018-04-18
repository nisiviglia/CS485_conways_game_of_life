#include "../include/execution/impl/pipe.h"

#include <cstdio>
#include <cstring>
#include <sys/stat.h> // for mkfifo
#include <sys/types.h>
#include <unistd.h>

namespace execution {
namespace pipe {

int Create(const char * pipeName, int mode) {
	return mkfifo(pipeName, mode);
}

int Delete(char *pipeName) {
  return unlink(pipeName);
}

Handle Open(const char *pipeName, int mode) {
  Handle handle;
  handle.descriptor_ = open(pipeName, mode);
  return handle;
}

int Close(Handle &handle) {
  if (handle.descriptor_ < 0) {
    return 0;  // Was alreday closed.
  }

  int ret = close(handle.descriptor_);
  handle.descriptor_ = -1;

  return ret;
}

int Write(const Handle &handle, const char * message) {
  return write(handle.descriptor(), message, strlen(message));
}
int Write(const Handle &handle, const char *message, size_t length) {
	return write(handle.descriptor(), message, length);
}
int Write(const Handle &handle, const std::string &message) {
  return write(handle.descriptor(), message.c_str(), message.size());
}
  
int Read(const Handle &handle, char* buffer, int bufferLength) {
	return read(handle.descriptor(), buffer, bufferLength);
}

}  // namespace pipe
}  // namespace execution
