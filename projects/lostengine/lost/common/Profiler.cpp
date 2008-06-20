#ifdef PROFILER_ENABLED

#include "lost/common/Profiler.h"

lost::common::ProfilerStatistics profilerStatistics;

lost::common::Profiler::Profiler( const unsigned int& inAddress, const std::string& inFileName, const std::string& inMethodName )
  : methodStart(lost::platform::currentTimeMilliSeconds()),
    address(inAddress),
    fileName(inFileName),
    methodName(inMethodName),
    durationMS(0)
{
}

lost::common::Profiler::~Profiler()
{
  durationMS = (unsigned int)(lost::platform::currentTimeMilliSeconds() - methodStart);
  profilerStatistics.finalize(this);
}

void lost::common::Profiler::initialize()
{
  profilerStatistics.add(this);
}

lost::common::ProfilerStatistics::ProfilerStatistics()
  : logger(log_info),
    totalStart(lost::platform::currentTimeMilliSeconds())
{
}

lost::common::ProfilerStatistics::~ProfilerStatistics()
{
  if (callCountList.size() > 0)
  {
    logger << " " << lendl;
    logger << "Profiler statistics:" << lendl;
    logger << " " << lendl;
    logger << "  total duration: " << (int)(lost::platform::currentTimeMilliSeconds() - totalStart) / 1000 << " seconds" << lendl;
    logger << " " << lendl;
    for (CallCountList::iterator callCount = callCountList.begin(); callCount != callCountList.end(); ++callCount)
    {
      logger << "  "+ callCount->first << lendl;
      logger << "    calls          : " << callCount->second << lendl;

      DurationList::iterator duration = durationList.find(callCount->first);
      if (duration != durationList.end())
      {
        logger << "    avg. duration  : " << (duration->second / callCount->second) << "ms" << lendl;
        logger << "    total duration : " << duration->second << "ms" << lendl;
      }
      logger << " " << lendl;
    }
  }
}

lost::common::Profiler* lost::common::ProfilerStatistics::initialize( const unsigned int& inAddress, const std::string& inFileName, const std::string& inMethodName )
{
  lost::common::Profiler* result = 0;

  if (find( profilers.begin(), profilers.end(), inAddress ) == profilers.end())
    result = new lost::common::Profiler( inAddress, inFileName, inMethodName );

  return result;
}

void lost::common::ProfilerStatistics::add( Profiler* profiler )
{
  if (find( profilers.begin(), profilers.end(), profiler->address ) == profilers.end())
    profilers.push_back( profiler->address );
}

void lost::common::ProfilerStatistics::finalize( Profiler* profiler )
{
  std::vector<unsigned int>::iterator tmpProfiler = find( profilers.begin(), profilers.end(), profiler->address );
  if (tmpProfiler != profilers.end())
  {
    profilers.erase( tmpProfiler );

    if (callCountList.find( profiler->methodName ) != callCountList.end())
    {
      ++callCountList[profiler->methodName];
    }
    else
    {
      callCountList[profiler->methodName] = 1;
    }
    if (durationList.find( profiler->methodName ) != durationList.end())
    {
      durationList[profiler->methodName] += profiler->durationMS;
    }
    else
    {
      durationList[profiler->methodName] = profiler->durationMS;
    }
  }
}

#endif
