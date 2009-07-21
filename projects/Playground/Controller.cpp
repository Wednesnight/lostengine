#include "Controller.h"
#include "lost/event/Receive.h"
#include "lost/common/Logger.h"
#include "lost/application/ApplicationEvent.h"

#include "lost/rg/Draw.h"
#include "lost/rg/DepthTest.h"
#include "lost/rg/Camera.h"

#include "lost/gl/TextureAtlas.h"
#include "lost/math/Bezier.h"
#include "lost/mesh/Rect.h"
#include "lost/mesh/Axes.h"
#include "lost/mesh/ScaleGrid.h"
#include "lost/mesh/Circular.h"
#include "lost/mesh/AABB.h"

#include <boost/filesystem.hpp>

using namespace std;
using namespace lost;
using namespace lost::gl;
using namespace lost::event;
using namespace lost::common;
using namespace lost::math;
using namespace lost::event;
using namespace lost::application;
using namespace lost::camera;
using namespace lost::mesh;
using namespace lost::meter;
using namespace lost::font;
using namespace luabind;
using namespace boost;

const static Vec2 screenSize(640,480);

Controller::Controller(const ApplicationPtr& inApplication)
: UiTasklet(WindowParams("Playground", Rect(50,200,screenSize.width, screenSize.height))),
  application(inApplication)
{
}


bool Controller::startup()
{
  bool result = Tasklet::startup();
  if(!result)
  {
    EOUT("Tasklet::startup() failed, aborting");
    return false;
  }
  // key handlers
  eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Controller::keyDownHandler, this, _1)));
  eventDispatcher->addEventListener(DropEvent::DROPPED_FILE(), receive<DropEvent>(bind(&Controller::fileDropHandler, this, _1)));

  textScene = lua->globals["textScene"];

  return result;
}

void Controller::draw()
{
  textScene->process(window->context);
  window->context->swapBuffers();  
}

bool Controller::main()
{
  draw();
  return true;
}

bool Controller::shutdown()
{
  return true;
}

void Controller::keyDownHandler(KeyEventPtr event)
{
  if (event->key == K_ESCAPE) 
    eventDispatcher->dispatchEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())));
}

void Controller::fileDropHandler(DropEventPtr event)
{
  DOUT("executing tasklet: " << event->filename);
  if (filesystem::is_directory(event->filename))
  {
    filesystem::path currentDir = filesystem::current_path();
    filesystem::path newDir(event->filename);
    filesystem::current_path(newDir);
    TaskletPtr tasklet = TaskletPtr(new UiTasklet(WindowParams(event->filename, Rect(50,200,screenSize.width, screenSize.height))));
    tasklet->script = (newDir / "main.lua").native_file_string();
    application->addTasklet(tasklet);
    filesystem::current_path(currentDir);
  }
}
