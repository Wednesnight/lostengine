#include "lost/common/FpsMeter.h"
#include "lost/gl/State.h"
#include <boost/lexical_cast.hpp>
#include "lost/gl/Context.h"
#include "lost/common/Color.h"
#include "lost/math/Rect.h"
#include "lost/math/Vec2.h"
#include "lost/gl/Canvas.h"
#include "lost/gl/State.h"

using namespace lost::gl;

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
        renderState = State::create(DepthTest::create(false),
                                    Blend::create(true),
                                    BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                    Texture2D::create(false),
                                    VertexArray::create(true));

        // create vertex array buffers from static sizes
        
        // label lines will be drawn with GL_LINES which means we must specify 2 vertices per line
        uint32_t numLines = numlabels;
        uint32_t numVertices = numLines * 2; 
        uint32_t numCoords = numVertices * 2;
        labelArray.reset(new float[numCoords]);
        
        // create static lines
        uint32_t curpos = 0;
        for(uint32_t n=0; n<numlabels; ++n)
        {
            labelArray[curpos++] = 0;
            labelArray[curpos++] = (10*(n+1))-1;
            labelArray[curpos++] = width-1;
            labelArray[curpos++] = (10*(n+1))-1;
        }
        
        // create actual line data buffer
        // this will be draw with GL_LINE_STRIP, which means we have one vertex per history entry
        numVertices = historylength; 
        numCoords = numVertices * 2;
        linesArray.reset(new float[numCoords]);
        // reset the coords to x = num and y = 0. y will be set later on during an update run to the current history value
        for(uint32_t i=0; i<numVertices; ++i)
        {
            linesArray[i+0] = i; // set x to [0,historylength-1]
            linesArray[i+1] = 0; // and current values to zero
        }
      }

      FpsMeter::~FpsMeter()
      {
      }

      void FpsMeter::resetHistory()
      {
        for(uint32_t i =0; i<historylength; ++i)
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

      void FpsMeter::render(uint32_t x,
                            uint32_t y,
                            const lost::shared_ptr<Canvas>& canvas,
                            double timeSinceLastCallSec)
      {
        addHistoryEntry(timeSinceLastCallSec);

        canvas->context->pushState(renderState);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // background
        canvas->setColor(Color(.75, .75, .75, alpha));
        canvas->drawRectFilled(math::Rect((float)x, (float)y, (float)width, (float)height));

        canvas->setColor(Color(0, 0, 0, alpha));
        glTranslatef(x,y,0);
        glVertexPointer(2, GL_FLOAT, 0, labelArray.get()); GLDEBUG;
        glDrawArrays(GL_LINES, 0,numlabels*2); GLDEBUG;
    
        for(uint32_t i=0; i<historylength; ++i)
        {
            linesArray[(i*2)+1] = history[(historycurpos+i)%historylength]; // update y values of the lines for display
        }

        glVertexPointer(2, GL_FLOAT, 0, linesArray.get()); GLDEBUG;
        glDrawArrays(GL_LINE_STRIP, 0, historylength/2); GLDEBUG;

        canvas->context->popState();
      }
  }
}
