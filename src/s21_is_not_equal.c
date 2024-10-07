#include "s21_decimal.h"

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  int res = 0;
  int prov = 0;

  if (a.bits[0] == 0 && a.bits[1] == 0 && a.bits[2] == 0 && b.bits[0] == 0 &&
      b.bits[1] == 0 && b.bits[2] == 0) {
    res = 0;
    prov = 1;
  }

  //Приведение к общему скейлу
  if (get_scale(a) > get_scale(b) && prov != 1)
    set_scale_decimal(get_scale(a), &b);
  if (get_scale(a) < get_scale(b) && prov != 1)
    set_scale_decimal(get_scale(b), &a);

  if ((a.bits[3] != b.bits[3] || a.bits[2] != b.bits[2] ||
       a.bits[1] != b.bits[1] || a.bits[0] != b.bits[0]) &&
      prov != 1)
    res = 1;

  return res;
}
