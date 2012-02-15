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

#ifndef LOST_COMMON_PROFILER_H
#define LOST_COMMON_PROFILER_H

#ifdef PROFILER_ENABLED


  #include "lost/fs/Path.h"

  // split macro calls to avoid compiler warnings
  #define PROFILE_METHOD() \
    lost::common::ProfilerPtr _profiler( profilerStatistics.initialize( (unsigned int)__builtin_frame_address(0), lost::fs::Path(__FILE__).file(), __PRETTY_FUNCTION__ ) ); \
    if (_profiler) _profiler->initialize();

namespace lost
{
  namespace common
  {

    struct Profiler
    {
    private:
      double methodStart;
    public:
      unsigned int address;
      string  fileName;
      string  methodName;
      unsigned int durationMS;

      Profiler( const unsigned int& inAddress, const string& inFileName, const string& inMethodName );
      ~Profiler();

      void initialize();
    };

    struct ProfilerPtr
    {
    private:
        Profiler* ptr;
    public:
        explicit ProfilerPtr( Profiler* p = 0 ) : ptr(p) {}
        ~ProfilerPtr()               {delete ptr;}
        Profiler* operator->()       {return ptr;}
        operator void const*() const {return ptr;}
    };

    typedef map<string, unsigned int> CallCountList;
    typedef map<string, unsigned int> DurationList;

    typedef vector<unsigned int> ProfilerList;

    struct ProfilerStatistics
    {
    private:
      ProfilerList      profilers;
      CallCountList     callCountList;
      DurationList      durationList;
      double            totalStart;
    public:
      ProfilerStatistics();
      ~ProfilerStatistics();

      Profiler* initialize( const unsigned int& inAddress, const string& inFileName, const string& inMethodName );
      void add( Profiler* profiler );
      void finalize( Profiler* profiler );
    };

  }
}

extern lost::common::ProfilerStatistics profilerStatistics;

#else
  #define PROFILE_METHOD()
#endif

#endif
