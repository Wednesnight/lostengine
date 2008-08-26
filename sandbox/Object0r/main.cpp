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
  catch(luabind::error& e)
  {
    luabind::object error_msg(luabind::from_stack(e.state(), -1));
    EOUT(error_msg);
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}
