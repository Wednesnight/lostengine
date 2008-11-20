#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <boost/shared_ptr.hpp>
#include "lost/common/DisplayAttributes.h"
#include "lost/common/FpsMeter.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/TimerEvent.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/gl/Texture.h"

#include "lost/bitmap/Packer.h"

#include "lost/font/TrueTypeFont.h"

struct Controller 
{
  lost::common::DisplayAttributes                 display;
  boost::shared_ptr<lost::common::FpsMeter>       fpsMeter;
  boost::shared_ptr<lost::gl::Context>            context;
  boost::shared_ptr<lost::gl::State>              renderState;

  boost::shared_ptr<lost::bitmap::Bitmap>         pic;
  boost::shared_ptr<lost::bitmap::Bitmap>         loadedPic;
  boost::shared_ptr<lost::gl::Texture>            tex;
  boost::shared_ptr<lost::gl::Texture>            tex2;

  std::vector<boost::shared_ptr<lost::bitmap::Bitmap> > characterBitmaps;
  boost::shared_ptr<lost::gl::Texture>            characterTexture;
  lost::math::Rect                                characterAtlasSize;
  lost::bitmap::Packer                            bitmapPacker;
  lost::bitmap::Packer::Result                    packerResult;
  
  boost::shared_ptr<lost::font::TrueTypeFont>     ttf;
  boost::shared_ptr<lost::font::Model>            renderedText1;
  boost::shared_ptr<lost::font::Model>            renderedText2;
  boost::shared_ptr<lost::font::Model>            renderedText3;
  
  void renderCharacterBitmaps(); // renders a character range for the given size into the characterBitmaps array
  void updateCharacterAtlas();

  uint32_t fontSize;

  Controller();
  void init(boost::shared_ptr<lost::event::Event> event);
  void keyboard(boost::shared_ptr<lost::application::KeyEvent> event );
  void redraw(boost::shared_ptr<lost::application::TimerEvent> event);

};

#endif