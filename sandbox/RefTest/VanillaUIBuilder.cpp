#include "VanillaUIBuilder.h"

#include "lost/guiro/factory/VanillaFactory.h"

using namespace std;
using namespace boost;
using namespace lost::common;
using namespace lost::guiro;
using namespace lost::guiro::control;
using namespace lost::guiro::common;
using namespace lost::guiro::factory;

VanillaUIBuilder::VanillaUIBuilder(lost::shared_ptr<lost::common::Config> inConfig)
: UserInterfaceBuilder(inConfig)
{
}


shared_ptr<ControlFactory> VanillaUIBuilder::createControlFactory(lost::shared_ptr<Config> inConfig)
{
  return shared_ptr<VanillaFactory>(new VanillaFactory(inConfig));
}

void VanillaUIBuilder::addControls(lost::shared_ptr<UserInterface> ui, lost::shared_ptr<ControlFactory> factory)
{
  // demo objects
  shared_ptr<Button>     simpleButton;
  shared_ptr<Label>      simpleButtonLabel;
  shared_ptr<Button>     stretchedButton;
  shared_ptr<Label>      stretchedButtonLabel;
  shared_ptr<Button>     stickyButton;
  shared_ptr<Label>      stickyButtonLabel;
  shared_ptr<Button>     disabledButton;
  shared_ptr<Label>      disabledButtonLabel;
  shared_ptr<Button>     invisibleButton;
  shared_ptr<Label>      invisibleButtonLabel;
  shared_ptr<Slider>     slider;
  shared_ptr<Label>      sliderLabel;
  shared_ptr<Slider>     steppedSlider;
  shared_ptr<Label>      steppedSliderLabel;
  shared_ptr<Slider>     sliderVertical;
  shared_ptr<Label>      sliderVerticalLabel;
  shared_ptr<Slider>     steppedSliderVertical;
  shared_ptr<Label>      steppedSliderVerticalLabel;
  shared_ptr<Scrollbar>  scrollbar;
  shared_ptr<Label>      scrollbarLabel;
  shared_ptr<ScrollView> scrollView;

  simpleButtonLabel = factory->createLabel("simpleButtonLabel");
  simpleButtonLabel->setArea( 5, 465, 100, 25 );
  simpleButtonLabel->caption( "Button" );
  simpleButtonLabel->fontColor( whiteColor );
  simpleButton = factory->createButton( "simpleButton", "simpleButtonTheme" );
  simpleButton->setArea( 5, 420, 100, 40 );

  stretchedButtonLabel = factory->createLabel("stretchedButtonLabel");
  stretchedButtonLabel->setArea( 260, 465, 100, 25 );
  stretchedButtonLabel->caption( "Stretched button" );
  stretchedButtonLabel->fontColor( whiteColor );
  stretchedButton = factory->createButton( "stretchedButton", "simpleButtonTheme" );
  stretchedButton->setArea( 260, 440, 50, 20 );

  stickyButtonLabel = factory->createLabel("stickyButtonLabel");
  stickyButtonLabel->setArea( 5, 405, 100, 25 );
  stickyButtonLabel->caption( "Sticky button" );
  stickyButtonLabel->fontColor( whiteColor );
  stickyButton = factory->createStickyButton( "stickyButton", "stickyButtonTheme" );
  stickyButton->setArea( 5, 360, 100, 40 );

  disabledButtonLabel = factory->createLabel("disabledButtonLabel");
  disabledButtonLabel->setArea( 5, 335, 100, 25 );
  disabledButtonLabel->caption( "Disabled button" );
  disabledButtonLabel->fontColor( whiteColor );
  disabledButton = factory->createButton( "disabledButton", "simpleButtonTheme" );
  disabledButton->setArea( 5, 290, 100, 40 );
  disabledButton->setEnabled( false );

  invisibleButtonLabel = factory->createLabel("invisibleButtonLabel");
  invisibleButtonLabel->setArea( 5, 265, 100, 25 );
  invisibleButtonLabel->caption( "Invisible button" );
  invisibleButtonLabel->fontColor( whiteColor );
  invisibleButton = factory->createButton( "invisibleButton", "invisibleButtonTheme" );
  invisibleButton->setArea( 5, 210, 150, 50 );

  sliderLabel = factory->createLabel("sliderLabel");
  sliderLabel->setArea( 5, 185, 250, 25 );
  sliderLabel->caption( "Slider (horizontal, updateAlways, min=0, max=100)" );
  sliderLabel->fontColor( whiteColor );
  slider = factory->createSlider( "slider", "sliderTheme" );
  slider->setArea( 5, 130, 150, 50 );

  steppedSliderLabel = factory->createLabel("steppedSliderLabel");
  steppedSliderLabel->setArea( 260, 185, 250, 25 );
  steppedSliderLabel->caption( "Stepped slider (horizontal, updateRelease, min=50, max=100, steps=10)" );
  steppedSliderLabel->fontColor( whiteColor );
  steppedSlider = factory->createSlider( "steppedSlider", "sliderTheme" );
  steppedSlider->setArea( 260, 130, 150, 50 );
  steppedSlider->minMax(50, 100);
  steppedSlider->steps(10);
  steppedSlider->updateMode(Slider::updateRelease);

  sliderVerticalLabel = factory->createLabel("sliderVerticalLabel");
  sliderVerticalLabel->setArea( 5, 105, 250, 25 );
  sliderVerticalLabel->caption( "Slider (vertical, updateRelease, min=-50, max=50)" );
  sliderVerticalLabel->fontColor( whiteColor );
  sliderVertical = factory->createSlider( "sliderVertical", "sliderVerticalTheme" );
  sliderVertical->setArea( 5, 0, 50, 100 );
  sliderVertical->layout(Slider::layoutVertical);
  sliderVertical->minMax(-50, 50);
  sliderVertical->updateMode(Slider::updateRelease);

  steppedSliderVerticalLabel = factory->createLabel("steppedSliderVerticalLabel");
  steppedSliderVerticalLabel->setArea( 260, 105, 250, 25 );
  steppedSliderVerticalLabel->caption( "Stepped slider (vertical, updateAlways, min=-100, max=0, steps=20)" );
  steppedSliderVerticalLabel->fontColor( whiteColor );
  steppedSliderVertical = factory->createSlider( "steppedSliderVertical", "sliderVerticalTheme" );
  steppedSliderVertical->setArea( 260, 0, 30, 100 );
  steppedSliderVertical->layout(Slider::layoutVertical);
  steppedSliderVertical->minMax(-100, 0);
  steppedSliderVertical->steps(20);

  scrollbarLabel = factory->createLabel("scrollbarLabel");
  scrollbarLabel->setArea( 260, 405, 100, 25 );
  scrollbarLabel->caption( "Scrollbar" );
  scrollbarLabel->fontColor( whiteColor );
  scrollbar = factory->createScrollbar( "scrollbar", "scrollbarTheme" );
  scrollbar->setArea( 260, 380, 100, 20 );

  scrollView = factory->createScrollView( "scrollView", "scrollViewTheme" );
  scrollView->setArea( make_area(xalignleft(), yalignbottom(), wrel(.5), hrel(.5)) );

/*
  ui->add( simpleButtonLabel );
  ui->add( simpleButton );
  ui->add( stretchedButtonLabel );
  ui->add( stretchedButton );
  ui->add( stickyButtonLabel );
  ui->add( stickyButton );
  ui->add( disabledButtonLabel );
  ui->add( disabledButton );
  ui->add( invisibleButtonLabel );
  ui->add( invisibleButton );
  ui->add( sliderLabel );
  ui->add( slider );
  ui->add( steppedSliderLabel );
  ui->add( steppedSlider );
  ui->add( sliderVerticalLabel );
  ui->add( sliderVertical );
  ui->add( steppedSliderVerticalLabel );
  ui->add( steppedSliderVertical );
  ui->add( scrollbarLabel );
  ui->add( scrollbar );
*/
  scrollView->add( simpleButtonLabel );
  scrollView->add( simpleButton );
  scrollView->add( stretchedButtonLabel );
  scrollView->add( stretchedButton );
  scrollView->add( stickyButtonLabel );
  scrollView->add( stickyButton );
  scrollView->add( disabledButtonLabel );
  scrollView->add( disabledButton );
  scrollView->add( invisibleButtonLabel );
  scrollView->add( invisibleButton );
  scrollView->add( sliderLabel );
  scrollView->add( slider );
  scrollView->add( steppedSliderLabel );
  scrollView->add( steppedSlider );
  scrollView->add( sliderVerticalLabel );
  scrollView->add( sliderVertical );
  scrollView->add( steppedSliderVerticalLabel );
  scrollView->add( steppedSliderVertical );
  scrollView->add( scrollbarLabel );
  scrollView->add( scrollbar );

  ui->setArea(make_area(xabs(0), yabs(0), wrel(1), hrel(.5)));
  ui->add( scrollView );
}
