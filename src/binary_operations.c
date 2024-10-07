#include <math.h>
#include <stdio.h>

#include "s21_decimal.h"

// utils scale
// -----------------------------------------------------------------------------------------------
void set_scale(s21_decimal *value, int exp) {
  if (exp != 0) {
    value->bits[3] &= 2147483648;
    int mask = exp << 16;
    value->bits[3] |= mask;
  } else {
    int sign = get_sign(*value);
    value->bits[3] &= 0;
    set_sign(&value->bits[3], sign);
  }
}

int get_scale(s21_decimal value) {
  int scale = (value.bits[3] << 1) >> 17;

  return scale;
}

void set_scale_decimal(int num, s21_decimal *res) {
  int first_scale = get_scale(*res);  //сохранение первоначальной степени

  set_scale(res, num);

  if (first_scale < num) {
    for (int i = 0; i < num - first_scale; i++) *res = mul10(*res);
  }
}

// utils scale
// -----------------------------------------------------------------------------------------------

// utils sign
// -----------------------------------------------------------------------------------------------

void set_sign(unsigned int *value, int sign) { set_bit(value, sign, 31); }

int get_sign(s21_decimal value) {
  int index = 31;
  index = 1 << index;
  if ((value.bits[3] & index) > 0)
    index = 1;
  else
    index = 0;
  return index;
}

// utils sign
// -----------------------------------------------------------------------------------------------

// shifts utils
// -----------------------------------------------------------------------------------------------

int shift_right(s21_decimal value, int count, s21_decimal *result) {
  int error = 0;
  while (count != 0) {
    int last_bit = 0;
    int last_bit_prev = 0;
    for (int i = 2; i >= 0; i--) {
      last_bit = get_decimal_bit(value, i * 32);
      value.bits[i] = value.bits[i] >> 1;
      set_decimal_bit(&value, last_bit_prev, (i + 1) * 32 - 1);
      last_bit_prev = last_bit;
    }
    count--;
  }
  assignment(value, result);
  // error = last_bit_prev;
  return error;
}

int shift_left(s21_decimal value, int count, s21_decimal *result) {
  int error = 0;
  while (count != 0) {
    int last_bit = 0;
    int last_bit_prev = 0;
    for (int i = 0; i < 3; i++) {
      last_bit = get_decimal_bit(value, (i + 1) * 32 - 1);
      value.bits[i] = value.bits[i] << 1;
      set_decimal_bit(&value, last_bit_prev, i * 32);
      last_bit_prev = last_bit;
    }
    count--;
  }
  assignment(value, result);
  // error = last_bit_prev;
  return error;
}

// shifts utils
// -----------------------------------------------------------------------------------------------

// bit utils
// ---------------------------------------------------------------------------------------------------

int get_decimal_bit(s21_decimal value, unsigned int index) {
  int count = index / 32;

  return get_bit(value.bits[count], index - count * 32);
}

void set_decimal_bit(s21_decimal *value, int set_value, unsigned int index) {
  int count = index / 32;

  set_bit(&(value->bits[count]), set_value, index - count * 32);
}

int get_bit(unsigned int value, int index) {
  index = 1 << index;
  if ((value & index) > 0)
    index = 1;
  else
    index = 0;
  return index;
}

void set_bit(unsigned int *value, int set_value, int index) {
  if (set_value) {
    index = 1 << index;
    *value = *value | index;
  } else {
    index = 1 << index;
    index = ~index;
    *value = *value & index;
  }
}
// bit utils
// ---------------------------------------------------------------------------------------------------

// mod:mul:div
// --------------------------------------------------------------------------------------------------

void decrease_exp(s21_decimal *value) {
  int sign = 0;
  if (get_sign(*value)) {
    set_sign(&value->bits[3], 0);
    sign = 1;
  }
  s21_decimal tmp1, tmp2, tmp3;
  s21_decimal tmp4 = {{6, 0, 0, 0}};
  set_scale(&tmp4, get_scale(*value));
  shift_right(*value, 1, &tmp1);
  shift_right(*value, 2, &tmp2);
  s21_add(tmp1, tmp2, &tmp1);
  shift_right(tmp1, 4, &tmp2);
  s21_add(tmp1, tmp2, &tmp1);
  shift_right(tmp1, 8, &tmp2);
  s21_add(tmp1, tmp2, &tmp1);
  shift_right(tmp1, 16, &tmp2);
  s21_add(tmp1, tmp2, &tmp1);
  shift_right(tmp1, 32, &tmp2);
  s21_add(tmp1, tmp2, &tmp1);
  shift_right(tmp1, 64, &tmp2);
  s21_add(tmp1, tmp2, &tmp1);
  shift_right(tmp1, 3, &tmp1);
  shift_left(tmp1, 3, &tmp2);
  shift_left(tmp1, 1, &tmp3);
  s21_add(tmp2, tmp3, &tmp2);
  s21_sub(*value, tmp2, &tmp2);  // q = tmp1 r = tmp2
  s21_add(tmp2, tmp4, &tmp2);
  shift_right(tmp2, 4, &tmp2);
  s21_add(tmp1, tmp2, value);
  int exp = get_scale(*value);
  exp -= 1;
  set_scale(value, exp);
  set_sign(&value->bits[3], sign);
}

