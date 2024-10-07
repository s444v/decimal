#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int sign_2 = get_sign(value_2);
  set_sign(&value_2.bits[3], !sign_2);

  int error = s21_add(value_1, value_2, result);
  return error;
}
