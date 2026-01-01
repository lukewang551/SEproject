#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void target_function(char* buf) {
  char local_buf[50];
  volatile int canary = 0xdeadbeef;
  strcpy(local_buf, buf);
  printf("Input processed: %s\n", local_buf);
  if (canary != 0xdeadbeef) abort();
}

int main() {
  char buffer[1024];
  ssize_t length = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
  if (length > 0) {
    buffer[length] = '\0';
    target_function(buffer);
  }
  return 0;
}