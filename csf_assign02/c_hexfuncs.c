// C implementation of hexdump functions

#include "hexfuncs.h"
#include <unistd.h> // this is the only system header file you may include!

unsigned hex_read(char data_buf[]) { return (unsigned)read(0, data_buf, 16); }

void hex_write_string(const char s[]) {}

void hex_format_offset(unsigned offset, char sbuf[]);
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]);
char hex_to_printable(unsigned char byteval);
