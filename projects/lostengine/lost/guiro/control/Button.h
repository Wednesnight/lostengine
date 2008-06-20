#ifndef LOST_GUIRO_BUTTON_H
#define LOST_GUIRO_BUTTON_H

#include <boost/signal.hpp>
#include <boost/any.hpp>
#include "lost/guiro/control/View.h"
#include "lost/guiro/common/Themable.h"
#include "lost/gl/Scissor.h"
#include "lost/engine/MouseSym.h"
#include "lost/object/ObjectReference.h"

#define BUTTON_buttonOnPressed    "OnPressed"
#define BUTTON_buttonOnReleased   "OnReleased"
#define BUTTON_buttonOnHover      "OnHover"
#define BUTTON_buttonOnDisabled   "OnDisabled"
#define BUTTON_buttonOffPressed   "OffPressed"
#define BUTTON_buttonOffReleased  "OffReleased"
#define BUTTON_buttonOffHover     "OffHover"
#define BUTTON_buttonOffDisabled  "OffDisabled"

namespace lost
{
  namespace guiro
  {
    namespace control
    {

      struct Button : public View, public common::Themable
      {
        typedef enum
        {
          buttonOnPressed,
          buttonOnReleased,
          buttonOnHover,
          buttonOnDisabled,
          buttonOffPressed,
          buttonOffReleased,
          buttonOffHover,
          buttonOffDisabled
        } ButtonState;

      private:
        ButtonState state;
        bool        enabled;
        bool        sticky;
        bool        on;
        bool        mouseDown;

      public:
        boost::signal<void (boost::any&)> buttonPress;
        boost::signal<void (boost::any&)> buttonRelease;
        boost::signal<void (boost::any&)> buttonClick;
        boost::signal<void (boost::any&)> buttonOn;
        boost::signal<void (boost::any&)> buttonOff;
        boost::signal<void (boost::any&)> buttonStateChange;

        Button( const std::string& inName )
          : View( inName ),
            state(buttonOffReleased),
            enabled(true),
            sticky(false),
            on(false),
            mouseDown(false)
        {
          controllable = true;
        }

        Button( Button& inButton )
          : View( inButton ),
            state(buttonOffReleased),
            enabled(inButton.enabled),
            sticky(inButton.sticky),
            on(inButton.on),
            mouseDown(false)
        {
          mTheme = inButton.theme();
          controllable = true;
        }

        virtual ~Button() {}

        virtual Button* clone()
        {
          return new Button(*this);
        }

        virtual void draw(const lost::math::Rect& parentRect, const lost::common::Color& parentColor )
        {
          if (visible)
          {
            if ((state == buttonOnPressed   && (*mTheme)(BUTTON_buttonOnPressed))   ||
                (state == buttonOnReleased  && (*mTheme)(BUTTON_buttonOnReleased))  ||
                (state == buttonOnHover     && (*mTheme)(BUTTON_buttonOnHover))     ||
                (state == buttonOnDisabled  && (*mTheme)(BUTTON_buttonOnDisabled))  ||
                (state == buttonOffPressed  && (*mTheme)(BUTTON_buttonOffPressed))  ||
                (state == buttonOffReleased && (*mTheme)(BUTTON_buttonOffReleased)) ||
                (state == buttonOffHover    && (*mTheme)(BUTTON_buttonOffHover))    ||
                (state == buttonOffDisabled && (*mTheme)(BUTTON_buttonOffDisabled)))
            {
              if ((*mTheme)(BUTTON_buttonOnPressed)) mTheme->viewByProperty( BUTTON_buttonOnPressed )->setVisible( state == buttonOnPressed   );
              if ((*mTheme)(BUTTON_buttonOnReleased)) mTheme->viewByProperty( BUTTON_buttonOnReleased )->setVisible( state == buttonOnReleased  );
              if ((*mTheme)(BUTTON_buttonOnHover)) mTheme->viewByProperty( BUTTON_buttonOnHover )->setVisible( state == buttonOnHover     );
              if ((*mTheme)(BUTTON_buttonOnDisabled)) mTheme->viewByProperty( BUTTON_buttonOnDisabled )->setVisible( state == buttonOnDisabled  );
              if ((*mTheme)(BUTTON_buttonOffPressed)) mTheme->viewByProperty( BUTTON_buttonOffPressed )->setVisible( state == buttonOffPressed  );
              if ((*mTheme)(BUTTON_buttonOffReleased)) mTheme->viewByProperty( BUTTON_buttonOffReleased )->setVisible( state == buttonOffReleased );
              if ((*mTheme)(BUTTON_buttonOffHover)) mTheme->viewByProperty( BUTTON_buttonOffHover )->setVisible( state == buttonOffHover    );
              if ((*mTheme)(BUTTON_buttonOffDisabled)) mTheme->viewByProperty( BUTTON_buttonOffDisabled )->setVisible( state == buttonOffDisabled );
            }

            View::draw( parentRect, parentColor );
          }
        }

