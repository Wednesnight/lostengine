#ifndef GL_STATEPARAM_H
#define GL_STATEPARAM_H

#include "lost/application/gl/Param.h"

namespace lost
{
  namespace application
  {
    namespace gl
    {
      
      typedef ServerStateParam<GL_ALPHA_TEST> AlphaTest;
      typedef ServerStateParam<GL_DEPTH_TEST> DepthTest;
      typedef ServerStateParam<GL_TEXTURE_2D> Texture2D;
      typedef ServerStateParam<GL_BLEND>      Blend;

      typedef ClientStateParam<GL_NORMAL_ARRAY>        NormalArray;
      typedef ClientStateParam<GL_VERTEX_ARRAY>        VertexArray;
      typedef ClientStateParam<GL_TEXTURE_COORD_ARRAY> TextureArray;

      typedef ColorParam<GL_COLOR_CLEAR_VALUE> ClearColor;

      typedef BlendFunc BlendFunc;

    }
  }
}

#endif
