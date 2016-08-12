#include "Log.h"

extern char *__progname;

Log::Log(int facility)
{
    facility_ = facility;
    priority_ = LOG_DEBUG;
    openlog(__progname, LOG_PID, facility_);
}

Log::~Log()
{
    closelog();
}

#ifdef DEBUG
void Log::debug(const char* fmt, ... )
{
    FMTPARCE syslog(LogPriority::Debug, "[%ld]%s", pthread_self(), buffer);
};
#else
void Log::debug(const char* fmt, ... )
{
    return;
};
#endif
