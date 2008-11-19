#include "lost/gl/gl.h"
#include "lost/gl/Context.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec2.h"
#include "lost/gl/Utils.h"
#include "lost/common/DisplayAttributes.h"
#include "lost/math/AABB.h"
#include "lost/common/Color.h"
#include "lost/math/Rect.h"
#include "lost/gl/Texture.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/lgl/lglu.h"

using namespace lost::bitmap;

namespace lost
{
namespace gl
{

std::string Context::getVersion()
{
  return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

void Context::setState(const boost::shared_ptr<State>& oldState, const boost::shared_ptr<State>& newState)
{
#define SET_STATE_BOOL(which, target, source, attribute)\
    if (target->attribute != source->attribute)\
    {\
      target->attribute = source->attribute;\
      (target->attribute) ? glEnable(which) : glDisable(which);GLDEBUG;\
    }
    
  SET_STATE_BOOL(GL_ALPHA_TEST, oldState, newState, alphaTest);
  SET_STATE_BOOL(GL_DEPTH_TEST, oldState, newState, depthTest);
  SET_STATE_BOOL(GL_TEXTURE_2D, oldState, newState, texture2D);
  SET_STATE_BOOL(GL_BLEND, oldState, newState, blend);

  if((oldState->blendSrc != newState->blendSrc) || (oldState->blendDest != newState->blendDest))
  {
    oldState->blendSrc = newState->blendSrc;
    oldState->blendDest = newState->blendDest;
    glBlendFunc(oldState->blendSrc, oldState->blendDest);GLDEBUG;
  }

  if (oldState->clearColor != newState->clearColor)
  {
    oldState->clearColor = newState->clearColor;
    glClearColor(oldState->clearColor.r(), oldState->clearColor.g(), oldState->clearColor.b(), oldState->clearColor.a());GLDEBUG;
  }

#define SET_CLIENT_STATE_BOOL(which, target, source, attribute)\
    if (target->attribute != source->attribute)\
    {\
      target->attribute = source->attribute;\
      (target->attribute) ? glEnableClientState(which) : glDisableClientState(which);GLDEBUG;\
    }

  SET_CLIENT_STATE_BOOL(GL_NORMAL_ARRAY, oldState, newState, normalArray);
  SET_CLIENT_STATE_BOOL(GL_VERTEX_ARRAY, oldState, newState, vertexArray);
  SET_CLIENT_STATE_BOOL(GL_TEXTURE_COORD_ARRAY, oldState, newState, textureCoordArray);
}


Context::Context(boost::shared_ptr<common::DisplayAttributes> inDisplayAttributes)
:displayAttributes(inDisplayAttributes)
{
  DOUT("lost::gl::Context::Context()");

  // initialize state
  stateStack.push_back(newState());
}

Context::~Context()
{
}

boost::shared_ptr<State> Context::newState()
{
  boost::shared_ptr<State> result(new State());
  
  glGetBooleanv(GL_ALPHA_TEST, (GLboolean*)&(result->alphaTest));GLDEBUG_THROW;
  glGetBooleanv(GL_DEPTH_TEST, (GLboolean*)&(result->depthTest));GLDEBUG_THROW;
  glGetBooleanv(GL_TEXTURE_2D, (GLboolean*)&(result->texture2D));GLDEBUG_THROW;

  glGetFloatv(GL_COLOR_CLEAR_VALUE, (GLfloat*)result->clearColor.fv);GLDEBUG_THROW;
  
  glGetBooleanv(GL_NORMAL_ARRAY, (GLboolean*)&(result->normalArray));GLDEBUG_THROW;
  glGetBooleanv(GL_VERTEX_ARRAY, (GLboolean*)&(result->vertexArray));GLDEBUG_THROW;

  GLfloat f = 0;
  glGetFloatv(GL_BLEND, &f);GLDEBUG_THROW;
  if(f != 0.0f)
    result->blend = true;
  else
    result->blend = false;
    
  f = 0;  
  glGetFloatv(GL_BLEND_SRC, &f);GLDEBUG_THROW;
  result->blendSrc = static_cast<GLenum>(f);

  f = 0;
  glGetFloatv(GL_BLEND_DST, &f);GLDEBUG_THROW;
  result->blendDest = static_cast<GLenum>(f);
    
  return result;
}
      
boost::shared_ptr<State> Context::copyState()
{
  return boost::shared_ptr<State>(new State(*(stateStack.back().get())));
}
      
void Context::pushState(const boost::shared_ptr<State>& inState)
{
  if (inState) setState(stateStack.back(), inState);
}

void Context::popState()
{
  if (stateStack.size() > 1)
  {
      std::list<boost::shared_ptr<State> >::iterator pos = stateStack.end();
      pos--;
      boost::shared_ptr<State> last = *pos;
      pos--;
      boost::shared_ptr<State> prev = *pos;
      
    setState(last, prev);
    stateStack.pop_back();
  }
}

void Context::clear(GLbitfield flags)
{
  glClear(flags);GLDEBUG;
}

void Context::set2DProjection(const lost::math::Vec2& offset, const lost::math::Vec2& dimension)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  lgluOrtho2D(offset.x, dimension.width-1, offset.y, dimension.height-1);
}
      
void Context::set3DProjection(const lost::math::Vec3& eye,
                              const lost::math::Vec3& at,
                              const lost::math::Vec3& up,
                              const float& fovy,
                              const lost::math::Vec2& depth)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double screenAspectRatio = (double)displayAttributes->width/(double)displayAttributes->height;
  lgluPerspective(fovy, screenAspectRatio, depth.min, depth.max);
  lgluLookAt(eye.x,  eye.y,  eye.z,
            at.x,   at.y,   at.z,
            up.x,   up.y,   up.z);
                                   
}

boost::shared_ptr<State>& Context::defaultVertexState()
{
  static boost::shared_ptr<State> newState;
  if (!newState)
  {
    newState = copyState();
    newState->vertexArray = true;
  }
  return newState;
}

void Context::drawLine(const lost::math::Vec2& start, const lost::math::Vec2& end)
{
  pushState(defaultVertexState());
  
  float p[4];
  p[0] = start.x;
  p[1] = start.y;
  p[2] = end.x;
  p[3] = end.y;
  
  glVertexPointer(2, GL_FLOAT, 0, p);
  glDrawArrays(GL_LINES, 0,2);    

  popState();
}

void Context::drawLine(const lost::math::Vec3& start, const lost::math::Vec3& end)
{
  pushState(defaultVertexState());

  float p[6];
  p[0] = start.x;
  p[1] = start.y;
  p[2] = start.z;
  p[3] = end.x;
  p[4] = end.y;
  p[5] = end.z;
  
  glVertexPointer(3, GL_FLOAT, 0, p);
  glDrawArrays(GL_LINES, 0,2);    

  popState();
}

void Context::setColor(const lost::common::Color& col)
{
  glColor4f(col.fv[0], col.fv[1], col.fv[2], col.fv[3]); // OpenGL ES compatible
}

void Context::drawPoint(const lost::math::Vec2& point)
{
  float p[2];
  p[0] = point.x;
  p[1] = point.y;
  
  glVertexPointer(2, GL_FLOAT, 0, p);
  glDrawArrays(GL_POINTS, 0, 1);    
}

void Context::drawRectOutline(const lost::math::Rect& rect)
{
  drawLine(rect.bottomLeft(), rect.bottomRight());
  drawLine(rect.bottomRight(), rect.topRight());
  drawLine(rect.topRight(), rect.topLeft());
  drawLine(rect.topLeft(), rect.bottomLeft());
}

void Context::drawRectFilled(const lost::math::Rect& rect)
{
  // points
  lost::math::Vec2 bl(rect.x, rect.y);
  lost::math::Vec2 br(rect.x+rect.width-1, rect.y);
  lost::math::Vec2 tr(rect.x+rect.width-1, rect.y+rect.height-1);
  lost::math::Vec2 tl(rect.x, rect.y+rect.height-1);
  
  float verts[] = {bl.x, bl.y, br.x, br.y, tr.x, tr.y, tl.x, tl.y};
  // indices for 2 triangles, first upper left, then lower right
  GLubyte idx[] = {0,2,3,0,1,2};

  glVertexPointer(2,GL_FLOAT,0,verts);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, idx);    
}

