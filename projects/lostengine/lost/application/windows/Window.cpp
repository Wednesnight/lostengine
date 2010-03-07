#include "lost/application/Window.h"
#include "lost/event/Event.h"
#include "lost/application/WindowEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/DragNDropEvent.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

namespace lost
{
  namespace application
  {

    void dispatchEvent(Window* window, event::EventPtr& event)
    {
      window->dispatcher->queueEvent(event);
    }

    KeyCode translateKeyCode(int nativeKeyCode)
    {
      switch (nativeKeyCode)
      {
        case 65 : return lost::application::K_A;
        case 66 : return lost::application::K_B;
        case 67 : return lost::application::K_C;
        case 68 : return lost::application::K_D;
        case 69 : return lost::application::K_E;
        case 70 : return lost::application::K_F;
        case 71 : return lost::application::K_G;
        case 72 : return lost::application::K_H;
        case 73 : return lost::application::K_I;
        case 74 : return lost::application::K_J;
        case 75 : return lost::application::K_K;
        case 76 : return lost::application::K_L;
        case 77 : return lost::application::K_M;
        case 78 : return lost::application::K_N;
        case 79 : return lost::application::K_O;
        case 80 : return lost::application::K_P;
        case 81 : return lost::application::K_Q;
        case 82 : return lost::application::K_R;
        case 83 : return lost::application::K_S;
        case 84 : return lost::application::K_T;
        case 85 : return lost::application::K_U;
        case 86 : return lost::application::K_V;
        case 87 : return lost::application::K_W;
        case 88 : return lost::application::K_X;
        case 89 : return lost::application::K_Y;
        case 90 : return lost::application::K_Z;

        case 48 : return lost::application::K_0;
        case 49 : return lost::application::K_1;
        case 50 : return lost::application::K_2;
        case 51 : return lost::application::K_3;
        case 52 : return lost::application::K_4;
        case 53 : return lost::application::K_5;
        case 54 : return lost::application::K_6;
        case 55 : return lost::application::K_7;
        case 56 : return lost::application::K_8;
        case 57 : return lost::application::K_9;

//        case VK_??? : return lost::application::K_CIRCUMFLEX;
        case VK_HELP : return lost::application::K_HELP;
//        case VK_??? : return lost::application::K_APOSTROPHE;

        case VK_RETURN : return lost::application::K_ENTER;
        case VK_TAB : return lost::application::K_TAB;
        case VK_SPACE : return lost::application::K_SPACE;
        case VK_BACK : return lost::application::K_BACKSPACE;
        case VK_ESCAPE : return lost::application::K_ESCAPE;

        case VK_LEFT : return lost::application::K_LEFT;
        case VK_RIGHT : return lost::application::K_RIGHT;
        case VK_UP : return lost::application::K_UP;
        case VK_DOWN : return lost::application::K_DOWN;
        case VK_PRIOR : return lost::application::K_PAGEUP;
        case VK_NEXT : return lost::application::K_PAGEDOWN;
        case VK_HOME : return lost::application::K_HOME;
        case VK_END : return lost::application::K_END;
          
        case VK_F1 : return lost::application::K_F1;
        case VK_F2 : return lost::application::K_F2;
        case VK_F3 : return lost::application::K_F3;
        case VK_F4 : return lost::application::K_F4;
        case VK_F5 : return lost::application::K_F5;
        case VK_F6 : return lost::application::K_F6;
        case VK_F7 : return lost::application::K_F7;
        case VK_F8 : return lost::application::K_F8;
        case VK_F9 : return lost::application::K_F9;
        case VK_F10 : return lost::application::K_F10;
        case VK_F11 : return lost::application::K_F11;
        case VK_F12 : return lost::application::K_F12;

        default : return lost::application::K_UNKNOWN;
      }
    }

    void dispatchKeyEvent(Window* window, event::Type which, int code, bool pressed, bool repeat)
    {
      KeyEventPtr event(new KeyEvent(KeyEvent::KEY_DOWN()));
      event->window    = window;
      event->key       = lost::application::translateKeyCode(code);
      event->character = "FIXME";
      event->pressed   = pressed;
      event->repeat    = repeat;
      window->dispatcher->queueEvent(event);
    }

