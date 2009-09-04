#ifndef LOST_GL_UTILS_H
#define LOST_GL_UTILS_H

#include <string>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include "lost/common/Logger.h"
#include "lost/gl/gltypes.h"

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
#   define GLDEBUG_THROW { GLenum err = glGetError(); if(err != GL_NO_ERROR) {DOUT("throwing from here"); throw std::runtime_error("GL ERROR:"+lost::gl::utils::getGlErrorAsString(err)+std::string(" Line: ")+boost::lexical_cast<std::string>(__LINE__));};}
#else
#   define GLDEBUG_THROW
#endif

#if TARGET_OPENGL
    std::string enum2string(GLenum inVal);
#endif
}

}

}


#endif
