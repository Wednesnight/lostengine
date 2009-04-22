#include "lost/gl/XContext.h"
#include "lost/gl/gl.h"


using namespace lost::mesh;

bool getBoolParam(GLenum pname)
{
  GLboolean result;
  glGetBooleanv(pname, &result);GLDEBUG;
  return result ? true : false;
}

int getIntParam(GLenum pname)
{
  int result;
  glGetIntegerv(pname, &result);GLDEBUG;
  return result;
}

template<typename T>
T getParam(GLenum pname);

template<> bool getParam(GLenum pname) { return getBoolParam(pname);GLDEBUG; }
template<> int getParam(GLenum pname) { return getIntParam(pname);GLDEBUG; }


#define CLIENTSTATE(member, newstate, pname)  \
  if(member != newstate) \
  { \
    if(newstate) \
    { \
      glEnableClientState(pname);GLDEBUG; \
    } \
    else \
    { \
      glDisableClientState(pname);GLDEBUG; \
    } \
    member = newstate; \
  }

#define SERVERSTATE(member, newstate, pname)  \
  if(member != newstate) \
  { \
    if(newstate) \
    { \
      glEnable(pname);GLDEBUG; \
    } \
    else \
    { \
      glDisable(pname);GLDEBUG; \
    } \
    member = newstate; \
  }


