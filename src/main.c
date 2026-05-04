/**
 * @file  main.c
 * @brief 動作確認用エントリーポイント
 */
#include <stdio.h>
#include "calculator.h"

int main(void)
{
    int result = 0;
    int ret;

    printf("=== Calculator Demo ===\n");
    printf("10 + 3 = %d\n", calc_add(10, 3));
    printf("10 - 3 = %d\n", calc_subtract(10, 3));
    printf("10 * 3 = %d\n", calc_multiply(10, 3));

    ret = calc_divide(10, 3, &result);
    if (ret == CALC_OK) {
        printf("10 / 3 = %d\n", result);
    }

    ret = calc_divide(10, 0, &result);
    if (ret == CALC_ERR_DIV_ZERO) {
        printf("10 / 0 = エラー（ゼロ除算）\n");
    }

    calc_modulo(10, 3, &result);
    printf("10 modulo 3 = %d\n", result);

    return 0;
}
