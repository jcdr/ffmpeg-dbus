#ifndef DEBUG_H
#define DEBUG_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>

extern int debug_level;
extern int debug_syslog;
extern int debug_console;

#define LOG_SIZE 1024

#define _log_text(_fmt, ...) \
	char _text[LOG_SIZE]; \
	int _len=0; \
	_len+=snprintf(_text+_len, LOG_SIZE-_len, "%d:%s:%d %s: ", getpid(), __FILE__, __LINE__, __func__); \
	_len+=snprintf(_text+_len, LOG_SIZE-_len, _fmt, __VA_ARGS__);

#define debug(_level, _fmt, ...) { \
	if(_level<=debug_level) { \
		_log_text(_fmt, __VA_ARGS__); \
		_len+=snprintf(_text+_len, LOG_SIZE-_len, "\n"); \
		if(debug_syslog) syslog(LOG_INFO, _text); \
		if(debug_console) { \
			fprintf(stdout, _text); \
			fflush(stdout); \
		} \
	} \
}

#define error(_fmt, ...) { \
	_log_text(_fmt, __VA_ARGS__); \
	_len+=snprintf(_text+_len, LOG_SIZE-_len, ": %s (errno=%d)\n", strerror(errno), errno); \
	if(debug_syslog) syslog(LOG_ERR, _text); \
	if(debug_console) { \
		fprintf(stderr, _text); \
		fflush(stderr); \
	} \
}

#define panic(_fmt, ...) { \
	_log_text(_fmt, __VA_ARGS__); \
	_len+=snprintf(_text+_len, LOG_SIZE-_len, "\n"); \
	if(debug_syslog) syslog(LOG_ERR, _text); \
	if(debug_console) { \
		fprintf(stderr, _text); \
		fflush(stderr); \
	} \
}

#endif /* DEBUG */
