#ifndef LOST_GUIRO_CONTROLLER_INPUTCONTROLLER_H
#define LOST_GUIRO_CONTROLLER_INPUTCONTROLLER_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/function.hpp>

#include "lost/guiro/control/View.h"
#include "lost/math/Vec2.h"
#include "lost/common/Logger.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/MouseEvent.h"
#include "lost/engine/MouseSym.h"

namespace lost
{
  namespace guiro
  {
    namespace controller
    {

      struct InputController
      {
      protected:
        bool                           moving;
        lost::math::Vec2               movingOffset;
        boost::weak_ptr<control::View> focusedView;
        boost::weak_ptr<control::View> hoveredView;
      public:
        boost::weak_ptr<control::View> rootView;

        /**
         * @param inRootView root view of the user interface the InputManager belongs to. This is
         *                   most probably the UserInterface itself.
         */
        InputController(boost::shared_ptr<control::View> inRootView )
          : moving(false),
            movingOffset(0, 0),
            rootView(inRootView)
        {
        }

        inline boost::shared_ptr<control::View> recursiveHitTest( const boost::shared_ptr<control::View>& inParent, const lost::math::Vec2& inPoint )
        {
          boost::shared_ptr<control::View>           result;
          boost::shared_ptr<control::View>           tempResult;
          control::View::ChildList&                  children = inParent->getChildList();
          control::View::ChildList::reverse_iterator child;

          for (child = children.rbegin(); !result && child != children.rend(); ++child)
          {
            if ((*child)->hitTest( inPoint ))
            {
              result = (*child);
            }
            tempResult = recursiveHitTest( (*child), inPoint );
            if (tempResult)
              result = tempResult;
          }
          if (!result && inParent->hitTest( inPoint ))
          {
            result = inParent;
          }
          return result;
        }

        const boost::shared_ptr<control::View> getTopMostView( const lost::math::Vec2& inPoint )
        {
          boost::shared_ptr<control::View> result;
          if (boost::shared_ptr<control::View> root = rootView.lock())
          {
            result = recursiveHitTest( root, inPoint );
          }
          return result;
        }

        void keyboardEvent( const lost::application::KeyEvent& inEvent )
        {
          boost::shared_ptr<control::View> view = focusedView.lock();
          // handle keyboard input
          if (view)
          {
            view->keyboardEvent( inEvent );
            while (view = view->parent.lock())
              view->keyboardEvent( inEvent);
          }
        }

        void mouseButtonEvent( const lost::application::MouseEvent& inEvent )
        {
          // get the clicked view (if any)
          boost::shared_ptr<control::View> view     = getTopMostView( inEvent.pos );
          boost::shared_ptr<control::View> prevView = focusedView.lock();
          // handle focus
          if (inEvent.pressed && view != prevView)
          {
            // unset focus
            if (prevView)
            {
              DOUT("'" << prevView->name << "' lost input focus");
              prevView->focus(false);
            }
            // set focused view
            focusedView = view;
            // set focus
            if (view)
            {
              DOUT("'" << view->name << "' received input focus");
              view->focus(true);
            }
          }

          if (inEvent.button == lost::engine::M_LEFT)
          {
            // handle moving state
            moving = inEvent.pressed;
            if (view)
            {
              // handle moving
              if (view->movable)
              {
                movingOffset = (moving) ? inEvent.pos : lost::math::Vec2(0, 0);
              }
            }
          }

          if (view = focusedView.lock())
          {
            DOUT("processing click event for '" << view->name << "'");
            view->mouseButtonEvent( inEvent );
            while (view = view->parent.lock())
              view->mouseButtonEvent( inEvent );
          }
        }

        void mouseMoveEvent( const lost::application::MouseEvent& inEvent )
        {
          // handle moving
          boost::shared_ptr<control::View> view = focusedView.lock();
          if (view)
          {
            if (moving && view->movable)
            {
              if (boost::shared_ptr<control::View> parent = view->parent.lock())
              {
                lost::math::Rect rect( view->getRect() );
                lost::math::Rect parentRect( parent->getRect() );
                view->setPos( std::min( parentRect.width - rect.width, std::max( 0.0f, (rect.x - parentRect.x) + (inEvent.pos.x - movingOffset.x) ) ),
                              std::min( parentRect.height - rect.height, std::max( 0.0f, (rect.y - parentRect.y) + (inEvent.pos.y - movingOffset.y) ) ) );
                movingOffset = inEvent.pos;
              }
            }
          }

          // handle mouse enter/leave
          boost::shared_ptr<lost::guiro::control::View> current = hoveredView.lock();
          view = getTopMostView( inEvent.pos );
          if (current != view)
          {
            if (current) current->mouseLeave();
            hoveredView = current = view;
            if (current) current->mouseEnter();
          }

          // call mouse events
          if (view = focusedView.lock())
          {
            view->mouseMoveEvent( inEvent );
            while (view = view->parent.lock())
              view->mouseMoveEvent( inEvent );
          }
        }

      }; // struct InputManager

    }
  } // namespace guiro
} // namespace lost

#endif
