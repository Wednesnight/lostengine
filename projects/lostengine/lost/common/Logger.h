/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_COMMON_LOGGER_H
#define LOST_COMMON_LOGGER_H

#include "lost/fs/Path.h"

namespace lost
{
  namespace common
  {
		namespace Logger
		{			
			void logMessage(const string& inLevel, const string& inLocation, const string& inMsg);
		}
  }
}

#define LOST_COMMON_LOGGER_FILE_LINE "(" << lost::fs::Path(__FILE__).file() << " " << __FUNCTION__ << " " << __LINE__ << ")"

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