s21_decimal mul10(s21_decimal value) {
  s21_decimal result;
  assignment(value, &result);
  shift_left(value, 3, &value);
  shift_left(result, 1, &result);
  s21_add(result, value, &result);
  return result;
}

s21_decimal mod10(s21_decimal value) {
  int sign = get_sign(value);
  set_sign(&value.bits[3], 0);
  s21_decimal result;
  assignment(value, &result);
  set_scale(&result, get_scale(value) + 1);
  decrease_exp(&result);
  result = mul10(result);
  s21_sub(value, result, &result);
  set_sign(&result.bits[3], sign);
  return result;
}
// mod:mul:div
// --------------------------------------------------------------------------------------------------

// print decimal
// ------------------------------------------------------------------------------------------------

void set_array_from_dec(s21_decimal dec, int *result) {
  int fract[96] = {-1};
  for (int i = 0; i < 96; i++) fract[i] = get_bit(dec.bits[i / 32], i % 32);
  int s[29] = {0};
  s[0] = 1;
  for (int i = 0; i < 96; i++) {
    if (fract[i]) {
      int vu = 0;
      for (int j = 0; j < 29; j++) {
        result[j] += s[j];
        result[j] += vu;
        if (result[j] > 9) {
          vu = 1;
          result[j] -= 10;
        } else {
          vu = 0;
        }
      }
    }
    int vu = 0;
    for (int j = 0; j < 29; j++) {
      s[j] *= 2;
      s[j] += vu;
      if (s[j] > 9) {
        vu = 1;
        s[j] -= 10;
      } else {
        vu = 0;
      }
    }
  }
}

void s21_dec_to_string(s21_decimal dec, char *str) {
  char *c = str;
  if (get_sign(dec)) {
    *c = '-';
    c++;
  }
  int ex = (int)get_scale(dec);
  int array_of_int[29] = {0};
  set_array_from_dec(dec, array_of_int);
  int flag = 0;
  for (int j = 28; j >= 0; j--) {
    if (flag || array_of_int[j] || j == ex) {
      *c = (char)(array_of_int[j]) + 48;
      c++;
      flag = 1;
      if (j == ex && j) {
        *c = '.';
        c++;
      }
    }
  }
  if (*str == '-' && c == str + 1 && *c == '0') {
    *str = 0;
    c = str + 1;
  }
  *c = '\0';
}

void print_decimal(s21_decimal dec) {
  char output_str[31];
  s21_dec_to_string(dec, output_str);
  printf("%s", output_str);
}

void printf_decimal(char *title, s21_decimal value) {
  printf("%s = %u, %u, %u, %u \n", title, value.bits[2], value.bits[1],
         value.bits[0], value.bits[3]);
}

// print decimal
// ------------------------------------------------------------------------------------------------

// big_decimal
// --------------------------------------------------------------------------------------------------

void div10(s21_big_decimal *value) {
  int sign = 0;
  if (value->sign) {
    sign = 1;
    value->sign = 0;
  }
  s21_big_decimal tmp1, tmp2, tmp3;
  s21_big_decimal tmp4 = {{6, 0, 0, 0, 0, 0}, value->scale, 0};
  shift_right_count_times_big_decimal(*value, 1, &tmp1);
  shift_right_count_times_big_decimal(*value, 2, &tmp2);
  add_mantise_big_decimal(tmp1, tmp2, &tmp1);
  // q = (n >> 1) + (n >> 2);
  shift_right_count_times_big_decimal(tmp1, 4, &tmp2);
  add_mantise_big_decimal(tmp1, tmp2, &tmp1);
  //  q = q + (q >> 4);
  shift_right_count_times_big_decimal(tmp1, 8, &tmp2);
  add_mantise_big_decimal(tmp1, tmp2, &tmp1);
  shift_right_count_times_big_decimal(tmp1, 16, &tmp2);
  add_mantise_big_decimal(tmp1, tmp2, &tmp1);
  shift_right_count_times_big_decimal(tmp1, 32, &tmp2);
  add_mantise_big_decimal(tmp1, tmp2, &tmp1);
  shift_right_count_times_big_decimal(tmp1, 64, &tmp2);
  add_mantise_big_decimal(tmp1, tmp2, &tmp1);
  shift_right_count_times_big_decimal(tmp1, 128, &tmp2);
  add_mantise_big_decimal(tmp1, tmp2, &tmp1);
  // q = q + (q >> 16);
  shift_right_count_times_big_decimal(tmp1, 3, &tmp1);
  // q = q >> 3;
  shift_left_count_times_big_decimal(tmp1, 3, &tmp2);
  shift_left_count_times_big_decimal(tmp1, 1, &tmp3);
  add_mantise_big_decimal(tmp2, tmp3, &tmp2);
  // q * 10
  sub_mantise_big_decimal(*value, tmp2, &tmp2);
  // n - q * 10;
  add_mantise_big_decimal(tmp2, tmp4, &tmp2);
  // (r + 6)
  shift_right_count_times_big_decimal(tmp2, 4, &tmp2);
  //(r + 6) >> 4;
  add_mantise_big_decimal(tmp1, tmp2, value);
  // q + ((r + 6) >> 4)
  value->sign = sign;
}

