#ifndef LOST_GUIRO_FACTORY_CONTROLFACTORY_H
#define LOST_GUIRO_FACTORY_CONTROLFACTORY_H

#include "lost/object/ObjectReferenceResolver.h"
#include "lost/guiro/control/Button.h"
#include "lost/guiro/control/Edit.h"
#include "lost/guiro/control/Image.h"
#include "lost/guiro/control/Label.h"
#include "lost/guiro/control/Scrollbar.h"
#include "lost/guiro/control/ScrollView.h"
#include "lost/guiro/control/Quad.h"
#include "lost/guiro/control/ShapeView.h"
#include "lost/guiro/control/Window.h"

namespace lost
{
  namespace guiro
  {
    namespace factory
    {

      /** This is the interface for all factories that want to implement construction of a particularly
       *  themed set of controls. It also defines the set of possible controls you can instantiate for guiro.
       */
      struct ControlFactory
      {
        virtual ~ControlFactory() {}

        virtual object::ObjectReferenceResolver&        getResolver         () = 0;
        virtual boost::shared_ptr<control::Button>      createButton        ( const std::string& inName, const std::string& themeId ) = 0;
        virtual boost::shared_ptr<control::Button>      createStickyButton  ( const std::string& inName, const std::string& themeId ) = 0;
        virtual boost::shared_ptr<control::Edit>        createEdit          ( const std::string& inName, const std::string& inBackgroundId ) = 0;
        virtual boost::shared_ptr<control::Image>       createImage         ( const std::string& inName, const std::string& inId ) = 0;
        virtual boost::shared_ptr<control::Label>       createLabel         ( const std::string& inName ) = 0;
        virtual boost::shared_ptr<control::Scrollbar>   createScrollbar     ( const std::string& inName, const std::string& themeId ) = 0;
        virtual boost::shared_ptr<control::ScrollView>  createScrollView    ( const std::string& inName, const std::string& themeId ) = 0;
        virtual boost::shared_ptr<control::Slider>      createSlider        ( const std::string& inName, const std::string& themeId ) = 0;
        virtual boost::shared_ptr<control::Quad>        createQuad          ( const std::string& quadname, const std::string& texid ) = 0;
        virtual boost::shared_ptr<control::ShapeView>   createShapeView     ( const std::string& inName) = 0;
        virtual boost::shared_ptr<control::Window>      createWindow        ( const std::string& inName, const std::string& themeId ) = 0;

      };

    }
  }
}

#endif


