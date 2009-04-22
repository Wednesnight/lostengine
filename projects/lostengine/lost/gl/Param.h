#ifndef GL_PARAM_H
#define GL_PARAM_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"
#include "lost/math/Rect.h"
#include "lost/platform/shared_ptr.h"

namespace lost
{
  namespace gl
  {
    
    struct Param;
    typedef lost::shared_ptr<Param> ParamPtr;

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
      
      static ParamPtr create(const lost::common::Color& inColor)
      {
        return ParamPtr(new ColorParam(inColor));
      }
      
      static ParamPtr create()
      {
        lost::common::Color color;
        glGetFloatv(inWhich, (GLfloat*)color.fv); GLDEBUG_THROW;
        return ParamPtr(new ColorParam(color));
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

      static ParamPtr create(const bool inEnable)
      {
        return ParamPtr(new ClientStateParam(inEnable));
      }
      
      static ParamPtr create()
      {
        bool flag;
        glGetBooleanv(inWhich, (GLboolean*)&flag); GLDEBUG_THROW;
        return ParamPtr(new ClientStateParam(flag));
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

      static ParamPtr create(const bool inEnable)
      {
        return ParamPtr(new ServerStateParam(inEnable));
      }
      
      static ParamPtr create()
      {
        bool flag;
        glGetBooleanv(inWhich, (GLboolean*)&flag); GLDEBUG_THROW;
        return ParamPtr(new ServerStateParam(flag));
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
      
      static ParamPtr create(const GLenum inSource, const GLenum inDestination)
      {
        return ParamPtr(new BlendFunc(inSource, inDestination));
      }
      
      static ParamPtr create()
      {
        GLfloat src, dest = 0;
        glGetFloatv(GL_BLEND_SRC, &src); GLDEBUG_THROW;
        glGetFloatv(GL_BLEND_DST, &dest); GLDEBUG_THROW;
        return ParamPtr(new BlendFunc(static_cast<GLenum>(src), static_cast<GLenum>(dest)));
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

    struct ScissorBox : public Param
    {
    private:
      math::Rect box;
    public:
      ScissorBox(const math::Rect& inBox)
      : Param(GL_SCISSOR_BOX),
        box(inBox)
      {
      }
      
      static ParamPtr create(const math::Rect& inBox)
      {
        return ParamPtr(new ScissorBox(inBox));
      }
      
      static ParamPtr create()
      {
        GLint box[4];
        glGetIntegerv(GL_SCISSOR_BOX, box); GLDEBUG_THROW;
        return ParamPtr(new ScissorBox(math::Rect(box[0], box[1], box[2], box[3])));
      }
      
      virtual void set()
      {
        glScissor(box.x, box.y, box.width, box.height); GLDEBUG;
      }
      
      virtual bool operator ==(ScissorBox& other)
      {
        return Param::operator ==(other) && (this->box == other.box);
      }
    };
    
  }
}

#endif
