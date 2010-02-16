#include "lost/application/Tasklet.h"
#include "lost/application/Queue.h"

using namespace std;

namespace lost
{
  namespace application
  {

    Queue::~Queue()
    {
      clear();
    }

    void Queue::queue(const QueueEntity* entity)
    {
      push_back(entity);
    }

    void Queue::process(const Tasklet* sender)
    {
      // get list snapshot since entities could modify the list
      Queue current(*this);
      // clear own list
      std::vector<const QueueEntity*>::clear();

      for (iterator entity = current.begin(); entity != current.end(); ++entity)
      {
        (*entity)->process(sender);
      }
      current.clear();
    }

    void Queue::clear()
    {
      for (iterator entity = begin(); entity != end(); ++entity)
      {
        delete *entity;
      }
      std::vector<const QueueEntity*>::clear();
    }

  }
}
