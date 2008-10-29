#include "lost/font/Engine.h"
#include <boost/shared_ptr.hpp>

namespace lost
{
namespace font
{
  
Engine::Engine()
{
}
  
Engine::~Engine()
{
}
 
void Engine::addFont(const std::string& inAlias,
                     boost::shared_ptr<freetype::Face> inFtFace)
{
}
  
boost::shared_ptr<Model> Engine::render(const std::string& inFontId,
                                        const std::string& inText)
{
    boost::shared_ptr<Model> result;    
    
    return result;
}

}
}
