#ifndef GL_PARAM_H
#define GL_PARAM_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"

namespace lost
{
  namespace gl
  {
    
    struct Param;
    typedef boost::shared_ptr<Param> SharedParam;

    struct Param
    {
    public:
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
      ColorParam(const lost::common::Color& inColor)
      : Param(inWhich),
        color(inColor)
      {
      }
      
      static SharedParam create(const lost::common::Color& inColor)
      {
        return SharedParam(new ColorParam(inColor));
      }
      
      static SharedParam create()
      {
        lost::common::Color color;
        glGetFloatv(inWhich, (GLfloat*)color.fv); GLDEBUG_THROW;
        return SharedParam(new ColorParam(color));
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
      
      static SharedParam create()
      {
        bool flag;
        glGetBooleanv(inWhich, (GLboolean*)&flag); GLDEBUG_THROW;
        return SharedParam(new ClientStateParam(flag));
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
      
      static SharedParam create()
      {
        bool flag;
        glGetBooleanv(inWhich, (GLboolean*)&flag); GLDEBUG_THROW;
        return SharedParam(new ServerStateParam(flag));
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
      
      static SharedParam create()
      {
        GLfloat src, dest = 0;
        glGetFloatv(GL_BLEND_SRC, &src); GLDEBUG_THROW;
        glGetFloatv(GL_BLEND_DST, &dest); GLDEBUG_THROW;
        return SharedParam(new BlendFunc(static_cast<GLenum>(src), static_cast<GLenum>(dest)));
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

  }
}

#endif
