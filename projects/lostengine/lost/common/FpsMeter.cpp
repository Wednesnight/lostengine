#include "lost/common/FpsMeter.h"
#include "lost/gl/State.h"
#include <boost/lexical_cast.hpp>
#include "lost/gl/Context.h"
#include "lost/common/Color.h"
#include "lost/math/Rect.h"
#include "lost/math/Vec2.h"
#include "lost/application/gl/Canvas.h"
#include "lost/application/gl/State.h"

using namespace lost::application::gl;

namespace lost
{
  namespace common
  {
      FpsMeter::FpsMeter()
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
        
        renderState = State::create(DepthTest::create(false),
                                    Blend::create(true),
                                    BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                    Texture2D::create(false),
                                    VertexArray::create(true));
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
        float currentfps = (float)(1.0/timeSinceLastCallSec)/10.0f;
        history[historycurpos] = currentfps;
        historycurpos = (historycurpos+1)%historylength;
      }

      void FpsMeter::render(unsigned long x, unsigned long y, const boost::shared_ptr<Canvas>& canvas, double timeSinceLastCallSec)
      {
        // FIXME: coordinates are off somehow ... we need to figure out the definitve way to draw precise coords. Somehow, OpenGL offsets things for a pixel sometimes
        addHistoryEntry(timeSinceLastCallSec);

        canvas->context->makeCurrent();
        canvas->context->pushState(renderState);

        // background
        canvas->setColor(Color(.75, .75, .75, alpha));
        canvas->drawRectFilled(math::Rect((float)x, (float)y, (float)width, (float)height));

        // top caption
        canvas->setColor(Color(0, 0, 0, alpha));

        // horizontal stripes
        canvas->setColor(Color(0, 0, 0, alpha));
        for(unsigned long n=0; n<numlabels; ++n)
        {
          canvas->drawLine(math::Vec2((float)x, (float)y+10*(n+1)-1), math::Vec2((float)x+width-1, (float)y+10*(n+1)-1));
        }

        // draw history ringbuffer
        canvas->setColor(Color(0, 0, 0, alpha));
        unsigned long curpos = historycurpos;
        for(unsigned long i=0; i<historylength-1; ++i)
        {
//          gl::drawLine(math::Vec2(x+i, y), math::Vec2(x+i, y+history[curpos]));
          canvas->drawLine(math::Vec2((float)x+i, (float)y+history[curpos]), math::Vec2((float)x+i+1, (float)y+history[(curpos+1)%historylength]));
          curpos = (curpos+1) % historylength;
        }

        canvas->context->popState();
      }
  }
}
