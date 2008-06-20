#ifndef LOST_ENGINE_GLFW_INPUTSUBSYSTEM_H
#define LOST_ENGINE_GLFW_INPUTSUBSYSTEM_H

#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include "GL/glfw.h"
#include "lost/math/Vec2.h"
#include "lost/event/KeyEvent.h"
#include "lost/engine/KeySym.h"
#include "lost/event/MouseEvent.h"
#include "lost/engine/MouseSym.h"

namespace lost
{
  namespace engine
  {
    namespace glfw
    {

      typedef boost::signal<void ( int key, bool pressed )> InputKeySignal;
      InputKeySignal inputKeySignal;
      void glfwKeyCallback( int key, int action )
      {
        inputKeySignal( key, action );
      }

      typedef boost::signal<void ( lost::math::Vec2 point )> InputMouseMoveSignal;
      InputMouseMoveSignal inputMouseMoveSignal;
      void glfwMouseMoveCallback( int x, int y )
      {
        inputMouseMoveSignal( lost::math::Vec2(x, y) );
      }

      typedef boost::signal<void ( int button, bool pressed, lost::math::Vec2 point )> InputMouseButtonSignal;
      InputMouseButtonSignal inputMouseButtonSignal;
      void glfwMouseButtonCallback( int button, int action )
      {
        int x, y;
        glfwGetMousePos( &x, &y );
        inputMouseButtonSignal( button, (action == GLFW_PRESS), lost::math::Vec2(x, y) );
      }

      template<typename Engine>
      struct InputSubSystem
      {
        Engine&                engine;
        boost::signal<void ( const lost::event::KeyEvent& )> keyboard;
        boost::signal<void ( const lost::event::MouseEvent& )>   mouseMove;
        boost::signal<void ( const lost::event::MouseEvent& )> mouseButton;

        InputSubSystem(Engine& inEngine) : engine(inEngine)
        {
        }

        void init()
        {
          inputKeySignal.connect( boost::bind( &InputSubSystem::keyCallback, this, _1, _2 ) );
          inputMouseMoveSignal.connect( boost::bind( &InputSubSystem::mouseMoveCallback, this, _1 ) );
          inputMouseButtonSignal.connect( boost::bind( &InputSubSystem::mouseButtonCallback, this, _1, _2, _3 ) );
        }

        void restart()
        {
          glfwSetKeyCallback( glfwKeyCallback );
          glfwSetMousePosCallback( glfwMouseMoveCallback );
          glfwSetMouseButtonCallback( glfwMouseButtonCallback );
        }

        void keyCallback( int key, bool pressed )
        {
          int l_key = key;
          if (key > GLFW_KEY_SPECIAL && key <= GLFW_KEY_LAST)
          {
            if (key >= GLFW_KEY_F1 && key <= GLFW_KEY_F12)
            {
              l_key = K_F1 + (key - GLFW_KEY_F1);
            }
            else
            {
              switch (key)
              {
                case GLFW_KEY_TAB :
                  l_key = K_TAB;
                  break;
                case GLFW_KEY_LSHIFT :
                  l_key = K_LSHIFT;
                  break;
                case GLFW_KEY_RSHIFT :
                  l_key = K_RSHIFT;
                  break;
                case GLFW_KEY_LCTRL :
                  l_key = K_LCTRL;
                  break;
                case GLFW_KEY_RCTRL :
                  l_key = K_RCTRL;
                  break;
                case GLFW_KEY_BACKSPACE :
                  l_key = K_BACKSPACE;
                  break;
                case GLFW_KEY_ENTER :
                  l_key = K_ENTER;
                  break;
                case GLFW_KEY_HOME :
                  l_key = K_HOME;
                  break;
                case GLFW_KEY_END :
                  l_key = K_END;
                  break;
                case GLFW_KEY_DEL :
                  l_key = K_DELETE;
                  break;
                case GLFW_KEY_INSERT :
                  l_key = K_INSERT;
                  break;
                case GLFW_KEY_PAGEUP :
                  l_key = K_PAGEUP;
                  break;
                case GLFW_KEY_PAGEDOWN :
                  l_key = K_PAGEDOWN;
                  break;
                case GLFW_KEY_UP :
                  l_key = K_UP;
                  break;
                case GLFW_KEY_DOWN :
                  l_key = K_DOWN;
                  break;
                case GLFW_KEY_LEFT :
                  l_key = K_LEFT;
                  break;
                case GLFW_KEY_RIGHT :
                  l_key = K_RIGHT;
                  break;
                case GLFW_KEY_ESC :
                  l_key = K_ESCAPE;
                  break;
                default :
                  l_key = key;
              }
            }
          }
          lost::event::KeyEvent event;
          event.key = l_key;
          event.pressed = pressed;
          keyboard(event);
        }

        void mouseMoveCallback( lost::math::Vec2 point )
        {
          lost::event::MouseEvent event;
          point.y = engine.video.attributes.height - point.y;
          event.type = lost::event::MouseMoveEventType;
          event.pos = point;
          event.button = M_UNKNOWN;
          event.pressed = false;
          mouseMove( event );
        }

        void mouseButtonCallback( int button, bool pressed, lost::math::Vec2 point )
        {
          lost::event::MouseEvent event;
          point.y = engine.video.attributes.height - point.y;
          event.type = lost::event::MouseButtonEventType;
          event.pos = point;
          event.button = (button == GLFW_MOUSE_BUTTON_1)
                           ? M_LEFT
                           : ((button == GLFW_MOUSE_BUTTON_2)
                               ? M_RIGHT
                               : ((button == GLFW_MOUSE_BUTTON_3)
                                   ? M_MIDDLE
                                   : M_UNKNOWN));
          event.pressed = pressed;
          mouseButton( event );
        }

      };

    }
  }
}

#endif
