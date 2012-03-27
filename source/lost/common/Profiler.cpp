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

#ifdef PROFILER_ENABLED

#include "lost/common/Profiler.h"

lost::common::ProfilerStatistics profilerStatistics;

lost::common::Profiler::Profiler( const unsigned int& inAddress, const string& inFileName, const string& inMethodName )
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
  : totalStart(lost::platform::currentTimeMilliSeconds())
{
}

lost::common::ProfilerStatistics::~ProfilerStatistics()
{
  if (callCountList.size() > 0)
  {
    DOUT("Profiler statistics:");
    DOUT("  total duration: " << (int)(lost::platform::currentTimeMilliSeconds() - totalStart) / 1000 << " seconds");
    for (CallCountList::iterator callCount = callCountList.begin(); callCount != callCountList.end(); ++callCount)
    {
      DOUT("  "+ callCount->first);
      DOUT("    calls          : " << callCount->second);

      DurationList::iterator duration = durationList.find(callCount->first);
      if (duration != durationList.end())
      {
        DOUT("    avg. duration  : " << (duration->second / callCount->second) << "ms");
        DOUT("    total duration : " << duration->second << "ms");
      }
    }
  }
}

lost::common::Profiler* lost::common::ProfilerStatistics::initialize( const unsigned int& inAddress, const string& inFileName, const string& inMethodName )
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
  vector<unsigned int>::iterator tmpProfiler = find( profilers.begin(), profilers.end(), profiler->address );
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
