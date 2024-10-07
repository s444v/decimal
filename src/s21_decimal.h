#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <math.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[6];
  unsigned int scale;
  unsigned int sign;
} s21_big_decimal;

// compr
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);

// math
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// conver
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// bonus
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// utils for big decimal
void printf_big_decimal(char *title, s21_big_decimal value);
void decrease_scale_big_decimal(s21_big_decimal *value);
int from_big_decimal_to_decimal(s21_big_decimal value, s21_decimal *result);
int shift_left_big_decimal(s21_big_decimal *value);
int shift_right_big_decimal(s21_big_decimal *value);
void shift_left_count_times_big_decimal(s21_big_decimal value, int count,
                                        s21_big_decimal *result);
void shift_right_count_times_big_decimal(s21_big_decimal value, int count,
                                         s21_big_decimal *result);
void add_mantise_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                             s21_big_decimal *result);
void sub_mantise_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                             s21_big_decimal *result);
void increase_scale_big_decimal(s21_big_decimal *value);
void set_biggest_scale_big_decimal(s21_big_decimal *value_1,
                                   s21_big_decimal *value_2);
int big_decimal_is_greater(s21_big_decimal value_1, s21_big_decimal value_2);
int big_decimal_is_greater_abs(s21_big_decimal value_1,
                               s21_big_decimal value_2);
int big_decimal_is_equal_abs(s21_big_decimal value_1, s21_big_decimal value_2);
s21_big_decimal from_decimal_to_big_decimal(s21_decimal value);
s21_big_decimal big_decimal_mod10(s21_big_decimal value);
int mul_bigD(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result);
int div_bigD(s21_big_decimal value_1, s21_big_decimal value_2,
             s21_big_decimal *result);
int is_greater_or_equal_bigD(s21_big_decimal value_1, s21_big_decimal value_2);
// utils for work with decimal

// utils for scale
void set_scale(s21_decimal *value, int exp);
int get_scale(s21_decimal value);
void set_scale_decimal(int num, s21_decimal *res);

// utils for bits
int get_bit(unsigned int value, int index);
void set_bit(unsigned int *value, int set_value, int index);
void set_decimal_bit(s21_decimal *value, int set_value, unsigned int index);
int get_decimal_bit(s21_decimal value, unsigned int index);

// utils for sign
int get_sign(s21_decimal value);
void set_sign(unsigned int *value, int sign);

// utils shift
int shift_left(s21_decimal value, int count, s21_decimal *result);
int shift_right(s21_decimal value, int count, s21_decimal *result);

// utils mod;mul;div
void decrease_exp(s21_decimal *value);
s21_decimal mod10(s21_decimal value);
s21_decimal mul10(s21_decimal value);
void div10(s21_big_decimal *value);

// utils output
void output_dec(s21_decimal value);
void print_decimal(s21_decimal dec);
void s21_dec_to_string(s21_decimal dec, char *str);
void set_array_from_dec(s21_decimal dec, int *result);
void printf_decimal(char *title, s21_decimal value);

// others
void assignment(s21_decimal value, s21_decimal *result);

#endif
