#include "tctest.h"
#include <stdio.h>
#include <stdlib.h>

#include "uint256.h"

typedef struct {
  UInt256 zero;
  UInt256 one;
  UInt256 large1;

  const char *hex1;
  const char *hex2;
  const char *hex3;
} TestObjs;

// Functions to create and cleanup the test fixture object
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// Helper functions for implementing tests
int check(UInt256 val, uint64_t val3, uint64_t val2, uint64_t val1,
          uint64_t val0);

// Declarations of test functions
void test_get_bits(TestObjs *objs);
void test_create_from_u64(TestObjs *objs);
void test_create(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_add_1(TestObjs *objs);
void test_add_2(TestObjs *objs);
void test_add_3(TestObjs *objs);
void test_sub_1(TestObjs *objs);
void test_sub_2(TestObjs *objs);
void test_sub_3(TestObjs *objs);
void test_mul_1(TestObjs *objs);
void test_mul_2(TestObjs *objs);

// Custom tests declarations
void test_add_0(TestObjs *objs);
void test_add_positive_overflow(TestObjs *objs);
void test_sub_0(TestObjs *objs);
void test_sub_negative_overflow(TestObjs *objs);
void test_simple_mul();
void test_simple_sub();
void test_add_genfact_1();
void test_add_genfact_2();
void test_sub_genfact_1();
void test_sub_genfact_2();
void test_mul_genfact_1();
void test_mul_genfact_2();
void test_mul_genfact_3();
// void test_mul_genfact_4();
void test_sub_itself(TestObjs *objs);
void test_create_from_hex_longer_than_64();
void test_create_from_hex_shorter_than_64();
void test_format_as_big_hex();
void test_format_as_hex_shorter_than_64();
void test_mul_1_big(TestObjs *objs);
void test_mul_0_big(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_get_bits);
  TEST(test_create_from_u64);
  TEST(test_create);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_add_1);
  TEST(test_add_2);
  TEST(test_add_3);
  TEST(test_sub_1);
  TEST(test_sub_2);
  TEST(test_sub_3);
  TEST(test_mul_1);
  TEST(test_mul_2);

  TEST(test_add_positive_overflow);
  TEST(test_sub_negative_overflow);

  TEST(test_simple_mul);
  TEST(test_simple_sub);

  TEST(test_add_genfact_1);
  TEST(test_add_genfact_2);

  TEST(test_sub_genfact_1);
  TEST(test_sub_genfact_2);

  TEST(test_mul_genfact_1);
  TEST(test_mul_genfact_2);
  TEST(test_mul_genfact_3);
  // TEST(test_mul_genfact_4);

  TEST(test_sub_itself);
  TEST(test_create_from_hex_longer_than_64);
  TEST(test_create_from_hex_shorter_than_64);
  TEST(test_format_as_big_hex);
  TEST(test_format_as_hex_shorter_than_64);
  TEST(test_mul_1_big);
  TEST(test_mul_0_big);

  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = (TestObjs *)malloc(sizeof(TestObjs));

  // initialize several UInt256 values "manually"

  objs->zero.data[3] = 0U;
  objs->zero.data[2] = 0U;
  objs->zero.data[1] = 0U;
  objs->zero.data[0] = 0U;

  objs->one.data[3] = 0UL;
  objs->one.data[2] = 0UL;
  objs->one.data[1] = 0UL;
  objs->one.data[0] = 1UL;

  objs->large1.data[3] = 0xAA;
  objs->large1.data[2] = 0xBB;
  objs->large1.data[1] = 0xCC;
  objs->large1.data[0] = 0xDD;

  // example hex strings

  objs->hex1 = "0";
  objs->hex2 = "cafe";
  objs->hex3 =
      "4a4b72ebb654226ef77ed83d884f4940e4243bc3913ceaf5781b28d25fb00b0";

  return objs;
}

void cleanup(TestObjs *objs) { free(objs); }

int check(UInt256 val, uint64_t val3, uint64_t val2, uint64_t val1,
          uint64_t val0) {
  return uint256_get_bits(val, 3) == val3 && uint256_get_bits(val, 2) == val2 &&
         uint256_get_bits(val, 1) == val1 && uint256_get_bits(val, 0) == val0;
}

