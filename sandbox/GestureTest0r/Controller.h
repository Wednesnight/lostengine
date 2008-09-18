#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/common/FpsMeter.h"
#include "lost/math/Vec2.h"

struct Controller
{
  boost::shared_ptr<lost::application::Timer> redrawTimer;
  lost::common::FpsMeter fpsMeter;

  bool traceMouse;
  std::vector<lost::math::Vec2> mouseCoords;

  Controller();
  
  void redraw(boost::shared_ptr<lost::application::TimerEvent> event);
  void preinit(boost::shared_ptr<lost::application::ApplicationEvent> event);
  void init(boost::shared_ptr<lost::application::ApplicationEvent> event);
  void resize(boost::shared_ptr<lost::application::ResizeEvent> event);
  void handleClick(boost::shared_ptr<lost::application::MouseEvent> event);
  void handleMove(boost::shared_ptr<lost::application::MouseEvent> event);
};

#endif
