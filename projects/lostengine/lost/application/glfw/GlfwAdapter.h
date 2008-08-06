#ifndef LOST_APPLICATION_GLFWADAPTER_H
#define LOST_APPLICATION_GLFWADAPTER_H

#include "lost/event/EventDispatcher.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/math/Vec2.h"
#include "lost/application/glfw/TimerManager.h"

namespace lost
{
namespace application
{

struct GlfwAdapter
{
  event::EventDispatcher* target;
  
  GlfwAdapter(event::EventDispatcher* inTarget);
  virtual ~GlfwAdapter();

  void init(const common::DisplayAttributes& displayAttributes); // call this to configure adapter before running it, creates OpenGL context 

  void run(); // runs the main loop
  void quit(); // quits the main loop

  void terminate(); // call this to shut the adapter down whe your application has quit
  
  void sleep(double seconds); // FIXME: move this to platform

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

  lost::application::TimerManager  timerManager;  
  int displayHeight;
  bool          running;  
};

}
}

#endif