#ifndef LOST_GUIRO_CONTROL_USERINTERFACE_H
#define LOST_GUIRO_CONTROL_USERINTERFACE_H

#include "lost/guiro/control/View.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/guiro/controller/InputController.h"
#include "lost/common/Logger.h"

namespace lost
{
  namespace guiro
  {
    namespace control
    {
    struct UserInterface : public View
    {
      boost::shared_ptr<lost::guiro::controller::InputController> inputController;
      
      static boost::shared_ptr<UserInterface> create( const std::string& inName )
      {
        boost::shared_ptr<UserInterface> result(new UserInterface( inName ));
        result->inputController.reset(new lost::guiro::controller::InputController(result));
        return result;
      }

      virtual ~UserInterface() {}

      void injectKeyboardEvent( const lost::application::KeyEvent& inEvent )
      {
        inputController->keyboardEvent(inEvent);
      }

      void injectMouseButtonEvent( const lost::application::MouseEvent& inEvent )
      {
        inputController->mouseButtonEvent(inEvent);
      }

      void injectMouseMoveEvent( const lost::application::MouseEvent& inEvent )
      {
        inputController->mouseMoveEvent(inEvent);
      }
    private:
      // use create() for a new object
      UserInterface( const std::string& inName)
      : View( inName )
      {
      }
    };
    }
  }
}

#endif