s21_big_decimal big_decimal_mod10(s21_big_decimal value) {
  int sign = value.sign;
  value.sign = 0;
  s21_big_decimal result = value;
  for (unsigned int i = 0; i < value.scale + 1; i++) {
    div10(&result);
    result.scale = value.scale + 1;
  }
  for (unsigned int i = 0; i < value.scale + 1; i++) {
    increase_scale_big_decimal(&result);
  }
  sub_mantise_big_decimal(value, result, &result);
  set_sign(&result.bits[3], sign);
  return result;
}

void printf_big_decimal(char *title, s21_big_decimal value) {
  printf("%s = %X, %X, %X, %X, %X, %X, %u, %u\n", title, value.bits[5],
         value.bits[4], value.bits[3], value.bits[2], value.bits[1],
         value.bits[0], value.scale, value.sign);
}

void decrease_scale_big_decimal(s21_big_decimal *value) {
  s21_big_decimal check = big_decimal_mod10(*value);
  s21_big_decimal tmp = {{5, 0, 0, 0, 0, 0}, value->scale, 0};
  if (value->scale == 1 && check.bits[0] % 10 == 5 &&
      (check.bits[0] / 10) % 2 == 1) {
    add_mantise_big_decimal(*value, tmp, value);
  }
  div10(value);
  value->scale -= 1;
  tmp.scale -= 1;
  tmp.bits[0] = tmp.bits[0] - 4;
  if ((!value->bits[5] && !value->bits[4] && !value->bits[3]) &&
      check.bits[0] % (int)pow(10, check.scale) >
          5 * (int)pow(10, check.scale - 1)) {
    add_mantise_big_decimal(*value, tmp, value);
  }
}

int from_big_decimal_to_decimal(s21_big_decimal value, s21_decimal *result) {
  int err = 0;
  while ((value.bits[5] || value.bits[4] || value.bits[3]) ||
         value.scale > 28) {
    if (value.scale > 0) {
      decrease_scale_big_decimal(&value);
    } else {
      if (value.sign) {
        err = 2;
      } else {
        err = 1;
      }
      break;
    }
  }

  if (err == 0) {
    result->bits[0] = value.bits[0];
    result->bits[1] = value.bits[1];
    result->bits[2] = value.bits[2];
    set_bit(&result->bits[3], value.sign, 31);
    set_scale(result, value.scale);
  }

  return err;
}

int shift_left_big_decimal(s21_big_decimal *value) {
  int error = 0;
  int last_bit = 0;
  int last_bit_prev = 0;
  for (int i = 0; i < 6; i++) {
    last_bit = get_bit(value->bits[i], 31);
    value->bits[i] = value->bits[i] << 1;
    set_bit(&value->bits[i], last_bit_prev, 0);
    last_bit_prev = last_bit;
  }
  error = last_bit_prev;
  return error;
}

void shift_right_count_times_big_decimal(s21_big_decimal value, int count,
                                         s21_big_decimal *result) {
  for (int i = 0; i < count; i++) {
    shift_right_big_decimal(&value);
  }
  *result = value;
}

void shift_left_count_times_big_decimal(s21_big_decimal value, int count,
                                        s21_big_decimal *result) {
  for (int i = 0; i < count; i++) {
    shift_left_big_decimal(&value);
  }
  *result = value;
}

int shift_right_big_decimal(s21_big_decimal *value) {
  int error = 0;
  int first_bit = 0;
  int first_bit_prev = 0;
  for (int i = 5; i > -1; i--) {
    first_bit = get_bit(value->bits[i], 0);
    value->bits[i] = value->bits[i] >> 1;
    set_bit(&value->bits[i], first_bit_prev, 31);
    first_bit_prev = first_bit;
  }
  error = first_bit_prev;
  return error;
}

