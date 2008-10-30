
#include "lost/gl/Utils.h"
#include "lost/gl/gl.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "stb_image.h"
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include "lost/common/DisplayAttributes.h"
#include "lost/lgl/lglu.h"
#include "lost/bitmap/Bitmap.h"

using namespace lost::bitmap;

namespace lost
{

namespace gl
{

namespace utils
{
  std::string getGlErrorAsString(GLenum err)
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


    void set2DProjection(const lost::math::Vec2& offset, const lost::math::Vec2& dimension)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        lgluOrtho2D(offset.x, dimension.w-1, offset.y, dimension.h-1);
    }

    std::string getVersion()
    {
      return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }

    void set3DProjection(int windowWidth, int windowHeight,
                         const lost::math::Vec3& eye,
                         const lost::math::Vec3& at,
                         const lost::math::Vec3& up,
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

    /** Uses glReadPixels to retrieve the current framebuffer data as rgba and saves it
     * as a tga file to the specified file path.
     *
     * @param fullPathName full path name of file to be saved. You must ensure that the location is writable.
     *        
     */
    void saveScreenshotTGA(const common::DisplayAttributes& displayAttributes,
                                      const std::string& fullPathName,
                                      bool withAlphaChannel)
    {
      GLenum format = withAlphaChannel ? GL_RGBA : GL_RGB;
      Bitmap::Components bitmapFormat = withAlphaChannel ? Bitmap::COMPONENTS_RGBA : Bitmap::COMPONENTS_RGB;
      Bitmap bmp(displayAttributes.width, displayAttributes.height, bitmapFormat);
            
      glReadPixels(0,
                   0,
                   bmp.width,
                   bmp.height,
                   format,
                   GL_UNSIGNED_BYTE,                        
                   bmp.data);GLDEBUG_THROW;
      bmp.flip();
      bmp.write(fullPathName);
    } 

#if defined(TARGET_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
#else
    #define enum2string_helper(glenum) case glenum: return #glenum;break;

    std::string enum2string(GLenum inVal)
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

