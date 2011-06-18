#include "lost/event/Pool.h"

using namespace std;

namespace lost
{
namespace event
{

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