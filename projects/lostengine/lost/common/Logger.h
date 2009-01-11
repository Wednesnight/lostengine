#ifndef LOST_COMMON_LOGGER_H
#define LOST_COMMON_LOGGER_H

#include <string>
#include <iostream>
#include <sstream>

#define __CALLER_FILE__ __FILE__ << ": "
#define __CALLER_FUNCTION__ __FUNCTION__ << "(): "
#define __CALLER_INFO__ __FILE__ << ": " << __FUNCTION__ << "(): "
#define __PRETTY_CALLER_INFO__ __FILE__ << ": " << __PRETTY_FUNCTION__ << ": "

namespace lost
{
  namespace common
  {
		namespace Logger
		{			
			std::string fileNameFromFullPath(const char* fullPath);
			void logMessage(const std::string& inLevel, const std::string& inLocation, const std::string& inMsg);
		}
  }
}

#define LOST_COMMON_LOGGER_FILE_LINE "(" << lost::common::Logger::fileNameFromFullPath(__FILE__) << " " << __FUNCTION__ << " " << __LINE__ << ")"

#define LOGLOG(m, s, fl) \
{ \
	std::ostringstream msg; \
	msg << s; \
	std::ostringstream loc; \
	loc << fl; \
	lost::common::Logger::logMessage(m, loc.str(), msg.str()); \
}

#define DOUT(s) LOGLOG("DEBUG", s, LOST_COMMON_LOGGER_FILE_LINE)
#define EOUT(s) LOGLOG("ERROR", s, LOST_COMMON_LOGGER_FILE_LINE)
#define IOUT(s) LOGLOG("INFO", s, LOST_COMMON_LOGGER_FILE_LINE)
#define WOUT(s) LOGLOG("WARNING", s, LOST_COMMON_LOGGER_FILE_LINE)

#endif
