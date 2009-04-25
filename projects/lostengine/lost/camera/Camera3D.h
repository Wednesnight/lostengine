#ifndef LOST_CAMERA_CAMERA3D_H
#define LOST_CAMERA_CAMERA3D_H

#include "lost/camera/Camera.h"

#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Matrix.h"
#include "lost/gl/Context.h"

namespace lost
{
  namespace camera
  {
    struct Camera3D;
    typedef lost::shared_ptr<Camera3D> Camera3DPtr;

    struct Camera3D : public Camera
    {
    private:
      math::Vec3 mPosition;
      math::Vec3 mDirection;
      math::Vec3 mRotation;
      float      mFovY;
      math::Vec2 mDepth;
      bool       mStickToTarget;
      math::Vec3 mPlane;
    public:
      // constructor
      Camera3D(const lost::math::Rect& inViewport);
      static Camera3DPtr create(const lost::math::Rect& inViewport);
      // getters
      math::Vec2 depth();
      float fovY();
      math::Vec3 position();
      math::Vec3 direction();
      math::Vec3 rotation();
      bool stickToTarget();
      math::Vec3 target();
      math::Vec3 up();

      // setters
      void plane(const math::Vec3& inPlane);
      void depth(const math::Vec2& inDepth);
      void fovY(const float inFovY);
      void position(const math::Vec3& newPosition);
      void direction(const math::Vec3& newDirection);
      void stickToTarget(const bool newStickToTarget);
      void rotation(const math::Vec3& newRotation);
      void target(const math::Vec3& newTarget);

      // methods
      void move(const math::Vec3& deltaMove);
      void rotate(const math::Vec3& deltaRotate);

      lost::math::Matrix& matrix();
    };

  }
}

#endif
