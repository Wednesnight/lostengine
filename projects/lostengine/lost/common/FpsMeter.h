#ifndef LOST_COMMON_FPSMETER_h
#define LOST_COMMON_FPSMETER_h

#include "lost/font/BitmapFont.h"
#include <vector>
#include <boost/lexical_cast.hpp>

namespace lost
{

  namespace common
  {

    struct FpsMeter
    {
      const static unsigned long historylength = 100;

      unsigned long width;
      unsigned long height;
      unsigned long historycurpos;
      unsigned long maxfps;
      unsigned long numlabels;
      unsigned long labelstepping;
      float alpha;
      boost::shared_array<float> history;
      lost::font::BitmapFont font;
      std::vector<std::string> labels;

      FpsMeter()
      : font(GLUT_BITMAP_HELVETICA_10)
      {
        width = historylength;
        height = 60;
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // FIXME: coordinates are off somehow ... we need to figure out the definitve way to draw precise coords. Somehow, OpenGL offsets things for a pixel sometimes
        addHistoryEntry(timeSinceLastCallSec);

        // background
        glColor4f(1, 0, 0, alpha);
        glBegin(GL_QUADS);
        glVertex2f(x-1, y);
        glVertex2f(x+width, y);
        glVertex2f(x+width,y+height);
        glVertex2f(x-1, y+height);
        glEnd();

        // top caption
        glColor4f(1, 1, 1, alpha);
        font.text("fps");
        font.render((x+width)-16, (y+height)-10);

        // horizontal stripes
        glColor4f(1, 1, 1, alpha);
        for(unsigned long n=0; n<numlabels; ++n)
        {
          glBegin(GL_LINES);
          glVertex2f(x, y+10*(n+1));
          glVertex2f(x+width-1, y+10*(n+1));
          glEnd();
          font.text(labels[n]);
          font.render(x, y+10*n+1);
        }

        // draw history ringbuffer
        glColor4f(1,.8,0,.4);
        unsigned long curpos = historycurpos;
        for(unsigned long i=0; i<historylength; ++i)
        {
          glBegin(GL_LINES);
          glVertex2f(x+i, y);
          glVertex2f(x+i, y+history[curpos]);
          glEnd();
          curpos = (curpos+1) % historylength;
        }

      }
    };

  }
}

#endif
