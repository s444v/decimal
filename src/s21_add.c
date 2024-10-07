#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal value_big_1 = from_decimal_to_big_decimal(value_1);

  s21_big_decimal value_big_2 = from_decimal_to_big_decimal(value_2);
  s21_big_decimal result_big = {{0, 0, 0, 0, 0, 0}, 0, 0};

  set_biggest_scale_big_decimal(&value_big_1, &value_big_2);

  int error = 0;

  if (value_big_1.sign == value_big_2.sign) {
    add_mantise_big_decimal(value_big_1, value_big_2, &result_big);
    result_big.sign = value_big_1.sign;
  } else {
    if (big_decimal_is_greater_abs(value_big_1, value_big_2)) {
      result_big.sign = value_big_1.sign;
      value_big_1.sign = 0;
      value_big_2.sign = 0;
      sub_mantise_big_decimal(value_big_1, value_big_2, &result_big);
    } else if (big_decimal_is_equal_abs(value_big_1, value_big_2)) {
      result_big.scale = value_big_1.scale;
    } else {
      result_big.sign = value_big_2.sign;
      value_big_1.sign = 0;
      value_big_2.sign = 0;
      sub_mantise_big_decimal(value_big_2, value_big_1, &result_big);
    }
  }
  result_big.scale = value_big_1.scale;
  error = from_big_decimal_to_decimal(result_big, result);

  return error;
}
