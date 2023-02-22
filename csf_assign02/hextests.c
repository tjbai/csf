/*
 * Unit tests for hex functions
 * CSF Assignment 2
 * TJ Bai
 * tbai4@jhu.edu
 * Halle Cho
 * hcho64@jhu.edu
 */

#include "hexfuncs.h"
#include "tctest.h"
#include <stdlib.h>

// test fixture object
typedef struct {
  char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) { free(objs); }

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);
void testRead();

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testRead);
  // TEST(testFormatOffset);
  // TEST(testFormatByteAsHex);
  // TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
}

void testRead() {
  char buf[17];
  buf[16] = '\0';
  unsigned res = hex_read(buf);
  printf("\nREAD IN THIS: %s\n", buf);
  printf("%u\n", res);
}

void testFormatOffset(TestObjs *objs) {
  (void)objs;

  char buf[16];
  hex_format_offset(0x00000001u, buf);
  ASSERT(0 == strcmp(buf, "00000001"));

  hex_format_offset(0xabcd1234u, buf);
  ASSERT(0 == strcmp(buf, "abcd1234"));

  hex_format_offset(0x1234abcdu, buf);
  ASSERT(0 == strcmp(buf, "1234abcd"));

  hex_format_offset(0x11111111u, buf);
  ASSERT(0 == strcmp(buf, "11111111"));

  hex_format_offset(0x00000000u, buf);
  ASSERT(0 == strcmp(buf, "00000000"));

  hex_format_offset(0x12345678u, buf);
  ASSERT(0 == strcmp(buf, "12345678"));
}

void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  buf[2] = '\0';
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));

  hex_format_byte_as_hex('a', buf);
  ASSERT(0 == strcmp(buf, "61"));

  hex_format_byte_as_hex('A', buf);
  ASSERT(0 == strcmp(buf, "41"));

  hex_format_byte_as_hex(' ', buf);
  ASSERT(0 == strcmp(buf, "20"));

  hex_format_byte_as_hex('\0', buf);
  ASSERT(0 == strcmp(buf, "00"));

  hex_format_byte_as_hex(':', buf);
  ASSERT(0 == strcmp(buf, "3a"));

  hex_format_byte_as_hex(255, buf);
  ASSERT(0 == strcmp(buf, "ff"));

  hex_format_byte_as_hex('&', buf);
  ASSERT(0 == strcmp(buf, "26"));

  hex_format_byte_as_hex('{', buf);
  ASSERT(0 == strcmp(buf, "7b"));

  hex_format_byte_as_hex(128, buf);
  ASSERT(0 == strcmp(buf, "80"));
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('f' == hex_to_printable('f'));
  ASSERT('.' == hex_to_printable('\t'));
  ASSERT('.' == hex_to_printable('\0'));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
  ASSERT('.' == hex_to_printable(200));
  ASSERT(46 == hex_to_printable(255));
  ASSERT(34 == hex_to_printable(34));
}
