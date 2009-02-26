#ifndef LOST_COMMON_FPSMETER_h
#define LOST_COMMON_FPSMETER_h

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <string>

namespace lost
{
  namespace gl { struct Canvas; struct State; };

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
      boost::shared_ptr<gl::Canvas> canvas;
      boost::shared_ptr<gl::State> renderState;

      FpsMeter();
      void resetHistory();
      void addHistoryEntry(double timeSinceLastCallSec);
      void render(unsigned long x, unsigned long y, const boost::shared_ptr<gl::Canvas>& canvas, double timeSinceLastCallSec);
    };
  }
}

#endif
