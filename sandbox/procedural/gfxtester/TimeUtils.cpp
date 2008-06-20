#include "TimeUtils.h"
#include <time.h>
#include <sys/time.h>

namespace timeutils
{
// returns current time in microseconds
double currentTimeMicroSeconds()
{
    struct timezone tz;
    struct timeval tv;
    
    gettimeofday(&tv, &tz);
    return ((double)tv.tv_sec)*1000000.0 + (double)tv.tv_usec;
}

}