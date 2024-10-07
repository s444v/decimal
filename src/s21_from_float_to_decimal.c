#include <stdio.h>
#include <string.h>

#include "s21_decimal.h"

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int res = 0;
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
  if (src < 0) {
    src *= -1;
    set_bit(&dst->bits[3], 1, 31);
  }
  if (src == floorf(src)) {
    res = s21_from_int_to_decimal((int)src, dst);
  } else {
    unsigned int k = 0;
    int scale = 0;
    char str[30];
    int dot = 0;
    int r = 7;
    sprintf(str, "%f", src);
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
      if (str[i] == '.') dot = i;
      if (str[i] != '.' && r > 0) {
        k += (str[i] - 48) * pow(10, r);
        r--;
      }
    }
    scale = 8 - dot;
    r = scale;
    for (int i = 0; i <= scale; i++) {
      if (k % 10 == 0) {
        r--;
        k /= 10;
      }
    }
    scale = r;
    set_scale(dst, scale);
    dst->bits[0] = k;
  }
  return res;
}