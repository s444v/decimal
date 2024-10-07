#include "s21_decimal.h"

int s21_is_less(s21_decimal a, s21_decimal b) {
  int res = 0;
  int prov = 0;

  //Если оба числа 0
  if (a.bits[0] == 0 && a.bits[1] == 0 && a.bits[2] == 0 && b.bits[0] == 0 &&
      b.bits[1] == 0 && b.bits[2] == 0) {
    res = 0;
    prov = 1;
  }

  if ((get_bit(a.bits[3], 31) > get_bit(b.bits[3], 31)) && prov != 1) res = 1;
  if ((get_bit(a.bits[3], 31) < get_bit(b.bits[3], 31)) && prov != 1) {
    res = 0;
    prov = 1;
  }

  //Приведение к общему скейлу
  if (get_scale(a) > get_scale(b) && res != 1 && prov != 1)
    set_scale_decimal(get_scale(a), &b);
  if (get_scale(a) < get_scale(b) && res != 1 && prov != 1)
    set_scale_decimal(get_scale(b), &a);

  if (get_bit(a.bits[3], 31) == 1 && get_bit(b.bits[3], 31) == 1) {
    for (int i = 2; i >= 0 && prov == 0 && res != 1; i--) {
      if (a.bits[i] > b.bits[i]) res = 1;
      if (a.bits[i] < b.bits[i]) {
        res = 0;
        prov = 1;
      }
    }
  }

  if (get_bit(a.bits[3], 31) == 0 && get_bit(b.bits[3], 31) == 0) {
    for (int i = 2; i >= 0 && prov == 0 && res != 1; i--) {
      if (a.bits[i] < b.bits[i]) res = 1;
      if (a.bits[i] > b.bits[i]) {
        res = 0;
        prov = 1;
      }
    }
  }
  return res;
}
