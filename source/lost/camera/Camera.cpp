/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/camera/Camera.h"

namespace lost
{
  namespace camera
  {
    
    Camera::Camera(const lost::math::Rect& inViewport)
    : mViewport(inViewport),
      needsUpdate(true)
    {
      mViewMatrix.initIdentity();
      mProjectionMatrix.initIdentity();
    }
    
    lost::math::Rect& Camera::viewport(const lost::math::Rect& inViewport)
    {
      if (mViewport != inViewport)
      {
        mViewport = inViewport;
        needsUpdate = true;
      }
      return mViewport;
    }

    lost::math::Rect& Camera::viewport()
    {
      return mViewport;
    }

    math::Vec2 Camera::depth()
    {
      return mDepth;
    }
    
    void Camera::depth(const math::Vec2& inDepth)
    {
      mDepth = inDepth;
      needsUpdate = true;
    }
    
    lost::math::Matrix& Camera::viewMatrix()
    {
      if (needsUpdate) update();
      return mViewMatrix;
    }

    lost::math::Matrix& Camera::projectionMatrix()
    {
      if (needsUpdate) update();
      return mProjectionMatrix;
    }

  }
}
