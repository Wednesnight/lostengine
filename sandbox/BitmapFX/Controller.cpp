#include "Controller.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/application/Application.h"
#include "lost/event/Receive.h"

#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/bitmap/Packer.h"

using namespace std;
using namespace boost;
using namespace lost;
using namespace lost::bitmap;
using namespace lost::font;
using namespace lost::common;
using namespace lost::resource;
using namespace lost::application;
using namespace lost::math;
using namespace lost::event;
using namespace lost::gl;
using namespace lost::gl::utils;

Controller::Controller()
{
  app = Application::create(bind(&Controller::runLoop, this));
  
  window = app->createWindow("window", WindowParams("BitmapFX", lost::math::Rect(100, 100, 800, 600)));
  
  app->addEventListener(KeyEvent::KEY_DOWN(), receive<KeyEvent>(bind(&Controller::keyboard, this, _1)));
  app->addEventListener(KeyEvent::KEY_UP(), receive<KeyEvent>(bind(&Controller::keyboard, this, _1)));

  fpsMeter.reset(new FpsMeter());
  
  renderState = State::create(Texture2D::create(true), Blend::create(true), BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), 
                              ClearColor::create(blackColor), DepthTest::create(false), AlphaTest::create(false), NormalArray::create(false), 
                              VertexArray::create(true), TextureArray::create(true));
  
  shared_ptr<freetype::Library> ftlib(new freetype::Library);
  shared_ptr<File> file = app->loader->load("Arial Unicode.ttf");//"suigeneris.ttf");
  shared_ptr<freetype::Face> fnt(new freetype::Face(ftlib, file));
  ttf.reset(new TrueTypeFont(ftlib, file));  
  ttf->atlasSize = Vec2(512,512);
  fontSize = 22;//appInstance->config["fontSize"].as<uint32_t>();
  string text ="guiroは驚くばかりである!";
  renderedText1 = ttf->render(text, 16);
  DOUT("16pt string size: "<<renderedText1->size);
  renderedText2 = ttf->render(text, 24);
  DOUT("24pt string size: "<<renderedText2->size);
  renderedText3 = ttf->render(text, 32);
  DOUT("32pt string size: "<<renderedText3->size);    
}

void Controller::run()
{
  app->run();
}

void Controller::keyboard( shared_ptr<KeyEvent> event )
{
  switch (event->key)
  {
    case K_ESCAPE :
      if (!event->pressed) app->quit();
      break;
    case K_SPACE :
      break;
    default :
      break;
  }
}

void Controller::render(const shared_ptr<Canvas>& canvas)
{
  canvas->context->makeCurrent();
  canvas->context->pushState(renderState);

  canvas->camera->apply();
  canvas->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  canvas->setColor(whiteColor);
  canvas->drawRectTextured(Rect(350,150,ttf->atlas->width, ttf->atlas->height), ttf->atlas, false);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0,10,0);
  canvas->setColor(redColor);
  renderedText1->render(canvas);

  glTranslatef(0,30,0);
  canvas->setColor(Color(0,1,0,0.75));
  renderedText2->render(canvas);

  glTranslatef(0,60,0);
  canvas->setColor(Color(0,0,1,0.5));
  renderedText3->render(canvas);
  
  glLoadIdentity();
  fpsMeter->render(canvas->camera->viewport.width - fpsMeter->width, 0, canvas, 1/100);

  canvas->context->popState();
  canvas->context->swapBuffers();
}

void Controller::runLoop()
{
  render(window->canvas);
}
