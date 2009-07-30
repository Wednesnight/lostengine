#include "Controller.h"
#include "lost/event/Receive.h"
#include "lost/common/Logger.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/SpawnTaskletEvent.h"

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
#include "lost/resource/Loader.h"
#include "lost/resource/FilesystemRepository.h"
#include "lost/resource/ApplicationResourceRepository.h"

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
using namespace lost::resource;
using namespace luabind;
using namespace boost;

const static Vec2 screenSize(400,128);

Controller::Controller()
{
  waitForEvents = true;
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

  rootNode = lua->globals["ui"];
  if(!rootNode)
  {
    throw std::runtime_error("couldn't find ui rootNode");
  }

  return result;
}

bool Controller::update()
{
  rootNode->process(window->context);
  window->context->swapBuffers();  
  return true;
}

bool Controller::shutdown()
{
  return true;
}

void Controller::keyDownHandler(KeyEventPtr event)
{
  if (event->key == K_ESCAPE) dispatchApplicationEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())));
}

void Controller::fileDropHandler(DropEventPtr event)
{
  if (filesystem::is_directory(event->filename))
  {
    DOUT("starting tasklet: " << event->filename);
    LoaderPtr loader(new Loader);
    loader->addRepository(RepositoryPtr(new FilesystemRepository(event->filename)));
    loader->addRepository(RepositoryPtr(new ApplicationResourceRepository()));
    SpawnTaskletEventPtr event(new SpawnTaskletEvent(loader));
    dispatchApplicationEvent(event);
  }
  else
  {
    EOUT("couldn't start tasklet: you need to drop a directory that contains a file called main.lua")
  }
}