void test_get_bits(TestObjs *objs) {
  ASSERT(0U == uint256_get_bits(objs->zero, 3));
  ASSERT(0U == uint256_get_bits(objs->zero, 2));
  ASSERT(0U == uint256_get_bits(objs->zero, 1));
  ASSERT(0U == uint256_get_bits(objs->zero, 0));

  ASSERT(0U == uint256_get_bits(objs->one, 3));
  ASSERT(0U == uint256_get_bits(objs->one, 2));
  ASSERT(0U == uint256_get_bits(objs->one, 1));
  ASSERT(1U == uint256_get_bits(objs->one, 0));
}

void test_create_from_u64(TestObjs *objs) {
  objs->zero = uint256_create_from_u64(0U);
  objs->one = uint256_create_from_u64(1U);

  ASSERT(check(objs->zero, 0U, 0U, 0U, 0U));
  ASSERT(check(objs->one, 0U, 0U, 0U, 1U));
}

void test_create(TestObjs *objs) {
  // Note that the initializers list the uint64_t values to
  // be stored in order from least significant to most significant
  uint64_t zero_init[] = {0U, 0U, 0U, 0U};
  uint64_t one_init[] = {1U, 0U, 0U, 0U};

  objs->zero = uint256_create(zero_init);
  objs->one = uint256_create(one_init);

  // In the calls to check, the uint64_t values are specified
  // in the order most significant to least significant
  // (i.e., the reverse of the order in the array passed to
  // uint256_create)
  ASSERT(check(objs->zero, 0U, 0U, 0U, 0U));
  ASSERT(check(objs->one, 0U, 0U, 0U, 1U));
}

void test_create_from_hex(TestObjs *objs) {
  UInt256 val;

  val = uint256_create_from_hex(objs->hex1);
  ASSERT(check(val, 0x0UL, 0x0UL, 0x0UL, 0x0UL));

  val = uint256_create_from_hex(objs->hex2);
  ASSERT(check(val, 0x0UL, 0x0UL, 0x0UL, 0xcafeUL));
}

void test_create_from_hex_longer_than_64() {
  UInt256 val;
  val = uint256_create_from_hex(
      "9c6037f9119d850b26d16149ad06ebc03e1d75da171ad209df383b10d5237d12f12de");
  ASSERT(12757943719096685278UL == val.data[0]);
  ASSERT(6746798705168216963UL == val.data[1]);
  ASSERT(1484728201191350743UL == val.data[2]);
  ASSERT(9192156731297393942UL == val.data[3]);
}

void test_create_from_hex_shorter_than_64() {
  UInt256 val;
  val =
      uint256_create_from_hex("19571d4e351c4ebb9119b2fa7ee235aada709ec41f986");
  ASSERT(6534062295444748678UL == val.data[0]);
  ASSERT(16985626827821674019UL == val.data[1]);
  ASSERT(445791112221124UL == val.data[2]);
  ASSERT(0UL == val.data[3]);
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = uint256_format_as_hex(objs->zero);
  ASSERT(0 == strcmp("0", s));
  free(s);

  s = uint256_format_as_hex(objs->one);
  ASSERT(0 == strcmp("1", s));
  free(s);
}

void test_add_0(TestObjs *objs) {
  UInt256 left, result;

  // 4db67a5fd770167f0139fb6587483f8e5f32efea965da9cf7a11076cbe01c70 +
  // 0 = 4db67a5fd770167f0139fb6587483f8e5f32efea965da9cf7a11076cbe01c70
  left.data[0] = 0xf7a11076cbe01c70UL;
  left.data[1] = 0xe5f32efea965da9cUL;
  left.data[2] = 0xf0139fb6587483f8UL;
  left.data[3] = 0x4db67a5fd770167UL;
  objs->zero = uint256_create_from_u64(0U);
  result = uint256_add(left, objs->zero);
  ASSERT(0xf7a11076cbe01c70UL == result.data[0]);
  ASSERT(0xe5f32efea965da9cUL == result.data[1]);
  ASSERT(0xf0139fb6587483f8UL == result.data[2]);
  ASSERT(0x4db67a5fd770167UL == result.data[3]);
}

void test_add_1(TestObjs *objs) {
  // basic addition tests

  UInt256 sum;

  sum = uint256_add(objs->zero, objs->one);

  ASSERT(0UL == sum.data[3]);
  ASSERT(0UL == sum.data[2]);
  ASSERT(0UL == sum.data[1]);
  ASSERT(1UL == sum.data[0]);
}

