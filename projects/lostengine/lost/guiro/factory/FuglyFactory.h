#ifndef LOST_GUIRO_FACTORY_FUGLYFACTORY_H
#define LOST_GUIRO_FACTORY_FUGLYFACTORY_H

#include <boost/shared_ptr.hpp>
#include "lost/object/ObjectReferenceResolver.h"
#include "lost/guiro/factory/FontFactory.h"
#include "lost/common/Config.h"
#include "lost/font/BitmapFont.h"
#include "lost/guiro/factory/ControlFactory.h"
#include "lost/guiro/factory/ThemeFactory.h"
#include "lost/guiro/factory/FontFactory.h"

namespace lost
{
namespace guiro
{
namespace factory
{
struct FuglyFactory : public ControlFactory
{
  boost::shared_ptr<lost::object::ObjectReferenceResolver> resolver;
  boost::shared_ptr<lost::guiro::factory::FontFactory>              fontFactory;
  boost::shared_ptr<lost::guiro::factory::ThemeFactory>             themeFactory;

  FuglyFactory(boost::shared_ptr<lost::common::Config> inConfig)
  {
    resolver.reset(new lost::object::ObjectReferenceResolver());
    fontFactory.reset(new lost::guiro::factory::FontFactory(inConfig));
    themeFactory.reset(new lost::guiro::factory::ThemeFactory(inConfig));
    resolver->registerFactory("font", fontFactory);
    resolver->registerFactory("theme", themeFactory);
    precache();
  }

  boost::shared_ptr<control::Label> createLabel( const std::string& inName )
  {
    boost::shared_ptr<control::Label> label(new control::Label(inName));
    label->font().type = "font";
    label->font().id   = "8x13";
    label->fontColor(lost::common::whiteColor);
    return label;
  }

  
  
  virtual lost::object::ObjectReferenceResolver& getResolver() { return *(resolver.get());}
  virtual boost::shared_ptr<control::Button> createButton( const std::string& inName, const std::string& themeId ) {throw std::runtime_error(std::string(__FUNCTION__)+"not implemented yet");}
  virtual boost::shared_ptr<control::Button> createStickyButton( const std::string& inName, const std::string& themeId ) {throw std::runtime_error(std::string(__FUNCTION__)+"not implemented yet");}
  virtual boost::shared_ptr<control::Edit> createEdit( const std::string& inName, const std::string& inBackgroundId ) {throw std::runtime_error(std::string(__FUNCTION__)+"not implemented yet");}
  virtual boost::shared_ptr<control::ShapeView> createShapeView( const std::string& inName)
  {
    return boost::shared_ptr<control::ShapeView>(new control::ShapeView(inName));
  }
  
  virtual boost::shared_ptr<control::Window>    createWindow( const std::string& inName, const std::string& themeId )
  {
    boost::shared_ptr<control::Window> result(new control::Window(inName));
    
    boost::shared_ptr<control::ShapeView> sv = createFilledRect("filledrect", lost::common::Color(1, 1, 0, 1));
    sv->setArea(make_area(common::xabs(0), common::yabs(0), common::wabs(50), common::habs(50)));
    result->add(sv);
    
    return result;
  }

  boost::shared_ptr<common::Theme> createButtonTheme( const std::string& themeId )
  {
    boost::shared_ptr<common::Theme> theme(new common::Theme());
    
    theme->view.reset(new control::View( themeId ));
    theme->view->add( createFilledRect( "ButtonPressed" , lost::common::Color(1, 0, 0, 1)));
    theme->view->add( createFilledRect( "ButtonReleased", lost::common::Color(0, 1, 0, 1)));
    theme->view->add( createFilledRect( "ButtonHover"   , lost::common::Color(0, 0, 1, 1)));
    theme->view->add( createFilledRect( "ButtonDisabled", lost::common::Color(.5, .5, .5, 1)));
    
    theme->properties[BUTTON_buttonOffReleased] = "ButtonReleased";
    theme->properties[BUTTON_buttonOffPressed] = "ButtonPressed";
    theme->properties[BUTTON_buttonOffHover] = "ButtonHover";
    theme->properties[BUTTON_buttonOffDisabled] = "ButtonDisabled";
    
    return theme;
  }
  
  virtual boost::shared_ptr<control::ShapeView> createFilledRect(const std::string& inName, const lost::common::Color& inFillColor)
  {
    boost::shared_ptr<control::ShapeView> result = createShapeView(inName);
    boost::shared_ptr<control::ShapeView::FilledRect> filledRect(new control::ShapeView::FilledRect(make_area(common::xabs(0), common::yabs(0), common::wrel(1), common::hrel(1))));
    filledRect->color = inFillColor;
    result->addShape(filledRect);
    
    return result;
  }

  virtual boost::shared_ptr<control::Image> createImage( const std::string& inName, const std::string& inId ) {throw std::runtime_error(std::string(__FUNCTION__)+"not implemented yet");}
  virtual boost::shared_ptr<control::Scrollbar> createScrollbar( const std::string& inName, const std::string& themeId ) {throw std::runtime_error(std::string(__FUNCTION__)+"not implemented yet");}
  virtual boost::shared_ptr<control::ScrollView> createScrollView( const std::string& inName, const std::string& themeId ) {throw std::runtime_error(std::string(__FUNCTION__)+"not implemented yet");}
  virtual boost::shared_ptr<control::Slider> createSlider( const std::string& inName, const std::string& themeId ) {throw std::runtime_error(std::string(__FUNCTION__)+"not implemented yet");}
  virtual boost::shared_ptr<control::Quad> createQuad( const std::string& quadname, const std::string& texid ) {throw std::runtime_error(std::string(__FUNCTION__)+"not implemented yet");}


private:
  /** Create all the resources we need at runtime.
   */
  void precache()
  {
    boost::shared_ptr<lost::font::Font> font;
    font.reset(new lost::font::BitmapFont(GLUT_BITMAP_8_BY_13));
    font->id = "8x13";
    fontFactory->add(font);
    font.reset(new lost::font::BitmapFont(GLUT_BITMAP_9_BY_15));
    font->id = "9x15";
    fontFactory->add(font);
  }
};
}
}
}

#endif


