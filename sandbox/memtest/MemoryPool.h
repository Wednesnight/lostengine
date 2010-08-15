#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <stdlib.h>

struct MemoryPool
{
  void* context;

  MemoryPool();
  ~MemoryPool();
  
  void* malloc(size_t size);
  void free(void* ptr);
  void* realloc(void* ptr, size_t size);  
};

#endif