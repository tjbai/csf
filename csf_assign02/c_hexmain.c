// C implementation of hexdump main function

#include "hexfuncs.h" // this is the only header file which may be included!
#include "stdio.h"

int main(void) {
  char abuf[17];
  char sbuf[17];
  unsigned chars_read = 0x0u;
  int just_read = 0;

  while ((just_read = hex_read(abuf))) {

    // write offset
    hex_format_offset(chars_read, sbuf);
    sbuf[8] = ':';
    sbuf[9] = ' ';
    sbuf[10] = '\0';
    hex_write_string(sbuf);

    // write each converted character
    int i = 0;
    for (; i < just_read; ++i) {
      hex_format_byte_as_hex((unsigned char)abuf[i], sbuf);
      sbuf[2] = ' ';
      sbuf[3] = '\0';
      hex_write_string(sbuf);

      abuf[i] = hex_to_printable(abuf[i]);
    }

    // fill the rest of line
    sbuf[0] = ' ';
    sbuf[1] = ' ';
    sbuf[2] = ' ';
    sbuf[3] = '\0';
    while (i < 16) {
      hex_write_string(sbuf);
      ++i;
    }

    sbuf[0] = ' ';
    sbuf[1] = '\0';
    hex_write_string(sbuf);

    // write original characters
    abuf[just_read] = '\0';
    hex_write_string(abuf);

    // start new line
    sbuf[0] = '\n';
    sbuf[1] = '\0';
    hex_write_string(sbuf);

    chars_read += 16;
  }
}
