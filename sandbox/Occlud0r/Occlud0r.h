#ifndef OCCLUD0R_H
#define OCCLUD0R_H

#include "lost/gl/gl.h"
#include "lost/gl/Draw.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"
#include "lost/math/Vec2.h"
#include "lost/application/Timer.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/engine/KeySym.h"
#include "lost/common/FpsMeter.h"
#include "lost/gl/ShaderHelper.h"
#include "lost/gl/FrameBuffer.h"
#include "lost/gl/RenderBuffer.h"
#include "lost/gl/Texture.h"
#include "lost/gl/PushAttrib.h"
#include "lost/math/Rect.h"

struct Occlud0r
{
  lost::common::FpsMeter  fpsMeter;
  lost::math::Vec3        eye;
  lost::math::Vec3        at;
  lost::math::Vec3        up;
  float                   fovy;
  float                   znear;
  float                   zfar;
  float                   passed;

  lost::shared_ptr<lost::gl::ShaderProgram>      lightingShader;
  lost::shared_ptr<lost::gl::ShaderProgram>      ssaoShader;
  lost::gl::FrameBuffer frameBuffer;
  lost::gl::Texture depthTexture;
  lost::gl::Texture colorTexture;
  int frameBufferWidth;
  int frameBufferHeight;

  lost::gl::FrameBuffer ssaoFrameBuffer;
  lost::gl::Texture ssaoTexture;

  Occlud0r(lost::common::DisplayAttributes& inDisplayAttributes)
  : eye(0,0,-5),
    at(0,0,0),
    up(0,1,0),
    displayAttributes(inDisplayAttributes)
  {
    fovy = 90;
    znear = 1;
    zfar = 500;
    passed = 0;
    
    frameBufferInit();
    lightingShaderInit();
    ssoaShaderInit();
  }

