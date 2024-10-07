#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (get_scale(value) > 28) {
    result->bits[0] = 0;
    result->bits[3] = 0;
    result->bits[2] = 0;
    result->bits[1] = 0;
    return 1;
  }
  set_sign(&result->bits[3], get_sign(value));
  int i = get_scale(value);
  if (i == 0) {
    for (int i = 0; i < 4; i++) {
      result->bits[i] = value.bits[i];
    }
  } else {
    for (int k = 0; k < i; k++) {
      decrease_exp(&value);
    }
    set_scale(&value, 0);
    for (int i = 0; i < 4; i++) {
      result->bits[i] = value.bits[i];
    }
  }
  return 0;
}