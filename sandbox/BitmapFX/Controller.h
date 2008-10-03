#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <boost/shared_ptr.hpp>
#include "lost/common/DisplayAttributes.h"
#include "lost/common/FpsMeter.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/TimerEvent.h"

struct Controller 
{
  lost::common::DisplayAttributes                 display;
  boost::shared_ptr<lost::common::FpsMeter>       fpsMeter;
  boost::shared_ptr<lost::gl::Context>            context;
  boost::shared_ptr<lost::gl::State>              renderState;

  boost::shared_ptr<lost::bitmap::Bitmap>         pic;

  Controller();
  void init(boost::shared_ptr<lost::event::Event> event);
  void keyboard(boost::shared_ptr<lost::application::KeyEvent> event );
  void redraw(boost::shared_ptr<lost::application::TimerEvent> event);
};

#endif