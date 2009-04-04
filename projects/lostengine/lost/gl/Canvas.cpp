#include "lost/gl/Canvas.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/camera/Camera2D.h"

using namespace lost::bitmap;
using namespace lost::gl;
using namespace lost::math;

namespace lost
{
  namespace gl
  {

    Canvas::Canvas(ContextPtr inContext, const lost::math::Rect& inViewport)
    : context(inContext)
    {
      camera.reset(new camera::Camera2D(context, inViewport));
    }

    Canvas::Canvas(ContextPtr inContext, camera::CameraPtr inCamera)
    : context(inContext),
      camera(inCamera)
    {
    }
    
    void Canvas::clear(GLbitfield flags)
    {
      glClear(flags); GLDEBUG;
    }

    void Canvas::drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end)
    {
      static StatePtr state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true));
      context->pushState(state);
      
      float p[4];
      p[0] = start.x;
      p[1] = start.y;
      p[2] = end.x;
      p[3] = end.y;
      
      glVertexPointer(2, GL_FLOAT, 0, p); GLDEBUG;
      glDrawArrays(GL_LINES, 0,2); GLDEBUG;
      
      context->popState();
    }

    void Canvas::drawLine(const lost::math::Vec3& start, const lost::math::Vec3& end)
    {
      static StatePtr state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true));
      context->pushState(state);
      
      float p[6];
      p[0] = start.x;
      p[1] = start.y;
      p[2] = start.z;
      p[3] = end.x;
      p[4] = end.y;
      p[5] = end.z;
      
      glVertexPointer(3, GL_FLOAT, 0, p); GLDEBUG;
      glDrawArrays(GL_LINES, 0,2); GLDEBUG;
      
      context->popState();
    }

    /**
        using this formula:
          Pnew = P1 * t^3 + P2 * 3 * t^2 * (1-t) + P3 * 3 * t * (1-t)^2 + P4 * (1-t)^3

        see: http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=28
    **/
    void Canvas::drawBezierCurve(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4,
                                 const unsigned int steps)
    {
      static StatePtr state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true),
                                               LineSmooth::create(true));
      context->pushState(state);

      float stepSize = 1.0/steps;
      int length = steps * 2 + 2;
      float p[length];
      int idx = 0;
      
      for (float t = 0.0f; t <= 1.0; t += stepSize)
      {
        Vec2 pStart = p1 * powf(t, 3.0) + p2 * 3.0 * powf(t, 2.0) * (1-t) + p3 * 3.0 * t * powf(1-t, 2.0) + p4 * powf(1-t, 3.0);
        p[idx++] = pStart.x;
        p[idx++] = pStart.y;
      }
      p[idx++] = p1.x;
      p[idx++] = p1.y;
      
      glVertexPointer(2, GL_FLOAT, 0, p); GLDEBUG;
      glDrawArrays(GL_LINE_STRIP, 0, length/2); GLDEBUG;

      context->popState();
    }

    void Canvas::drawBezierCurveFilled(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4,
                                       const Vec2& basePoint, const unsigned int steps)
    {
      static StatePtr state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true),
                                               LineSmooth::create(true));
      context->pushState(state);

      float stepSize = 1.0/steps;
      int length = steps * 4 + 2;
      float p[length];
      int idx = 0;
      
      for (float t = 0.0f; t <= 1.0; t += stepSize)
      {
        Vec2 pStart = p1 * powf(t, 3.0) + p2 * 3.0 * powf(t, 2.0) * (1-t) + p3 * 3.0 * t * powf(1-t, 2.0) + p4 * powf(1-t, 3.0);
        p[idx++] = pStart.x;
        p[idx++] = pStart.y;
        p[idx++] = basePoint.x;
        p[idx++] = basePoint.y;
      }
      p[idx++] = p1.x;
      p[idx++] = p1.y;

      glVertexPointer(2, GL_FLOAT, 0, p); GLDEBUG;
      glDrawArrays(GL_TRIANGLE_STRIP, 0, length/2); GLDEBUG;

      context->popState();
    }
    
    void Canvas::setColor(const lost::common::Color& color)
    {
      glColor4f(color.fv[0], color.fv[1], color.fv[2], color.fv[3]); GLDEBUG; // OpenGL ES compatible
    }
    
    void Canvas::drawPoint(const lost::math::Vec2& point)
    {
      static StatePtr state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true));
      context->pushState(state);
      
      float p[2];
      p[0] = point.x;
      p[1] = point.y;
      
      glVertexPointer(2, GL_FLOAT, 0, p); GLDEBUG;
      glDrawArrays(GL_POINTS, 0, 1); GLDEBUG;

      context->popState();
    }
    
    void Canvas::drawPoint(const lost::math::Vec3& point)
    {
      static StatePtr state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true));
      context->pushState(state);
      
      float p[3];
      p[0] = point.x;
      p[1] = point.y;
      p[2] = point.z;
      
      glVertexPointer(3, GL_FLOAT, 0, p); GLDEBUG;
      glDrawArrays(GL_POINTS, 0, 1); GLDEBUG;
      
      context->popState();
    }
    
    void Canvas::drawRectOutline(const lost::math::Rect& rect)
    {
      drawLine(rect.bottomLeft(), rect.bottomRight());
      drawLine(rect.bottomRight(), rect.topRight());
      drawLine(rect.topRight(), rect.topLeft());
      drawLine(rect.topLeft(), rect.bottomLeft());
    }
    
    void Canvas::drawRectOutlineRounded(const lost::math::Rect& rect, const lost::math::Vec2& cornerSize, const unsigned int steps)
    {
      // bottom line
      Vec2 bottomLeft(rect.x + cornerSize.x, rect.y);
      Vec2 bottomRight(rect.maxX() - cornerSize.x, rect.y);
      // top line
      Vec2 topLeft(rect.x + cornerSize.x, rect.maxY());
      Vec2 topRight(rect.maxX() - cornerSize.x, rect.maxY());
      // left line
      Vec2 leftBottom(rect.x, rect.y + cornerSize.y);
      Vec2 leftTop(rect.x, rect.maxY() - cornerSize.y);
      // right line
      Vec2 rightBottom(rect.maxX(), rect.y + cornerSize.y);
      Vec2 rightTop(rect.maxX(), rect.maxY() - cornerSize.y);
      
      drawLine(bottomLeft, bottomRight);
      drawLine(topLeft, topRight);
      drawLine(leftBottom, leftTop);
      drawLine(rightBottom, rightTop);

      // bottom-left corner
      Vec2 bottomLeft1 = leftBottom;
      Vec2 bottomLeft2 = rect.bottomLeft();
      Vec2 bottomLeft3 = rect.bottomLeft();
      Vec2 bottomLeft4 = bottomLeft;
      // bottom-right corner
      Vec2 bottomRight1 = rightBottom;
      Vec2 bottomRight2 = rect.bottomRight();
      Vec2 bottomRight3 = rect.bottomRight();
      Vec2 bottomRight4 = bottomRight;
      // top-left corner
      Vec2 topLeft1 = leftTop;
      Vec2 topLeft2 = rect.topLeft();
      Vec2 topLeft3 = rect.topLeft();
      Vec2 topLeft4 = topLeft;
      // top-right corner
      Vec2 topRight1 = rightTop;
      Vec2 topRight2 = rect.topRight();
      Vec2 topRight3 = rect.topRight();
      Vec2 topRight4 = topRight;

      drawBezierCurve(bottomLeft1, bottomLeft2, bottomLeft3, bottomLeft4, steps);
      drawBezierCurve(bottomRight1, bottomRight2, bottomRight3, bottomRight4, steps);
      drawBezierCurve(topLeft1, topLeft2, topLeft3, topLeft4, steps);
      drawBezierCurve(topRight1, topRight2, topRight3, topRight4, steps);
    }
    
    void Canvas::drawRectFilled(const lost::math::Rect& rect)
    {
      static StatePtr state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true));
      context->pushState(state);
      
      // points
      lost::math::Vec2 bl = rect.bottomLeft();
      lost::math::Vec2 br = rect.bottomRight();
      lost::math::Vec2 tr = rect.topRight();
      lost::math::Vec2 tl = rect.topLeft();
      
      float verts[] = {br.x+0.5, br.y-0.5, bl.x-0.5, bl.y-0.5, tr.x+0.5, tr.y+0.5, tl.x-0.5, tl.y+0.5};
      glVertexPointer(2, GL_FLOAT, 0, verts); GLDEBUG;
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); GLDEBUG;

      context->popState();
    }
    
    void Canvas::drawRectFilledRounded(const lost::math::Rect& rect, const lost::math::Vec2& cornerSize, const unsigned int steps)
    {
      // middle rect
      Rect middleRect(rect.x + cornerSize.x, rect.y + cornerSize.y, rect.width - cornerSize.x*2, rect.height - cornerSize.y*2);
      // left rect
      Rect leftRect(rect.x, rect.y + cornerSize.y, cornerSize.x + 1, rect.height - cornerSize.y*2);
      // right rect
      Rect rightRect(rect.maxX() - cornerSize.x, rect.y + cornerSize.y, cornerSize.x + 1, rect.height - cornerSize.y*2);
      // top rect
      Rect topRect(rect.x + cornerSize.x, rect.maxY() - cornerSize.y, rect.width - cornerSize.x*2, cornerSize.y + 1);
      // bottom rect
      Rect bottomRect(rect.x + cornerSize.x, rect.y, rect.width - cornerSize.x*2, cornerSize.y + 1);

      /**
          draw the "cross" lacking rounded edges
      **/
      drawRectFilled(middleRect);
      drawRectFilled(leftRect);
      drawRectFilled(rightRect);
      drawRectFilled(topRect);
      drawRectFilled(bottomRect);

      // bottom line
      Vec2 bottomLeft(rect.x + cornerSize.x, rect.y);
      Vec2 bottomRight(rect.maxX() - cornerSize.x, rect.y);
      // top line
      Vec2 topLeft(rect.x + cornerSize.x, rect.maxY());
      Vec2 topRight(rect.maxX() - cornerSize.x, rect.maxY());
      // left line
      Vec2 leftBottom(rect.x, rect.y + cornerSize.y);
      Vec2 leftTop(rect.x, rect.maxY() - cornerSize.y);
      // right line
      Vec2 rightBottom(rect.maxX(), rect.y + cornerSize.y);
      Vec2 rightTop(rect.maxX(), rect.maxY() - cornerSize.y);
      
      // bottom-left corner
      Vec2 bottomLeft1 = leftBottom;
      Vec2 bottomLeft2 = rect.bottomLeft();
      Vec2 bottomLeft3 = rect.bottomLeft();
      Vec2 bottomLeft4 = bottomLeft;
      Vec2 bottomLeftBase(bottomLeft.x-0.5, leftBottom.y-0.5);
      // bottom-right corner
      Vec2 bottomRight1 = rightBottom;
      Vec2 bottomRight2 = rect.bottomRight();
      Vec2 bottomRight3 = rect.bottomRight();
      Vec2 bottomRight4 = bottomRight;
      Vec2 bottomRightBase(bottomRight.x+0.5, rightBottom.y-0.5);
      // top-left corner
      Vec2 topLeft1 = leftTop;
      Vec2 topLeft2 = rect.topLeft();
      Vec2 topLeft3 = rect.topLeft();
      Vec2 topLeft4 = topLeft;
      Vec2 topLeftBase(topLeft.x-0.5, leftTop.y+0.5);
      // top-right corner
      Vec2 topRight1 = rightTop;
      Vec2 topRight2 = rect.topRight();
      Vec2 topRight3 = rect.topRight();
      Vec2 topRight4 = topRight;
      Vec2 topRightBase(topRight.x+0.5, rightTop.y+0.5);
      
      /**
          simple but effective hack: draw antialiased lines over the edges to get rid of pitches
      **/
      drawBezierCurveFilled(bottomLeft1, bottomLeft2, bottomLeft3, bottomLeft4, bottomLeftBase, steps);
      drawBezierCurve(bottomLeft1, bottomLeft2, bottomLeft3, bottomLeft4, steps);
      drawBezierCurveFilled(bottomRight1, bottomRight2, bottomRight3, bottomRight4, bottomRightBase, steps);
      drawBezierCurve(bottomRight1, bottomRight2, bottomRight3, bottomRight4, steps);
      drawBezierCurveFilled(topLeft1, topLeft2, topLeft3, topLeft4, topLeftBase, steps);
      drawBezierCurve(topLeft1, topLeft2, topLeft3, topLeft4, steps);
      drawBezierCurveFilled(topRight1, topRight2, topRight3, topRight4, topRightBase, steps);
      drawBezierCurve(topRight1, topRight2, topRight3, topRight4, steps);
    }
    
    // rect is drawn counterclockwise
    // requires vertex arrays, texture arrays, index arrays
    // FIXME: needs serious optimisation/rethinking, but at least it works
    void Canvas::drawRectTextured(const lost::math::Rect& rect,
                                  boost::shared_ptr<const lost::gl::Texture> tex,
                                  const lost::math::Vec2& bottomLeft,
                                  const lost::math::Vec2& bottomRight,
                                  const lost::math::Vec2& topLeft,
                                  const lost::math::Vec2& topRight,
                                  bool flip)
    {
      static StatePtr state = State::create(DepthTest::create(false),
                                               Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(true),
                                               VertexArray::create(true),
                                               TextureArray::create(true));
//      context->makeCurrent();
      context->pushState(state);
      
      tex->bind();
      
      // points
      lost::math::Vec2 bl(rect.x, rect.y);
      lost::math::Vec2 br(rect.maxX(), rect.y);
      lost::math::Vec2 tr(rect.maxX(), rect.maxY());
      lost::math::Vec2 tl(rect.x, rect.maxY());
      
      // build arrays
      float verts[] = {bl.x-0.5, bl.y-0.5, br.x+0.5, br.y-0.5, tr.x+0.5, tr.y+0.5, tl.x-0.5, tl.y+0.5};
      float texcoordsNormal[] = {bottomLeft.x, bottomLeft.y, bottomRight.x, bottomRight.y, topRight.x, topRight.y, topLeft.x, topLeft.y};
      float texcoordsFlipped[] = {topLeft.x, topLeft.y, topRight.x, topRight.y, bottomRight.x, bottomRight.y, bottomLeft.x, bottomLeft.y};
      // indices for 2 triangles, first upper left, then lower right
      GLubyte idx[] = {0,2,3,0,1,2};
      
      glVertexPointer(2,GL_FLOAT,0,verts); GLDEBUG;
      if(flip)
      {
        glTexCoordPointer(2,GL_FLOAT,0,texcoordsFlipped); GLDEBUG;
      }
      else
      {
        glTexCoordPointer(2,GL_FLOAT,0,texcoordsNormal); GLDEBUG;
      }
      
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, idx); GLDEBUG;

      context->popState();
    }
    
    void Canvas::drawRectTextured(const lost::math::Rect& rect,
                                  boost::shared_ptr<const lost::gl::Texture> tex,
                                  bool flip)
    {
      drawRectTextured(rect, tex, Vec2(0,0), Vec2(1,0), Vec2(0,1), Vec2(1,1), flip);
    }
    
    void Canvas::drawMesh2D(const boost::shared_ptr<Mesh2D>& mesh, GLenum mode)
    {
      static StatePtr state = State::create(DepthTest::create(false),
                                               Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(true),
                                               VertexArray::create(true),
                                               TextureArray::create(true));
//      context->makeCurrent();
      context->pushState(state);

      mesh->texture->bind();
      
      glVertexPointer(2, GL_FLOAT, 0, mesh->vertices.get()); GLDEBUG;
      glTexCoordPointer(2, GL_FLOAT, 0, mesh->texcoords.get()); GLDEBUG;
      glDrawElements(mode, mesh->faceCount, GL_UNSIGNED_BYTE, mesh->faces.get()); GLDEBUG;
      
      context->popState();
    }
    
    void Canvas::drawMesh3D(const boost::shared_ptr<Mesh3D>& mesh, GLenum mode)
    {
      static StatePtr state = State::create(DepthTest::create(false),
                                               Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true),
                                               TextureArray::create(false));