void test_add_2(TestObjs *objs) {
  // "medium" addition tests

  (void)objs;

  UInt256 left, right, result;

  // a932467b27ccb2b4cb6c96c4810cb5e + b37aef92a5179d60af2f9cad0a4b38a =
  // 15cad360dcce450157a9c33718b57ee8
  left.data[0] = 0x4cb6c96c4810cb5eUL;
  left.data[1] = 0xa932467b27ccb2bUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x0af2f9cad0a4b38aUL;
  right.data[1] = 0xb37aef92a5179d6UL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_add(left, right);
  ASSERT(0x57a9c33718b57ee8UL == result.data[0]);
  ASSERT(0x15cad360dcce4501UL == result.data[1]);
  ASSERT(0x0UL == result.data[2]);
  ASSERT(0x0UL == result.data[3]);
}

void test_add_3(TestObjs *objs) {
  // "large" addition tests

  (void)objs;

  UInt256 left, right, result;

  // 9515af0631ecc4779e0c122009e87b1ff1076115dab87f8190bc8c2ceb84ceb +
  // 173ba2210b102e74dc94683c12d20ceda74af7d4cf5fc80aa154504e7e58712 =
  // ac5151273cfcf2ec7aa07a5c1cba880d985258eaaa18478c3210dc7b69dd3fd
  left.data[0] = 0x190bc8c2ceb84cebUL;
  left.data[1] = 0xff1076115dab87f8UL;
  left.data[2] = 0x79e0c122009e87b1UL;
  left.data[3] = 0x9515af0631ecc47UL;
  right.data[0] = 0xaa154504e7e58712UL;
  right.data[1] = 0xda74af7d4cf5fc80UL;
  right.data[2] = 0x4dc94683c12d20ceUL;
  right.data[3] = 0x173ba2210b102e7UL;
  result = uint256_add(left, right);
  ASSERT(0xc3210dc7b69dd3fdUL == result.data[0]);
  ASSERT(0xd985258eaaa18478UL == result.data[1]);
  ASSERT(0xc7aa07a5c1cba880UL == result.data[2]);
  ASSERT(0xac5151273cfcf2eUL == result.data[3]);
}

void test_add_positive_overflow(TestObjs *objs) {
  UInt256 left = {{~(0UL), ~(0UL), ~(0UL), ~(0UL)}};
  UInt256 res = uint256_add(left, objs->one);
  for (int i = 0; i < 4; ++i) {
    ASSERT(0UL == res.data[i]);
  }
}

void test_add_genfact_1() {
  UInt256 left, right, result;

  // d56ba10baeb819c954e4ba3e4510dc85d33383857afa15f80fa6ae118ceeead +
  // eb88e1067ee70ecba403a4d7d11d77d4c29df747b82aa7e6f0244537aadcd6c =
  // 1c0f482122d9f2894f8e85f16162e545a95d17acd3324bddeffcaf34937cbc19
  left.data[0] = 0x80fa6ae118ceeeadUL;
  left.data[1] = 0x5d33383857afa15fUL;
  left.data[2] = 0x954e4ba3e4510dc8UL;
  left.data[3] = 0xd56ba10baeb819cUL;
  right.data[0] = 0x6f0244537aadcd6cUL;
  right.data[1] = 0x4c29df747b82aa7eUL;
  right.data[2] = 0xba403a4d7d11d77dUL;
  right.data[3] = 0xeb88e1067ee70ecUL;
  result = uint256_add(left, right);
  ASSERT(0xeffcaf34937cbc19UL == result.data[0]);
  ASSERT(0xa95d17acd3324bddUL == result.data[1]);
  ASSERT(0x4f8e85f16162e545UL == result.data[2]);
  ASSERT(0x1c0f482122d9f289UL == result.data[3]);
}

