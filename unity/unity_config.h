#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

/* 
 * 1. 基本的な型サイズの設定 
 * 組み込み環境で int が 16bit の場合などはここを調整します。
 * デフォルト（32bit/64bit環境）なら未定義のままでも動作します。
 */
// #define UNITY_INT_WIDTH 32
// #define UNITY_LONG_WIDTH 32

/* 
 * 2. 浮動小数点のサポート 
 * float や double の比較（TEST_ASSERT_EQUAL_FLOAT など）を使う場合は定義します。
 */
//#define UNITY_INCLUDE_FLOAT
//#define UNITY_INCLUDE_DOUBLE

/* 
 * 3. 出力のカスタマイズ
 * 標準の putchar ではなく、独自の関数（UART出力など）を使いたい場合に定義します。
 */
// extern void MyCustom_PutChar(int c);
// #define UNITY_OUTPUT_CHAR(a) MyCustom_PutChar(a)

/* 
 * 4. 表示のカスタマイズ
 * 出力をカラーにしたい場合は、対応したターミナルで有効になります。
 */
#define UNITY_OUTPUT_COLOR

#endif /* UNITY_CONFIG_H */
