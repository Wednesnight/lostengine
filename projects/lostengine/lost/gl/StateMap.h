#ifndef LOST_GL_STATEMAP_H
#define LOST_GL_STATEMAP_H

#include "lost/gl/StateParams.h"
#include <map>

namespace lost
{
namespace gl
{
struct StateMap
{
	StateMap();
	~StateMap();
	
	void set(boost::shared_ptr<StateParam> param);
	
	std::map<GLenum, boost::shared_ptr<StateParam> > id2param;
};
}
}

#endif