void test_add_genfact_2() {
  UInt256 left, right, result;

  // 8e64799a2a9b5124bd8c9ab21f245a477b362a3b471876e16b8edac2b9ff21e +
  // dbf9ba46e5fea8f5c32fdb253be6a8933dac09726061a09c91811f0eeffab3d =
  // 16a5e33e11099fa1a80bc75d75b0b02dab8e233ada77a177dfd0ff9d1a9f9d5b
  left.data[0] = 0x16b8edac2b9ff21eUL;
  left.data[1] = 0x77b362a3b471876eUL;
  left.data[2] = 0x4bd8c9ab21f245a4UL;
  left.data[3] = 0x8e64799a2a9b512UL;
  right.data[0] = 0xc91811f0eeffab3dUL;
  right.data[1] = 0x33dac09726061a09UL;
  right.data[2] = 0x5c32fdb253be6a89UL;
  right.data[3] = 0xdbf9ba46e5fea8fUL;
  result = uint256_add(left, right);
  ASSERT(0xdfd0ff9d1a9f9d5bUL == result.data[0]);
  ASSERT(0xab8e233ada77a177UL == result.data[1]);
  ASSERT(0xa80bc75d75b0b02dUL == result.data[2]);
  ASSERT(0x16a5e33e11099fa1UL == result.data[3]);
}

void test_simple_sub() {
  UInt256 left = {{~0UL, ~0UL, 0UL, 0UL}};
  UInt256 right = {{3UL, 0UL, 0UL, 0UL}};
  UInt256 res = uint256_sub(left, right);
  ASSERT(res.data[0] == ~0UL - 3UL);
  ASSERT(res.data[1] == ~0UL);
}

void test_sub_0(TestObjs *objs) {
  UInt256 left, result;

  // 5fc5363b1c3b09b0bb7392ef10f6c458703f2f31c08e0a442b6b15c56a6af26 -
  // 0 = 5fc5363b1c3b09b0bb7392ef10f6c458703f2f31c08e0a442b6b15c56a6af26
  left.data[0] = 0x42b6b15c56a6af26UL;
  left.data[1] = 0x8703f2f31c08e0a4UL;
  left.data[2] = 0x0bb7392ef10f6c45UL;
  left.data[3] = 0x5fc5363b1c3b09bUL;
  objs->zero = uint256_create_from_u64(0U);
  result = uint256_sub(left, objs->zero);
  ASSERT(0x42b6b15c56a6af26UL == result.data[0]);
  ASSERT(0x8703f2f31c08e0a4UL == result.data[1]);
  ASSERT(0x0bb7392ef10f6c45UL == result.data[2]);
  ASSERT(0x5fc5363b1c3b09bUL == result.data[3]);
}

void test_sub_1(TestObjs *objs) {
  // basic subtraction tests

  UInt256 result;

  result = uint256_sub(objs->one, objs->zero);
  ASSERT(0UL == result.data[3]);
  ASSERT(0UL == result.data[2]);
  ASSERT(0UL == result.data[1]);
  ASSERT(1UL == result.data[0]);
}

void test_sub_2(TestObjs *objs) {
  // "medium" subtraction tests

  (void)objs;

  UInt256 left, right, result;

  // af7939833038cfa010f734368aaad66 - 6473865d77af47d71f7aedf64364960 =
  // 4b05b325b88987c8f17c46404746406
  left.data[0] = 0x010f734368aaad66UL;
  left.data[1] = 0xaf7939833038cfaUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x71f7aedf64364960UL;
  right.data[1] = 0x6473865d77af47dUL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_sub(left, right);
  ASSERT(0x8f17c46404746406UL == result.data[0]);
  ASSERT(0x4b05b325b88987cUL == result.data[1]);
  ASSERT(0x0UL == result.data[2]);
  ASSERT(0x0UL == result.data[3]);
}

void test_sub_3(TestObjs *objs) {
  // "large" subtraction tests

  (void)objs;

  UInt256 left, right, result;

  // bc556287a225313cc07a1509f4ebb335034f5d413945ac7d0bdb42962a6ae8c -
  // 7209ef9bebd10ecdc8fb3ccc6c9c69f41f2b217da808c18793c019c3cabaddc =
  // 4a4b72ebb654226ef77ed83d884f4940e4243bc3913ceaf5781b28d25fb00b0
  left.data[0] = 0xd0bdb42962a6ae8cUL;
  left.data[1] = 0x5034f5d413945ac7UL;
  left.data[2] = 0xcc07a1509f4ebb33UL;
  left.data[3] = 0xbc556287a225313UL;
  right.data[0] = 0x793c019c3cabaddcUL;
  right.data[1] = 0x41f2b217da808c18UL;
  right.data[2] = 0xdc8fb3ccc6c9c69fUL;
  right.data[3] = 0x7209ef9bebd10ecUL;
  result = uint256_sub(left, right);
  ASSERT(0x5781b28d25fb00b0UL == result.data[0]);
  ASSERT(0x0e4243bc3913ceafUL == result.data[1]);
  ASSERT(0xef77ed83d884f494UL == result.data[2]);
  ASSERT(0x4a4b72ebb654226UL == result.data[3]);
}

