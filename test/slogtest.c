#include <stdio.h>

#if defined(WIN32)
#include <Windows.h>
#include <process.h>
#elif defined(linux)
#include <pthread.h>
#include <unistd.h>
#endif

#include "slog.h"

static void _test_st_log()
{
    SLOG_ST_ERROR("this is st error log, %s, %d", "test", 123123);
    SLOG_ST_WARN("this is st warn log, %s, %d", "test", 123123);
    SLOG_ST_INFO("this is st info log, %s, %d", "test", 123123);
    SLOG_ST_DEBUG("this is st debug log, %s, %d", "test", 123123);
    SLOG_ST_TRACE("this is st trace log, %s, %d", "test", 123123);
}

static void _test_log()
{
    SLOG_ERROR("this is error log, %s, %d", "test", 123123);
    SLOG_WARN("this is warn log, %s, %d", "test", 123123);
    SLOG_INFO("this is info log, %s, %d", "test", 123123);
    SLOG_DEBUG("this is debug log, %s, %d", "test", 123123);
    SLOG_TRACE("this is trace log, %s, %d", "test", 123123);
}

static volatile int g_stop = 0;
#if defined(WIN32)
static unsigned int __stdcall _do_multithread_test(void *param)
{
    while (1 != g_stop) {
        _test_log();
        _test_st_log();
    }
    return 0;
}
#elif defined(linux)
static void *_do_multithread_test(void *param)
{
    while (1 != g_stop) {
        _test_log();
        _test_st_log();
    }
    return NULL;
}
#endif

static void _test_multithread()
{
#define THREAD_COUNT             (10)
#define TEST_TIME                (30)
    int i = 0;

#if defined(WIN32)
    for (i = 0; i < THREAD_COUNT; ++i) {
        _beginthreadex(NULL, 0, _do_multithread_test, NULL, 0, NULL);
    }
    Sleep(1000 * TEST_TIME);
#elif defined(linux)
    pthread_t ptt;
    for (i = 0; i < THREAD_COUNT; ++i) {
        pthread_create(&ptt, NULL, _do_multithread_test, NULL);
    }
    sleep(TEST_TIME);
#endif

    g_stop = 1;
}

#define DO_MULTITHREAD_TEST            (1)
int main(int argc, char **argv)
{
    if (init_logger("./logs", S_DEBUG) != TRUE) {
        printf("init logger failed\n");
        goto end;
    }

    printf("test begin...\n");

    _test_log();
    _test_st_log();

#if defined(DO_MULTITHREAD_TEST)
    printf("do multithread test...\n");
    _test_multithread();
    printf("multithread over \n");
#endif


    printf("test over \n");

end:
    getchar();
    return 0;
}
