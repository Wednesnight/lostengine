#include "GuiroTest.h"

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    GuiroTest   guiroTest(app);
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
