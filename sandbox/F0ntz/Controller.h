#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/font/freetype/Glyph.h"
#include "lost/font/freetype/Renderer.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/common/FpsMeter.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/TimerEvent.h"

struct Controller 
{
  lost::common::DisplayAttributes                      display;
  lost::shared_ptr<lost::common::FpsMeter>            fpsmeter;
  lost::shared_ptr<lost::font::freetype::Library>     freetypeLibrary;
  lost::shared_ptr<lost::font::freetype::Face>        defaultFont;
  lost::shared_ptr<lost::font::freetype::Renderer>    renderer;
  lost::shared_ptr<lost::gl::Texture>                 glyphtex;
  lost::shared_ptr<lost::gl::DisplayList>             smallText;
  lost::shared_ptr<lost::gl::DisplayList>             midText;
  lost::shared_ptr<lost::gl::DisplayList>             largeText;
  
  double passedTime;

  Controller();
  void init(lost::shared_ptr<lost::event::Event> event);
  void keyboard(lost::shared_ptr<lost::application::KeyEvent> event );
  void redraw(lost::shared_ptr<lost::application::TimerEvent> event);
};

#endif
