#ifndef CONTROLLER_H
#define CONTROLLER_H

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
  lost::shared_ptr<lost::gl::Texture> texture;
  lost::shared_ptr<lost::common::FpsMeter> fpsMeter;
  lost::shared_ptr<lost::gl::State> textureRenderState;
  lost::shared_ptr<lost::gl::State> vanillaRenderState;
      
  void redraw(lost::shared_ptr<lost::application::TimerEvent> event);
  void keyboard(lost::shared_ptr<lost::application::KeyEvent> inEvent);
  void init(lost::shared_ptr<lost::application::ApplicationEvent> event);
};

#endif
