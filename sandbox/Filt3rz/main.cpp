#include "Filt3rz.h"

void batchtest();

int main(int argn, char** args)
{
  try
  {
    static Filt3rz filt3rz;
    batchtest();
    filt3rz.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return 0;
}

