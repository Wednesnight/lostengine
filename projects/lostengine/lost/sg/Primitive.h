#ifndef LOST_SG_PRIMITIVE_H
#define LOST_SG_PRIMITIVE_H

#include "lost/gl/Context.h"

namespace lost
{
namespace sg
{

  struct Primitive;
  typedef lost::shared_ptr<Primitive> PrimitivePtr;

/** Base class for all objects that can be rendered in batches and contained in
 *  batches.
 *  
 */
struct Primitive
{
private: 
  bool dirty;
  
public:
  /** primitives store their type as an int to avoid probably costly
   * dynamic casts.
   */
  enum PrimitiveType {
    PT_UNDEFINED = 0,
    PT_MESH,
    PT_BATCH,
    PT_NUM
  };
  
  PrimitiveType      type; 

  Primitive()
  {
    type = PT_UNDEFINED;
    dirty = false;
  }
  
  virtual ~Primitive()
  {    
  }

  void needsUpdate(bool v)
  {
    dirty = v;
  }

  // return true here if any properties were modified and an update/recalculatoin of
  // the Primitive is required
  bool          needsUpdate()
  {
    return dirty; 
  } 
  
  // overwrite this to provide update functionality that recalculates 
  virtual void  update()
  {
    needsUpdate(false);
  }    
};

}
}

#endif 
