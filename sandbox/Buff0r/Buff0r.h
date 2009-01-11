#ifndef BUFF0R_H
#define BUFF0R_H

#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"
#include "lost/math/Vec2.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ApplicationEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/application/KeySym.h"
#include "lost/common/FpsMeter.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/Texture.h"
#include "lost/gl/PushAttrib.h"
#include "lost/math/Rect.h"
#include "lost/application/Application.h"
#include "lost/application/Timer.h"
#include "lost/application/TimerEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/event/Receive.h"

using namespace boost;
using namespace lost::application;
using namespace lost::common;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::gl::utils;
using namespace lost::math;
using namespace lost::resource;

struct Buff0r
{
  Vec3  eye;
  Vec3  at;
  Vec3  up;
  float fovy;
  float znear;
  float zfar;
  float passed;

  shared_ptr<ShaderProgram> program;
  shared_ptr<FrameBuffer>   frameBuffer;
  shared_ptr<Texture>       depthTexture;
  shared_ptr<Texture>       colorTexture;

  int frameBufferWidth;
  int frameBufferHeight;

  shared_ptr<Loader>   loader;
  shared_ptr<FpsMeter> fpsMeter;
  shared_ptr<Timer>    redrawTimer;
  shared_ptr<State>    bufferState;
  shared_ptr<State>    renderState;
  
  Buff0r(DisplayAttributes& inDisplayAttributes, shared_ptr<Loader> inLoader)
  : eye(0,0,-5),
    at(0,0,0),
    up(0,1,0),
    displayAttributes(inDisplayAttributes),
    loader(inLoader)
  {
    fovy = 90;
    znear = 1;
    zfar = 100;
    passed = 0;    

    appInstance->addEventListener(ApplicationEvent::INIT(), receive<Event>(bind(&Buff0r::init, this, _1)));
  }

