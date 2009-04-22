#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "lost/common/FpsMeter.h"
#include "lost/application/KeyEvent.h"
#include "lost/font/TrueTypeFont.h"
#include "lost/gl/Context.h"
#include "lost/gl/State.h"
#include "lost/gl/Canvas.h"
#include "lost/application/Application.h"
#include "lost/application/Window.h"

struct Controller 
{
  lost::shared_ptr<lost::application::Application> app;
  lost::shared_ptr<lost::application::Window> window;
  lost::shared_ptr<lost::common::FpsMeter> fpsMeter;
  lost::gl::SharedState        renderState;
  
  lost::shared_ptr<lost::font::TrueTypeFont>     ttf;
  lost::shared_ptr<lost::font::Model>            renderedText1;
  lost::shared_ptr<lost::font::Model>            renderedText2;
  lost::shared_ptr<lost::font::Model>            renderedText3;
  
  boost::uint32_t fontSize;

  Controller();
  void keyboard(lost::shared_ptr<lost::application::KeyEvent> event );
  void run();
  void runLoop();
  void render(const lost::shared_ptr<lost::gl::Canvas>& canvas);
};

#endif
