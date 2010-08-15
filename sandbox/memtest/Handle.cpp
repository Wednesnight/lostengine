#include "Handle.h"
#include "HandlePool.h"
#include <memory>
#include <iostream>

using namespace std;

Handle::Handle()
{
  pool = NULL;
  handleId = undefinedHandleId;
}

Handle::Handle(HandleId hid, HandlePool* inPool)
{
  pool = inPool;
  handleId = hid;
}

Handle::Handle(const Handle& other)
{
  pool = other.pool;
  handleId = other.handleId;
  if(pool)
  {
    pool->inc(handleId);
  }
}

void* Handle::getRawPointer() const
{
  void* result = NULL;
  
  if(pool)
  {
    result = pool->getRawPointer(handleId);
  }
  cout << "returning raw pointer " << result << endl;
  return result;
}

RefCountType Handle::getRefCount() const
{
  RefCountType result = 0;
  
  if(pool)
  {
    result = pool->getRefCount(handleId);
  }
  
  return result;
}

Handle::~Handle()
{
  cout << "// HANDLE DESTRUCTOR " << endl;
  if(pool)
  {
    pool->dec(handleId);
  }
  else {
    cout << "destroying handle without pool" << endl;
  }
}

std::ostream& operator<<(std::ostream& os, const Handle& h)
{
  os << "handle id:"<<h.handleId<<" mem:"<<h.getRawPointer()<< " refcount: " << h.getRefCount();
  return os;
}

bool operator<(const Handle& lhs, const Handle& rhs)
{
  bool result = true;
  
  if(rhs.handleId >= lhs.handleId)
  {
    result = false;
  }
  
  return result;
}

Handle& Handle::operator=(const Handle& other)
{
  if(other.pool)
  {
    other.pool->inc(other.handleId);
  }
  if(pool)
  {
    pool->dec(handleId);
  }
  
  pool = other.pool;
  handleId = other.handleId;
  return *this;
}
