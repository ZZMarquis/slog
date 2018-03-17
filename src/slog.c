#include "slog.h"

#include <stdio.h>
#include <io.h>
#include <time.h>
#include <stdarg.h>
#include <direct.h>

#if defined(WIN32)
#include <Windows.h>
#include <DbgHelp.h>
#endif

#define MAX_LEVEL_STR               (10)
#define MAX_DATE_STR                (10)
#define DATE_STR_FMT                "%04d%02d%02d"
#define MAX_TIME_STR                (20)
#define TIME_STR_FMT                "%04d/%02d/%02d %02d:%02d:%02d"
#define MAX_FILE_PATH               (260)
#define MAX_LOG_LINE                (4096)

#if defined(WIN32)
	#define snprintf _snprintf
	#define vsnprintf _vsnprintf
	#define PROC_HANDLE HANDLE
#elif defined(linux)
	#define PROC_HANDLE
#endif

typedef struct _logger_cfg {
	PROC_HANDLE curr_proc;
	FILE *log_file;
	slog_level filter_levle;
	int inited;
} logger_cfg;

static logger_cfg g_logger_cfg = {
	NULL, NULL, S_INFO, FALSE};

static void _get_curr_date(int datestr_size, char datestr[])
{
	time_t tt = { 0 };
	struct tm *curr_time = NULL;

	time(&tt);
	curr_time = localtime(&tt);
	snprintf(datestr, datestr_size - 1, DATE_STR_FMT,
		curr_time->tm_year + 1900, curr_time->tm_mon + 1, curr_time->tm_mday);
}

static void _get_curr_time(int timestr_size, char timestr[]) 
{
	time_t tt = {0};
	struct tm *curr_time = NULL;
	
	time(&tt);
	curr_time = localtime(&tt);
	snprintf(timestr, timestr_size - 1, TIME_STR_FMT,
		curr_time->tm_year + 1900, curr_time->tm_mon + 1, curr_time->tm_mday,
		curr_time->tm_hour, curr_time->tm_min, curr_time->tm_sec);
}

static char *_get_level_str(slog_level level)
{
	switch (level) {
	case S_TRACE:
		return "[TRACE]";
	case S_DEBUG:
		return "[DEBUG]";
	case S_INFO:
		return "[INFO ]";
	case S_WARN:
		return "[WARN ]";
	case S_ERROR:
		return "[ERROR]";
	default:
		return "[     ]";
	}
}

static void _write_stacktrace()
{
#define INNER_DEEP         (2)
#define MAX_DEEP           (24)
#define MAX_ST_INFO        (256)
#define MAX_ST_LINE        (512)
	unsigned int i = 0;
	void *stack[MAX_DEEP] = { 0 };
	unsigned short frames = 0;
	SYMBOL_INFO *symbol = NULL;
	char st_line[MAX_ST_LINE] = { 0 };

	frames = CaptureStackBackTrace(INNER_DEEP, MAX_DEEP, stack, NULL);
	symbol = (SYMBOL_INFO *)calloc(sizeof(SYMBOL_INFO) + sizeof(char) * MAX_ST_INFO, 1);
	symbol->MaxNameLen = MAX_ST_INFO - 1;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	for (i = 0; i < frames; ++i) {
		SymFromAddr(g_logger_cfg.curr_proc, (DWORD64)(stack[i]), 0, symbol);
		snprintf(st_line, sizeof(st_line) - 1, "    %d: %s 0x%X\n", frames - i - 1, symbol->Name, symbol->Address);
		fwrite(st_line, sizeof(char), strlen(st_line), g_logger_cfg.log_file);
	}
	fflush(g_logger_cfg.log_file);
}

int init_logger(const char *log_dir, slog_level level)
{
	char log_filepath[MAX_FILE_PATH] = { 0 };
	char datestr[MAX_DATE_STR] = { 0 };

	if (TRUE == g_logger_cfg.inited) {
		return TRUE;
	}
	if (access(log_dir, 0) != 0) {
		if (mkdir(log_dir) != 0) {
			return FALSE;
		}
	}
	
#if defined(WIN32)
	g_logger_cfg.curr_proc = GetCurrentProcess();
	if (NULL == g_logger_cfg.curr_proc) {
		return FALSE;
	}
	if (SymInitialize(g_logger_cfg.curr_proc, NULL, TRUE) != TRUE) {
		return FALSE;
	}
#elif defined(linux)
#endif
	_get_curr_date(sizeof(datestr), datestr);
	snprintf(log_filepath, sizeof(log_filepath) - 1, "%s/%s.log", log_dir, datestr);
	g_logger_cfg.log_file = fopen(log_filepath, "a+");
	if (NULL == g_logger_cfg.log_file) {
		return FALSE;
	}
	g_logger_cfg.filter_levle = level;
	g_logger_cfg.inited = TRUE;

	return TRUE;
}

void write_log(slog_level level, int print_stacktrace, const char *func_name, int line, const char *fmt, ...)
{
	va_list args;
	char *level_str = NULL;
	char timestr[MAX_TIME_STR] = { 0 };
	char log_content[MAX_LOG_LINE] = { 0 };
	char log_line[MAX_LOG_LINE] = { 0 };
	
	if (g_logger_cfg.filter_levle > level) {
		return;
	}
	va_start(args, fmt);
	vsnprintf(log_content, sizeof(log_content) - 1, fmt, args);
	va_end(args);
	_get_curr_time(sizeof(timestr), timestr);
	level_str = _get_level_str(level);
	snprintf(log_line, sizeof(log_line) - 1, "%s %s %s:%d -| %s\n",
		level_str, timestr, func_name, line, log_content);
	fwrite(log_line, sizeof(char), strlen(log_line), g_logger_cfg.log_file);
	fflush(g_logger_cfg.log_file);
	if (TRUE == print_stacktrace) {
		_write_stacktrace();
	}
}