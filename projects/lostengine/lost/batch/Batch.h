#ifndef LOST_BATCH_BATCH
#define LOST_BATCH_BATCH

#include "lost/batch/Primitive.h"
#include <list>

namespace lost
{
namespace batch
{

struct Batch;
typedef boost::shared_ptr<Batch> BatchPtr;
  
struct Batch : public Primitive
{
  Batch();
  virtual ~Batch();

  void add(PrimitivePtr primitive);

  std::list<PrimitivePtr> primitives;
};

}
}

#endif