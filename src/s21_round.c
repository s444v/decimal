#include "s21_decimal.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int res = 0;
  int prov = 0;
  s21_decimal plus3 = {.bits[0] = 1, .bits[1] = 0, .bits[2] = 0, .bits[3] = 0};
  int scale = get_scale(value);
  if (scale > 29) {
    res = 1;
    result->bits[0] = 0;
    result->bits[1] = 0;
    result->bits[2] = 0;
    result->bits[3] = 0;
  }
  if (scale == 0) *result = value;
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 &&
      res == 0 && scale != 0) {
    prov = 1;
    *result = value;
    set_scale(result, 0);
  }
  if (scale != 0 && res != 1 && prov == 0) {
    for (int i = 0; i < scale - 1; i++)
      decrease_exp(&value);  //Деление до последнего остатка
    if (mod10(value).bits[0] >= 5) {
      decrease_exp(&value);
      if (get_sign(value) == 1)
        s21_sub(value, plus3, result);
      else
        s21_add(value, plus3, result);
    } else {
      decrease_exp(&value);
      *result = value;
    }
  }
  return res;
}
