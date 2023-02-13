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

void hex_format_offset(unsigned offset, char sbuf[]);

void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]);

char hex_to_printable(unsigned char byteval) {
  if (byteval >= 32 && byteval <= 126) {
    return (char)byteval;
  }

  return '.';
}
