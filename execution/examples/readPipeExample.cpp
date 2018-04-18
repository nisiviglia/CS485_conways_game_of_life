#include "execution/impl/pipe.h"
#include <cstdio>
#include <fnctl.h>

using namespace execution;

int main() {

  const char myfifo[] = "/tmp/myfifo";

  int MAX_BUF = 1024;
  // Guarentee null byte
  char buf[MAX_BUF + 1] = {0};

  // Open pipe in read only mode
  auto handle = pipe::Open(myfifo, O_RDONLY);

  if(handle.descriptor() == -1) {
    printf("Pipe not available.\n");
    return 0;
  }

  // Read to char buf, up to 1024 bytes
  pipe::Read(handle, buf, MAX_BUF);

  // Print received message
  printf("Received: %s\n", buf);

  // NOTE: handle will auto-close itself!

  return 0;
}