        virtual void resolve( object::ObjectReferenceResolver& inResolver )
        {
          mTheme.resolve( inResolver );
          View::resolve( inResolver );

          add( mTheme->view );
        }

        void setSticky( bool enable )
        {
          sticky = enable;
        }

        void doSignal( const boost::signal<void (boost::any&)>& signal )
        {
          if (signal.num_slots() > 0)
          {
            boost::any sender = this;
            signal(sender);
          }
        }

        virtual void mouseButtonEvent( const lost::event::MouseEvent& inEvent )
        {
          if (enabled && inEvent.button == lost::engine::M_LEFT)
          {
            if (mouseDown && !inEvent.pressed) doSignal( buttonClick );

            bool currentOn    = on;
            bool hitTestValid = hitTest( inEvent.pos );

            if (sticky && !inEvent.pressed && mouseDown && hitTestValid) currentOn = !on;
            if (currentOn != on)
            {
              on = currentOn;
              doSignal( (on) ? buttonOn : buttonOff );
            }
            mouseDown = inEvent.pressed;

            if (inEvent.pressed)
            {
              switch (state)
              {
                case buttonOnReleased  :
                case buttonOffReleased :
                case buttonOnHover     :
                case buttonOffHover    :
                  setState( (on) ? buttonOnPressed : buttonOffPressed );
                  break;
                default :
                  break;
              }
            }
            else
            {
              switch (state)
              {
                case buttonOnPressed  :
                case buttonOffPressed :
                  setState( (on) ? ((hitTestValid) ? buttonOnHover : buttonOnReleased) : ((hitTestValid) ? buttonOffHover : buttonOffReleased) );
                  break;
                default :
                  break;
              }
            }
          }
        }

        virtual void mouseEnter()
        {
          switch (state)
          {
            case buttonOnReleased :
              setState( (mouseDown) ? buttonOnPressed : buttonOnHover );
              break;
            case buttonOffReleased :
              setState( (mouseDown) ? buttonOffPressed : buttonOffHover );
              break;
            default :
              break;
          }
        }

        virtual void mouseLeave()
        {
          switch (state)
          {
            case buttonOnHover    :
            case buttonOffHover   :
            case buttonOnPressed  :
            case buttonOffPressed :
              setState( (on) ? buttonOnReleased : buttonOffReleased );
              break;
            default :
              break;
          }
        }

        virtual void setEnabled( bool inEnabled )
        {
          enabled = inEnabled;
          if (!enabled) setState( (on) ? buttonOnDisabled : buttonOffDisabled );
        }

        virtual void setState( const ButtonState& newState )
        {
          if (state != newState)
          {
            if (newState == buttonOnPressed || newState == buttonOffPressed) doSignal( buttonPress );
              else if (state == buttonOnReleased || state == buttonOffReleased) doSignal( buttonRelease );

            state = newState;

            doSignal( buttonStateChange );
          }
        }

      };

    }
  }
}

#endif