  void frameBufferInit()
  {
    frameBufferWidth = 640;
    frameBufferHeight = 480;

    frameBuffer.enable();

    depthTexture.bind();
    depthTexture.reset(0,GL_DEPTH_COMPONENT32,frameBufferWidth, frameBufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);GLDEBUG_THROW; // GL_NEAREST to switch off filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);GLDEBUG_THROW;
    int clampparam = GL_CLAMP; // clamp the texture, don't repeat it if the coords are larger than 1s
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampparam);GLDEBUG_THROW;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampparam);GLDEBUG_THROW;
    frameBuffer.attachDepth(depthTexture);

    colorTexture.bind();
    colorTexture.reset(0,GL_RGBA8,frameBufferWidth, frameBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    frameBuffer.attachColor(0, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);GLDEBUG_THROW; // GL_NEAREST to switch off filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);GLDEBUG_THROW;
    clampparam = GL_CLAMP; // clamp the texture, don't repeat it if the coords are larger than 1s
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampparam);GLDEBUG_THROW;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampparam);GLDEBUG_THROW;

    
    if(!frameBuffer.status() == GL_FRAMEBUFFER_COMPLETE_EXT)
      throw std::runtime_error("FrameBuffer status: "+lost::gl::utils::enum2string(frameBuffer.status()));
    frameBuffer.disable();
    
    // SSOA setup
    ssaoFrameBuffer.enable();
    ssaoTexture.bind();
    ssaoTexture.reset(0,GL_RGBA8,frameBufferWidth, frameBufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);GLDEBUG_THROW; // GL_NEAREST to switch off filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);GLDEBUG_THROW;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampparam);GLDEBUG_THROW;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampparam);GLDEBUG_THROW;
    ssaoFrameBuffer.attachColor(0, ssaoTexture);
    if(!ssaoFrameBuffer.status() == GL_FRAMEBUFFER_COMPLETE_EXT)
      throw std::runtime_error("FrameBuffer status: "+lost::gl::utils::enum2string(ssaoFrameBuffer.status()));
    ssaoFrameBuffer.disable();
  }

  void lightingShaderInit()
  {
    lightingShader = lost::gl::loadShader("lighting");
    lightingShader->enable();
    lightingShader->validate();
    if(!lightingShader->validated())
    {
      DOUT("Problem found during validation: \n"<<lightingShader->log())
    }
    else
    {
      DOUT("Program validated OK");
    }
    (*lightingShader)["LightPosition"] = lost::math::Vec3(0,5,-5);
    (*lightingShader)["LightColor"]    = lost::common::Color(1,1,1);
    (*lightingShader)["EyePosition"]   = eye;

/*    (*lightingShader)["Specular"]      = lost::common::Color(1,1,.1);
    (*lightingShader)["Ambient"]       = lost::common::Color(.1,.5,1);
    (*lightingShader)["SurfaceColor"]  = lost::common::Color(1,.1,.1);*/


    (*lightingShader)["Specular"]      = lost::common::Color(1,1,1);
    (*lightingShader)["Ambient"]       = lost::common::Color(1,.1,.1);
    (*lightingShader)["SurfaceColor"]  = lost::common::Color(1,1,.1);

    (*lightingShader)["Kd"]            = 0.8f;
    lightingShader->disable();
  }

  void ssoaShaderInit()
  {
    ssaoShader = lost::gl::loadShader("ssao1");
    ssaoShader->enable();
    ssaoShader->validate();
    if(!ssaoShader->validated())
    {
      DOUT("Problem found during validation: \n"<<ssaoShader->log())
    }
    else
    {
      DOUT("Program validated OK");
    }
    ssaoShader->disable();
  }

  void resetViewPort(int width, int height)
  {
    glViewport(0, 0, width-1, height-1);GLDEBUG;
    lost::gl::utils::set3DProjection(displayAttributes.width, displayAttributes.height, eye, at, up, fovy, znear, zfar);
  }

  void keyboard( const lost::event::KeyEvent& inEvent )
  {
    switch (inEvent.key)
    {
      case lost::engine::K_ESCAPE :
        if (!inEvent.pressed)
        {
          quit();
        }
        break;
      case lost::engine::K_A:zfar++;DOUT("++");break;
      case lost::engine::K_S:zfar--;DOUT("--");break;
      case lost::engine::K_F1:
        if(inEvent.pressed)
        {
          DOUT("screenshot start");
          std::ostringstream os;
          unsigned long timeMsec = (glfwGetTime() * 1000.0);
          os << "screenshot-"<<timeMsec<<".tga";
          lost::gl::utils::saveScreenshotTGA(displayAttributes, "/Users/tony/"+os.str());
          DOUT("screenshot end");
        };
        break;
      default :
        break;
    }
  }

  void redraw(double deltaSec, lost::application::Timer* timer)
  {
      glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
      glEnable(GL_DEPTH_TEST);GLDEBUG;
      glEnable(GL_BLEND);GLDEBUG;
      glDisable(GL_TEXTURE_2D);GLDEBUG;
    // enable framebuffer and set viewport
    {
      frameBuffer.enable();
      glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );GLDEBUG;
      lost::gl::PushAttrib pushAttrib(GL_VIEWPORT_BIT);
      glViewport(0, 0, frameBufferWidth, frameBufferHeight);
      
      lost::gl::utils::set3DProjection(frameBufferWidth, frameBufferHeight, eye, at, up, fovy, znear, zfar);
      glColor3f(0,.8,.4);
      glBegin(GL_QUADS);
      glVertex2f(0,-frameBufferHeight);
      glVertex2f(4*frameBufferWidth,-frameBufferHeight);
      glVertex2f(4*frameBufferWidth,frameBufferHeight);
      glVertex2f(0,frameBufferHeight);
      glEnd();
      lightingShader->enable();
      glColor3f(1,1,0);
      passed += deltaSec;
      glMatrixMode(GL_MODELVIEW);GLDEBUG;
      glLoadIdentity();GLDEBUG;
      glTranslatef(1, -1, 0);
      glRotatef(180*sin(passed/4), .3,1,-.4);
      glutSolidTeapot(2);

      glLoadIdentity();GLDEBUG;
      glTranslatef(3, 0, 0);GLDEBUG;
      glRotatef(180*sin(passed), .2,1,0);
      glutSolidCube(1);

      lightingShader->disable();
      frameBuffer.disable();
    }
    
    // draw the stuff to the ssoa framebuffer
    {
//      ssaoFrameBuffer.enable();
      glClearColor( 0.0, 0.0, 0.0, 0.0 );GLDEBUG;
      glClear( GL_COLOR_BUFFER_BIT);GLDEBUG; // no depth buffer present in this framebuffer
      lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(displayAttributes.width, displayAttributes.height)); // framebuffer is same width as window
      glMatrixMode(GL_MODELVIEW);GLDEBUG; // whis is why we need to switch back to modelview here
      glLoadIdentity();GLDEBUG;
      glDisable(GL_DEPTH_TEST);GLDEBUG;// we'Re 2d from here on, so we need to disable depth test for rendering
      glDisable(GL_TEXTURE_2D);GLDEBUG;
      glColor3f(1, 1, 1); // just to be on the safe side, shouldn't really matter
      ssaoShader->enable();
      glActiveTexture(GL_TEXTURE0);
      depthTexture.bind();
      glActiveTexture(GL_TEXTURE1);
      colorTexture.bind();
      (*ssaoShader)["camerarange"] = lost::math::Vec2(znear, zfar);
      (*ssaoShader)["screensize"] = lost::math::Vec2(displayAttributes.width, displayAttributes.height);
      (*ssaoShader)["texture0"] = (GLuint)0;
      (*ssaoShader)["texture1"] = (GLuint)1;
      
      lost::math::Rect rect(0, 0, displayAttributes.width, displayAttributes.height);
      glBegin(GL_QUADS);
      glTexCoord2f(0, 0);glVertex2f(rect.x, rect.y);
      glTexCoord2f(1, 0);glVertex2f(rect.x+rect.width-1, rect.y);
      glTexCoord2f(1, 1);glVertex2f(rect.x+rect.width-1, rect.y+rect.height-1);
      glTexCoord2f(0, 1);glVertex2f(rect.x, rect.y+rect.height-1);
      glEnd();
      ssaoShader->disable();      
