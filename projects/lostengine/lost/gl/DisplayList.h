#ifndef LOST_GL_DISPLAYLIST_H
#define LOST_GL_DISPLAYLIST_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"

namespace lost
{
  namespace gl
  {
    struct DisplayList
    {
      DisplayList()
      {
        displayList = glGenLists(1);GLDEBUG_THROW;
      }

      void begin()
      {
        glNewList(displayList, GL_COMPILE);GLDEBUG_THROW;
      }

      void end()
      {
        glEndList();GLDEBUG_THROW;
      }

      void call()
      {
        glCallList(displayList);
      }

      ~DisplayList()
      {
        glDeleteLists(displayList, 1);GLDEBUG;
      }

    private:
      GLuint displayList;
    };
  }
}

#endif
