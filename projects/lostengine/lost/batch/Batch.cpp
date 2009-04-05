#include "lost/batch/Batch.h"

namespace lost
{
namespace batch
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