#include "lost/common/FpsMeter.h"
#include "lost/gl/State.h"

namespace lost
{
  namespace common
  {
      FpsMeter::FpsMeter(boost::shared_ptr<lost::gl::Context> inContext)
#if !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_IPHONE)
      : font(GLUT_BITMAP_TIMES_ROMAN_10), context(inContext)
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
        
        renderState = context->copyState();
        renderState->depthTest = false;
        renderState->blend = true;
        renderState->blendSrc = GL_SRC_ALPHA;
        renderState->blendDest = GL_ONE_MINUS_SRC_ALPHA;
        renderState->vertexArray = true;
      }

      void FpsMeter::resetHistory()
      {
        for(unsigned long i =0; i<historylength; ++i)
        {
          history[i] = 0;
        }
      }

      void FpsMeter::addHistoryEntry(double timeSinceLastCallSec)
      {
        float currentfps = 1.0/timeSinceLastCallSec;
        history[historycurpos] = currentfps;
        historycurpos = (historycurpos+1)%historylength;
      }

      void FpsMeter::render(unsigned long x, unsigned long y, double timeSinceLastCallSec)
      {
        // FIXME: coordinates are off somehow ... we need to figure out the definitve way to draw precise coords. Somehow, OpenGL offsets things for a pixel sometimes
        addHistoryEntry(timeSinceLastCallSec);
        context->pushState(renderState);
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
        context->popState();
      }
  }
}