namespace lost
{
namespace gl
{

XContext::XContext(ContextPtr inCtx)
: ctx(inCtx)
{
  vertexArrayEnabled = getParam<bool>(GL_VERTEX_ARRAY);
  normalArrayEnabled = getParam<bool>(GL_NORMAL_ARRAY);
  texCoordArrayEnabled = getParam<bool>(GL_TEXTURE_COORD_ARRAY);
  indexArrayEnabled = getParam<bool>(GL_INDEX_ARRAY);
  depthTestEnabled = getParam<bool>(GL_DEPTH_TEST);  
  blendEnabled = getParam<bool>(GL_BLEND);
  currentBlendFuncSource = getParam<int>(GL_BLEND_SRC);
  currentBlendFuncDestination = getParam<int>(GL_BLEND_DST);
  scissorEnabled = getParam<bool>(GL_SCISSOR_TEST);
  texture2DEnabled = getParam<bool>(GL_TEXTURE_2D);
  currentActiveTexture = getParam<int>(GL_ACTIVE_TEXTURE);
  currentModelTransform.initIdentity();
}

XContext::~XContext()
{
}

// FIXME: replace macros by dedficated functions that operate on member reference
void XContext::vertexArray(bool enable) { CLIENTSTATE(vertexArrayEnabled, enable, GL_VERTEX_ARRAY); }
void XContext::normalArray(bool enable) { CLIENTSTATE(normalArrayEnabled, enable, GL_NORMAL_ARRAY); }
void XContext::colorArray(bool enable) { CLIENTSTATE(colorArrayEnabled, enable, GL_COLOR_ARRAY); }
void XContext::texCoordArray(bool enable){ CLIENTSTATE(texCoordArrayEnabled, enable, GL_TEXTURE_COORD_ARRAY); }
void XContext::indexArray(bool enable){ CLIENTSTATE(indexArrayEnabled, enable, GL_INDEX_ARRAY); }
void XContext::depthTest(bool enable) { SERVERSTATE(depthTestEnabled, enable, GL_DEPTH_TEST); }
void XContext::blend(bool enable) {SERVERSTATE(blendEnabled, enable, GL_BLEND);}
void XContext::blendFunc(GLenum src, GLenum dest)
{
  if((currentBlendFuncSource != src)
     ||
     (currentBlendFuncDestination != dest)
    )
  {
    glBlendFunc(src, dest);
    currentBlendFuncSource = src;
    currentBlendFuncDestination = dest;
  }
}

void XContext::scissor(bool enable) {SERVERSTATE(scissorEnabled, enable, GL_SCISSOR_TEST);}
void XContext::texture2D(bool enable) {SERVERSTATE(texture2DEnabled, enable, GL_TEXTURE_2D);}

void XContext::color(const common::Color& col)
{
  if(currentColor != col)
  {
    glColor4f(col.fv[0], col.fv[1], col.fv[2], col.fv[3]); GLDEBUG; // OpenGL ES compatible 
    currentColor = col;
  }
}
void XContext::clearColor(const common::Color& col)
{
  if(currentClearColor != col)
  {
    glClearColor(col.r(), col.g(), col.b(), col.a()); GLDEBUG;
    currentClearColor = col;
  }
}

void XContext::frameBuffer(FrameBufferPtr buffer) { ctx->bindFramebuffer(buffer->buffer); }
void XContext::defaultFrameBuffer() {ctx->bindDefaultFramebuffer(); }

// FIXME: can't cache camera by memorizing pointer since the object might have been updted since the last call. We need a better way for this!
// one possible solution: define a dirty flag for the cam that is set upon changes
// memorize current cam here
// only update if the new cam is either a new one or the same one, but with the dirty flag set. 
// the context would reset the dirty flag once the camera was applied.
void XContext::camera(camera::CameraPtr cam)
{
//  if(currentCam != cam)
//  {
    cam->apply();
    currentCam = cam;
//  }
} // FIXME: remove context reference from cam

void XContext::modelTransform(const math::Matrix& inTransform)
{
  if(currentModelTransform != inTransform)
  {
    glMatrixMode(GL_MODELVIEW);GLDEBUG;
    glLoadMatrixf(inTransform.m);GLDEBUG;
    currentModelTransform = inTransform;
  }
}

void XContext::clear(GLbitfield flags) { glClear(flags);GLDEBUG; }
void XContext::swapBuffers() { ctx->swapBuffers(); }
void XContext::makeCurrent() { ctx->makeCurrent(); }

void XContext::activeTexture(GLenum tex)
{
  if(currentActiveTexture != tex)
  {
    glActiveTexture(tex);GLDEBUG;
    currentActiveTexture = tex;
  }
}

void XContext::bindActiveTextures(const std::vector<TexturePtr>& textures)
{
  if(textures.size() > 0)
  {
    uint32_t num = textures.size();
    for(uint32_t i=0; i<num; ++i)
    {
      activeTexture(GL_TEXTURE0+i); // the standard guarantees GL_TEXTUREi = GL_TEXTURE0+i
      textures[0]->bind();
    }
    activeTexture(GL_TEXTURE0); // reset 
  }
}

void XContext::shader(ShaderProgramPtr prog)
{
  if(currentShader && !prog)
    currentShader->disable();
  currentShader = prog;
  if(currentShader)
    currentShader->enable();
}


void XContext::material(MaterialPtr mat)
{
  color(mat->color);
  if(mat->textures.size()>0)
  {
    bindActiveTextures(mat->textures);
    texture2D(true);
  }
  else
  {
    texture2D(false);
  }
  shader(mat->shader);
}


void XContext::draw(MeshPtr mesh)
{
  gl::Buffer* ib = mesh->getIndexBuffer();
  gl::Buffer* vb = mesh->getVertexBuffer();
  gl::Buffer* nb = mesh->getNormalBuffer();
  gl::Buffer* cb = mesh->getColorBuffer();
  gl::Buffer* tcb = mesh->getTexCoordBuffer();
  
  vertexArray(true);
  vb->bindVertexPointer();
  indexArray(true);
  ib->bind();
  if(nb)
  {
    normalArray(true);
    nb->bindNormalPointer();
  }
  else
  {
    normalArray(false);
  }
  
  if(cb)
  {
    colorArray(true);
    cb->bindColorPointer();
  }
  else
  {
    colorArray(false);
  }
  
  if(tcb)
  {
    texCoordArray(true);
    tcb->bindTexCoordPointer();
  }
  else
  {
    texCoordArray(false);
  }
  
  if(mesh->material)
    material(mesh->material);

  modelTransform(mesh->modelTransform);
  ib->drawElements(mesh->drawMode);
}

}
}
