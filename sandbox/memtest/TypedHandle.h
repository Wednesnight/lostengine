#ifndef TYPEDHANDLE_H
#define TYPEDHANDLE_H

#include "Handle.h"
#include "HandlePool.h"
//#include <iostream>

template<typename T>
struct ObjectHandle : Handle
{
  typedef T Type;

  ObjectHandle()
  {
    pool = NULL;
    handleId = undefinedHandleId;
  }

  void destroyIfNeeded()
  {
//    std::cout << __PRETTY_FUNCTION__ << std::endl;
    if(getRefCount() == 1) // explicitly call destructor if we're the last handle thats decrementing
    {
//      std::cout << "handle is valid, refcount is 1, calling destructor" << std::endl;
      T* p = (T*)getRawPointer();
      if(p)
      {
//        std::cout << "calling destructor on " << p << std::endl;
        p->~T();
      }
      else {
//        std::cout << "couldn'T call destructor because pointer was NULL" << std::endl;
      }
    }
  }
    
  virtual ~ObjectHandle()
  {
//    std::cout << "/////////////////////// DESTRUCTOR" << std::endl;
    destroyIfNeeded();
  }
  
  T* operator->()
  {
    T* result = (T*)getRawPointer();
    return result;
  }
  
  ObjectHandle<T>& operator=(const ObjectHandle<T>& other)
  {
    if(other.pool)
    {
      other.pool->inc(other.handleId);
    }
    destroyIfNeeded();
    if(pool)
    {
      pool->dec(handleId);
    }
    pool = other.pool;
    handleId = other.handleId;
    return *this;
  }
};

template<typename T>
ObjectHandle<T> createObjectHandle(HandlePool& pool)
{
  ObjectHandle<T> result;
  result.handleId = pool.createHandle(sizeof(T), HD_delete);
  result.pool = &pool;
  return result;
}



#endif