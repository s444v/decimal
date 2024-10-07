#include <check.h>

#include "../s21_decimal.h"

static s21_decimal num1[] = {
    {{16, 0x00000000, 0x00000000, 0x00000000}},  // 0 10
    {{8, 0x00000000, 0x00000000, 0x00000000}},   // 1 10
};

static s21_decimal num2[] = {
    {{4, 0x00000000, 0x00000000, 0x00000000}},  // 0 5
    {{2, 0x00000000, 0x00000000, 0x00000000}},  // 1 20010
};

static s21_decimal result[] = {
    {{4, 0x00000000, 0x00000000, 0x00000000}},  // 0: 2
    {{4, 0, 0, 0}},
};

START_TEST(test) {
  for (size_t i = 0; i < sizeof(num1) / sizeof(s21_decimal); ++i) {
    s21_decimal tmp;
    int ret = s21_div(num1[i], num2[i], &tmp);
    ck_assert_int_eq(tmp.bits[0], result[i].bits[0]);
    ck_assert_int_eq(tmp.bits[1], result[i].bits[1]);
    ck_assert_int_eq(tmp.bits[2], result[i].bits[2]);
    ck_assert_int_eq(tmp.bits[3], result[i].bits[3]);
    ck_assert_int_eq(ret, 0);
  }
}
END_TEST

Suite *suite_div(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("s21_div");
  tc = tcase_create("s21_div");

  if (s != NULL && tc != NULL) {
    tcase_add_test(tc, test);
    suite_add_tcase(s, tc);
  }

  return (s);
}