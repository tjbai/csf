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

  char sbuf[len];
  for (int i = 0; i < len; ++i) {
    sbuf[i] = hex_to_printable(s[i]);
  }

  write(STDOUT_FILENO, sbuf, len);
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
  sbuf[1] = (byteval & 0xF) + '0';
  byteval >>= 4;
  sbuf[0] = (byteval & 0xF) + '0';
}

char hex_to_printable(unsigned char byteval) {
  if (byteval >= 32 && byteval <= 126) {
    return (char)byteval;
  }
  return '.';
}
