/* unity.h - Unity Test Framework (minimal compatible implementation)
 * Based on Unity by ThrowTheSwitch.org (MIT License)
 * https://github.com/ThrowTheSwitch/Unity
 */
#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- 内部カウンタ --- */
extern int Unity_TestCount;
extern int Unity_FailCount;
extern const char *Unity_CurrentTestName;
extern const char *Unity_CurrentFile;
extern int         Unity_CurrentLine;

/* --- テストランナー --- */
void UnityBegin(const char *filename);
int  UnityEnd(void);
void UnityFail(const char *msg);

/* --- RUN_TEST マクロ --- */
#define RUN_TEST(func) do {                         \
    Unity_CurrentTestName = #func;                  \
    Unity_TestCount++;                               \
    func();                                          \
} while(0)

/* --- アサーションマクロ --- */
#define TEST_ASSERT_TRUE(cond) do {                                         \
    if (!(cond)) {                                                          \
        printf("FAIL: %s:%d: %s - Expected TRUE\n",                        \
               __FILE__, __LINE__, Unity_CurrentTestName);                  \
        Unity_FailCount++;                                                   \
        return;                                                              \
    }                                                                       \
} while(0)

#define TEST_ASSERT_FALSE(cond) TEST_ASSERT_TRUE(!(cond))

#define TEST_ASSERT_EQUAL_INT(expected, actual) do {                        \
    if ((expected) != (actual)) {                                           \
        printf("FAIL: %s:%d: %s - Expected %d but was %d\n",               \
               __FILE__, __LINE__, Unity_CurrentTestName,                   \
               (int)(expected), (int)(actual));                             \
        Unity_FailCount++;                                                   \
        return;                                                              \
    }                                                                       \
} while(0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    TEST_ASSERT_EQUAL_INT(expected, actual)

#define TEST_ASSERT_NOT_EQUAL(expected, actual) do {                        \
    if ((expected) == (actual)) {                                           \
        printf("FAIL: %s:%d: %s - Expected NOT %d\n",                      \
               __FILE__, __LINE__, Unity_CurrentTestName, (int)(expected)); \
        Unity_FailCount++;                                                   \
        return;                                                              \
    }                                                                       \
} while(0)

#define TEST_ASSERT_NULL(ptr) do {                                          \
    if ((ptr) != NULL) {                                                    \
        printf("FAIL: %s:%d: %s - Expected NULL\n",                        \
               __FILE__, __LINE__, Unity_CurrentTestName);                  \
        Unity_FailCount++;                                                   \
        return;                                                              \
    }                                                                       \
} while(0)

#define TEST_ASSERT_NOT_NULL(ptr) do {                                      \
    if ((ptr) == NULL) {                                                    \
        printf("FAIL: %s:%d: %s - Expected NOT NULL\n",                    \
               __FILE__, __LINE__, Unity_CurrentTestName);                  \
        Unity_FailCount++;                                                   \
        return;                                                              \
    }                                                                       \
} while(0)

#define TEST_FAIL_MESSAGE(msg) do {                                         \
    printf("FAIL: %s:%d: %s - %s\n",                                       \
           __FILE__, __LINE__, Unity_CurrentTestName, msg);                 \
    Unity_FailCount++;                                                       \
    return;                                                                  \
} while(0)

#define TEST_PASS() return

#endif /* UNITY_H */
