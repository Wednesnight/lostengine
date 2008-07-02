#include "lost/common/Logger.h"
#include "lost/obj/Parser.h"
#include "lost/platform/Platform.h"
#include "lost/common/FileHelpers.h"

using namespace std;
using namespace lost::common;
using namespace lost::obj;
using namespace lost::obj::parser;

int main(int argn, char** args)
{
  LogLevel( log_all );
  try
  {
    string buffer = loadFile(string("cube.obj"));
    Scene scene;
    parse(buffer, scene);
    
    cout << scene << endl;
  }
  catch (exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return 0;
}