// don't forget glEnable(GL_TEXTURE_2D);
// rect is drawn counterclockwise
// requires vertex arrays, texture arrays, index arrays
// FIXME: needs serious optimisation/rethinking, but at least it works
void Context::drawRectTextured(const lost::math::Rect& rect,
                             boost::shared_ptr<const lost::gl::Texture> tex,
                             bool flip)
{
  tex->bind();

  // points
  lost::math::Vec2 bl(rect.x, rect.y);
  lost::math::Vec2 br(rect.x+rect.width-1, rect.y);
  lost::math::Vec2 tr(rect.x+rect.width-1, rect.y+rect.height-1);
  lost::math::Vec2 tl(rect.x, rect.y+rect.height-1);
  
  // texcoords
  lost::math::Vec2 tbl(0,0);
  lost::math::Vec2 tbr(1,0);
  lost::math::Vec2 ttr(1,1);
  lost::math::Vec2 ttl(0,1);

  // build arrays
  float verts[] = {bl.x, bl.y, br.x, br.y, tr.x, tr.y, tl.x, tl.y};
  float texcoordsNormal[] = {tbl.x, tbl.y, tbr.x, tbr.y, ttr.x, ttr.y, ttl.x, ttl.y};
  float texcoordsFlipped[] = {ttl.x, ttl.y, ttr.x, ttr.y, tbr.x, tbr.y, tbl.x, tbl.y};
  // indices for 2 triangles, first upper left, then lower right
  GLubyte idx[] = {0,2,3,0,1,2};
  
  glVertexPointer(2,GL_FLOAT,0,verts);
  if(flip)
    glTexCoordPointer(2,GL_FLOAT,0,texcoordsFlipped);
  else
    glTexCoordPointer(2,GL_FLOAT,0,texcoordsNormal);
    
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, idx);    
}

void Context::drawAABB(const lost::math::AABB& box)
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

void Context::drawAxes(const lost::math::Vec3& length)
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
void Context::writeScreenshot(const std::string& fullPathName,
                              bool withAlphaChannel)
{
  GLenum format = withAlphaChannel ? GL_RGBA : GL_RGB;
  Bitmap::Components bitmapFormat = withAlphaChannel ? Bitmap::COMPONENTS_RGBA : Bitmap::COMPONENTS_RGB;
  Bitmap bmp(displayAttributes->width, displayAttributes->height, bitmapFormat);
        
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
        
}
}

