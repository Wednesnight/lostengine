#include "HandlePool.h"
#include <iostream>
#include "MemoryPool.h"

using namespace std;

HandlePool::HandlePool(MemoryPool* mempool)
{
  cout << __PRETTY_FUNCTION__ << endl;
  nextFree = 0;
  memoryPool = mempool;
}

HandlePool::~HandlePool()
{
  cout << __PRETTY_FUNCTION__ << endl;
  cout << "objects still alive: " << handleId2mem.size() << endl;
}

bool HandlePool::exists(HandleId hid)
{
  bool result = false;
  
  std::map<HandleId, RefCountType>::iterator pos = handleId2refCount.find(hid);
  std::map<HandleId, RefCountType>::iterator end = handleId2refCount.end();
  
  if(pos != end)
  {
    result = true;
  }
  
  return result;
}

//naive implementation, doesn't check for wrap around or reusable slots
HandleId HandlePool::createHandle(size_t size, HandleDeleter deleter)
{
  cout << "create" << endl;
  HandleId res = nextFree;
  nextFree++;
  handleId2refCount[res] = 1;
  handleId2mem[res] = memoryPool->malloc(size);
  handleId2deleter[res] = deleter;
  
  return res;
}

void HandlePool::destroyHandle(HandleId hid)
{
  if(exists(hid))
  {
    cout << "destroy" << endl;
    handleId2refCount.erase(hid);
    void* mem = handleId2mem[hid];
    // if mem contained an object or an array, the appropriate destructor must have been called in advance
    memoryPool->free(mem);  
    handleId2mem.erase(hid);
    
    cout << "refsize " << handleId2refCount.size() << endl;
    cout << "memsize " << handleId2mem.size() << endl;
  }
  else
  {
    cout << "tried to destroy nonexistent hid" << hid << endl;
  }
}

void HandlePool::inc(HandleId hid)
{
  if(exists(hid))
  {
    handleId2refCount[hid]++;
    cout << "inc "<<hid << " : " << handleId2refCount[hid] << endl;
  }
  else
  {
    cout << "tried to inc nonexistent hid "<< hid << endl;
  }
}

void HandlePool::dec(HandleId hid)
{
  if(exists(hid))
  {
    cout << "--" << endl;
    cout << "pre dec size "<< handleId2refCount.size() << " : " << handleId2refCount[hid] << endl;
    handleId2refCount[hid]--;
    RefCountType count = handleId2refCount[hid];
    if(count == 0)  
    {
      destroyHandle(hid);
      cout << "dec " << hid << " : 0" << endl;  
    }
    else
    {
      cout << "dec " << hid << " : " << count << endl;
    }
  }
  else
  {
    cout << "tried to dec nonexistant hid " << hid << endl;
  }
}

void* HandlePool::getRawPointer(HandleId hid)
{
  void* result = NULL;
  if(exists(hid))
  {
    result = handleId2mem[hid];
  }
  return result;
}

RefCountType HandlePool::getRefCount(HandleId hid)
{
  RefCountType result = -1;
  if(exists(hid))
  {
    result = handleId2refCount[hid];
  }
  return result;
}