void test_sub_itself(TestObjs *objs) {
  // "large" subtraction tests

  (void)objs;

  UInt256 left, right, result;

  // bc556287a225313cc07a1509f4ebb335034f5d413945ac7d0bdb42962a6ae8c -
  // 7209ef9bebd10ecdc8fb3ccc6c9c69f41f2b217da808c18793c019c3cabaddc =
  // 4a4b72ebb654226ef77ed83d884f4940e4243bc3913ceaf5781b28d25fb00b0
  left.data[0] = 0xd0bdb42962a6ae8cUL;
  left.data[1] = 0x5034f5d413945ac7UL;
  left.data[2] = 0xcc07a1509f4ebb33UL;
  left.data[3] = 0xbc556287a225313UL;
  right.data[0] = 0xd0bdb42962a6ae8cUL;
  right.data[1] = 0x5034f5d413945ac7UL;
  right.data[2] = 0xcc07a1509f4ebb33UL;
  right.data[3] = 0xbc556287a225313UL;
  result = uint256_sub(left, right);
  ASSERT(0UL == result.data[0]);
  ASSERT(0UL == result.data[1]);
  ASSERT(0UL == result.data[2]);
  ASSERT(0UL == result.data[3]);
}

void test_sub_negative_overflow(TestObjs *objs) {
  UInt256 res = uint256_sub(objs->zero, objs->one);
  for (int i = 0; i < 4; ++i) {
    ASSERT(~(0UL) == res.data[i]);
  }
}

void test_sub_genfact_1() {
  UInt256 left, right, result;

  // 18a3ad8c5edb7debb667148a09abfefe685709ece99c39ae439bf8b8dac4210 -
  // 6ae64b688c7d80b1a7a7a75ef8982993b7757200e5679d066ff3f19449403c =
  // 11f548d5d613a5e09bec9a141a227c652cdfb2ccdb45bfdddc9cb99f96301d4
  left.data[0] = 0xe439bf8b8dac4210UL;
  left.data[1] = 0xe685709ece99c39aUL;
  left.data[2] = 0xbb667148a09abfefUL;
  left.data[3] = 0x18a3ad8c5edb7deUL;
  right.data[0] = 0x066ff3f19449403cUL;
  right.data[1] = 0x93b7757200e5679dUL;
  right.data[2] = 0xb1a7a7a75ef89829UL;
  right.data[3] = 0x6ae64b688c7d80UL;
  result = uint256_sub(left, right);
  ASSERT(0xddc9cb99f96301d4UL == result.data[0]);
  ASSERT(0x52cdfb2ccdb45bfdUL == result.data[1]);
  ASSERT(0x09bec9a141a227c6UL == result.data[2]);
  ASSERT(0x11f548d5d613a5eUL == result.data[3]);
}

void test_sub_genfact_2() {
  UInt256 left, right, result;

  // cdd11898c75b8bcc5d333428ca1c01f064ff8bad5d558095a47c83423acafd7 -
  // 411a13c74ef35b19eef07d63da91d84a23237937dd530f8d41382fede3c23a =
  // c9bf775c526c561abe442c528c72e46bc2cd5419df804f9cd06900435c8ed9d
  left.data[0] = 0x5a47c83423acafd7UL;
  left.data[1] = 0x064ff8bad5d55809UL;
  left.data[2] = 0xc5d333428ca1c01fUL;
  left.data[3] = 0xcdd11898c75b8bcUL;
  right.data[0] = 0x8d41382fede3c23aUL;
  right.data[1] = 0x4a23237937dd530fUL;
  right.data[2] = 0x19eef07d63da91d8UL;
  right.data[3] = 0x411a13c74ef35bUL;
  result = uint256_sub(left, right);
  ASSERT(0xcd06900435c8ed9dUL == result.data[0]);
  ASSERT(0xbc2cd5419df804f9UL == result.data[1]);
  ASSERT(0xabe442c528c72e46UL == result.data[2]);
  ASSERT(0xc9bf775c526c561UL == result.data[3]);
}

