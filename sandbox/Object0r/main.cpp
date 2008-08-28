#include "Object0r.h"

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    Application app;
    Object0r    object0r(app);
    app.run();
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
