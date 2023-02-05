#include "uint256.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a UInt256 value from a single uint64_t value.
// Only the least-significant 64 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u64(uint64_t val) {
  return (UInt256){{val, 0, 0, 0}};
}

// Create a UInt256 value from an array of 4 uint64_t values.
// The element at index 0 is the least significant, and the element
// at index 3 is the most significant.
UInt256 uint256_create(const uint64_t data[4]) {
  return (UInt256){{data[0], data[1], data[2], data[3]}};
}

int is_dig(char c) { return c >= '0' && c <= '9'; }

int min(int a, int b) { return a > b ? a : b; }

int ceiling(float f) {
  int i = (int)f;
  if (i == f) {
    return i;
  }
  return i + 1;
}

void copy_substring(char *buffer, const char *source, int offset) {
  for (int i = 15; i >= 0; --i) {
    buffer[i] = offset + i >= 0 ? source[offset + i] : '0';
  }
}

// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 res = {0UL};
  int len = strlen(hex);

  for (int i = 1; i <= min(4, ceiling((float)len / 16)); ++i) {
    char buffer[16];
    copy_substring(buffer, hex, len - i * 16);
    res.data[i - 1] = strtoul(buffer, NULL, 16);
  }

  return res;
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  char buffer[65];
  sprintf(buffer, "%016lx%016lx%016lx%016lx", val.data[3], val.data[2],
          val.data[1], val.data[0]);

  int i = 0;
  while (buffer[i] == '0' && i < 64)
    ++i;

  int new_len = 64 - i;
  char *result;
  if (!new_len) {
    result = malloc(2 * sizeof(char));
    result[0] = '0', result[1] = '\0';
  } else {
    result = malloc((new_len + 1) * sizeof(char));
    for (int j = 0; j < new_len; ++j) {
      result[j] = buffer[i + j];
    }
    result[new_len] = '\0';
  }

  return result;
}

// Get 64 bits of data from a UInt256 value.
// Index 0 is the least significant 64 bits, index 3 is the most
// significant 64 bits.
uint64_t uint256_get_bits(UInt256 val, unsigned index) {
  return val.data[index];
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {
  UInt256 res;
  int carry = 0;

  for (int i = 0; i < 4; ++i) {
    uint64_t sum = left.data[i] + right.data[i] + carry;
    carry = sum < left.data[i] || (sum == left.data[i] && carry);
    res.data[i] = sum;
  }

  return res;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 invertedRight = {0UL};

  for (int i = 0; i < 4; ++i) {
    invertedRight.data[i] = ~right.data[i];
  }

  invertedRight = uint256_add(invertedRight, uint256_create_from_u64(1UL));
  return uint256_add(left, invertedRight);
}

int ith_bit_set(UInt256 val, int i) {
  int index = i / 64, extra = i % 64;
  return (val.data[index] & (1UL << extra)) != 0;
}

UInt256 leftshift(UInt256 val, int shift) {
  UInt256 res = uint256_create(val.data);

  // Copy all bits shift positions forward
  for (int i = 0; i < 256; ++i) {
    if (i + shift >= 256)
      continue;

    int repIndex = (i + shift) / 64;
    int repExtra = (i + shift) % 64;

    int curBit = ith_bit_set(val, i);
    int repBit = ith_bit_set(val, i + shift);

    if (curBit == repBit)
      continue;
    else if (curBit)
      res.data[repIndex] |= 1UL << repExtra;
    else
      res.data[repIndex] &= ~(1UL << repExtra);
  }

  // Set the rightmost shift digits to 0
  for (int i = 0; i < shift; ++i) {
    int index = i / 64, extra = i % 64;
    res.data[index] &= ~(1UL << extra);
  }

  return res;
}

// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 res = {0UL};

  for (int i = 0; i < 256; ++i) {
    if (ith_bit_set(right, i)) {
      res = uint256_add(res, leftshift(left, i));
    }
  }

  return res;
}
