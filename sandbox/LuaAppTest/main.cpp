#include "lost/common/Logger.h"
#include <iostream>
#include "lost/lua/lua.h"
#include "lost/lua/State.h"
#include "lost/common/ResourcePath.h"
#include "Application.h"

#include "Event.h"
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "EventQueue.h"
#include "EventDispatcher.h"

using namespace std;
using namespace luabind;
using namespace lost::common;


void genericHandler(boost::shared_ptr<Event> event)
{
  cout << "got event: " << event->type << endl;
}

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;

  
    app.addEventListener(KeyEvent::KEY_DOWN, genericHandler);
    app.addEventListener(MouseEvent::MOUSE_MOVE, genericHandler);

    app.dispatchEvent(KeyEvent::createKeyDownEvent(13));
    app.dispatchEvent(MouseEvent::createMouseMoveEvent());
    
    app.doDispatch();

  }
  catch(exception& ex)
  {
    cout << "error: " << ex.what() << endl;
  }

  return 0;
}