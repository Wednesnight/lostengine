#ifndef LOST_COMMON_SPIRITHELPERS_H
#define LOST_COMMON_SPIRITHELPERS_H

#include <string>
#include <iostream>

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
      std::string msg;
      print(const std::string& m) : msg(m) {}
      
      template<typename IteratorT>
      void operator()(IteratorT first, IteratorT last) const
      {
        std::cout << msg << std::string(first, last) << std::endl;
      }
    };
    
  }
  }
}

#endif