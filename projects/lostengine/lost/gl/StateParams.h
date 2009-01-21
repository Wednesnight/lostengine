#ifndef LOST_GL_STATEPARAMS_H
#define LOST_GL_STATEPARAMS_H

#include "lost/gl/gl.h"
#include "lost/gl/Utils.h"
#include "lost/common/Color.h"
#include <boost/shared_ptr.hpp> 

namespace lost
{
namespace gl
{

struct StateParam;
typedef boost::shared_ptr<StateParam> StateParamPtr;

struct StateParam
{
	StateParam(GLenum paramid) : paramId(paramid) {}
	GLenum paramId;	// the GLenum value used to identify this set of params when getting from OpenGL

	virtual StateParamPtr clone() = 0;

	virtual void get() = 0; // reads current state param(s) from gl context
	virtual void set() = 0; // sets the state params 
	virtual bool equals(boost::shared_ptr<StateParam> sp) = 0; // compares this param with another one, returning true if equal
};

struct ClearColor;
typedef boost::shared_ptr<ClearColor> ClearColorPtr;

struct ClearColor : public StateParam
{
	common::Color color;
	
	ClearColor() : StateParam(GL_COLOR_CLEAR_VALUE), color(0,0,0,0) {}
	ClearColor(const common::Color& col) : StateParam(GL_COLOR_CLEAR_VALUE), color(col) {}
	
	static ClearColorPtr create() { return boost::shared_ptr<ClearColor>(new ClearColor);};
	static ClearColorPtr create(const common::Color& col) { return boost::shared_ptr<ClearColor>(new ClearColor(col));};
	StateParamPtr clone() { ClearColorPtr result = create(); result->color = color; return result;}
	
	void get() { glGetFloatv(paramId, color.fv);GLDEBUG; }
	void set() { glClearColor(color.r(), color.g(), color.b(), color.a()); }
	bool equals(boost::shared_ptr<StateParam> sp)
	{
		bool result = false;
		if(sp && (sp->paramId == paramId))
		{
			ClearColor* otherp = static_cast<ClearColor*>(sp.get());
			if(color == otherp->color)
				result = true;
		}
		return result;
	}
};

}
}

#endif