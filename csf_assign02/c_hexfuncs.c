/*
 * C implementation of hexdump functions
 * CSF Assignment 2
 * TJ Bai
 * tbai4@jhu.edu
 * Halle Cho
 * hcho64@jhu.edu
 */

#include "hexfuncs.h"
#include <unistd.h> // this is the only system header file you may include!

/* Read up to 16 bytes from standard input into data_buf.
 * Returns the number of characters read.
 *
 * Parameters:
 *   data_buf - location for 16 bytes
 *
 * Returns:
 *   the number of characters read
 */
unsigned hex_read(char data_buf[]) {
  return (unsigned)read(STDIN_FILENO, data_buf, 16);
}

/* Write given null-terminated string to standard output.
 *
 * Parameters:
 *   s - null-terminated string
 */
void hex_write_string(const char s[]) {
  int len = 0;
  while (s[len] != '\0')
    ++len;
  write(STDOUT_FILENO, s, len);
}

/* Format an unsigned value as an offset string consisting of exactly 8
 * hex digits.  The formatted offset is stored in sbuf, which must
 * have enough room for a string of length 8.
 *
 * Parameters:
 *   offset - amount of offset
 *   sbuf - location for offset
 */
void hex_format_offset(unsigned offset, char sbuf[]) {
  sbuf[8] = '\0';
  for (int i = 7; i >= 0; --i) {
    int ls4 = (offset & 0xF);
    sbuf[i] = ls4 < 10 ? '0' + ls4 : 'a' + ls4 - 10;
    offset >>= 4;
  }
}

/* Format a byte value (in the range 0-255) as string consisting
 * of two hex digits.  The string is stored in sbuf.
 *
 * Parameters:
 *   byteval - byte value
 *   sbuf - buffer to store hex representation of byteval
 */
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
  sbuf[2] = '\0';
  for (int i = 1; i >= 0; --i) {
    int ls4 = (byteval & 0x0F);
    sbuf[i] = ls4 < 10 ? (char)'0' + ls4 : (char)'a' + ls4 - 10;
    byteval >>= 4;
  }
}

/* Convert a byte value (in the range 0-255) to a printable character
 * value.  If byteval is already a printable character, it is returned
 * unmodified.  If byteval is not a printable character, then the
 * ASCII code for '.' should be returned.
 *
 * Parameters:
 *   byteval - byte value
 *
 * Returns:
 *   byteval if printable
 *   '.' if not printable
 */
char hex_to_printable(unsigned char byteval) {
  if (byteval >= 32 && byteval <= 126)
    return (char)byteval;
  return '.';
}
