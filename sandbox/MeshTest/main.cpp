#include <iostream>
#include <stdexcept>
#include "lost/common/Logger.h"

using namespace std;

int main(int argn, char** args)
{
  try
  {
    DOUT("asd");
/*    static Filt3rz filt3rz;
    batchtest();
    filt3rz.run();*/
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

