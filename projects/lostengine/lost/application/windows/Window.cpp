#include "lost/application/Window.h"
#include "lost/common/Logger.h"
#include "lost/gl/Context.h"
#include <windows.h>

namespace lost
{
  namespace application
  {

    LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
      Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
      switch (message)
      {
        case WM_CREATE:
          break;

        case WM_CLOSE:
          break;

        case WM_DESTROY:
          break;

        case WM_KEYDOWN:
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
                                             WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
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
