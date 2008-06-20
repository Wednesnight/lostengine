#ifndef IMAGOMBIES_H
#define IMAGOMBIES_H

#include "Visual.h"
#include "lost/image/ImageLoader.h"
#include "lost/fonts/TrueTypeFont.h"
#include "lost/math/Generator.h"
#include "lost/guiro/ResourceLoader.h"
#include <boost/shared_ptr.hpp>

struct Imagombies : public Visual
{
  const lost::DisplayAttributes& display;
  lost::image::ImageLoader loader;
  boost::shared_ptr<lost::fonts::TrueTypeFont> blindfold;
  boost::shared_ptr<lost::image::Image> zombie;
  lost::math::generator::sine xgen;

  float curx;
  lost::guiro::ResourceLoader&                  resourceLoader;

  Imagombies(const lost::DisplayAttributes& attr, lost::guiro::ResourceLoader& inResourceLoader)
  : display(attr),
    resourceLoader(inResourceLoader)
  {
    blindfold = resourceLoader.loadFont( "Blindfold.ttf" , 30);
      zombie =  resourceLoader.loadImage("nomnomnom.jpg");
      xgen.target(curx);
      xgen.period = 1;
      xgen.periodoffset = 0;
      xgen.ampoffset = display.width / 2;
      xgen.absamplitude = 100;
  }


  void redraw(double timeSinceLastCallSec)
  {
    xgen.update(timeSinceLastCallSec);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(1, 1, 1);
    blindfold->render(display.width/2-100, display.height-50, "I can has BRAINS?!");

    zombie->render(curx, 100);

    blindfold->render(display.width/2-100, 50, "NOM NOM NOM");
  }
};

#endif
