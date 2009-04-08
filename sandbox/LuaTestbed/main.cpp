#include "lost/application/Application.h"
#include "lost/application/Tasklet.h"
#include "lost/common/Logger.h"

using namespace std;
using namespace boost;
using namespace lost::application;

struct MyFunkyTasklet : public Tasklet
{
  MyFunkyTasklet(const lost::resource::LoaderPtr& inLoader)
  : Tasklet::Tasklet(inLoader)
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
    static ApplicationPtr app = Application::create(argn, args);
    app->addTasklet(TaskletPtr(new MyFunkyTasklet(app->loader)));
    app->run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
