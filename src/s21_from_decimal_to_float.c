#include "s21_decimal.h"

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int res = 1;
  if (dst) {
    double tmp = 0;
    int exp = 0;
    for (int i = 0; i < 96; i++) {
      if ((src.bits[i / 32] & (1 << i % 32)) != 0) tmp += pow(2, i);
    }
    if ((exp = get_scale(src)) > 0) {
      for (int i = exp; i > 0; i--, tmp /= 10.0)
        ;
    }
    *dst = (float)tmp;
    if (get_sign(src)) {
      *dst *= -1;
    }
    res = 0;
  }
  return res;
}