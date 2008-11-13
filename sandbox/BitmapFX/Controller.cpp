#include "Controller.h"
#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/resource/DefaultLoader.h"
#include "lost/application/Application.h"
#include "lost/application/KeySym.h"

#include "lost/font/freetype/Library.h"
#include "lost/font/freetype/Face.h"
#include "lost/font/Engine.h"
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
using namespace lost::application;

Controller::Controller()
{
}

void Controller::renderCharacterBitmaps()
{
  characterBitmaps.clear();
  shared_ptr<freetype::Library> ftlib(new freetype::Library);
  shared_ptr<File> file = appInstance->loader->load("Vera.ttf");
  shared_ptr<freetype::Face> fnt(new freetype::Face(ftlib, file));
  
  uint8_t maxc = 254;
  for(uint8_t c=0; c<=maxc; ++c)
  {
    shared_ptr<Bitmap> characterBitmap = Engine::renderGlyphToBitmap(fnt, fontSize, c);    
    characterBitmaps.push_back(characterBitmap);
  }
  DOUT("created "<<characterBitmaps.size()<<" bitmaps");
}

void Controller::updateCharacterAtlas()
{
  renderCharacterBitmaps();
  characterAtlasSize = Rect(0,0,256,256);
  bitmapPacker.pack(packerResult,
                    Vec2(characterAtlasSize.width,characterAtlasSize.height),
                    characterBitmaps);
  if(packerResult.packedBitmap)
    characterTexture.reset(new Texture(packerResult.packedBitmap));
  else
    characterTexture.reset();
}

void Controller::init(shared_ptr<Event> event)
{
  context = appInstance->context;
  fpsMeter.reset(new FpsMeter(context));

  renderState = appInstance->context->copyState();
  renderState->texture2D = true;
  renderState->blend = true;
  renderState->blendSrc = GL_SRC_ALPHA;
  renderState->blendDest = GL_ONE_MINUS_SRC_ALPHA;
  renderState->clearColor = blackColor;
  renderState->depthTest = false;  
  renderState->alphaTest = false;  
  renderState->normalArray = false;  
  renderState->vertexArray = true;  
  renderState->textureCoordArray = true;  

  pic.reset(new Bitmap(640, 480, Bitmap::COMPONENTS_RGBA));
  pic->clear(blackColor);
  pic->hline(50, 0, pic->width, redColor);
//  pic->write("/Users/tony/test.tga");

  shared_ptr<freetype::Library> ftlib(new freetype::Library);
  shared_ptr<File> file = appInstance->loader->load("Vera.ttf");
  shared_ptr<freetype::Face> fnt(new freetype::Face(ftlib, file));
  
  fontSize = appInstance->config["fontSize"].as<uint32_t>();
//  createCharacterMap();
  updateCharacterAtlas();
}

void Controller::keyboard( shared_ptr<KeyEvent> event )
{
  switch (event->key)
  {
    case K_ESCAPE :
      if (!event->pressed) appInstance->quit();
      break;
    case K_UP:
      if(event->pressed)
      {
        fontSize++;
        DOUT("FONTSIZE: "<<fontSize);
        updateCharacterAtlas();
      }
      break;
    case K_DOWN:
      if(event->pressed)
      {
        fontSize--;
        DOUT("FONTSIZE: "<<fontSize);
        updateCharacterAtlas();
      }
      break;
    case K_SPACE :
      break;
    default :
      break;
  }
}

void Controller::redraw(shared_ptr<TimerEvent> event)
{
  glViewport(0, 0, display.width, display.height);GLDEBUG;
  context->set2DProjection(Vec2(0, 0), Vec2(appInstance->displayAttributes->width, appInstance->displayAttributes->height));
  context->pushState(renderState);
  context->clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  if(characterTexture)
  {
    context->setColor(whiteColor);
    context->drawRectTextured(Rect(0,0,characterTexture->width, characterTexture->height), characterTexture);
  //  context->drawRectTextured(Rect(50,50,tex2->width, tex2->height), tex2);
    context->popState();
  }

  fpsMeter->render( appInstance->displayAttributes->width - (fpsMeter->width + 10), 0, event->passedSec );
  appInstance->swapBuffers();
//  context->writeScreenshot("/Users/tony/screenshot.tga");
//  appInstance->quit();
}
