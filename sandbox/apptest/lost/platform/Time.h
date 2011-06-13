#ifndef LOST_PLATFORM_TIME_H
#define LOST_PLATFORM_TIME_H

namespace lost
{
namespace platform
{
    double currentTimeSeconds(); // returns current time in seconds
    double currentTimeMilliSeconds(); // returns current time in milliseconds
    double currentTimeMicroSeconds(); // returns current time in microseconds
    eastl::string currentTimeFormat(); // returns current time as formatted string (e.g. "2007/11/26 23:30:37:123")
}
}

#endif