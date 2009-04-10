#include "Filt3rz.h"

using namespace lost::application;

int main(int argn, char** args)
{
  try
  {
    static ApplicationPtr app = Application::create(TaskletPtr(new Filt3rz()));
    app->run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
