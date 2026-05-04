/**
 * @file  calculator.c
 * @brief 整数四則演算ライブラリ 実装
 */
#include <stddef.h>
#include "calculator.h"

int calc_add(int a, int b)
{
    return a + b;
}

int calc_subtract(int a, int b)
{
    return a - b;
}

int calc_multiply(int a, int b)
{
    return a * b;
}

int calc_divide(int a, int b, int *result)
{
    if (b == 0) {
        return CALC_ERR_DIV_ZERO;
    }
    if (result != NULL) {
        *result = a / b;
    }
    return CALC_OK;
}

/* b が 0 のとき CALC_ERR_DIV_ZERO を返す */
int calc_modulo(int a, int b, int *result)
{
  if ( 0 == b) {
    return CALC_ERR_DIV_ZERO;
  }
  if (NULL != result) {
    *result = a % b;
  }
  return CALC_OK;
}


