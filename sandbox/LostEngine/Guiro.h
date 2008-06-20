#ifndef GUIRO_H
#define GUIRO_H

#include <sstream>
#include "Visual.h"
#include "lost/fonts/BitmapFont.h"
#include "lost/image/ImageLoader.h"
#include "lost/guiro/UserInterface.h"
#include "lost/guiro/ResourceLoader.h"
#include "lost/guiro/Window.h"
#include "lost/guiro/Image.h"
#include "lost/guiro/Button.h"
#include "lost/guiro/Label.h"
#include "lost/guiro/ProgressBar.h"
#include "lost/guiro/Slider.h"
#include "lost/guiro/Edit.h"

struct Guiro : public Visual
{
  const lost::DisplayAttributes&                display;
  boost::shared_ptr<lost::guiro::UserInterface> ui;
  boost::shared_ptr<lost::guiro::ProgressBar>   progress;
  double                                        progressTime;
  lost::guiro::ResourceLoader                   resourceLoader;
  boost::shared_ptr<lost::guiro::Label>         sliderValue;

  Guiro(const lost::DisplayAttributes& attr, lost::guiro::ResourceLoader& inResourceLoader)
  : display(attr), progressTime(0), resourceLoader(inResourceLoader)
  {
    ui.reset( new lost::guiro::UserInterface( "Guiro", display ) );

    boost::shared_ptr<lost::guiro::Window> window;
    boost::shared_ptr<lost::guiro::View>   title;
    boost::shared_ptr<lost::guiro::View>   titleDrag;
    boost::shared_ptr<lost::guiro::View>   content;
    boost::shared_ptr<lost::guiro::Label>  label;
    boost::shared_ptr<lost::guiro::Button> button;
    boost::shared_ptr<lost::guiro::Button> button2;

    boost::shared_ptr<lost::guiro::Slider> slider;
    boost::shared_ptr<lost::guiro::Image>  sliderBackground;
    boost::shared_ptr<lost::guiro::Image>  sliderHandle;

    boost::shared_ptr<lost::guiro::Edit>  edit;
    boost::shared_ptr<lost::guiro::Image> image;

    boost::shared_ptr<lost::fonts::Font> font1;
    boost::shared_ptr<lost::fonts::Font> font2;

    font1.reset(new lost::fonts::BitmapFont(GLUT_BITMAP_8_BY_13));
    font2 = resourceLoader.loadFont("suigeneris.ttf", 18);

    window.reset( new lost::guiro::Window( "GuiroDialog" ) );
    window->rect.reset( 100, 100, 440, 280 );
    window->color = lost::whiteColor;

    title.reset( new lost::guiro::View( "TitleBar" ) );
    title->rect.reset( 1, 253, 438, 25 );
    title->color = lost::blackColor;
    titleDrag.reset( new lost::guiro::View( "TitleOverLabelDrag" ) );
    titleDrag->rect.reset( 0, 0, 438, 25 );
    label.reset( new lost::guiro::Label( "TitleLabel" ) );
    label->rect.reset( 5, 10, 200, 10 );
    label->font = font1;
    label->setCaption("GUIIIIIIROOOOOO!!!!");
    label->setFontColor( lost::whiteColor );
    button.reset( new lost::guiro::Button( "TitleButton1" ) );
    button->rect.reset( 410, 2, 20, 20 );
    button2.reset( new lost::guiro::Button( "TitleButton2" ) );
    button2->rect.reset( 380, 2, 20, 20 );

    content.reset( new lost::guiro::View( "Content" ) );
    content->rect.reset( 0, 0, 440, 250 );

    title->add( titleDrag );
    title->add( label );
    titleDrag->add( button );
    titleDrag->add( button2 );

    window->setTitleView( title );
    window->setContentView( content );
    window->registerDragView( titleDrag );

    progress.reset( new lost::guiro::ProgressBar( "Progress1" ) );
    progress->rect.reset( 200, 100, 200, 25 );
    progress->color    = lost::redColor;
    progress->stepSize = 5;
    window->add( progress );

    sliderValue.reset( new lost::guiro::Label( "Slider1_label" ) );
    sliderValue->rect.reset( 200, 200, 100, 25 );
    sliderValue->font = font1;
    sliderValue->setCaption("0");
    sliderValue->color = lost::blackColor;
    window->add( sliderValue );

    slider.reset( new lost::guiro::Slider( "Slider1" ) );
    slider->rect.reset( 200, 150, 200, 25 );
    // set stepping / non-stepless
    slider->steps = 10;
    sliderBackground.reset( new lost::guiro::Image( "Slider1_background" ) );
    sliderBackground->image = resourceLoader.loadImage( "SliderBackground.jpg" );
    sliderBackground->rect.reset( 0, 0, 200, 25 );
    sliderHandle.reset( new lost::guiro::Image( "Slider1_image" ) );
    sliderHandle->image = resourceLoader.loadImage( "nomnomnom.jpg" );
    sliderHandle->rect.reset( 0, 1, 50, 23 );
    slider->setBackground( sliderBackground );
    slider->setHandle( sliderHandle );
    slider->valueChanged.connect(boost::bind(&Guiro::Slider1Changed, this, _1));
    window->add( slider );

    edit.reset( new lost::guiro::Edit( "Edit1", "", font1 ) );
    edit->rect.reset( 200, 50, 200, 20 );
    window->add( edit );

    image.reset( new lost::guiro::Image( "BeforeEditImage1" ) );
    image->image = resourceLoader.loadImage( "nomnomnom.jpg" );
    image->rect.reset( 25, 50, image->image->width, image->image->height );
    image->setDraggable( true );
    window->add( image );

    ui->add( window );
  }

  ~Guiro()
  {
  }

  void Slider1Changed( const float value )
  {
    std::ostringstream stream;
    stream << value;
    sliderValue->caption = stream.str();
  }

  void redraw(double timeSinceLastCallSec)
  {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    progressTime += timeSinceLastCallSec;
    if (progressTime >= .5)
    {
      progressTime = 0;
      progress->step();
    }

    ui->update( timeSinceLastCallSec );
    ui->draw();
  }

  virtual void keyboard( int key, bool pressed )
  {
    ui->keyboard( key, pressed );
  }

  virtual void mouseButton( int button, bool pressed, lost::math::Vec2 point )
  {
    point.y = display.height - point.y;
    ui->mouseButton( button, pressed, point );
  }

  virtual void mouseMove( lost::math::Vec2 point )
  {
    point.y = display.height - point.y;
    ui->mouseMove( point );
  }

};

#endif
