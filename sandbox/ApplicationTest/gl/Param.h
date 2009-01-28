#ifndef GL_PARAM_H
#define GL_PARAM_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"

struct Param;
typedef boost::shared_ptr<Param> SharedParam;

struct Param
{
  GLenum which;

  Param(const GLenum inWhich) : which(inWhich) {}
  virtual void set() {}
  
  virtual bool operator ==(Param& other)
  {
    return (this->which == other.which);
  }
};

struct InvalidParam : public Param
{
  InvalidParam() : Param(GL_INVALID_ENUM) {}
};

template <GLenum inWhich>
struct ColorParam : public Param
{
protected:
  lost::common::Color color;
public:
  ColorParam(const lost::common::Color& inColor = lost::common::Color(0,0,0,0))
  : Param(inWhich),
  color(inColor)
  {
  }
  
  static SharedParam create(const lost::common::Color& inColor = lost::common::Color(0,0,0,0))
  {
    return SharedParam(new ColorParam(inColor));
  }
  
  virtual void set()
  {
    glClearColor(color.r(), color.g(), color.b(), color.a()); GLDEBUG;
  }
  
  virtual bool operator ==(ColorParam& other)
  {
    return Param::operator ==(other) && (this->color == other.color);
  }
};

struct StateParam : public Param
{
protected:
  bool enable;
public:
  StateParam(const GLenum inWhich, const bool inEnable)
  : Param(inWhich),
  enable(inEnable)
  {
  }
  
  virtual bool operator ==(StateParam& other)
  {
    return Param::operator ==(other) && (this->enable == other.enable);
  }
};

template <GLenum inWhich>
struct ClientStateParam : public StateParam
{
  ClientStateParam(const bool inEnable) : StateParam(inWhich, inEnable) {}

  static SharedParam create(const bool inEnable)
  {
    return SharedParam(new ClientStateParam(inEnable));
  }
    
  virtual void set()
  {
    if (enable) glEnableClientState(which);
    else glDisableClientState(which);
    GLDEBUG;
  }
};

template <GLenum inWhich>
struct ServerStateParam : public StateParam
{
  ServerStateParam(const bool inEnable) : StateParam(inWhich, inEnable) {}

  static SharedParam create(const bool inEnable)
  {
    return SharedParam(new ServerStateParam(inEnable));
  }
  
  virtual void set()
  {
    if (enable) glEnable(which);
    else glDisable(which);
    GLDEBUG;
  }
};

struct BlendFunc : public Param
{
private:
  GLenum source;
  GLenum destination;
public:
  BlendFunc(const GLenum inSource, const GLenum inDestination)
  : Param(GL_BLEND_SRC),
  source(inSource),
  destination(inDestination)
  {
  }
  
  static SharedParam create(const GLenum inSource, const GLenum inDestination)
  {
    return SharedParam(new BlendFunc(inSource, inDestination));
  }
  
  virtual void set()
  {
    glBlendFunc(source, destination); GLDEBUG;
  }
  
  virtual bool operator ==(BlendFunc& other)
  {
    return Param::operator ==(other) && (this->source == other.source && this->destination == other.destination);
  }
};

typedef ServerStateParam<GL_ALPHA_TEST> AlphaTest;
typedef ServerStateParam<GL_DEPTH_TEST> DepthTest;
typedef ServerStateParam<GL_TEXTURE_2D> Texture2D;
typedef ServerStateParam<GL_BLEND>      Blend;

typedef ClientStateParam<GL_NORMAL_ARRAY>        NormalArray;
typedef ClientStateParam<GL_VERTEX_ARRAY>        VertexArray;
typedef ClientStateParam<GL_TEXTURE_COORD_ARRAY> TextureArray;

typedef ColorParam<GL_CLEAR> ClearColor;

#endif
