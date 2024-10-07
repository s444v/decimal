#include "s21_decimal.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
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
  mul_bigD(val1, val2, &res);
  res.scale = scale;
  res.sign = sign;
  error = from_big_decimal_to_decimal(res, result);
  set_sign(&result->bits[3], sign);
  return error;
}

int mul_bigD(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result) {
  int error = 0;
  for (int i = 0; i < 192; i++) {
    s21_big_decimal tmp = value_2;
    int index = i / 32;
    if (get_bit(value_1.bits[index], i % 32)) {
      shift_left_count_times_big_decimal(tmp, i, &tmp);
      add_mantise_big_decimal(*result, tmp, result);
    }
  }
  return error;
}