#include "MemoryPool.h"
#include "nedmalloc.h"

MemoryPool::MemoryPool()
{
  context = nedcreatepool(1024, 2);
}

MemoryPool::~MemoryPool()
{
  neddestroypool((nedpool*)context);
}

void* MemoryPool::malloc(size_t size)
{
  return nedpmalloc((nedpool*)context, size);
}

void MemoryPool::free(void* ptr)
{
  nedpfree((nedpool*)context, ptr);
}

void* MemoryPool::realloc(void* ptr, size_t size)
{
  return nedprealloc((nedpool*)context, ptr, size);
}