//      ssaoFrameBuffer.disable();      
    }

    // draw the ssao result to the screen
    // this leaves projection matrix on
/*    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(displayAttributes.width, displayAttributes.height));
    glMatrixMode(GL_MODELVIEW);GLDEBUG; // whis is why we need to switch back to modelview here
    glLoadIdentity();GLDEBUG;
    glDisable(GL_DEPTH_TEST);GLDEBUG;// we'Re 2d from here on, so we need to disable depth test for rendering
    glEnable(GL_TEXTURE_2D);GLDEBUG;
    glColor3f(1,1,1);*/
  //  lost::gl::drawRectTextured(lost::math::Rect(0, 0, frameBufferWidth, frameBufferHeight), ssaoTexture);


/*    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(displayAttributes.width, displayAttributes.height)); // framebuffer is same width as window
    glMatrixMode(GL_MODELVIEW);GLDEBUG; // whis is why we need to switch back to modelview here
    glLoadIdentity();GLDEBUG;
    glEnable(GL_TEXTURE_2D);
    lost::gl::drawRectTextured(lost::math::Rect(0, 0, frameBufferWidth, frameBufferHeight), colorTexture);*/
    
    glDisable(GL_TEXTURE_2D);GLDEBUG;
    glDisable(GL_DEPTH_TEST);GLDEBUG;// we'Re 2d from here on, so we need to disable depth test for rendering

    glMatrixMode(GL_MODELVIEW);GLDEBUG; // whis is why we need to switch back to modelview here
    glLoadIdentity();GLDEBUG;
    lost::gl::utils::set2DProjection(lost::math::Vec2(0,0), lost::math::Vec2(displayAttributes.width, displayAttributes.height)); // framebuffer is same width as window
    fpsMeter.render(1, 0, deltaSec);
    glfwSwapBuffers();
  }

  lost::common::DisplayAttributes& displayAttributes;
  boost::signal<void(void)> quit;
};

#endif
