#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int status = 0;
  dst->bits[0] = 0;
  dst->bits[1] = 0;
  dst->bits[2] = 0;
  dst->bits[3] = 0;
  if (src >= 0) {
    dst->bits[0] = src;
  } else if (src == -2147483648) {
    dst->bits[0] = -src;
    set_bit(&dst->bits[3], 1, 31);
  } else {
    dst->bits[0] = -src;
    set_bit(&dst->bits[3], 1, 31);
  }
  return status;
}