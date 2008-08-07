#ifndef LOST_COMMON_FPSMETER_h
#define LOST_COMMON_FPSMETER_h

#include <vector>
#include <boost/lexical_cast.hpp>
#include "lost/gl/Utils.h"
#include "lost/gl/Draw.h"
#include "lost/common/Color.h"
#include "lost/math/Rect.h"
#include "lost/math/Vec2.h"

namespace lost
{

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
      void* font;
      std::vector<std::string> labels;

      FpsMeter()
#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
      : font(GLUT_BITMAP_TIMES_ROMAN_10)
#endif
      {
        width = historylength;
        height = 100;
        alpha = .6f;
        history.reset(new float[historylength]);
        historycurpos = 0;
        resetHistory();
        maxfps = height;
        labelstepping = 10;
        numlabels = maxfps / labelstepping;
        for(unsigned long i=0; i<numlabels; ++i)
        {
          labels.push_back(boost::lexical_cast<std::string>((i+1)*labelstepping));
        }
      }

      void resetHistory()
      {
        for(unsigned long i =0; i<historylength; ++i)
        {
          history[i] = 0;
        }
      }

      void addHistoryEntry(double timeSinceLastCallSec)
      {
        float currentfps = 1.0/timeSinceLastCallSec;
        history[historycurpos] = currentfps;
        historycurpos = (historycurpos+1)%historylength;
      }

      void render(unsigned long x, unsigned long y, double timeSinceLastCallSec)
      {
        glDisable(GL_DEPTH_TEST);GLDEBUG;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // FIXME: coordinates are off somehow ... we need to figure out the definitve way to draw precise coords. Somehow, OpenGL offsets things for a pixel sometimes
        addHistoryEntry(timeSinceLastCallSec);

        // background
        gl::setColor(Color(.75, .75, .75, alpha));
        gl::drawRectFilled(math::Rect(x, y, width, height));

        // top caption
        gl::setColor(Color(0, 0, 0, alpha));
#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
        gl::drawString((x+width)-17, (y+height)-9, "FPS", font);
#endif

        // horizontal stripes
        gl::setColor(Color(0, 0, 0, alpha));
        for(unsigned long n=0; n<numlabels; ++n)
        {
          gl::drawLine(math::Vec2(x, y+10*(n+1)-1), math::Vec2(x+width-1, y+10*(n+1)-1));
#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
          gl::drawString(x+1, y+10*n+1, labels[n], font);
#endif
        }

        // draw history ringbuffer
        gl::setColor(Color(0, 0, 0, alpha));
        unsigned long curpos = historycurpos;
        for(unsigned long i=0; i<historylength-1; ++i)
        {
//          gl::drawLine(math::Vec2(x+i, y), math::Vec2(x+i, y+history[curpos]));
          gl::drawLine(math::Vec2(x+i, y+history[curpos]), math::Vec2(x+i+1, y+history[(curpos+1)%historylength]));
          curpos = (curpos+1) % historylength;
        }

      }
    };

  }
}

#endif
