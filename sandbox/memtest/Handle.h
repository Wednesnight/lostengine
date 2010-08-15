#ifndef HANDLE_H
#define HANDLE_H

#include <stdint.h>
#include <iosfwd>
#include "Types.h"

struct HandlePool;

typedef enum
{
  HD_undefined = 0,
  HD_free,
  HD_delete,
  HD_deleteArray
} HandleDeleter;

struct Handle
{
  Handle();
  Handle(HandleId hid, HandlePool* inPool);
  Handle(const Handle& other);
  virtual ~Handle();
    
  void* getRawPointer() const;
  RefCountType getRefCount() const;
  
  Handle& operator=(const Handle& other);
  
  HandlePool*     pool;
  HandleId  handleId;
};

std::ostream& operator<<(std::ostream& os, const Handle& h);
bool operator<(const Handle& lhs, const Handle& rhs);

#endif
