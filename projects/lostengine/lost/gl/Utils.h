#ifndef LOST_GL_UTILS_H
#define LOST_GL_UTILS_H

#include "lost/gl/gl.h"
#include "lost/common/Logger.h"
#include "lost/math/Vec2.h"
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include "lost/math/Vec3.h"
#include "stb_image.h"
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include "lost/common/DisplayAttributes.h"

#include "lost/lgl/lglu.h"

namespace lost
{

namespace gl
{

namespace utils
{
  static inline std::string getGlErrorAsString(GLenum err)
  {
    // FIXME: we need a cleaner way to handle this, maybe move error handling/debugging to a dedicated header
    #if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_IPHONE)
      // there are only a handful of error codes in OpenGL ES, so we switch them by hand
      std::string result;
      switch(err)
      {
        case GL_NO_ERROR:result="GL_NO_ERROR";break;
        case GL_INVALID_ENUM:result="GL_INVALID_ENUM";break;
        case GL_INVALID_VALUE:result="GL_INVALID_VALUE";break;
        case GL_INVALID_OPERATION:result="GL_INVALID_OPERATION";break;
        case GL_STACK_OVERFLOW:result="GL_STACK_OVERFLOW";break;
        case GL_STACK_UNDERFLOW:result="GL_STACK_UNDERFLOW";break;
        case GL_OUT_OF_MEMORY:result="GL_OUT_OF_MEMORY";break;
        default:
          std::ostringstream os;
          os << "error 0x" << std::hex << err;
          result = os.str();
      }
      return result;
    #else
      const char* buf = reinterpret_cast<const char*>(gluErrorString(err));
      if(buf)
        return std::string(buf);
      else
      {
        // null pointer most probably means some extension produced an error that standard gluErrorString doesn't know
        // so we just output a hex number
        std::ostringstream os;
        os << "error 0x" << std::hex <<  err;
        return os.str();
      }
    #endif
  }

#define GLDEBUG { GLenum err = glGetError(); if(err != GL_NO_ERROR) {DOUT("GL ERROR:"+lost::gl::utils::getGlErrorAsString(err));};}
#define GLDEBUG_THROW { GLenum err = glGetError(); if(err != GL_NO_ERROR) {DOUT("throwing from here"); throw std::runtime_error("GL ERROR:"+lost::gl::utils::getGlErrorAsString(err)+std::string(" Line: ")+boost::lexical_cast<std::string>(__LINE__));};}

    inline void set2DProjection(const lost::math::Vec2& offset, const lost::math::Vec2& dimension)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        lgluOrtho2D(offset.x, dimension.w-1, offset.y, dimension.h-1);
    }

    inline std::string getVersion()
    {
      return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }

    inline void set3DProjection(int windowWidth, int windowHeight,
                         lost::math::Vec3 eye, lost::math::Vec3 at, lost::math::Vec3 up,
                         float fovy, float znear, float zfar)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double screenAspectRatio = (double)windowWidth/(double)windowHeight;
        lgluPerspective(fovy, screenAspectRatio, znear, zfar);
        lgluLookAt(eye.x,  eye.y,  eye.z,
                  at.x,   at.y,   at.z,
                  up.x,   up.y,   up.z);
    }

    inline unsigned long nextPowerOf2(unsigned long inVal)
    {
      unsigned long result = 1;

      while(result < inVal)
      {
        result <<= 1;
      }

      return result;
    }

