#ifndef LOST_GUIRO_FACTORY_VANILLAFACTORY_H
#define LOST_GUIRO_FACTORY_VANILLAFACTORY_H

#include <map>
#include <boost/shared_ptr.hpp>

#include "lost/common/Color.h"
#include "lost/common/Logger.h"

#include "lost/guiro/control/Image.h"
#include "lost/guiro/control/Quad.h"
#include "lost/guiro/common/Texture.h"
#include "lost/guiro/control/Label.h"
#include "lost/guiro/control/Edit.h"
#include "lost/guiro/control/Button.h"
#include "lost/guiro/control/Slider.h"
#include "lost/guiro/control/Scrollbar.h"
#include "lost/guiro/control/ScrollView.h"

#include "lost/guiro/common/Theme.h"

#include "lost/object/Object.h"
#include "lost/object/ObjectReference.h"
#include "lost/object/ObjectReferenceContainer.h"
#include "lost/object/ObjectReferenceResolver.h"
#include "lost/resource/Loader.h"

#include "lost/guiro/factory/ControlFactory.h"
#include "lost/guiro/common/Theme.h"
#include "lost/guiro/factory/ThemeFactory.h"

#include "lost/guiro/common/Texture.h"
#include "lost/guiro/factory/TextureFactory.h"
#include "lost/resource/LocalRepository.h"
#include "lost/guiro/factory/FontFactory.h"
#include "lost/guiro/factory/BitmapFactory.h"

namespace lost
{
  namespace guiro
  {
    namespace factory
    {
      struct VanillaFactory : public ControlFactory
      {
        boost::shared_ptr<lost::object::ObjectReferenceResolver>  resolver;
        boost::shared_ptr<lost::resource::Loader>                        resourceLoader;
        boost::shared_ptr<lost::resource::LocalRepository>               repository;
        boost::shared_ptr<FontFactory>                                   fontFactory;
        boost::shared_ptr<BitmapFactory>                                 bitmapFactory;
        boost::shared_ptr<TextureFactory>                                textureFactory;
        boost::shared_ptr<ThemeFactory>                                  themeFactory;

        VanillaFactory(boost::shared_ptr<lost::common::Config> inConfig)
        {
          resolver.reset(new lost::object::ObjectReferenceResolver());
          // resourceloader and repositories
          resourceLoader.reset(new resource::Loader());
          repository.reset(new resource::LocalRepository(inConfig));
          // register repositories
          resourceLoader->registerRepository( repository );

          fontFactory.reset(new FontFactory( resourceLoader, inConfig ));
          bitmapFactory.reset(new BitmapFactory( resourceLoader, inConfig ));
          textureFactory.reset(new TextureFactory( resourceLoader, inConfig ));
          themeFactory.reset(new ThemeFactory( resourceLoader, inConfig ));

          // register factories
          resolver->registerFactory( "font", fontFactory );
          resolver->registerFactory( "bitmap", bitmapFactory );
          resolver->registerFactory( "texture", textureFactory );
          resolver->registerFactory( "theme", themeFactory );

          precache();
        }

        void precache()
        {
          //*** FIXME: temporary creation of theme objects until we finished the loading part
          themeFactory->themes["simpleButtonTheme"]      = createButtonTheme( "simpleButtonTheme" );
          themeFactory->themes["stickyButtonTheme"]      = createStickyButtonTheme( "stickyButtonTheme" );
          themeFactory->themes["invisibleButtonTheme"]   = createInvisibleButtonTheme( "invisibleButtonTheme" );
          themeFactory->themes["sliderTheme"]            = createSliderTheme( "sliderTheme", "simpleButtonTheme" );
          themeFactory->themes["sliderVerticalTheme"]    = createSliderTheme( "sliderVerticalTheme", "simpleButtonTheme", control::Slider::layoutVertical );
          themeFactory->themes["scrollbarTheme"]         = createScrollbarTheme( "scrollbarTheme", "sliderTheme", "simpleButtonTheme" );
          themeFactory->themes["scrollbarVerticalTheme"] = createScrollbarTheme( "scrollbarVerticalTheme", "sliderVerticalTheme", "simpleButtonTheme", control::Scrollbar::layoutVertical );
          themeFactory->themes["scrollViewTheme"]        = createScrollViewTheme( "scrollViewTheme", "scrollbarTheme", "scrollbarVerticalTheme" );
          //***
        }


