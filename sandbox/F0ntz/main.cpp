#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/Timer.h"
#include "lost/application/KeyEvent.h"
#include "lost/event/Receive.h"
#include "Controller.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::common;
using namespace lost::resource;
using namespace lost::application;
using namespace lost::event;

/*struct A
{
  std::string s;
  A() { DOUT("A::A()"); }
};

struct B
{
  B() { DOUT("B::B()"); a_ptr.reset(new A()); }
  A a;
  lost::shared_ptr<A> a_ptr;
};

lost::shared_ptr<A> get_a(B* instance)
{
  return lost::shared_ptr<A>(&instance->a);
}

void set_a(B* instance, lost::shared_ptr<A> a)
{
  instance->a = *(a.get());
}

void test_a(luabind::object a)
{
  DOUT("test_a()");
  A* myA = luabind::object_cast<A*>(a);
  DOUT(myA->s);
}

void test_b(luabind::object b)
{
  DOUT("test_b()");
  A* myA = luabind::object_cast<A*>(b["a"]);
  DOUT(myA->s);
}*/

int main( int argc, char *argv[] )
{
  LogLevel( log_all );

  try
  {
    Application app;
    Controller  controller;
    Timer       redrawTimer("redraw", 1.0/60.0); 

    redrawTimer.addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, &controller, _1)));
    app.addEventListener(ApplicationEvent::INIT(), bind(&Controller::init, &controller, _1));
    app.addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));
    app.addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&Controller::keyboard, &controller, _1)));

/*    luabind::module(*app.interpreter)
    [
      luabind::def("test_a", &test_a),
      luabind::def("test_b", &test_b),
      luabind::class_<A, lost::shared_ptr<A> >("A")
        .def(luabind::constructor<>())
        .def_readwrite("s", &A::s)
     ];
    
    luabind::module(*app.interpreter)
    [
      luabind::class_<B, lost::shared_ptr<B> >("B")
        .def(luabind::constructor<>())
//        .def_readwrite("a", &B::a)
        .property("a", &get_a, &set_a)
        .def_readwrite("a_ptr", &B::a_ptr)
    ];*/
    
    app.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return EXIT_SUCCESS;
}
