#ifndef LOST_RG_CLEARCOLOR_H
#define LOST_RG_CLEARCOLOR_H

#include "lost/rg/Node.h"
#include "lost/common/Color.h"

namespace lost
{
namespace rg
{

struct ClearColor;
typedef lost::shared_ptr<ClearColor> ClearColorPtr;

struct ClearColor : public Node
{
  common::Color col;
  ClearColor(const common::Color& inCol);
  static ClearColorPtr create(const common::Color& inCol);
  void process(gl::ContextPtr ctx);  
};

}
}

#endif