void add_mantise_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                             s21_big_decimal *result) {
  int memory = 0;
  if (value_1.sign == value_2.sign) {
    for (unsigned int i = 0; i < 192; i++) {
      int index = i / 32;
      int bit_1 = get_bit(value_1.bits[index], i % 32);
      int bit_2 = get_bit(value_2.bits[index], i % 32);
      int sum = bit_1 + bit_2 + memory;
      set_bit(&result->bits[index], sum % 2, i % 32);
      memory = sum / 2;
    }
  }
  result->scale = value_1.scale;
}

void sub_mantise_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                             s21_big_decimal *result) {
  int j = 0;
  if (value_1.sign == value_2.sign) {
    for (int i = 192; i >= 0; i--) {
      unsigned int index = i / 32;
      int bit_1 = get_bit(value_1.bits[index], i % 32);
      int bit_2 = get_bit(value_2.bits[index], i % 32);
      if (bit_1) {
        j = i;
      }
      if (bit_1 == 1 && bit_2 == 1) {
        set_bit(&result->bits[index], 0, i % 32);
      } else if (bit_1 == 1 && bit_2 == 0) {
        set_bit(&result->bits[index], 1, i % 32);
      } else if (bit_1 == 0 && bit_2 == 1) {
        while (get_bit(result->bits[j / 32], j % 32) == 0 && (j < 192)) {
          j++;
        }
        set_bit(&result->bits[j / 32], 0, j % 32);
        for (int p = j - 1; p >= i; p--) {
          set_bit(&result->bits[p / 32], 1, p % 32);
        }
        j = i;
      }
      //   else {
      //     set_bit(&result->bits[index], 0, i % 32);
      //   }
    }
  }
  result->scale = value_1.scale;
}

void increase_scale_big_decimal(s21_big_decimal *value) {
  s21_big_decimal plus1 = {{value->bits[0], value->bits[1], value->bits[2],
                            value->bits[3], value->bits[4], value->bits[5]},
                           value->scale,
                           value->sign};
  s21_big_decimal plus2 = {{value->bits[0], value->bits[1], value->bits[2],
                            value->bits[3], value->bits[4], value->bits[5]},
                           value->scale,
                           value->sign};

  for (int i = 0; i < 3; i++) {  // 2^3
    shift_left_big_decimal(&plus1);
  }

  shift_left_big_decimal(&plus2);  // 2

  add_mantise_big_decimal(plus1, plus2, value);

  value->scale += 1;
}

void set_biggest_scale_big_decimal(s21_big_decimal *value_1,
                                   s21_big_decimal *value_2) {
  int diff_scale = value_1->scale > value_2->scale
                       ? value_1->scale - value_2->scale
                       : value_2->scale - value_1->scale;

  s21_big_decimal *smallest_scale =
      value_1->scale > value_2->scale ? value_2 : value_1;
  for (int i = 0; i < diff_scale; i++) {
    increase_scale_big_decimal(smallest_scale);
  }
}

int big_decimal_is_greater(s21_big_decimal value_1, s21_big_decimal value_2) {
  int res = 0;

  set_biggest_scale_big_decimal(&value_1, &value_2);

  if (value_1.sign == value_2.sign) {
    for (int i = 5; i > -1; i--) {
      if ((value_1.bits[i] < value_2.bits[i] && value_1.sign == 1) ||
          (value_1.bits[i] > value_2.bits[i] && value_1.sign == 0)) {
        res = 1;
        break;
      }
    }
  } else if (value_1.sign < value_2.sign) {
    res = 1;
  }

  return res;
}

int big_decimal_is_equal_abs(s21_big_decimal value_1, s21_big_decimal value_2) {
  int res = 0;

  set_biggest_scale_big_decimal(&value_1, &value_2);

  if (value_1.bits[0] == value_2.bits[0] &&
      value_1.bits[1] == value_2.bits[1] &&
      value_1.bits[2] == value_2.bits[2] &&
      value_1.bits[3] == value_2.bits[3] &&
      value_1.bits[4] == value_2.bits[4] &&
      value_1.bits[5] == value_2.bits[5]) {
    res = 1;
  }

  return res;
}

s21_big_decimal from_decimal_to_big_decimal(s21_decimal value) {
  s21_big_decimal value_big = {.bits[0] = value.bits[0],
                               .bits[1] = value.bits[1],
                               .bits[2] = value.bits[2],
                               .bits[3] = 0,
                               .bits[4] = 0,
                               .bits[5] = 0};

  value_big.sign = get_sign(value);
  value_big.scale = get_scale(value);

  return value_big;
}

int big_decimal_is_greater_abs(s21_big_decimal value_1,
                               s21_big_decimal value_2) {
  value_1.sign = 0;
  value_2.sign = 0;
  return big_decimal_is_greater(value_1, value_2);
}

// big_decimal
// --------------------------------------------------------------------------------------------------

void assignment(s21_decimal value, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value.bits[i];
  }
}