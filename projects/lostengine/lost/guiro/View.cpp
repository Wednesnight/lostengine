#include <boost/shared_ptr.hpp>
#include "lost/guiro/View.h"

namespace lost
{
  namespace guiro
  {
    
    View::View()
    {
    }

    View::~View()
    {
    }

    void View::dispatchEvent(lost::event::EventPtr event)
    {
      for (ChildHierarchy::iterator idx = children.begin(); idx != children.end(); ++idx)
      {
        idx->second->dispatchEvent(event);
      }
      lost::event::EventDispatcher::dispatchEvent(event);
    }

    void View::appendChild(const boost::shared_ptr<View>& child)
    {
      validateChild(child);

      if (children.find(child->id) != children.end())
      {
        throw std::runtime_error("[View::appendChild] child '"+ child->id +"' already exists");
      }

      children[child->id] = child;
    }

    void View::removeChild(const boost::shared_ptr<View>& child)
    {
      validateChild(child);

      if (children.find(child->id) == children.end())
      {
        throw std::runtime_error("[View::appendChild] child '"+ child->id +"' does not exist");
      }

      children.erase(child->id);
    }

    void View::validateChild(const boost::shared_ptr<View>& child)
    {
      if (!child)
      {
        throw std::runtime_error("[View::validateChild] invalid child reference");
      }
      else if (child->id.size() == 0)
      {
        throw std::runtime_error("[View::validateChild] cannot handle empty child IDs");
      }
    }

  }
}
