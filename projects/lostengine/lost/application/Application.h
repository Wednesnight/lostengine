#ifndef LOST_APPLICATION_APPLICATION
#define LOST_APPLICATION_APPLICATION

#include "lost/common/DisplayAttributes.h"
#include "lost/common/Config.h"
#include <boost/signal.hpp>
#include "lost/common/Logger.h"
#include "lost/event/KeyEvent.h"
#include "lost/event/MouseEvent.h"
#include <list>

namespace lost
{
namespace application
{

struct Timer;

struct Application
{
public:
  Application();
  Application(common::Config& config);
  Application(const common::DisplayAttributes& attribs);
  virtual ~Application();

  void run();
  void quit();

  boost::signal<void(const lost::event::KeyEvent&)>     key;
  boost::signal<void(const lost::event::MouseEvent&)>   mouseMove;
  boost::signal<void(const lost::event::MouseEvent&)>   mouseButton;
  boost::signal<void(int, int)>                         windowResize;

  void resize(int width, int height); // FIXME: can we make this private?

  common::DisplayAttributes displayAttributes;
private:
  void addTimer(Timer* timer);
  void removeTimer(Timer* timer);

  friend void glfwKeyCallback( int key, int action );
  friend void glfwMouseButtonCallback( int button, int action );
  friend void glfwMouseMoveCallback( int x, int y );
  friend void glfwWindowSizeCallback(int width, int height);
  friend struct lost::application::Timer;

  void injectKey(int key, int pressed);
  void injectMouseMove(const lost::math::Vec2& pos);
  void injectMouseButton(int button, bool pressed, const lost::math::Vec2& pos);
  void injectWindowResize(int x, int y);

  void init();
  void initDisplay();
  void initCallbacks();

  void updateTimers(double deltaSec);
  bool running;
  std::list<Timer*> timers;
};
}
}

#endif
