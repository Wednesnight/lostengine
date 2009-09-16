#include "lost/application/Application.h"
#include "lost/application/Tasklet.h"
#include "lost/common/Logger.h"

using namespace std;
using namespace boost;
using namespace lost::application;

struct MyFunkyTasklet : public Tasklet
{
  MyFunkyTasklet()
  {
  }

  virtual bool startup()
  {
    DOUT("MyFunkyTasklet::startup()");
    return true;
  }

  virtual bool main()
  {
//    DOUT("MyFunkyTasklet::main()");
    return true;
  }

  virtual bool shutdown()
  {
    DOUT("MyFunkyTasklet::shutdown()");
    return true;
  }
};

int main(int argn, char** args)
{
  try
  {
    static ApplicationPtr app = Application::create(new MyFunkyTasklet());
    app->run();  
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
