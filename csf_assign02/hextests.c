// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include "hexfuncs.h"
#include "tctest.h"
#include <stdio.h>
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

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(testFormatOffset);
  TEST(testFormatByteAsHex);
  TEST(testHexToPrintable);

  TEST_FINI();

  return 0;
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
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));

  hex_format_byte_as_hex('a', buf);
  ASSERT(0 == strcmp(buf, "61"));

  hex_format_byte_as_hex(' ', buf);
  ASSERT(0 == strcmp(buf, "20"));

  hex_format_byte_as_hex('\0', buf);
  ASSERT(0 == strcmp(buf, "00"));

  hex_format_byte_as_hex(':', buf);
  ASSERT(0 == strcmp(buf, "3a"));
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('f' == hex_to_printable('f'));
  ASSERT('.' == hex_to_printable('\t'));
  ASSERT('.' == hex_to_printable('\0'));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
}
