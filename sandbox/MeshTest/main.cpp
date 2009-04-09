#include <iostream>
#include <stdexcept>
#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "lost/application/KeyEvent.h"
#include "lost/event/Receive.h"
#include "lost/mesh/Line.h"
#include <stdlib.h>

using namespace std;
using namespace lost;
using namespace lost::gl;
using namespace lost::common;
using namespace lost::math;
using namespace lost::event;
using namespace lost::application;

ApplicationPtr  app;
WindowPtr       window;
StatePtr        renderstate;
StatePtr        linestate;
mesh::Line2D*    line;

void update(ApplicationPtr app)
{
  window->context->makeCurrent();
  window->canvas->camera->apply();
  window->canvas->context->pushState(renderstate);
  window->canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  window->canvas->setColor(whiteColor);
  glMatrixMode(GL_MODELVIEW);GLDEBUG;
  glLoadIdentity();GLDEBUG;
  
  try
  {
    line->update(Vec2(0,0), Vec2(400,400));
    line->draw(window->context);
    
  }
  catch(std::exception& ex)
  {
    EOUT("caught error: "<<ex.what());
  }
  
  window->canvas->context->popState();
  window->context->swapBuffers();   
  
//  ::exit(EXIT_SUCCESS);
}


void keyHandler(KeyEventPtr event)
{
  if (event->key == K_ESCAPE) app->quit();
}

int main(int argn, char** args)
{
  try
  {
    DOUT("asd");
    app = Application::create(update);
    app->addEventListener(lost::application::KeyEvent::KEY_DOWN(), receive<KeyEvent>(keyHandler));
    window = app->createWindow("window", WindowParams("Filt3rz", Rect(50,200,640,480)));
    window->context->makeCurrent();
    renderstate = State::create(ClearColor::create(blackColor), DepthTest::create(false));
    linestate = State::create();
    line = new mesh::Line2D;
    
    app->run();      
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