    void dispatchDragNDropEvent(Window* window, HDROP drop)
    {
      UINT numFiles = DragQueryFile(drop, 0xFFFFFFFF, NULL, 0);
      if (numFiles > 0)
      {
        wchar_t filenameW[MAX_PATH];
        char filename[MAX_PATH];
        POINT absPos;
        POINT pos;
        for (unsigned int idx = 0; idx < numFiles; idx++)
        {
          if (DragQueryFile(drop, idx, filenameW, MAX_PATH) > 0 &&
            WideCharToMultiByte(CP_UTF8, NULL, filenameW, -1, filename, MAX_PATH,
            NULL, NULL) > 0 && DragQueryPoint(drop, &pos) && GetCursorPos(&absPos))
          {
            DragNDropEventPtr dragNDropEvent(new DragNDropEvent(DragNDropEvent::DROP(),
              std::string(filename)));
            dragNDropEvent->window  = window;
            dragNDropEvent->pos     = lost::math::Vec2((float)pos.x, (float)pos.y);
            dragNDropEvent->absPos  = lost::math::Vec2((float)absPos.x, (float)absPos.y);
            window->dispatcher->queueEvent(dragNDropEvent);
            // FIXME: we only support single files at the moment
            break;
          }
        }
      }
      DragFinish(drop);
    }

    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
      Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      switch (message)
      {
        case WM_CREATE:
          break;

        case WM_CLOSE:
          dispatchEvent(window, event::EventPtr(new WindowEvent(WindowEvent::CLOSE(), window)));
          break;

        case WM_DESTROY:
          break;

        case WM_SIZE:
          dispatchEvent(window, event::EventPtr(new ResizeEvent(LOWORD(lParam), HIWORD(lParam))));
          break;

        case WM_KEYDOWN:
          dispatchKeyEvent(window, KeyEvent::KEY_DOWN(), wParam, true,
            (lParam & KF_REPEAT) == KF_REPEAT);
          break;

        case WM_DROPFILES:
          dispatchDragNDropEvent(window, (HDROP)wParam);
          break;
      }

      return DefWindowProc(hwnd, message, wParam, lParam);
    }

    struct Window::WindowHiddenMembers
    {
      ATOM  windowClass;
      HWND  handle;
      HDC   deviceContext;
      HGLRC glContext;
    };

    void Window::initialize()
    {
      DOUT("Window::initialize()");

      // initialize hiddenMembers
      hiddenMembers = new WindowHiddenMembers;

      // Set window class parameters
      WNDCLASS wc;
      wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
      wc.lpfnWndProc   = (WNDPROC)WndProc;
      wc.cbClsExtra    = 0;
      wc.cbWndExtra    = 0;
      wc.hInstance     = GetModuleHandle(NULL);
      wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = NULL;
      wc.lpszMenuName  = NULL;
      wc.lpszClassName = L"LostEngineApplicationWindow";
      wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);

      // Register the window class
      hiddenMembers->windowClass = RegisterClass(&wc);

      std::wstring wTitle(params.caption.begin(), params.caption.end());
      // Create window
      hiddenMembers->handle = CreateWindowEx(WS_EX_APPWINDOW,
                                             L"LostEngineApplicationWindow",
                                             wTitle.c_str(),
                                             WS_SIZEBOX | WS_CLIPSIBLINGS | 
                                             WS_CLIPCHILDREN | WS_VISIBLE | WS_EX_ACCEPTFILES,
                                             (int)params.rect.x,
                                             (int)params.rect.y,
                                             (int)params.rect.width,
                                             (int)params.rect.height,
                                             NULL,
                                             NULL,
                                             GetModuleHandle(NULL),
                                             NULL);

      // Get a device context
      hiddenMembers->deviceContext = GetDC(hiddenMembers->handle);

      // set pixel format
      PIXELFORMATDESCRIPTOR pfd;
      ZeroMemory( &pfd, sizeof( pfd ) );
      pfd.nSize = sizeof( pfd );
      pfd.nVersion = 1;
      pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
      pfd.iPixelType = PFD_TYPE_RGBA;
      pfd.cColorBits = 24;
      pfd.cDepthBits = 16;
      pfd.iLayerType = PFD_MAIN_PLANE;
      GLuint pixelFormat = ChoosePixelFormat(hiddenMembers->deviceContext, &pfd);
      SetPixelFormat(hiddenMembers->deviceContext, pixelFormat, &pfd);

      // get a rendering context
      hiddenMembers->glContext = wglCreateContext(hiddenMembers->deviceContext);

      // activate the OpenGL rendering context
      wglMakeCurrent(hiddenMembers->deviceContext, hiddenMembers->glContext);

      // set window param to this
      SetWindowLongPtr(hiddenMembers->handle, GWLP_USERDATA, (LONG)this);

      // accept drag&drop
      DragAcceptFiles(hiddenMembers->handle, TRUE);

      context.reset(new gl::Context);
    }

    void Window::finalize()
    {
    //  DOUT("Window::finalize()");

      // Do we have a rendering context?
      if (hiddenMembers->glContext)
      {
        // Release the DC and RC contexts
        wglMakeCurrent(NULL, NULL);
        // Delete the rendering context
        wglDeleteContext(hiddenMembers->glContext);
      }

      // Do we have a device context?
      if (hiddenMembers->deviceContext)
      {
        // Release the device context
        ReleaseDC(hiddenMembers->handle, hiddenMembers->deviceContext);
      }

      // Do we have a window?
      if (hiddenMembers->handle)
      {
        DestroyWindow(hiddenMembers->handle);
      }

      delete hiddenMembers;
    }

    void Window::open()
    {
      DOUT("Window::open()");
      ShowWindow(hiddenMembers->handle, SW_SHOW);
    }

    void Window::close()
    {
      DOUT("Window::close()");
      ShowWindow(hiddenMembers->handle, SW_HIDE);
    }

  }
}
