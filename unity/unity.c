/* unity.c - Unity Test Framework (minimal compatible implementation) */
#include "unity.h"
#include <stdio.h>

int         Unity_TestCount = 0;
int         Unity_FailCount = 0;
const char *Unity_CurrentTestName = "";
const char *Unity_CurrentFile     = "";
int         Unity_CurrentLine     = 0;

void UnityBegin(const char *filename)
{
    Unity_TestCount = 0;
    Unity_FailCount = 0;
    printf("\n----- %s -----\n", filename);
}

int UnityEnd(void)
{
    (void)(Unity_TestCount - Unity_FailCount);
    printf("\n%d Tests  %d Failures  %d Ignored\n",
           Unity_TestCount, Unity_FailCount, 0);
    if (Unity_FailCount == 0) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
    }
    return (Unity_FailCount == 0) ? 0 : 1;
}
