#include "lost/application/BasicTasklet.h"
#include "lost/event/EventDispatcher.h"

namespace lost
{
namespace application
{

using namespace lost::event;

BasicTasklet::BasicTasklet()
: eventDispatcher(new event::EventDispatcher())
{
  waitForEvents = false;
}

BasicTasklet::~BasicTasklet()
{
}

void BasicTasklet::queueApplicationEvent(EventPtr event)
{
  QueueEventPtr queue(new QueueEvent(event));
  eventDispatcher->dispatchEvent(queue);
}

void BasicTasklet::dispatchApplicationEvent(EventPtr event)
{
  queueApplicationEvent(event);
  processApplicationEvents();
}

void BasicTasklet::processApplicationEvents()
{
  ProcessEventPtr process(new ProcessEvent());
  eventDispatcher->dispatchEvent(process);
}

}
}