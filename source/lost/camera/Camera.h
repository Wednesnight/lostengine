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

#ifndef LOST_CAMERA_CAMERA_H
#define LOST_CAMERA_CAMERA_H

#include "lost/math/Rect.h"
#include "lost/math/Matrix.h"
#include "lost/math/Vec2.h"
#include "lost/platform/shared_ptr.h"

namespace lost
{
  namespace camera
  {
    struct Camera;
    typedef lost::shared_ptr<Camera> CameraPtr;
    struct Camera
    {
    protected:
      lost::math::Rect mViewport;
      math::Vec2       mDepth;

      lost::math::Matrix mViewMatrix;
      lost::math::Matrix mProjectionMatrix;
      
      virtual void update() = 0;
    public:
      bool needsUpdate;

      Camera(const lost::math::Rect& inViewport);
      virtual ~Camera() {};
      
      lost::math::Rect& viewport(const lost::math::Rect& inViewport);
      lost::math::Rect& viewport();

      math::Vec2 depth();
      void depth(const math::Vec2& inDepth);

      lost::math::Matrix& viewMatrix();
      lost::math::Matrix& projectionMatrix();
    };

  }
}

#endif