void test_simple_mul() {
  UInt256 big_thing = uint256_create_from_u64(~0UL);
  UInt256 four = uint256_create_from_u64(4UL);
  UInt256 res = uint256_mul(big_thing, four);
  ASSERT(res.data[0] == (~(0UL) - 3));
  ASSERT(res.data[1] == 3UL);
}

void test_mul_1(TestObjs *objs) {
  UInt256 result;

  result = uint256_mul(objs->one, objs->one);
  ASSERT(check(result, 0UL, 0UL, 0UL, 1UL));

  result = uint256_mul(objs->one, objs->zero);
  ASSERT(check(result, 0UL, 0UL, 0UL, 0UL));
}

void test_mul_1_big(TestObjs *objs) {
  UInt256 left, result;

  // 1159b14e3e852321ede4512ddf84eab524a34e431a9308de2b4b9e16b54ae0 * 1
  // = 1159b14e3e852321ede4512ddf84eab524a34e431a9308de2b4b9e16b54ae0
  left.data[0] = 0xde2b4b9e16b54ae0UL;
  left.data[1] = 0xb524a34e431a9308UL;
  left.data[2] = 0x21ede4512ddf84eaUL;
  left.data[3] = 0x1159b14e3e8523UL;
  result = uint256_mul(left, objs->one);
  ASSERT(0xde2b4b9e16b54ae0UL == result.data[0]);
  ASSERT(0xb524a34e431a9308UL == result.data[1]);
  ASSERT(0x21ede4512ddf84eaUL == result.data[2]);
  ASSERT(0x1159b14e3e8523UL == result.data[3]);
}

void test_mul_0_big(TestObjs *objs) {
  UInt256 right, result;

  // 1159b14e3e852321ede4512ddf84eab524a34e431a9308de2b4b9e16b54ae0 * 1
  // = 1159b14e3e852321ede4512ddf84eab524a34e431a9308de2b4b9e16b54ae0
  right.data[0] = 0xde2b4b9e16b54ae0UL;
  right.data[1] = 0xb524a34e431a9308UL;
  right.data[2] = 0x21ede4512ddf84eaUL;
  right.data[3] = 0x1159b14e3e8523UL;
  result = uint256_mul(objs->zero, right);
  ASSERT(0UL == result.data[0]);
  ASSERT(0UL == result.data[1]);
  ASSERT(0UL == result.data[2]);
  ASSERT(0UL == result.data[3]);
}

void test_mul_2(TestObjs *objs) {
  (void)objs;

  UInt256 left, right, result;

  // 761544a98b82abc63f23766d1391782 * 14bf658bd8053a9484c32d955a47a2f =
  // 991f2125eacd361abad710163aa9be6117fa57cddf52e73c97a28d7f744de
  left.data[0] = 0x63f23766d1391782UL;
  left.data[1] = 0x761544a98b82abcUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x484c32d955a47a2fUL;
  right.data[1] = 0x14bf658bd8053a9UL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_mul(left, right);
  ASSERT(0x73c97a28d7f744deUL == result.data[0]);
  ASSERT(0xe6117fa57cddf52eUL == result.data[1]);
  ASSERT(0x61abad710163aa9bUL == result.data[2]);
  ASSERT(0x991f2125eacd3UL == result.data[3]);
}

void test_mul_genfact_1() {
  UInt256 left, right, result;

  // 2fa069a56ab19b5a2980d469e30a337 *
  // f22c1f9954d30055f28b6921e6d13fa =
  // 2d0dd9517abc4b1b8c6c8da9e611d71089160bfd60adb8b47271e8080778b6
  left.data[0] = 0xa2980d469e30a337UL;
  left.data[1] = 0x2fa069a56ab19b5UL;
  left.data[2] = 0UL;
  left.data[3] = 0UL;
  right.data[0] = 0x5f28b6921e6d13faUL;
  right.data[1] = 0xf22c1f9954d3005UL;
  right.data[2] = 0UL;
  right.data[3] = 0UL;
  result = uint256_mul(left, right);
  ASSERT(0xb47271e8080778b6UL == result.data[0]);
  ASSERT(0x1089160bfd60adb8UL == result.data[1]);
  ASSERT(0x1b8c6c8da9e611d7UL == result.data[2]);
  ASSERT(0x2d0dd9517abc4bUL == result.data[3]);
}

