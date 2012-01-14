#ifndef LOST_COMMON_LOGGER_H
#define LOST_COMMON_LOGGER_H

namespace lost
{
  namespace common
  {
		namespace Logger
		{			
			string fileNameFromFullPath(const char* fullPath);
			void logMessage(const string& inLevel, const string& inLocation, const string& inMsg);
		}
  }
}

#define LOST_COMMON_LOGGER_FILE_LINE "(" << lost::common::Logger::fileNameFromFullPath(__FILE__) << " " << __FUNCTION__ << " " << __LINE__ << ")"

#define LOGLOG(m, s, fl) \
{ \
	lost::common::StringStream LOGLOG_WE_DONT_WANT_TO_CONFLICT_NAMES_msg; \
	LOGLOG_WE_DONT_WANT_TO_CONFLICT_NAMES_msg << s; \
	lost::common::StringStream LOGLOG_WE_DONT_WANT_TO_CONFLICT_NAMES_loc; \
	LOGLOG_WE_DONT_WANT_TO_CONFLICT_NAMES_loc << fl; \
	lost::common::Logger::logMessage(m, LOGLOG_WE_DONT_WANT_TO_CONFLICT_NAMES_loc.str().c_str(), LOGLOG_WE_DONT_WANT_TO_CONFLICT_NAMES_msg.str().c_str()); \
}


#if defined(LOST_LOGGER_ENABLE_DOUT)
# define DOUT(s) LOGLOG("DEBUG", s, LOST_COMMON_LOGGER_FILE_LINE)
#else
# define DOUT(s)
#endif

#if defined(LOST_LOGGER_ENABLE_EOUT)
# define EOUT(s) LOGLOG("ERROR", s, LOST_COMMON_LOGGER_FILE_LINE)
# define LOGTHROW(e) EOUT("throwing from here");throw e;
#else
# define EOUT(s)
#define LOGTHROW(e) throw e;
#endif

#if defined(LOST_LOGGER_ENABLE_IOUT)
# define IOUT(s) LOGLOG("INFO", s, LOST_COMMON_LOGGER_FILE_LINE)
#else
# define IOUT(s)
#endif

#if defined(LOST_LOGGER_ENABLE_WOUT)
# define WOUT(s) LOGLOG("WARNING", s, LOST_COMMON_LOGGER_FILE_LINE)
#else
# define WOUT(s)
#endif

#endif
