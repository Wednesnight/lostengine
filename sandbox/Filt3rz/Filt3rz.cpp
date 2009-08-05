#include "Filt3rz.h"
#include "lost/application/Applicationevent.h"
#include "lost/event/Receive.h"

using namespace std;
using namespace lost;
using namespace luabind;
using namespace lost::application;
using namespace lost::event;

Filt3rz::Filt3rz()
{
  //  waitsForEvents = true;
  eventDispatcher->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Filt3rz::keyHandler, this, _1)));

  passedSec = lost::platform::currentTimeSeconds();
  angle = 0;
  animated = true;
}

bool Filt3rz::startup()
{
  DOUT("startup");
  call_function<void>(lua->globals["init"]);
  
  fboSize = lua->globals["fboSize"];
  numPanels = lua->globals["numPanels"];
  numRows = lua->globals["numRows"];
  windowParams = lua->globals["windowParams"];
  
  if(!Tasklet::startup()) return false;
  model = lua->globals["mesh"];
  scene = lua->globals["scene"];
  return true;
}

bool Filt3rz::shutdown()
{
  DOUT("shutdown");
  return true;
}

void Filt3rz::update(double dt)
{
  if (animated)
  {
    angle = fmod(dt*50+angle, 360);
    model->transform = math::MatrixRotX(angle) * math::MatrixRotY(angle);
  }
}

void Filt3rz::draw()
{
  scene->process(window->context);
  // draw outlines
/*  window->canvas->setColor(grayColor);
  for (unsigned int row = 0; row < numRows; row++)
  {
    for (unsigned int idx = 0; idx < numPanels; idx++)
    {
      window->canvas->drawRectOutline(Rect(idx*fboSize.width, row*fboSize.height, fboSize.width, fboSize.height));
    }
  }*/

  window->context->swapBuffers();
}

bool Filt3rz::update()
{
  double currentSec = lost::platform::currentTimeSeconds();
  double delta = currentSec - passedSec;
  
  update(delta);
  draw();
  
  passedSec = currentSec;
  return true;
}

void Filt3rz::keyHandler(lost::application::KeyEventPtr event)
{
  if (event->key == K_ESCAPE) dispatchApplicationEvent(ApplicationEventPtr(new ApplicationEvent(ApplicationEvent::QUIT())));
  if (event->key == K_SPACE) animated = !animated;
}
