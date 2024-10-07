#include <check.h>
#include <float.h>
#include <stdio.h>

#include "../s21_decimal.h"

START_TEST(test) {
  float input_array[] = {-12784.01, 0.00125, -0.001, 30, 0.000001, 123456.1};

  FILE *fp = fopen("tests/s21_from_float.txt", "r");
  if (fp) {
    char str_original[1024];
    char str_s21[1024];

    for (int i = 0; i < 6; i++) {
      fgets(str_original, sizeof(str_original), fp);
      if (i != 5) {
        str_original[strlen(str_original) - 1] = '\0';
      }
      s21_decimal output = {0};
      s21_from_float_to_decimal(input_array[i], &output);
      s21_dec_to_string(output, str_s21);

      ck_assert_str_eq(str_s21, str_original);
    }
    fclose(fp);
  }
}
END_TEST

Suite *suite_from_float_to_decimal(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("s21_from_float_to_decimal");
  tc = tcase_create("s21_from_float_to_decimal");

  if (s != NULL && tc != NULL) {
    tcase_add_test(tc, test);
    // tcase_add_test(tc, error_test);
    suite_add_tcase(s, tc);
  }

  return (s);
}
