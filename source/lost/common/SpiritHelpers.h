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

#ifndef LOST_COMMON_SPIRITHELPERS_H
#define LOST_COMMON_SPIRITHELPERS_H

#include <string>
#include <iostream>
#include "lost/model/Vertex.h"

namespace lost
{
  namespace common
  {

    namespace spirithelpers
    {

      template<typename CONTAINER, typename ENTITY>
      struct push_back_a
      {
        CONTAINER& cont;
        ENTITY& ent;

        push_back_a(CONTAINER& c, ENTITY& e) : cont(c), ent(e) {}
        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          cont.push_back(ent);
        }
      };

      template<typename CONTAINER, typename ENTITY>
      push_back_a<CONTAINER, ENTITY>
      push_back(CONTAINER& c, ENTITY& e)
      {
        return push_back_a<CONTAINER, ENTITY>(c, e);
      }


      struct print
      {
        string msg;
        print(const string& m) : msg(m) {}

        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          std::cout << msg << string(first, last) << std::endl;
        }
      };

      struct increment
      {
        unsigned int& number;
        int amount;

        increment(unsigned int& inNumber, const int inAmount = 1)
        : number(inNumber), amount(inAmount)
        {
        }

        template<typename IteratorT>
        void operator()(IteratorT first, IteratorT last) const
        {
          number += amount;
        }
      };

    }
  }
}

#endif
