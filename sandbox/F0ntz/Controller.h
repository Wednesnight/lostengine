#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <boost/shared_ptr.hpp>
#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/font/freetype/Glyph.h"
#include "lost/font/freetype/Renderer.h"
#include "lost/font/freetype/Loader.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/common/FpsMeter.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/TimerEvent.h"

struct Controller 
{
  lost::common::DisplayAttributes                      display;
  boost::shared_ptr<lost::common::FpsMeter>            fpsmeter;
  boost::shared_ptr<lost::font::freetype::Library>     freetypeLibrary;
  boost::shared_ptr<lost::font::freetype::Face>        defaultFont;
  boost::shared_ptr<lost::font::freetype::Renderer>    renderer;
  boost::shared_ptr<lost::font::freetype::Loader>      fontLoader;
  boost::shared_ptr<lost::gl::Texture>                 glyphtex;
  boost::shared_ptr<lost::gl::DisplayList>             smallText;
  boost::shared_ptr<lost::gl::DisplayList>             midText;
  boost::shared_ptr<lost::gl::DisplayList>             largeText;
  
  double passedTime;

  Controller();
  void init(boost::shared_ptr<lost::event::Event> event);
  void keyboard(boost::shared_ptr<lost::application::KeyEvent> event );
  void redraw(boost::shared_ptr<lost::application::TimerEvent> event);
};

#endif