        lost::object::ObjectReferenceResolver& getResolver()
        {
          return *(resolver.get());
        }



        boost::shared_ptr<control::Button> createButton( const std::string& inName, const std::string& themeId )
        {
          boost::shared_ptr<control::Button> button(new control::Button(inName));
          button->theme().type = "theme";
          button->theme().id   = themeId;
          return button;
        }

        boost::shared_ptr<common::Theme> createButtonTheme( const std::string& themeId )
        {
          boost::shared_ptr<common::Theme> theme(new common::Theme());

          theme->view.reset(new control::View( themeId ));
          theme->view->add( createQuad( "ButtonPressed" , "ButtonPressedTex"  ) );
          theme->view->add( createQuad( "ButtonReleased", "ButtonReleasedTex" ) );
          theme->view->add( createQuad( "ButtonHover"   , "ButtonHoverTex"    ) );
          theme->view->add( createQuad( "ButtonDisabled", "ButtonDisabledTex" ) );

          theme->properties[BUTTON_buttonOffReleased] = "ButtonReleased";
          theme->properties[BUTTON_buttonOffPressed]  = "ButtonPressed";
          theme->properties[BUTTON_buttonOffHover]    = "ButtonHover";
          theme->properties[BUTTON_buttonOffDisabled] = "ButtonDisabled";

          return theme;
        }

        boost::shared_ptr<common::Theme> createInvisibleButtonTheme( const std::string& themeId )
        {
          boost::shared_ptr<common::Theme> theme(new common::Theme());
          theme->view.reset(new control::View( themeId ));
          return theme;
        }

        boost::shared_ptr<control::Button> createStickyButton( const std::string& inName, const std::string& themeId )
        {
          boost::shared_ptr<control::Button> button(new control::Button(inName));
          button->theme().type = "theme";
          button->theme().id   = themeId;
          button->setSticky(true);
          return button;
        }

        boost::shared_ptr<common::Theme> createStickyButtonTheme( const std::string& themeId )
        {
          boost::shared_ptr<common::Theme> theme(new common::Theme());

          theme->view.reset(new control::View( themeId ));
          theme->view->add( createQuad( "StickyButtonOnReleased" , "ButtonOnReleasedTex" ) );
          theme->view->add( createQuad( "StickyButtonOnPressed"  , "ButtonOnPressedTex"  ) );
          theme->view->add( createQuad( "StickyButtonOnHover"    , "ButtonOnHoverTex"    ) );
          theme->view->add( createQuad( "StickyButtonOnDisabled" , "ButtonOnDisabledTex" ) );
          theme->view->add( createQuad( "StickyButtonOffReleased", "ButtonReleasedTex"   ) );
          theme->view->add( createQuad( "StickyButtonOffPressed" , "ButtonPressedTex"    ) );
          theme->view->add( createQuad( "StickyButtonOffHover"   , "ButtonHoverTex"      ) );
          theme->view->add( createQuad( "StickyButtonOffDisabled", "ButtonDisabledTex"   ) );

          theme->properties[BUTTON_buttonOnReleased]  = "StickyButtonOnReleased";
          theme->properties[BUTTON_buttonOnPressed]   = "StickyButtonOnPressed";
          theme->properties[BUTTON_buttonOnHover]     = "StickyButtonOnHover";
          theme->properties[BUTTON_buttonOnDisabled]  = "StickyButtonOnDisabled";
          theme->properties[BUTTON_buttonOffReleased] = "StickyButtonOffReleased";
          theme->properties[BUTTON_buttonOffPressed]  = "StickyButtonOffPressed";
          theme->properties[BUTTON_buttonOffHover]    = "StickyButtonOffHover";
          theme->properties[BUTTON_buttonOffDisabled] = "StickyButtonOffDisabled";

          return theme;
        }

