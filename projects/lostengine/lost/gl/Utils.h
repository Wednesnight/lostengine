#ifndef LOST_GL_UTILS_H
#define LOST_GL_UTILS_H

#include "lost/gl/gl.h"
#include <string>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include "lost/common/Logger.h"


namespace lost
{
namespace common {  struct DisplayAttributes; };
namespace math { struct Vec2; struct Vec3; };

namespace gl
{

namespace utils
{
  std::string getGlErrorAsString(GLenum err);
#define GLDEBUG { GLenum err = glGetError(); if(err != GL_NO_ERROR) {DOUT("GL ERROR:"+lost::gl::utils::getGlErrorAsString(err));};}
#define GLDEBUG_THROW { GLenum err = glGetError(); if(err != GL_NO_ERROR) {DOUT("throwing from here"); throw std::runtime_error("GL ERROR:"+lost::gl::utils::getGlErrorAsString(err)+std::string(" Line: ")+boost::lexical_cast<std::string>(__LINE__));};}

    void set2DProjection(const lost::math::Vec2& offset, const lost::math::Vec2& dimension);
    std::string getVersion();

    void set3DProjection(int windowWidth, int windowHeight,
                         const lost::math::Vec3& eye,
                         const lost::math::Vec3& at,
                         const lost::math::Vec3& up,
                         float fovy, float znear, float zfar);

    /** Uses glReadPixels to retrieve the current framebuffer data as rgba and saves it
     * as a tga file to the specified file path.
     *
     * @param fullPathName full path name of file to be saved. You must ensure that the location is writable.
     *        
     */
    void saveScreenshotTGA(const common::DisplayAttributes& displayAttributes,
                                      const std::string& fullPathName,
                                      bool withAlphaChannel=false);

#if defined(TARGET_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
#else
    std::string enum2string(GLenum inVal);
#endif
}

}

}


#endif
