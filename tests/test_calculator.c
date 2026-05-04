/**
 * @file  test_calculator.c
 * @brief calculator.c の単体テスト（Unity使用）
 *
 * テスト観点：
 *   - 正常系：各四則演算の正しい計算結果
 *   - 境界値：0・負の数・大きな数
 *   - 異常系：ゼロ除算のエラーコード確認
 */
#include "../unity/unity.h"
#include "../src/calculator.h"
#include<limits.h>

/* ============================================================
 * setUp / tearDown
 * Unity の慣習。各テスト前後に呼ばれる。
 * 今回は初期化不要なため空実装。
 * ============================================================ */
void setUp(void)    {}
void tearDown(void) {}

/* ============================================================
 * calc_add のテスト
 * ============================================================ */
void test_add_positive_numbers(void)
{
    TEST_ASSERT_EQUAL_INT(13, calc_add(10, 3));
}

void test_add_with_zero(void)
{
    TEST_ASSERT_EQUAL_INT(5, calc_add(5, 0));
    TEST_ASSERT_EQUAL_INT(5, calc_add(0, 5));
}
void test_add_negative_numbers(void)
{
    TEST_ASSERT_EQUAL_INT(-3, calc_add(-10, 7));
    TEST_ASSERT_EQUAL_INT(-13, calc_add(-10, -3));
}

void test_add_large_numbers(void)
{
  TEST_ASSERT_EQUAL_INT(3000, calc_add(1000, 2000));
}

void test_add_overflow(void)
{
  int result = calc_add(INT_MAX, INT_MAX);
  printf("INT_MAX+INT_MAX= %d\n", result);
  TEST_PASS();
}
/* ============================================================
 * calc_subtract のテスト
 * ============================================================ */
void test_subtract_positive_numbers(void)
{
    TEST_ASSERT_EQUAL_INT(7, calc_subtract(10, 3));
}

void test_subtract_result_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, calc_subtract(5, 5));
}

void test_subtract_result_negative(void)
{
    TEST_ASSERT_EQUAL_INT(-3, calc_subtract(0, 3));
}

void test_subtract_negative_from_negative(void)
{
  TEST_ASSERT_EQUAL_INT(-2, calc_subtract(-5, -3));
}

/* ============================================================
 * calc_multiply のテスト
 * ============================================================ */
void test_multiply_positive_numbers(void)
{
    TEST_ASSERT_EQUAL_INT(30, calc_multiply(10, 3));
}

void test_multiply_by_zero(void)
{
    TEST_ASSERT_EQUAL_INT(0, calc_multiply(10, 0));
    TEST_ASSERT_EQUAL_INT(0, calc_multiply(0, 10));
}

void test_multiply_negative_numbers(void)
{
    TEST_ASSERT_EQUAL_INT(-30, calc_multiply(-10, 3));
    TEST_ASSERT_EQUAL_INT(30,  calc_multiply(-10, -3));
}

void test_multiply_large_numbers(void)
{
  TEST_ASSERT_EQUAL_INT(10000, calc_multiply(100, 100));
}

/* ============================================================
 * calc_divide のテスト
 * ============================================================ */
void test_divide_normal(void)
{
    int result = 0;
    int ret = calc_divide(10, 3, &result);
    TEST_ASSERT_EQUAL_INT(CALC_OK, ret);
    TEST_ASSERT_EQUAL_INT(3, result);   /* 整数除算：切り捨て */
}

void test_divide_exact(void)
{
    int result = 0;
    int ret = calc_divide(10, 2, &result);
    TEST_ASSERT_EQUAL_INT(CALC_OK, ret);
    TEST_ASSERT_EQUAL_INT(5, result);
}

void test_divide_by_zero_returns_error(void)
{
    int result = 0;
    int ret = calc_divide(10, 0, &result);
    TEST_ASSERT_EQUAL_INT(CALC_ERR_DIV_ZERO, ret);
}

void test_divide_result_null_pointer(void)
{
    /* result に NULL を渡してもクラッシュしないことを確認 */
    int ret = calc_divide(10, 2, NULL);
    TEST_ASSERT_EQUAL_INT(CALC_OK, ret);
}

void test_divide_zero_by_nonzero(void)
{
    int result = 0;
    int ret = calc_divide(0, 5, &result);
    TEST_ASSERT_EQUAL_INT(CALC_OK, ret);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_divide_negative_dividend(void)
{
  int result = 0;
  int ret = calc_divide(-10, 3, &result);
  TEST_ASSERT_EQUAL_INT(CALC_OK, ret);
  TEST_ASSERT_EQUAL_INT(-3, result);
}

void test_divide_by_minus(void)
{
  int result = 0;
  int ret = calc_divide(-10, -1, &result);
  TEST_ASSERT_EQUAL_INT(CALC_OK, ret);
  TEST_ASSERT_EQUAL_INT(10, result);
}

void test_divide_by_zero(void)
{
  int result = 0;
  int ret = calc_divide(0, 0, &result);
  TEST_ASSERT_EQUAL_INT(CALC_ERR_DIV_ZERO, ret);
}

/* ============================================================
 * calc_modulo のテスト
 * ============================================================ */

void test_modulo_normal(void)
{
  int result = 0;
  int ret = calc_modulo(8, 3, &result);
  TEST_ASSERT_EQUAL_INT(CALC_OK, ret);
  TEST_ASSERT_EQUAL_INT(2, result);
}

/* ============================================================
 * テストランナー（main）
 * ============================================================ */
int main(void)
{
    UnityBegin("test_calculator.c");

    /* calc_add */
    RUN_TEST(test_add_positive_numbers);
    RUN_TEST(test_add_with_zero);
    RUN_TEST(test_add_negative_numbers);
    RUN_TEST(test_add_large_numbers);
    RUN_TEST(test_add_overflow);

    /* calc_subtract */
    RUN_TEST(test_subtract_positive_numbers);
    RUN_TEST(test_subtract_result_zero);
    RUN_TEST(test_subtract_result_negative);
    RUN_TEST(test_subtract_negative_from_negative);

    /* calc_multiply */
    RUN_TEST(test_multiply_positive_numbers);
    RUN_TEST(test_multiply_by_zero);
    RUN_TEST(test_multiply_negative_numbers);
    RUN_TEST(test_multiply_large_numbers);

    /* calc_divide */
    RUN_TEST(test_divide_normal);
    RUN_TEST(test_divide_exact);
    RUN_TEST(test_divide_by_zero_returns_error);
    RUN_TEST(test_divide_result_null_pointer);
    RUN_TEST(test_divide_zero_by_nonzero);
    RUN_TEST(test_divide_negative_dividend);
    RUN_TEST(test_divide_by_minus);
    RUN_TEST(test_divide_by_zero);

    /* calc_modulo */
    RUN_TEST(test_modulo_normal);

    return UnityEnd();
}
