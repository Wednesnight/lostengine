#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <boost/shared_ptr.hpp>
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/gl/Texture.h"
#include "lost/bitmap/BitmapLoader.h"
#include "lost/common/FpsMeter.h"
#include "lost/resource/Loader.h"

struct Controller
{
  lost::application::Timer* redrawTimer;
  lost::bitmap::BitmapLoader loader;
  boost::shared_ptr<lost::bitmap::Bitmap> bitmap;
  boost::shared_ptr<lost::gl::Texture> texture;
  lost::common::FpsMeter fpsMeter;
  
  Controller(boost::shared_ptr<lost::resource::Loader> inLoader);
  
  void redraw(boost::shared_ptr<lost::application::TimerEvent> event);
  void keyboard(boost::shared_ptr<lost::application::KeyEvent> inEvent);
  void init(boost::shared_ptr<lost::application::ApplicationEvent> event);
};

#endif