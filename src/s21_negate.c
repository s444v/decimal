#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int error = 0;
  for (int i = 0; i < 4; i++) result->bits[i] = value.bits[i];

  set_sign(&result->bits[3], !get_sign(value));
  return error;
}