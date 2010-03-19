#ifndef LOST_GL_UTILS_H
#define LOST_GL_UTILS_H

#include <string>
#include <stdexcept>
#include "lost/common/Logger.h"
#include "lost/gl/gltypes.h"
#include <sstream>

namespace lost
{
namespace common {  struct DisplayAttributes; };
namespace math { struct Vec2; struct Vec3; };

namespace gl
{

namespace utils
{
  std::string getGlErrorAsString(GLenum err);
  
#if defined(LOST_GL_ENABLE_DEBUG)
#   define GLDEBUG { GLenum err = glGetError(); if(err != GL_NO_ERROR) {DOUT("GL ERROR:"+lost::gl::utils::getGlErrorAsString(err));};}
#else
#   define GLDEBUG
#endif

#if defined(LOST_GL_ENABLE_DEBUG_THROW)
#   define GLDEBUG_THROW { GLenum err = glGetError(); if(err != GL_NO_ERROR) { \
    DOUT("throwing from here"); \
    std::ostringstream os; os << "GL ERROR:" << lost::gl::utils::getGlErrorAsString(err) << " Line: " << __LINE__; \
    throw std::runtime_error(os.str());};}
#else
#   define GLDEBUG_THROW
#endif

    std::string enum2string(GLenum inVal);
}

}

}


#endif
