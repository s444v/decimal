#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int res = 0;
  int prov = 0;
  int zero = 0;
  *result = value;
  int scale1 = get_scale(*result);
  if (scale1 > 29) {
    res = 1;
    result->bits[0] = 0;
    result->bits[1] = 0;
    result->bits[2] = 0;
    result->bits[3] = 0;
  }
  if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0 &&
      res == 0) {
    prov = 1;
    set_scale(result, 0);
  }
  if (get_sign(*result) == 0 && scale1 != 0 && prov == 0 &&
      res == 0) {  //Если число положительное
    for (int i = 0; i < scale1; i++) {
      decrease_exp(result);
    }
  }
  if (get_sign(*result) == 1 && scale1 != 0 && prov == 0 &&
      res == 0) {  //Если число отрицательное
    s21_decimal plus1 = {
        .bits[0] = 0, .bits[1] = 0, .bits[2] = 0, .bits[3] = 0};
    s21_decimal plus2 = {
        .bits[0] = 1, .bits[1] = 0, .bits[2] = 0, .bits[3] = 0};
    int scale = get_scale(*result);
    for (int i = 0; i < scale; i++) {
      plus1 = *result;
      if (mod10(plus1).bits[0] == 0) zero++;
      decrease_exp(result);
    }
    if (zero != scale1) {
      plus1 = *result;
      s21_sub(plus1, plus2, result);  //-1
    }
  }
  return res;
}
