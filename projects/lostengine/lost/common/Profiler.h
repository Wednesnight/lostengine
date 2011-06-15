#ifndef LOST_COMMON_PROFILER_H
#define LOST_COMMON_PROFILER_H

#ifdef PROFILER_ENABLED


//  #include "lost/platform/Platform.h"
  #include "lost/common/Logger.h"

  // split macro calls to avoid compiler warnings
  #define PROFILE_METHOD() \
    lost::common::ProfilerPtr _profiler( profilerStatistics.initialize( (unsigned int)__builtin_frame_address(0), lost::common::Logger::fileNameFromFullPath(__FILE__), __PRETTY_FUNCTION__ ) ); \
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
