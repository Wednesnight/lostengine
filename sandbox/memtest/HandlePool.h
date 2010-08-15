#ifndef HANDLEPOOL_H
#define HANDLEPOOL_H

#include <map>
#include "Handle.h"
#include "Types.h"

struct HandlePool
{
  HandlePool();  
  ~HandlePool();
  
  HandleId createHandle(size_t size, HandleDeleter hdt);
  void destroyHandle(HandleId hid);
  void inc(HandleId hid);
  void dec(HandleId hid);

  void* getRawPointer(HandleId hid);
  RefCountType getRefCount(HandleId hid);
  bool exists(HandleId hid);
  
private:  
  std::map<HandleId, RefCountType> handleId2refCount;
  std::map<HandleId, HandleDeleter> handleId2deleter;
  std::map<HandleId, void*> handleId2mem;
  
  HandleId nextFree;
};

#endif 
