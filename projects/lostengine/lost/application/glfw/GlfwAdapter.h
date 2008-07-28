#ifndef LOST_APPLICATION_GLFWADAPTER_H
#define LOST_APPLICATION_GLFWADAPTER_H

#include "lost/event/EventDispatcher.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/math/Vec2.h"

namespace lost
{
namespace application
{

struct GlfwAdapter
{
  event::EventDispatcher* target;
  
  GlfwAdapter(event::EventDispatcher* inTarget);
  virtual ~GlfwAdapter();

  void init(const common::DisplayAttributes& displayAttributes);

  void terminate();
  double getTime();
  void pollEvents();
  void sleep(double seconds);
  bool displayOpen(); // FIXME:  should we convert this to an event that is sent to the application?

private: 
  void initDisplay(const common::DisplayAttributes& displayAttributes);
  void initCallbacks();
  friend void glfwKeyCallback( int key, int action, int repeat );
  friend void glfwMouseButtonCallback( int button, int action );
  friend void glfwMouseMoveCallback( int x, int y );
  friend void glfwWindowSizeCallback(int width, int height);

  void injectKey(int key, int pressed, int repeat);
  void injectMouseMove(const lost::math::Vec2& pos);
  void injectMouseButton(int button, bool pressed, const lost::math::Vec2& pos);
  void injectWindowResize(int x, int y);
  
  int displayHeight;
};

}
}

#endif