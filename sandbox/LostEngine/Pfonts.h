#ifndef PFONTS_H
#define PFONTS_H

#include "Visual.h"
#include "lost/gl/Utils.h"
#include "lost/fonts/BitmapFont.h"
#include "lost/fonts/TrueTypeFont.h"
#include "lost/guiro/ResourceLoader.h"

struct Pfonts : public Visual
{
  const lost::DisplayAttributes& display;
  lost::shared_ptr<lost::fonts::BitmapFont> bitmap9x15;
  lost::shared_ptr<lost::fonts::BitmapFont> bitmap8x13;
  lost::shared_ptr<lost::fonts::BitmapFont> timesRoman10;
  lost::shared_ptr<lost::fonts::BitmapFont> timesRoman24;
  lost::shared_ptr<lost::fonts::BitmapFont> helvetica10;
  lost::shared_ptr<lost::fonts::BitmapFont> helvetica12;
  lost::shared_ptr<lost::fonts::BitmapFont> helvetica18;

  lost::shared_ptr<lost::fonts::TrueTypeFont> zig;
  lost::shared_ptr<lost::fonts::TrueTypeFont> blindfold;
  lost::shared_ptr<lost::fonts::TrueTypeFont> slaytanic;
  lost::shared_ptr<lost::fonts::TrueTypeFont> suigeneris;

  lost::guiro::ResourceLoader&                  resourceLoader;

  Pfonts(const lost::DisplayAttributes& attr, lost::guiro::ResourceLoader& inResourceLoader)
    : display(attr), resourceLoader(inResourceLoader)
  {
    bitmap9x15.reset(new lost::fonts::BitmapFont(GLUT_BITMAP_9_BY_15));
    bitmap8x13.reset(new lost::fonts::BitmapFont(GLUT_BITMAP_8_BY_13));
    timesRoman10.reset(new lost::fonts::BitmapFont(GLUT_BITMAP_TIMES_ROMAN_10));
    timesRoman24.reset(new lost::fonts::BitmapFont(GLUT_BITMAP_TIMES_ROMAN_24));
    helvetica10.reset(new lost::fonts::BitmapFont(GLUT_BITMAP_HELVETICA_10));
    helvetica12.reset(new lost::fonts::BitmapFont(GLUT_BITMAP_HELVETICA_12));
    helvetica18.reset(new lost::fonts::BitmapFont(GLUT_BITMAP_HELVETICA_18));
    zig = resourceLoader.loadFont( "zig.ttf" , 20);
    blindfold = resourceLoader.loadFont( "Blindfold.ttf", 30);
    slaytanic = resourceLoader.loadFont( "Slaytanic.ttf", 40);
    suigeneris = resourceLoader.loadFont( "suigeneris.ttf", 50);
  }

  void redraw(double timeSinceLastCallSec)
  {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1, 1, 1);

    // calculate and draw seperator lines and titles
    unsigned long topseperatoroffset = 60;
    unsigned long halfwidth = display.width / 2;
    unsigned long topseparator = display.height - topseperatoroffset;

    bitmap9x15->render(30, display.height -topseperatoroffset/2, "builtin GLUT Bitmap Fonts");
    bitmap9x15->render(display.width-200, display.height -topseperatoroffset/2, "TrueType Fonts");

    glBegin(GL_LINES);
    glVertex2f(halfwidth, 0);
    glVertex2f(halfwidth, display.height);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(0, topseparator);
    glVertex2f(display.width, topseparator);
    glEnd();

    // draw bitmap fonts
    unsigned long leftpos = 50;
    unsigned long yoffset = 200;

    bitmap9x15->render( leftpos,  10+yoffset, "Bitmap Font 9x15" );
    bitmap8x13->render( leftpos,  26+yoffset, "Bitmap Font 8x13" );
    timesRoman10->render( leftpos,  40+yoffset, "Times Roman 10" );
    timesRoman24->render( leftpos,  51+yoffset, "Times Roman 24" );
    helvetica10->render( leftpos,  76+yoffset, "Helvetica 10" );
    helvetica12->render( leftpos,  87+yoffset, "Helvetica 12" );
    helvetica18->render( leftpos,  100+yoffset, "Helvetica 18" );

    // draw truetype fonts
    zig->render(halfwidth+leftpos, 10+yoffset, "zig");
    blindfold->render(halfwidth+leftpos, 41+yoffset, "Blindfold");
    slaytanic->render(halfwidth+leftpos, 72+yoffset, "Slaytanic");
    suigeneris->render(halfwidth+leftpos, 123+yoffset, "Suigeneris");
  }
};

#endif
