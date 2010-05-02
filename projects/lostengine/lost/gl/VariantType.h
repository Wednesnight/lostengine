#ifndef LOST_GL_VARIANTTYPE_H
#define LOST_GL_VARIANTTYPE_H

namespace lost
{
  namespace gl
  {

    enum VariantType
    {
      VT_undefined=0,
      VT_int,
      VT_float,
      VT_bool,
      VT_color,
      VT_vec2,
      VT_vec3,
      VT_vec4,
      VT_matrix
    };

  }
}

#endif