        boost::shared_ptr<control::Edit> createEdit( const std::string& inName, const std::string& inBackgroundId )
        {
          boost::shared_ptr<control::Edit> edit(new control::Edit(inName));
          edit->background.type = "bitmap";
          edit->background.id   = inBackgroundId;
          edit->font.type       = "font";
          edit->font.id         = "suigeneris.ttf";
          edit->setFontColor(lost::common::whiteColor);
          edit->setFontSize(18);
          return edit;
        }

        boost::shared_ptr<control::Image> createImage( const std::string& inName, const std::string& inId )
        {
          boost::shared_ptr<control::Image> image(new control::Image(inName));
          image->bitmap.type = "bitmap";
          image->bitmap.id   = inId;
          image->color = lost::common::whiteColor;
          return image;
        }

        boost::shared_ptr<control::Label> createLabel( const std::string& inName )
        {
          boost::shared_ptr<control::Label> label(new control::Label(inName));
          label->font().type = "font";
          label->font().id   = "suigeneris.ttf";
          label->fontColor(lost::common::whiteColor);
          label->fontSize(18);
          return label;
        }

        boost::shared_ptr<control::Scrollbar> createScrollbar( const std::string& inName, const std::string& themeId )
        {
          boost::shared_ptr<control::Scrollbar> scrollbar(new control::Scrollbar(inName));
          scrollbar->theme().type = "theme";
          scrollbar->theme().id   = themeId;
          return scrollbar;
        }

        boost::shared_ptr<common::Theme> createScrollbarTheme( const std::string& themeId, const std::string& sliderThemeId,
                                                               const std::string& buttonThemeId,
                                                               const control::Scrollbar::ScrollbarLayout layout = control::Scrollbar::layoutHorizontal )
        {
          boost::shared_ptr<common::Theme> theme(new common::Theme());

          theme->view.reset(new control::View( themeId ));
          boost::shared_ptr<control::Slider> slider( createSlider( "scrollbarSlider", sliderThemeId ) );
          boost::shared_ptr<control::Button> buttonUp( createButton( "scrollbarButtonUp", buttonThemeId ) );
          boost::shared_ptr<control::Button> buttonDown( createButton( "scrollbarButtonDown", buttonThemeId ) );
          theme->view->add( slider );
          theme->view->add( buttonUp );
          theme->view->add( buttonDown );
          switch (layout)
          {
            case control::Scrollbar::layoutHorizontal :
              slider->layout( control::Slider::layoutHorizontal );
              slider->setArea( make_area( common::xcenter(), common::ycenter(), common::wrel(.7), common::hrel(.75) ) );
              buttonUp->setArea( make_area( common::xalignright(), common::ycenter(), common::wrel(.15), common::hrel(.75) ) );
              buttonDown->setArea( make_area( common::xalignleft(), common::ycenter(), common::wrel(.15), common::hrel(.75) ) );
              break;
            case control::Scrollbar::layoutVertical :
              slider->layout( control::Slider::layoutVertical );
              slider->setArea( make_area( common::xcenter(), common::ycenter(), common::wrel(.75), common::hrel(.7) ) );
              buttonUp->setArea( make_area( common::xcenter(), common::yaligntop(), common::wrel(.75), common::hrel(.15) ) );
              buttonDown->setArea( make_area( common::xcenter(), common::yalignbottom(), common::wrel(.75), common::hrel(.15) ) );
              break;
          }
          theme->properties[SCROLLBAR_scrollbarSlider]     = "scrollbarSlider";
          theme->properties[SCROLLBAR_scrollbarButtonUp]   = "scrollbarButtonUp";
          theme->properties[SCROLLBAR_scrollbarButtonDown] = "scrollbarButtonDown";

          return theme;
        }

