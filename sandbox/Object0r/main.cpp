#include "Object0r.h"

int main(int argn, char** args)
{
  int result = 0;
  try
  {
    Object0r controller;
    result = controller.run();
  }
  catch (std::exception& e)
  {
    EOUT("exception: " << e.what());
  }
  
  return result;
}
