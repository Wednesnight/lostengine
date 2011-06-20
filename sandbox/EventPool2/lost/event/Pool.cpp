#include "lost/event/Pool.h"
#include <boost/thread/once.hpp>

using namespace std;

namespace lost
{
namespace event
{

Handle::Handle()
{
  event = NULL;
}

Handle::Handle(const Handle& other)
{
  event = other.event;
}

Handle::~Handle()
{
}

void test()
{
  std::cout << "13" << std::endl;
}

Pool::Pool()
{
}

Pool::~Pool()
{
}

static Pool* _globalEventPool = NULL;

void initGlobalEventPool()
{
  cout << "+++ initGlobalEventPool" << endl;
  _globalEventPool = new Pool();
}

static boost::once_flag lostEventPoolOnceFlag = BOOST_ONCE_INIT;

Pool* Pool::instance()
{
  boost::call_once(initGlobalEventPool, lostEventPoolOnceFlag);
  return _globalEventPool;
}

}
}