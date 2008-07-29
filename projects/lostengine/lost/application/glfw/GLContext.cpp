#include "lost/application/GLContext.h"
#include "GL/glfw.h"
namespace lost
{
  namespace application
  {
    void GLContext::swapBuffers()
    {
      glfwSwapBuffers();
    }
  }
}