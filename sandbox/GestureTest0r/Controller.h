#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/common/FpsMeter.h"
#include "lost/math/Vec2.h"

struct Controller
{
  lost::shared_ptr<lost::application::Timer> redrawTimer;
  lost::common::FpsMeter fpsMeter;

  bool traceMouse;
  std::vector<lost::math::Vec2> mouseCoords;

  Controller();
  
  void redraw(lost::shared_ptr<lost::application::TimerEvent> event);
  void preinit(lost::shared_ptr<lost::application::ApplicationEvent> event);
  void init(lost::shared_ptr<lost::application::ApplicationEvent> event);
  void resize(lost::shared_ptr<lost::application::ResizeEvent> event);
  void handleClick(lost::shared_ptr<lost::application::MouseEvent> event);
  void handleMove(lost::shared_ptr<lost::application::MouseEvent> event);
};

#endif