    /** Uses glReadPixels to retrieve the current framebuffer data as rgba and saves it
     * as a tga file to the specified file path.
     *
     * @param fullPathName full path name of file to be saved. You must ensure that the location is writable.
     *        
     */
    static inline void saveScreenshotTGA(const common::DisplayAttributes& displayAttributes,
                                      const std::string& fullPathName,
                                      bool withAlphaChannel=false)
    {
      // reserve memory for read data
      unsigned long pixelSizeBytes = withAlphaChannel ? 4 : 3;
      GLenum format = withAlphaChannel ? GL_RGBA : GL_RGB;
      
      unsigned long memSizeBytes = displayAttributes.width * displayAttributes.height * pixelSizeBytes;
      boost::scoped_array<unsigned char> data(new unsigned char[memSizeBytes]);
      
      glReadPixels(0,
                   0,
                   displayAttributes.width,
                   displayAttributes.height,
                   format,
                   GL_UNSIGNED_BYTE,                        
                   data.get());GLDEBUG_THROW;
      // flip vertically because OpenGL returns it the other way round
      unsigned long lineInBytes = displayAttributes.width * pixelSizeBytes;
      unsigned long halfHeight = displayAttributes.height / 2; // deliberately round down if height is odd
      unsigned char* dataBytes = data.get();
      for(unsigned long bottomLine=0; bottomLine<halfHeight; ++bottomLine)
      {
        unsigned long topLine = (displayAttributes.height - 1) - bottomLine;
        for(unsigned long bi=0; bi<lineInBytes; ++bi)
        {
          unsigned long topLineByte = displayAttributes.width*topLine*pixelSizeBytes+bi;
          unsigned long bottomLineByte = displayAttributes.width*bottomLine*pixelSizeBytes+bi;
          unsigned char b = dataBytes[topLineByte];
          dataBytes[topLineByte] = dataBytes[bottomLineByte];
          dataBytes[bottomLineByte] = b;
        }
      }

      if(!stbi_write_tga(fullPathName.c_str(), displayAttributes.width, displayAttributes.height, pixelSizeBytes, data.get()))
        throw std::runtime_error("screenshot save failed");
    } 

#ifdef LOST_IPHONE
#else
    #define enum2string_helper(glenum) case glenum: return #glenum;break;

    inline static std::string enum2string(GLenum inVal)
    {
      switch(inVal)
      {
        enum2string_helper(GL_FLOAT)
        enum2string_helper(GL_FLOAT_VEC2)
        enum2string_helper(GL_FLOAT_VEC3)
        enum2string_helper(GL_FLOAT_VEC4)
        enum2string_helper(GL_INT)
        enum2string_helper(GL_INT_VEC2)
        enum2string_helper(GL_INT_VEC3)
        enum2string_helper(GL_INT_VEC4)
        enum2string_helper(GL_BOOL)
        enum2string_helper(GL_BOOL_VEC2)
        enum2string_helper(GL_BOOL_VEC3)
        enum2string_helper(GL_BOOL_VEC4)
        enum2string_helper(GL_FLOAT_MAT2)
        enum2string_helper(GL_FLOAT_MAT3)
        enum2string_helper(GL_FLOAT_MAT4)
        enum2string_helper(GL_FLOAT_MAT2x3)
        enum2string_helper(GL_FLOAT_MAT2x4)
        enum2string_helper(GL_FLOAT_MAT3x2)
        enum2string_helper(GL_FLOAT_MAT3x4)
        enum2string_helper(GL_FLOAT_MAT4x2)
        enum2string_helper(GL_FLOAT_MAT4x3)
        enum2string_helper(GL_SAMPLER_1D)
        enum2string_helper(GL_SAMPLER_2D)
        enum2string_helper(GL_SAMPLER_3D)
        enum2string_helper(GL_SAMPLER_CUBE)
        enum2string_helper(GL_SAMPLER_1D_SHADOW)
        enum2string_helper(GL_SAMPLER_2D_SHADOW)
        enum2string_helper(GL_FRAMEBUFFER_COMPLETE_EXT)
        enum2string_helper(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT)
        enum2string_helper(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT)
        enum2string_helper(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)
        enum2string_helper(GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT)
        enum2string_helper(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT)
        enum2string_helper(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT)
        enum2string_helper(GL_FRAMEBUFFER_UNSUPPORTED_EXT)

        default: throw std::runtime_error("enum2string: don't know enum: "+boost::lexical_cast<std::string>(inVal));
      }
    }
#endif
}

}

}

#endif