void test_mul_genfact_2() {
  UInt256 left, right, result;

  // 3bc67a0b2f5be075e31640d220e711c *
  // 541e8507868308b0e8ef980621a5b94 =
  // 13a4405dd81754cdd49fb126aa68bab088868b900219268c42497707665830
  left.data[0] = 0x5e31640d220e711cUL;
  left.data[1] = 0x3bc67a0b2f5be07UL;
  left.data[2] = 0UL;
  left.data[3] = 0UL;
  right.data[0] = 0x0e8ef980621a5b94UL;
  right.data[1] = 0x541e8507868308bUL;
  right.data[2] = 0UL;
  right.data[3] = 0UL;
  result = uint256_mul(left, right);
  ASSERT(0x8c42497707665830UL == result.data[0]);
  ASSERT(0xb088868b90021926UL == result.data[1]);
  ASSERT(0xcdd49fb126aa68baUL == result.data[2]);
  ASSERT(0x13a4405dd81754UL == result.data[3]);
}

void test_mul_genfact_3() {
  UInt256 left, right, result;

  // f8e143880643e1b27065f7aecba3a6a *
  // 8451d3714b47b41319cd80c84c658a5 =
  // 80a3b3a8508c9c62ac3fe702f12c7c002920b21a0ab027afad16ba6e181652
  right.data[0] = 0x27065f7aecba3a6aUL;
  right.data[1] = 0xf8e143880643e1bUL;
  right.data[2] = 0UL;
  right.data[3] = 0UL;
  left.data[0] = 0x319cd80c84c658a5UL;
  left.data[1] = 0x8451d3714b47b41UL;
  left.data[2] = 0UL;
  left.data[3] = 0UL;
  result = uint256_mul(left, right);
  ASSERT(0xafad16ba6e181652UL == result.data[0]);
  ASSERT(0x002920b21a0ab027UL == result.data[1]);
  ASSERT(0x62ac3fe702f12c7cUL == result.data[2]);
  ASSERT(0x80a3b3a8508c9cUL == result.data[3]);
}

void test_format_as_big_hex() {
  char *s;
  UInt256 val;
  val.data[0] = ~(0UL);
  val.data[1] = ~(0UL);
  val.data[2] = ~(0UL);
  val.data[3] = ~(0UL);

  s = uint256_format_as_hex(val);
  ASSERT(
      0 ==
      strcmp("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
             s));
  free(s);
}

void test_format_as_hex_shorter_than_64() {
  char *s;
  UInt256 val;
  val.data[0] = 0x7a055816a0ae11b7UL;
  val.data[1] = 0xcd8c5e6af3a5467UL;
  val.data[2] = 0UL;
  val.data[3] = 0UL;

  s = uint256_format_as_hex(val);
  ASSERT(0 == strcmp("cd8c5e6af3a54677a055816a0ae11b7", s));
  free(s);
}

// void test_mul_genfact_4() {
//   UInt256 left, right, result;

//   // e409935d588addeca5e84c094d01577 *
//   // 5db792507f26718855c7a2c5721f2a6 =
//   // 537affb66b31063754b9d5ff38fc41ba31be8f9926a12c69f06e1f688f692a
//   right.data[0] = 0xa5e84c094d01577UL;
//   right.data[1] = 0xe409935d588addecUL;
//   right.data[2] = 0UL;
//   right.data[3] = 0UL;
//   left.data[0] = 0x55c7a2c5721f2a6UL;
//   left.data[1] = 0x5db792507f267188UL;
//   left.data[2] = 0UL;
//   left.data[3] = 0UL;
//   result = uint256_mul(left, right);
//   ASSERT(0x69f06e1f688f692aUL == result.data[0]);
//   ASSERT(0xba31be8f9926a12cUL == result.data[1]);
//   ASSERT(0x3754b9d5ff38fc41UL == result.data[2]);
//   ASSERT(0x537affb66b3106UL == result.data[3]);
// }