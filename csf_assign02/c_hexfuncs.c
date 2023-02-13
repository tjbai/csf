// C implementation of hexdump functions

#include "hexfuncs.h"
#include <unistd.h> // this is the only system header file you may include!

unsigned hex_read(char data_buf[]) {
  return (unsigned)read(STDIN_FILENO, data_buf, 16);
}

void hex_write_string(const char s[]) {
  int len = 0;
  while (s[len] != '\0')
    ++len;

  write(STDOUT_FILENO, s, len);
}

void hex_format_offset(unsigned offset, char sbuf[]) {
  sbuf[8] = '\0';
  for (int i = 7; i >= 0; --i) {
    int ls4 = (offset & 0xF);
    sbuf[i] = ls4 < 10 ? '0' + ls4 : 'a' + ls4 - 10;
    offset >>= 4;
  }
}

void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
  for (int i = 1; i >= 0; --i) {
    int ls4 = (byteval & 0xF);
    sbuf[i] = ls4 < 10 ? '0' + ls4 : 'a' + ls4 - 10;
    byteval >>= 4;
  }
}

char hex_to_printable(unsigned char byteval) {
  if (byteval >= 32 && byteval <= 126) {
    return (char)byteval;
  }
  return '.';
}
