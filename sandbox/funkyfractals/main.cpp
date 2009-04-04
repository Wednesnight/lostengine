#include "FunkyFractals.h"

int main(int argn, char** args)
{
  try
  {
    static FunkyFractals funkyfractals;
    funkyfractals.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }

  return EXIT_SUCCESS;
}
