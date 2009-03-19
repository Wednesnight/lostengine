#ifndef LOST_APPLICATION_GL_CANVAS_H
#define LOST_APPLICATION_GL_CANVAS_H

#include "lost/gl/Context.h"
#include "lost/camera/Camera.h"
#include "lost/gl/gl.h"
#include "lost/gl/Texture.h"
#include "lost/gl/Mesh.h"
#include "lost/math/AABB.h"
#include "lost/math/Vec2.h"
#include "lost/math/Vec3.h"

namespace lost
{
  namespace gl
  {
  
    struct Canvas
    {
    public:
      boost::shared_ptr<Context> context;
      boost::shared_ptr<camera::Camera> camera;

      Canvas(const boost::shared_ptr<Context>& inContext, const lost::math::Rect& inViewport);
      Canvas(const boost::shared_ptr<Context>& inContext, const boost::shared_ptr<camera::Camera>& inCamera);

      void clear(GLbitfield flags);

      void setColor(const lost::common::Color& color);

      void drawPoint(const lost::math::Vec2& point);
      void drawPoint(const lost::math::Vec3& point);

      void drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end);
      void drawLine(const lost::math::Vec3& start, const lost::math::Vec3& end);

      void drawBezierCurve(const lost::math::Vec2& p1, const lost::math::Vec2& p2, const lost::math::Vec2& p3, const lost::math::Vec2& p4,
                           const unsigned int steps = 25);
      void drawBezierCurveFilled(const lost::math::Vec2& p1, const lost::math::Vec2& p2, const lost::math::Vec2& p3, const lost::math::Vec2& p4,
                                 const lost::math::Vec2& basePoint, const unsigned int steps = 25);

      void drawRectOutline(const lost::math::Rect& rect);
      void drawRectOutlineRounded(const lost::math::Rect& rect, const lost::math::Vec2& cornerSize, const unsigned int steps = 25);
      void drawRectFilled(const lost::math::Rect& rect);
      void drawRectFilledRounded(const lost::math::Rect& rect, const lost::math::Vec2& cornerSize, const unsigned int steps = 25);
      void drawRectTextured(const lost::math::Rect& rect, boost::shared_ptr<const lost::gl::Texture> tex, const lost::math::Vec2& bottomLeft,
                            const lost::math::Vec2& bottomRight, const lost::math::Vec2& topLeft, const lost::math::Vec2& topRight,
                            bool flip);
      void drawRectTextured(const lost::math::Rect& rect, boost::shared_ptr<const lost::gl::Texture> tex, bool flip);

      void drawMesh2D(const boost::shared_ptr<lost::gl::Mesh2D>& mesh, GLenum mode);
      void drawMesh3D(const boost::shared_ptr<lost::gl::Mesh3D>& mesh, GLenum mode);

      void drawAABB(const lost::math::AABB& box);
      void drawAxes(const lost::math::Vec3& length);

#if TARGET_OPENGL
      void drawSolidCube(GLfloat size);
#endif

      void writeScreenshot(const std::string& fullPathName, bool withAlphaChannel);
    };

  }
}

#endif
