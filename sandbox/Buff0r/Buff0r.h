#ifndef BUFF0R_H
#define BUFF0R_H

#include "lost/gl/gl.h"
#include "lost/gl/Draw.h"
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
#include <boost/shared_ptr.hpp>
#include "lost/gl/FrameBuffer.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/Texture.h"
#include "lost/gl/PushAttrib.h"
#include "lost/math/Rect.h"

struct Buff0r
{
  lost::common::FpsMeter  fpsMeter;
  lost::math::Vec3        eye;
  lost::math::Vec3        at;
  lost::math::Vec3        up;
  float                   fovy;
  float                   znear;
  float                   zfar;
  float                   passed;

  boost::shared_ptr<lost::gl::ShaderProgram>      program;
  boost::shared_ptr<lost::gl::FrameBuffer> frameBuffer;
  boost::shared_ptr<lost::gl::Texture> depthTexture;
  boost::shared_ptr<lost::gl::Texture> colorTexture;
  int frameBufferWidth;
  int frameBufferHeight;
  boost::shared_ptr<lost::resource::Loader> loader;
  
  Buff0r(lost::common::DisplayAttributes& inDisplayAttributes, boost::shared_ptr<lost::resource::Loader> inLoader)
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
  }

  void init(boost::shared_ptr<lost::event::Event> event)
  {
    frameBuffer.reset(new lost::gl::FrameBuffer);
    depthTexture.reset(new lost::gl::Texture);
    colorTexture.reset(new lost::gl::Texture);

    frameBufferInit();
    shaderInit();
    if(GLEE_EXT_framebuffer_object)
    {  DOUT("Framebuffer objects OK");}
    else
      DOUT("NO framebuffer objects");
  }
  
  void frameBufferInit()
  {
    frameBufferWidth = 512;
    frameBufferHeight = 512;

    frameBuffer->enable();

    depthTexture->bind();
    depthTexture->reset(0,GL_DEPTH_COMPONENT32,frameBufferWidth, frameBufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    frameBuffer->attachDepth(*depthTexture);

    colorTexture->bind();
    colorTexture->reset(0,GL_RGBA8,frameBufferWidth, frameBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    frameBuffer->attachColor(0, *colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_NEAREST to switch off filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int clampparam = GL_CLAMP; // clamp the texture, don't repeat it if the coords are larger than 1s
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampparam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampparam);

    if(!frameBuffer->status() == GL_FRAMEBUFFER_COMPLETE_EXT)
      throw std::runtime_error("FrameBuffer status: "+lost::gl::utils::enum2string(frameBuffer->status()));
    frameBuffer->disable();
  }

  void shaderInit()
  {
    program = lost::gl::loadShader(loader, "lattice");
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
    (*program)["LightPosition"] = lost::math::Vec3(0,5,-5);
    (*program)["LightColor"]    = lost::common::Color(1,1,1);
    (*program)["EyePosition"]   = eye;
    (*program)["Specular"]      = lost::common::Color(1,1,.1);
    (*program)["Ambient"]       = lost::common::Color(.1,.5,1);
    (*program)["Kd"]            = 0.8f;
    (*program)["Scale"]         = lost::math::Vec2(0.7, 3.7);
    (*program)["Threshold"]     = lost::math::Vec2(.3, .2);
    (*program)["SurfaceColor"]  = lost::common::Color(1,.1,.1);
    program->disable();
  }

  void resetViewPort(boost::shared_ptr<lost::application::ResizeEvent> event)
  {
    glViewport(0, 0, event->width-1, event->height-1);GLDEBUG;
    lost::gl::utils::set3DProjection(displayAttributes.width, displayAttributes.height, eye, at, up, fovy, znear, zfar);
  }

  void keyboard(boost::shared_ptr<lost::application::KeyEvent> inEvent )
  {
    switch (inEvent->key)
    {
      case lost::application::K_ESCAPE :
        if (!inEvent->pressed)
        {
          quit();
        }
        break;
        default :
        break;
    }
  }

  void redraw(boost::shared_ptr<lost::application::TimerEvent> event)
  {
      glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
      glEnable(GL_DEPTH_TEST);GLDEBUG;
      glEnable(GL_BLEND);GLDEBUG;
      glDisable(GL_TEXTURE_2D);GLDEBUG;
    // enable framebuffer and set viewport
    {
      frameBuffer->enable();
      glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
      lost::gl::PushAttrib pushAttrib(GL_VIEWPORT_BIT);
      glViewport(0, 0, frameBufferWidth, frameBufferHeight);
      
      lost::gl::utils::set3DProjection(frameBufferWidth, frameBufferHeight, eye, at, up, fovy, znear, zfar);
      glColor3f(1, 1, 0);
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
      frameBuffer->disable();
    }
    
  

    // this leaves projection matrix on
    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(displayAttributes.width, displayAttributes.height));
    glMatrixMode(GL_MODELVIEW);GLDEBUG; // whis is why we need to switch back to modelview here
    glLoadIdentity();GLDEBUG;
    glDisable(GL_DEPTH_TEST);GLDEBUG;

    glEnable(GL_TEXTURE_2D);GLDEBUG;

//    glRotatef(15, 0,0,1);
    glColor3f(1, 1, 1);
    lost::math::Rect rect(50, 50, 512, 512);
    colorTexture->bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);glVertex2f(rect.x, rect.y);
    glTexCoord2f(1, 0);glVertex2f(rect.x+rect.width-1, rect.y);
    glTexCoord2f(1, 1);glVertex2f(rect.x+rect.width-1, rect.y+rect.height-1);
    glTexCoord2f(0, 1);glVertex2f(rect.x, rect.y+rect.height-1);
    glEnd();

    glTranslatef(50, -100, 0);

    glColor4f(1, 1, 1, .5);
    rect = lost::math::Rect(60, 50, 512, 512);
    colorTexture->bind();
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);glVertex2f(rect.x, rect.y);
    glTexCoord2f(1, 0);glVertex2f(rect.x+rect.width-1, rect.y);
    glTexCoord2f(1, 1);glVertex2f(rect.x+rect.width-1, rect.y+rect.height-1);
    glTexCoord2f(0, 1);glVertex2f(rect.x, rect.y+rect.height-1);
    glEnd();

    glDisable(GL_TEXTURE_2D);GLDEBUG;

    glLoadIdentity();GLDEBUG;
    fpsMeter.render(1, 0, event->passedSec);
    glfwSwapBuffers();
  }

  lost::common::DisplayAttributes& displayAttributes;
  boost::signal<void(void)> quit;
};

#endif
