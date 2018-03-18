#include <stdio.h>
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

int main(int argc, char **argv)
{
    if (init_logger("./logs", S_DEBUG) != TRUE) {
        printf("init logger failed\n");
        goto end;
    }

    _test_log();
    _test_st_log();

    printf("test over \n");

end:
    getchar();
    return 0;
}