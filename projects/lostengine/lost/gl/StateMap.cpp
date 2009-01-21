#include "lost/gl/StateMap.h"

#include "lost/common/Logger.h"

namespace lost
{
namespace gl
{

StateMap::StateMap()
{
}

StateMap::~StateMap()
{
}

void StateMap::set(boost::shared_ptr<StateParam> param)
{
	if(param)
	{
		id2param[param->paramId] = param;
	}
	else
	{
		WOUT("param is NULL");
	}
}

}
}