  void init(shared_ptr<Event> event)
  {
    frameBuffer.reset(new FrameBuffer);
    depthTexture.reset(new Texture);
    colorTexture.reset(new Texture);
    fpsMeter.reset(new FpsMeter(appInstance->context));

    frameBufferInit();
    shaderInit();
    if(GLEE_EXT_framebuffer_object)
    {
      DOUT("Framebuffer objects OK");
    }
    else
    {
      DOUT("NO framebuffer objects");
    }

    renderState = appInstance->context->copyState();
    renderState->alphaTest = true;
    renderState->clearColor = blackColor;
    renderState->depthTest = false;
    renderState->blend = true;
    renderState->blendSrc = GL_SRC_ALPHA;
    renderState->blendDest = GL_ONE_MINUS_SRC_ALPHA;
    renderState->texture2D = true;
    renderState->normalArray = false;  
    renderState->vertexArray = true;  
    renderState->textureCoordArray = true;
    
    bufferState = appInstance->context->copyState();
    bufferState->alphaTest = true;
    bufferState->clearColor = blackColor;
    bufferState->depthTest = true;
    bufferState->blend = false;
    bufferState->blendSrc = GL_SRC_ALPHA;
    bufferState->blendDest = GL_ONE_MINUS_SRC_ALPHA;
    bufferState->texture2D = false;
    bufferState->normalArray = false;  
    bufferState->vertexArray = true;  
    bufferState->textureCoordArray = false;
    
    appInstance->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Buff0r::keyboard, this, _1)));
    appInstance->addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&Buff0r::keyboard, this, _1)));
    appInstance->addEventListener(ResizeEvent::MAIN_WINDOW_RESIZE(), receive<ResizeEvent>(bind(&Buff0r::resetViewPort, this, _1)));
    quit.connect(bind(&Application::quit, appInstance));
    
    redrawTimer.reset(new Timer("redrawTimer", 1.0/60.0));
    redrawTimer->addEventListener(TimerEvent::TIMER_FIRED(), receive<TimerEvent>(bind(&Buff0r::redraw, this, _1)));
  }
  
  void frameBufferInit()
  {
    frameBufferWidth = 512;
    frameBufferHeight = 512;

    frameBuffer->enable();

    depthTexture->bind();
    depthTexture->init(0,GL_DEPTH_COMPONENT32,frameBufferWidth, frameBufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    frameBuffer->attachDepth(depthTexture);

    colorTexture->bind();
    colorTexture->init(0,GL_RGBA8,frameBufferWidth, frameBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    frameBuffer->attachColor(0, colorTexture);
    colorTexture->filter(GL_NEAREST);
    colorTexture->wrap(GL_CLAMP);

    if(!frameBuffer->status() == GL_FRAMEBUFFER_COMPLETE_EXT)
      throw std::runtime_error("FrameBuffer status: "+ enum2string(frameBuffer->status()));
    frameBuffer->disable();
  }

  void shaderInit()
  {
    program = loadShader(loader, "lattice");
    program->enable();
    program->validate();
    if(!program->validated())
    {
      DOUT("Problem found during validation: \n"<<program->log())
    }
    else
    {
      DOUT("Program validated OK");
    }
    (*program)["LightPosition"] = Vec3(0,5,-5);
    (*program)["LightColor"]    = Color(1,1,1);
    (*program)["EyePosition"]   = eye;
    (*program)["Specular"]      = Color(1,1,.1);
    (*program)["Ambient"]       = Color(.1,.5,1);
    (*program)["Kd"]            = 0.8f;
    (*program)["Scale"]         = Vec2(0.7, 3.7);
    (*program)["Threshold"]     = Vec2(.3, .2);
    (*program)["SurfaceColor"]  = Color(1,.1,.1);
    program->disable();
  }

  void resetViewPort(shared_ptr<ResizeEvent> event)
  {
    glViewport(0, 0, event->width, event->height);
    appInstance->context->set3DProjection(eye, at, up, fovy, Vec2(znear, zfar));
  }

  void keyboard(shared_ptr<KeyEvent> inEvent )
  {
    switch (inEvent->key)
    {
      case K_ESCAPE :
        if (!inEvent->pressed)
        {
          quit();
        }
        break;
        default :
        break;
    }
  }

// lobotony: copied from freeglut
void  glutSolidCube( GLdouble dSize )
{
    double size = dSize * 0.5;

#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define NN(a,b,c) glNormal3d( a, b, c );

    /* PWO: Again, I dared to convert the code to use macros... */
    glBegin( GL_QUADS );
        NN( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+);
        NN( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+);
        NN( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+);
        NN(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-);
        NN( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+);
        NN( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-);
    glEnd();

#   undef V
#   undef NN
}

  void redraw(shared_ptr<TimerEvent> event)
  {
    // enable framebuffer and set viewport
    {
      frameBuffer->enable();

      appInstance->context->pushState(bufferState);
      appInstance->context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      PushAttrib pushAttrib(GL_VIEWPORT_BIT);
      glViewport(0, 0, frameBufferWidth, frameBufferHeight);

      appInstance->context->set3DProjection(eye, at, up, fovy, Vec2(znear, zfar));
      appInstance->context->setColor(Color(1,1,0));
      program->enable();
      passed += event->passedSec;

      glMatrixMode(GL_MODELVIEW);GLDEBUG;
      glLoadIdentity();GLDEBUG;
      glRotatef(180*sin(passed), 0,1,0);
      glutSolidCube(3);

      glLoadIdentity();GLDEBUG;
      glTranslatef(3, 0, 0);GLDEBUG;
      glRotatef(180*sin(passed), 0,1,0);
      glutSolidCube(1);

      program->disable();

      appInstance->context->popState();
      frameBuffer->disable();
    }
    
    appInstance->context->pushState(renderState);
    appInstance->context->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // this leaves projection matrix on
    appInstance->context->set2DProjection(Vec2(0,0), Vec2(appInstance->displayAttributes->width, appInstance->displayAttributes->height));
    glMatrixMode(GL_MODELVIEW);GLDEBUG; // this is why we need to switch back to modelview here
    glLoadIdentity();GLDEBUG;

    appInstance->context->setColor(whiteColor);
    Rect rect(50, 50, 512, 512);
    appInstance->context->drawRectTextured(rect, colorTexture, false);

    glTranslatef(50, -100, 0);

    appInstance->context->setColor(Color(1,1,1,0.5));
    rect = Rect(60, 50, 512, 512);
    appInstance->context->drawRectTextured(rect, colorTexture, false);

    glLoadIdentity();GLDEBUG;
    fpsMeter->render(appInstance->displayAttributes->width - fpsMeter->width, 0, event->passedSec);
                                   
    appInstance->context->popState();
    appInstance->swapBuffers();
  }

  DisplayAttributes& displayAttributes;
  boost::signal<void(void)> quit;
};

#endif
