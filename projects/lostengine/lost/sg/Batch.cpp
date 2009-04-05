#include "lost/sg/Batch.h"

namespace lost
{
namespace sg
{

Batch::Batch()
{
  
}

Batch::~Batch()
{
  
}

void Batch::add(PrimitivePtr primitive)
{
  primitives.push_back(primitive);
}

void render(gl::ContextPtr ctx)
{
  
}


}
}