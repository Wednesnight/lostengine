#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <boost/shared_ptr.hpp>
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/gl/Texture.h"
#include "lost/common/FpsMeter.h"
#include "lost/resource/Loader.h"

struct Controller
{
  lost::application::Timer* redrawTimer;
  boost::shared_ptr<lost::gl::Texture> texture;
  boost::shared_ptr<lost::common::FpsMeter> fpsMeter;
  boost::shared_ptr<lost::gl::State> textureRenderState;
  boost::shared_ptr<lost::gl::State> vanillaRenderState;
      
  void redraw(boost::shared_ptr<lost::application::TimerEvent> event);
  void keyboard(boost::shared_ptr<lost::application::KeyEvent> inEvent);
  void init(boost::shared_ptr<lost::application::ApplicationEvent> event);
};

#endif