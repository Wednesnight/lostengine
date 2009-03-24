#include "Filt3rz.h"

int main(int argn, char** args)
{
  try
  {
    Filt3rz filt3rz;
    filt3rz.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

