#ifndef LOST_COMMON_FPSMETER_h
#define LOST_COMMON_FPSMETER_h

#include <vector>
#include <string>
#include <boost/shared_array.hpp>

namespace lost
{
  namespace gl { struct Canvas; struct State; };

  namespace common
  {
    struct FpsMeter;
    typedef lost::shared_ptr<FpsMeter> FpsMeterPtr;

    struct FpsMeter
    {
      const static unsigned long historylength = 160;

      uint32_t width;
      uint32_t height;
      uint32_t historycurpos;
      uint32_t maxfps;
      uint32_t numlabels;
      uint32_t labelstepping;
      float alpha;
      boost::shared_array<float> history;
      boost::shared_array<float> labelArray;
      boost::shared_array<float> linesArray;
      lost::shared_ptr<gl::Canvas> canvas;
      lost::shared_ptr<gl::State> renderState;

      FpsMeter();
      ~FpsMeter();
      void resetHistory();
      void addHistoryEntry(double timeSinceLastCallSec);
      void render(uint32_t x,
                  uint32_t y,
                  const lost::shared_ptr<gl::Canvas>& canvas,
                  double timeSinceLastCallSec);
    };
  }
}

#endif
