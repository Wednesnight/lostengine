#ifndef GL_STATEPARAM_H
#define GL_STATEPARAM_H

#include "lost/gl/Param.h"

namespace lost
{
  namespace gl
  {
    
    typedef ServerStateParam<GL_ALPHA_TEST>   AlphaTest;
    typedef ServerStateParam<GL_DEPTH_TEST>   DepthTest;
    typedef ServerStateParam<GL_TEXTURE_2D>   Texture2D;
    typedef ServerStateParam<GL_BLEND>        Blend;
    typedef ServerStateParam<GL_SCISSOR_TEST> Scissor;
    typedef ServerStateParam<GL_POINT_SMOOTH> PointSmooth;
    typedef ServerStateParam<GL_LINE_SMOOTH>  LineSmooth;

    typedef ClientStateParam<GL_COLOR_ARRAY>        ColorArray;
    typedef ClientStateParam<GL_NORMAL_ARRAY>        NormalArray;
    typedef ClientStateParam<GL_VERTEX_ARRAY>        VertexArray;
    typedef ClientStateParam<GL_TEXTURE_COORD_ARRAY> TextureArray;

    typedef ColorParam<GL_COLOR_CLEAR_VALUE> ClearColor;

    // just for the listing...
    typedef BlendFunc  BlendFunc;
    typedef ScissorBox ScissorBox;

  }
}

#endif
