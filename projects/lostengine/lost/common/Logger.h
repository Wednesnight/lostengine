#ifndef LOST_COMMON_LOGGER_H
#define LOST_COMMON_LOGGER_H

#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/signal.hpp>
#include "lost/platform/Platform.h"
#include <iostream>

#ifndef __GNUC__
  #define __FUNCTION__ "(unknown)"
  #define __PRETTY_FUNCTION__ "(unknown)()"
#endif

#define __CALLER_FILE__ __FILE__ << ": "
#define __CALLER_FUNCTION__ __FUNCTION__ << "(): "
#define __CALLER_INFO__ __FILE__ << ": " << __FUNCTION__ << "(): "
#define __PRETTY_CALLER_INFO__ __FILE__ << ": " << __PRETTY_FUNCTION__ << ": "

typedef enum
{
  lendl
} LendlType;

namespace lost
{
  namespace common
  {

    typedef enum
    {
      log_none    = 0,
      log_debug   = 1,
      log_info    = 2,
      log_warning = 4,
      log_error   = 8,
      log_all     = 15
    } LogEntryType;

    typedef enum
    {
      log_console  = 1,
      log_file     = 2,
      log_callback = 4
    } LogEntryTarget;

    struct Logger
    {
      static std::string fileNameFromFullPath(const char* fullPath)
      {
        char* p = strrchr(fullPath, '/'); // FIXME: this might fail on windows due to different separators, we might need to use something like boost::fs here
        if(p)
        {
          return std::string(p+1); // add 1 to pointer to remove leading '/'
        }
        else
        {
          return std::string(fullPath);
        }
      }

      // FIXME: this is a hack because currentTimeFormat requires a parameter
      static std::string logtime()
      {
        std::string t;
        return lost::platform::currentTimeFormat( t );
      }

      typedef boost::signal<void ( const LogEntryType, const std::string& )> LogSignal;

      // global states for << operator
      std::string  currentEntry;
      LogEntryType currentLevel;
      std::string  currentSource;

      std::string  filename;
      unsigned     level;
      LogSignal    signal;
      unsigned     targets;

      Logger()
      {
        currentEntry.clear();
        currentLevel = log_info;
        filename.clear();
        level   = log_info | log_warning | log_error;
        targets = log_console | log_file | log_callback;
      }

      template<typename InValue>
      friend Logger& operator << ( Logger& logger, InValue value )
      {
        logger.currentEntry.append( boost::lexical_cast<std::string>( value ) );
        return logger;
      }

      friend Logger& operator << ( Logger& logger, LogEntryType level )
      {
        logger.currentLevel = level;
        return logger;
      }

      friend void operator << ( Logger& logger, const LendlType& lendlInstance )
      {
        if (!logger.currentEntry.empty())
        {
          logger.log( logger.currentLevel, logger.currentEntry );
          logger.currentEntry.clear();
        }
      }

      std::string logEntry2String( LogEntryType type )
      {
        switch (type)
        {
          case log_info    : return "INFO";
          case log_warning : return "WARNING";
          case log_error   : return "ERROR";
          case log_debug   : return "DEBUG";
          default: throw std::runtime_error("tried to set undefined log level");
        }
      }

      void log( LogEntryType type, const std::string& msg )
      {
        if (level & type)
        {
          std::string       time;
          std::stringstream entry;

          if (!currentSource.empty()) entry << currentSource << ": ";
          entry << lost::platform::currentTimeFormat( time ) << " \t " <<
                   logEntry2String( type )                   << " \t " <<
                   msg                                       << std::endl;

          // console output
          if (targets & log_console)
            std::cout << entry.rdbuf();

          // callbacks
          if (targets & log_callback)
            signal( type, msg );

          // logfile output
          if (targets & log_file && !filename.empty())
          {
            std::ofstream file( filename.c_str(), std::ios_base::app );
            assert( file.good() );
            file.write( entry.str().c_str(), entry.str().size() );
          }
        }
      }

    };

  }
}

static lost::common::Logger logger;

struct SpecializedLogger : public lost::common::Logger
{
  SpecializedLogger( lost::common::LogEntryType levelValue )
  {
    Logger::Logger();
    currentLevel = levelValue;
    filename     = lost::platform::getApplicationFilename( true ).append( ".log" );
    level        = levelValue;
  }
};

extern SpecializedLogger dout;
extern SpecializedLogger eout;
extern SpecializedLogger iout;
extern SpecializedLogger wout;

#define LOST_COMMON_LOGGER_FILE_LINE "(" << lost::common::Logger::fileNameFromFullPath(__FILE__) << " " << __FUNCTION__ << " " << __LINE__ << ")"
#define DOUT(s) dout << LOST_COMMON_LOGGER_FILE_LINE << " " << s << lendl;
#define EOUT(s) eout << LOST_COMMON_LOGGER_FILE_LINE << " " << s << lendl;
#define IOUT(s) iout << LOST_COMMON_LOGGER_FILE_LINE << " " << s << lendl;
#define WOUT(s) wout << LOST_COMMON_LOGGER_FILE_LINE << " " << s << lendl;

void LogLevel( unsigned level );

#endif
