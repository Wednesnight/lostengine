/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef LOST_RG_SCISSORRECT_H
#define LOST_RG_SCISSORRECT_H

#include "lost/rg/Node.h"
#include "lost/math/Rect.h"
#include "lost/rg/forward.h"

namespace lost
{
  namespace rg
  {
    
    /** sets scissoring rect.
     *  does not process children.
     */
    struct ScissorRect : public Node
    {
      lost::math::Rect rect;
      ScissorRect(const math::Rect& inRect);
      static ScissorRectPtr create(const math::Rect& inRect);
      
      void process(gl::ContextPtr& ctx);
    };
  }
}

#endif
