/**
 * @file    calculator.h
 * @brief   整数四則演算ライブラリ
 *
 * C言語演習：ビルド・単体テスト・静的解析・CI の題材として使用する。
 */
#ifndef CALCULATOR_H
#define CALCULATOR_H

/* エラーコード */
#define CALC_OK            0
#define CALC_ERR_DIV_ZERO -1

/**
 * @brief 加算
 * @param a 被加数
 * @param b 加数
 * @return  a + b
 */
int calc_add(int a, int b);

/**
 * @brief 減算
 * @param a 被減数
 * @param b 減数
 * @return  a - b
 */
int calc_subtract(int a, int b);

/**
 * @brief 乗算
 * @param a 被乗数
 * @param b 乗数
 * @return  a * b
 */
int calc_multiply(int a, int b);

/**
 * @brief 除算
 * @param a       被除数
 * @param b       除数
 * @param result  演算結果の格納先（NULLの場合は無視）
 * @return  CALC_OK(0) 正常 / CALC_ERR_DIV_ZERO(-1) ゼロ除算エラー
 */
int calc_divide(int a, int b, int *result);

#endif /* CALCULATOR_H */
