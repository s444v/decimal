#include "test_decimal.h"

#include <check.h>
#include <stdlib.h>

static int run_test_suite(Suite *test_suite) {
  int number_failed = 0;
  SRunner *sr = NULL;

  sr = srunner_create(test_suite);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed);
}

int main(void) {
  int number_failed = 0;
  Suite *suite_array[] = {suite_add(),
                          suite_sub(),
                          suite_div(),
                          suite_from_int_to_decimal(),
                          suite_from_float_to_decimal(),
                          suite_from_decimal_to_int(),
                          suite_from_decimal_to_float(),
                          suite_is_less(),
                          suite_is_less_or_equal(),
                          suite_is_greater(),
                          suite_is_greater_or_equal(),
                          suite_is_equal(),
                          suite_is_not_equal(),
                          suite_truncate(),
                          suite_negate(),
                          suite_floor(),
                          suite_mul(),
                          suite_round(),
                          NULL};

  for (size_t i = 0; suite_array[i]; ++i) {
    number_failed += run_test_suite(suite_array[i]);
  }

  return (number_failed ? EXIT_FAILURE : EXIT_SUCCESS);
}