        boost::shared_ptr<control::ScrollView> createScrollView( const std::string& inName, const std::string& themeId )
        {
          boost::shared_ptr<control::ScrollView> scrollView(control::ScrollView::create(inName));
          scrollView->theme().type = "theme";
          scrollView->theme().id   = themeId;
          return scrollView;
        }

        boost::shared_ptr<common::Theme> createScrollViewTheme( const std::string& themeId, const std::string& scollbarHorizontalThemeId,
                                                                const std::string& scollbarVerticalThemeId )
        {
          boost::shared_ptr<common::Theme> theme(new common::Theme());

          theme->view.reset(new control::View( themeId ));
          boost::shared_ptr<control::Scrollbar> scrollbarHorizontal( createScrollbar( "scrollViewScrollbarHorizontal", scollbarHorizontalThemeId ) );
          scrollbarHorizontal->layout( control::Scrollbar::layoutHorizontal );
          boost::shared_ptr<control::Scrollbar> scrollbarVertical( createScrollbar( "scrollViewScrollbarVertical", scollbarVerticalThemeId ) );
          scrollbarVertical->layout( control::Scrollbar::layoutVertical );
          theme->view->add( scrollbarHorizontal );
          theme->view->add( scrollbarVertical );
          scrollbarHorizontal->setArea( make_area( common::xalignleft(), common::yalignbottom(), common::wrel(1,-20), common::habs(20) ) );
          scrollbarVertical->setArea( make_area( common::xalignright(), common::yalignbottom(), common::wabs(20), common::hrel(1) ) );

          theme->properties[SCROLLVIEW_scrollViewScrollbarHorizontal] = "scrollViewScrollbarHorizontal";
          theme->properties[SCROLLVIEW_scrollViewScrollbarVertical]   = "scrollViewScrollbarVertical";

          return theme;
        }

        boost::shared_ptr<control::Slider> createSlider( const std::string& inName, const std::string& themeId )
        {
          boost::shared_ptr<control::Slider> slider(new control::Slider(inName));
          slider->theme().type = "theme";
          slider->theme().id   = themeId;
          return slider;
        }

        boost::shared_ptr<common::Theme> createSliderTheme( const std::string& themeId, const std::string& buttonThemeId,
                                                    const control::Slider::SliderLayout layout = control::Slider::layoutHorizontal )
        {
          boost::shared_ptr<common::Theme> theme(new common::Theme());

          theme->view.reset(new control::View( themeId ));
          boost::shared_ptr<control::Button> handle( createButton( "sliderHandle", buttonThemeId ) );
          theme->view->add( handle );
          switch (layout)
          {
            case control::Slider::layoutHorizontal :
              handle->setArea( make_area( common::xalignleft(), common::ycenter(), common::wrel(.15), common::hrel(.75) ) );
              break;
            case control::Slider::layoutVertical :
              handle->setArea( make_area( common::xcenter(), common::yalignbottom(), common::wrel(.75), common::hrel(.15) ) );
              break;
          }
          theme->properties[SLIDER_sliderHandle] = "sliderHandle";

          return theme;
        }

        boost::shared_ptr<control::Quad> createQuad( const std::string& quadname, const std::string& texid )
        {
          boost::shared_ptr<control::Quad> quad(new control::Quad(quadname));
          quad->texture.type = "texture";
          quad->texture.id   = texid;
          quad->color = lost::common::whiteColor;
          return quad;
        }

        virtual boost::shared_ptr<control::ShapeView> createShapeView   ( const std::string& inName) 
        {
          return boost::shared_ptr<control::ShapeView>(new control::ShapeView(inName));
        }
        
        virtual boost::shared_ptr<control::Window>    createWindow      ( const std::string& inName, const std::string& themeId )
        {throw std::runtime_error(std::string(__FUNCTION__)+"not implemented yet");}
      };

    }
  }
}

#endif


