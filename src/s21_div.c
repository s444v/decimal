#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  int sign = 0;
  int scale = get_scale(value_1) + get_scale(value_2);
  set_scale(&value_1, 0);
  set_scale(&value_2, 0);
  result->bits[0] = 0;
  result->bits[1] = 0;
  result->bits[2] = 0;
  result->bits[3] = 0;
  if (get_sign(value_1) != get_sign(value_2)) {
    sign = 1;
  }
  set_sign(&value_1.bits[3], 0);
  set_sign(&value_2.bits[3], 0);
  s21_big_decimal val1 = from_decimal_to_big_decimal(value_1);
  s21_big_decimal val2 = from_decimal_to_big_decimal(value_2);
  s21_big_decimal res = from_decimal_to_big_decimal(*result);
  div_bigD(val1, val2, &res);
  res.scale = scale;
  res.sign = sign;
  error = from_big_decimal_to_decimal(res, result);
  set_sign(&result->bits[3], sign);
  return error;
}

int div_bigD(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result) {
  int error = 0;
  for (int i = 0; i < 192; i++) {
    s21_big_decimal tmp1 = {{1, 0, 0, 0, 0, 0}, 0, 0};
    s21_big_decimal tmp = value_2;
    shift_left_count_times_big_decimal(tmp, i, &tmp);
    if (is_greater_or_equal_bigD(tmp, value_1) == 2) {
      shift_left_count_times_big_decimal(tmp1, i, &tmp1);
      add_mantise_big_decimal(*result, tmp1, result);
      break;
    } else if (is_greater_or_equal_bigD(tmp, value_1) == 1) {
      // printf("value1 = %d tmp = %d\n",value_1.bits[0],tmp.bits[0]);
      shift_right_count_times_big_decimal(tmp, 1, &tmp);
      shift_left_count_times_big_decimal(tmp1, i - 1, &tmp1);
      add_mantise_big_decimal(*result, tmp1, result);
      sub_mantise_big_decimal(value_1, tmp, &value_1);
      if (i == 1) {
        i = 192;
      } else
        i = 0;
    }
  }
  return error;
}

int is_greater_or_equal_bigD(s21_big_decimal value_1, s21_big_decimal value_2) {
  int result_greater = 0;
  int result_equal = 0;
  int result = 0;
  int check = 0;
  for (int i = 5; i >= 0 && check == 0 && result_greater != 1; i--) {
    if (value_1.bits[i] > value_2.bits[i]) {
      result_greater = 1;
    } else if (value_1.bits[i] < value_2.bits[i]) {
      result_greater = 0;
      check = 1;
    }
  }
  if (result_greater == 0) {
    for (int i = 5; i >= 0; i--) {
      if (value_1.bits[i] == value_2.bits[i]) result_equal += 1;
    }
  }
  if (result_equal == 6) result = 2;
  if (result_greater) result = 1;
  return result;
}