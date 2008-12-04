#ifndef LOST_COMMON_FPSMETER_h
#define LOST_COMMON_FPSMETER_h

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <string>

namespace lost
{
  namespace gl { struct Context; struct State; };

  namespace common
  {
    struct FpsMeter
    {
      const static unsigned long historylength = 160;

      unsigned long width;
      unsigned long height;
      unsigned long historycurpos;
      unsigned long maxfps;
      unsigned long numlabels;
      unsigned long labelstepping;
      float alpha;
      boost::shared_array<float> history;
      std::vector<std::string> labels;
      boost::shared_ptr<lost::gl::Context> context;
      boost::shared_ptr<lost::gl::State> renderState;

      FpsMeter(boost::shared_ptr<lost::gl::Context> inContext);
      void resetHistory();
      void addHistoryEntry(double timeSinceLastCallSec);
      void render(unsigned long x, unsigned long y, double timeSinceLastCallSec);
    };
  }
}

#endif
