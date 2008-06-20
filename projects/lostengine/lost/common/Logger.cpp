#include "lost/common/Logger.h"

static unsigned logLevel = lost::common::log_info | lost::common::log_warning | lost::common::log_error;

SpecializedLogger dout( (logLevel & lost::common::log_debug)   ? lost::common::log_debug   : lost::common::log_none );
SpecializedLogger eout( (logLevel & lost::common::log_error)   ? lost::common::log_error   : lost::common::log_none );
SpecializedLogger iout( (logLevel & lost::common::log_info)    ? lost::common::log_info    : lost::common::log_none );
SpecializedLogger wout( (logLevel & lost::common::log_warning) ? lost::common::log_warning : lost::common::log_none );

void LogLevel( unsigned level )
{
  logLevel = level;

  dout.level = dout.currentLevel = (logLevel & lost::common::log_debug)   ? lost::common::log_debug   : lost::common::log_none;
  eout.level = eout.currentLevel = (logLevel & lost::common::log_error)   ? lost::common::log_error   : lost::common::log_none;
  iout.level = iout.currentLevel = (logLevel & lost::common::log_info)    ? lost::common::log_info    : lost::common::log_none;
  wout.level = wout.currentLevel = (logLevel & lost::common::log_warning) ? lost::common::log_warning : lost::common::log_none;
}
