#include "Controller.h"
#include "lost/application/Application.h"
#include "lost/application/KeySym.h"


using namespace std;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::math;
using namespace lost::common;
using namespace lost::event;
using namespace lost::application;
using namespace lost::platform;
using namespace lost::bitmap;
using namespace lost::resource;
using namespace lost::application;
using namespace boost;


void Controller::redraw(shared_ptr<TimerEvent> event)
{
  glViewport(0, 0, appInstance->displayAttributes.width, appInstance->displayAttributes.height);GLDEBUG;
  set2DProjection(Vec2(0,0), Vec2(appInstance->displayAttributes.width, appInstance->displayAttributes.height));

  appInstance->context->pushState(textureRenderState);
  appInstance->context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  setColor(whiteColor);
  drawRectTextured(Rect(10,10,texture->width,texture->height), texture);
  appInstance->context->popState();  
  
  appInstance->context->pushState(vanillaRenderState);
  setColor(whiteColor);
  drawRectFilled(Rect(400,400,50,50));
  appInstance->context->popState();  
  
  fpsMeter->render(2,2,event->passedSec);
  appInstance->swapBuffers();
}

void Controller::keyboard(shared_ptr<KeyEvent> inEvent)
{
  switch (inEvent->key)
  {
    case K_ESCAPE:appInstance->quit();break;
    default:break;
  }
}

void Controller::init(shared_ptr<ApplicationEvent> event)
{
  //setup resources
  //string filename = "gay_zombie.jpg";
  //string filename = "nomnomnom.jpg";
  //string filename = "buttonReleased.png";
  fpsMeter.reset(new FpsMeter(appInstance->context));
  string filename = "stubs.jpg";
  bitmap.reset(new Bitmap(appInstance->loader->load(filename)));
  
  
  texture.reset(new Texture());
  texture->bind();
  texture->reset(0, GL_RGBA8, false, bitmap);
  texture->wrap(GL_CLAMP_TO_EDGE);
  texture->filter(GL_LINEAR);
  
  textureRenderState = appInstance->context->copyState();
  textureRenderState->texture2D = true;
  textureRenderState->blend = true;
  textureRenderState->blendSrc = GL_SRC_ALPHA;
  textureRenderState->blendDest = GL_ONE_MINUS_SRC_ALPHA;
  textureRenderState->clearColor = Color(1,0,1,1);
  textureRenderState->depthTest = false;  
  textureRenderState->alphaTest = false;  
  textureRenderState->normalArray = false;  
  textureRenderState->vertexArray = true;  
  textureRenderState->textureCoordArray = true;  

  vanillaRenderState = appInstance->context->copyState();
  vanillaRenderState->texture2D = false;
  vanillaRenderState->blend = false;
  vanillaRenderState->clearColor = Color(1,0,1,1);
  vanillaRenderState->depthTest = false;  
  vanillaRenderState->alphaTest = false;  
  vanillaRenderState->normalArray = false;  
  vanillaRenderState->vertexArray = true;  
  vanillaRenderState->textureCoordArray = false;  
  
  DOUT("width: "<<texture->width<< " height: "<<texture->height);
  
  redrawTimer = new Timer("redrawTimer", 1.0/60.0);
  redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Controller::redraw, this, _1)));
}