//      context->makeCurrent();
      context->pushState(state);
      
      glVertexPointer(3, GL_FLOAT, 0, mesh->vertices.get()); GLDEBUG;
      glDrawElements(mode, mesh->faceCount, GL_UNSIGNED_BYTE, mesh->faces.get()); GLDEBUG;
      
      context->popState();
    }
    
    void Canvas::drawAABB(const lost::math::AABB& box)
    {
      drawLine(math::Vec3(box.origin.x, box.origin.y, box.origin.z), math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z));    
      drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z), math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z));
      drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z), math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z));    
      drawLine(math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z), math::Vec3(box.origin.x, box.origin.y, box.origin.z));
      drawLine(math::Vec3(box.origin.x, box.origin.y, box.origin.z),math::Vec3(box.origin.x, box.origin.y, box.origin.z + box.size.z));    
      drawLine(math::Vec3(box.origin.x, box.origin.y, box.origin.z + box.size.z), math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z + box.size.z));    
      drawLine(math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z + box.size.z), math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z));    
      drawLine(math::Vec3(box.origin.x, box.origin.y, box.origin.z + box.size.z), math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z + box.size.z));
      drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z + box.size.z), math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z + box.size.z));
      drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z + box.size.z), math::Vec3(box.origin.x, box.origin.y + box.size.y, box.origin.z + box.size.z));
      drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z + box.size.z),math::Vec3(box.origin.x + box.size.x, box.origin.y, box.origin.z));
      drawLine(math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z + box.size.z),math::Vec3(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z));
    }
    
    void Canvas::drawAxes(const lost::math::Vec3& length)
    {
      // Draw the positive side of the lines x,y,z
      setColor(lost::common::greenColor);         // Green for x axis
      drawLine(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3(length.x, 0.0f, 0.0f));
      setColor(lost::common::redColor);           // Red for y axis
      drawLine(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3(0.0f, length.y, 0.0f));
      setColor(lost::common::blueColor);          // Blue for z axis
      drawLine(math::Vec3(0.0f, 0.0f, 0.0f), math::Vec3(0.0f, 0.0f, length.z));
      
      // transparent lines for the negative sides of x,y,z coordinates
      // this is kinda stupid, but OpenGL ES doesn't have line stippling anymore
      float a = .15f;
      glColor4f(0,1,0,a);         // Green for x axis
      drawLine(lost::math::Vec3(0.0f, 0.0f, 0.0f), lost::math::Vec3(-length.x, 0.0f, 0.0f));
      glColor4f(1,0,0,a);           // Red for y axis
      drawLine(lost::math::Vec3(0.0f, 0.0f, 0.0f), lost::math::Vec3(0.0f, -length.y, 0.0f));
      glColor4f(0,0,1,a);          // Blue for z axis
      drawLine(lost::math::Vec3(0.0f, 0.0f, 0.0f), lost::math::Vec3(0.0f, 0.0f, -length.z));
    }

    /** Uses glReadPixels to retrieve the current framebuffer data as rgba and saves it
     * as a tga file to the specified file path.
     *
     * @param fullPathName full path name of file to be saved. You must ensure that the location is writable.
     *        
     */
    void Canvas::writeScreenshot(const std::string& fullPathName,
                                 bool withAlphaChannel)
    {
      GLenum format = withAlphaChannel ? GL_RGBA : GL_RGB;
      Bitmap::Components bitmapFormat = withAlphaChannel ? Bitmap::COMPONENTS_RGBA : Bitmap::COMPONENTS_RGB;
      Bitmap bmp(camera->viewport.width, camera->viewport.height, bitmapFormat);
      
      glReadPixels(0,
                   0,
                   bmp.width,
                   bmp.height,
                   format,
                   GL_UNSIGNED_BYTE,                        
                   bmp.data); GLDEBUG_THROW;
      bmp.flip();
      bmp.write(fullPathName);
    } 

    // jomanto: copied from http://www.songho.ca/opengl/gl_vertexarray.html
    void  Canvas::drawSolidCube( GLfloat dSize )
    {
      float size = dSize * 0.5;
      
      //    v6----- v5
      //   /|      /|
      //  v1------v0|
      //  | |     | |
      //  | |v7---|-|v4
      //  |/      |/
      //  v2------v3

      #define V(a,b,c) a size, b size, c size
      GLfloat vertices[] = { V(+,+,+),  V(-,+,+),  V(-,-,+),  V(+,-,+),         // v0 - v1 - v2 - v3
                             V(+,+,+),  V(+,-,+),  V(+,-,-),  V(+,+,-),         // v0 - v3 - v4 - v5
                             V(+,+,+),  V(+,+,-),  V(-,+,-),  V(-,+,+),         // v0 - v5 - v6 - v1
                             V(-,+,+),  V(-,+,-),  V(-,-,-),  V(-,-,+),         // v1 - v6 - v7 - v2
                             V(-,-,-),  V(+,-,-),  V(+,-,+),  V(-,-,+),         // v7 - v4 - v3 - v2
                             V(+,-,-),  V(-,-,-),  V(-,+,-),  V(+,+,-) };       // v4 - v7 - v6 - v5
      #undef V

      static GLfloat normals[] = {  0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,         // v0 - v1 - v2 - v3
                                    1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,         // v0 - v3 - v4 - v5
                                    0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,         // v0 - v5 - v6 - v1
                                   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,         // v1 - v6 - v7 - v2
                                    0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,         // v7 - v4 - v3 - v2
                                    0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1};        // v4 - v7 - v6 - v5

      static GLubyte indices[] = { 0,   1,  2,  3,
                                   4,   5,  6,  7,
                                   8,   9, 10, 11,
                                   12, 13, 14, 15,
                                   16, 17, 18, 19,
                                   20, 21, 22, 23 };

      static StatePtr state = State::create(VertexArray::create(true),
                                            NormalArray::create(true));
      context->pushState(state);
      glNormalPointer(GL_FLOAT, 0, normals); GLDEBUG;
      glVertexPointer(3, GL_FLOAT, 0, vertices); GLDEBUG;
      glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indices); GLDEBUG;
      context->popState();
    }

  }
}
