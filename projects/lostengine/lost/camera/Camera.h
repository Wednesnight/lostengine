#ifndef LOST_CAMERA_CAMERA_H
#define LOST_CAMERA_CAMERA_H

#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"
#include "lost/math/Matrix.h"

namespace lost
{
  namespace camera
  {

    extern const math::Vec3 positionBase;
    extern const math::Vec3 directionBase;
    extern const math::Vec3 rotationBase;
    extern const float      fovYBase;
    extern const math::Vec2 depthBase;
    extern const math::Vec3 upBase;

    struct Camera
    {
    private:
      math::Vec3 mPosition;
      math::Vec3 mDirection;
      math::Vec3 mRotation;
      float      mFovY;
      math::Vec2 mDepth;
    public:
      // constructor
      Camera();

      // getters
      math::Vec2& depth();
      float& fovY();
      math::Vec3& position();
      math::Vec3 direction();
      math::Vec3& rotation();
      math::Vec3 target();
      math::Vec3 up();

      // setters
      void fovY(const float newFovY);
      void depth(const math::Vec2& newDepth);
      void position(const math::Vec3& newPosition);
      void direction(const math::Vec3& newDirection);
      void rotation(const math::Vec3& newRotation);
      void target(const math::Vec3& newTarget);

      // methods
      void move(const math::Vec3& deltaMove);
      void rotate(const math::Vec3& deltaRotate);
    };

  }
}

#endif
