#include "execution/impl/pipe.h"
#include <iostream>
#include <string>

using namespace execution;

int main() {
  const char *myfifo = "/tmp/myfifo";

  // Create the pipe with permissions rw-rw-rw-
  pipe::Create(myfifo, 0666);

  std::cout << "Enter Message: ";
  std::string message;
  getline(std::cin, message);

  // Open in write only mode
  auto handle = pipe::Open(myfifo, O_WRONLY);

  // Write message to the pipe
  pipe::Write(handle, message.c_str());
  // Or more optimally
  pipe::Write(handle, message.c_str(), message.size());

  // Force handle to close before deleting pipe (good idea).
  // NOTE: Also could've enclosed assignment of handle to a sub-scope
  //       and let the end of that scope close the handle
  pipe::Close(handle);

  // Delete the pipe
  pipe::Delete(myfifo);

  return 0;
}
