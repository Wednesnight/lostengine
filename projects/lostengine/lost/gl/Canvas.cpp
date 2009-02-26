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

    Canvas::Canvas(const boost::shared_ptr<Context>& inContext, const lost::math::Rect& inViewport)
    : context(inContext)
    {
      camera.reset(new camera::Camera2D(context, inViewport));
    }

    Canvas::Canvas(const boost::shared_ptr<Context>& inContext, const boost::shared_ptr<camera::Camera>& inCamera)
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
      static SharedState state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true));
      context->makeCurrent();
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
      static SharedState state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true));
      context->makeCurrent();
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
    
    void Canvas::setColor(const lost::common::Color& color)
    {
      glColor4f(color.fv[0], color.fv[1], color.fv[2], color.fv[3]); GLDEBUG; // OpenGL ES compatible
    }
    
    void Canvas::drawPoint(const lost::math::Vec2& point)
    {
      static SharedState state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true));
      context->makeCurrent();
      context->pushState(state);
      
      float p[2];
      p[0] = point.x;
      p[1] = point.y;
      
      glVertexPointer(2, GL_FLOAT, 0, p); GLDEBUG;
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
    
    void Canvas::drawRectFilled(const lost::math::Rect& rect)
    {
      static SharedState state = State::create(Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true));
      context->makeCurrent();
      context->pushState(state);
      
      // points
      lost::math::Vec2 bl(rect.x, rect.y);
      lost::math::Vec2 br(rect.maxX(), rect.y);
      lost::math::Vec2 tr(rect.maxX(), rect.maxY());
      lost::math::Vec2 tl(rect.x, rect.maxY());
      
      float verts[] = {bl.x, bl.y, br.x, br.y, tr.x, tr.y, tl.x, tl.y};
      // indices for 2 triangles, first upper left, then lower right
      GLubyte idx[] = {0,2,3,0,1,2};
      
      glVertexPointer(2,GL_FLOAT,0,verts); GLDEBUG;
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, idx); GLDEBUG;

      context->popState();
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
      static SharedState state = State::create(DepthTest::create(false),
                                               Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(true),
                                               VertexArray::create(true),
                                               TextureArray::create(true));
      context->makeCurrent();
      context->pushState(state);
      
      tex->bind();
      
      // points
      lost::math::Vec2 bl(rect.x, rect.y);
      lost::math::Vec2 br(rect.maxX(), rect.y);
      lost::math::Vec2 tr(rect.maxX(), rect.maxY());
      lost::math::Vec2 tl(rect.x, rect.maxY());
      
      // build arrays
      float verts[] = {bl.x, bl.y, br.x, br.y, tr.x, tr.y, tl.x, tl.y};
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
      static SharedState state = State::create(DepthTest::create(false),
                                               Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(true),
                                               VertexArray::create(true),
                                               TextureArray::create(true));
      context->makeCurrent();
      context->pushState(state);

      mesh->texture->bind();
      
      glVertexPointer(2, GL_FLOAT, 0, mesh->vertices.get()); GLDEBUG;
      glTexCoordPointer(2, GL_FLOAT, 0, mesh->texcoords.get()); GLDEBUG;
      glDrawElements(mode, mesh->faceCount, GL_UNSIGNED_BYTE, mesh->faces.get()); GLDEBUG;
      
      context->popState();
    }
    
    void Canvas::drawMesh3D(const boost::shared_ptr<Mesh3D>& mesh, GLenum mode)
    {
      static SharedState state = State::create(DepthTest::create(false),
                                               Blend::create(true),
                                               BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                               Texture2D::create(false),
                                               VertexArray::create(true),
                                               TextureArray::create(false));
      context->makeCurrent();
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

  }
}
