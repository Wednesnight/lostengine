#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "Buff0r.h"

using namespace std;
using namespace lost::common;
using namespace lost::application;

int main(int argn, char** args)
{
  try
  {
    Application app;    
    Buff0r buff0r(*(app.displayAttributes), app.loader);
    app.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}
