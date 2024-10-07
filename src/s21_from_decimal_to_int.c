#include "s21_decimal.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = 1;
  if (get_scale(src) > 28) {
    *dst = 0;
    res = 1;
  } else {
    while (get_scale(src) != 0) {
      decrease_exp(&src);
    }
    if ((src.bits[1] == 0 && src.bits[2] == 0) &&
        ((src.bits[0] <= 2147483647 && !get_sign(src)) ||
         (src.bits[0] <= 2147483648 && get_sign(src)))) {
      *dst = src.bits[0];
      *dst /= (int)pow(10, get_scale(src));
      if (get_sign(src)) {
        *dst *= -1;
      }
      res = 0;
    }
  }
  return res;
}