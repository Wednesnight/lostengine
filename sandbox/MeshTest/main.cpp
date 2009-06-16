#include <iostream>
#include <stdexcept>
#include "lost/application/Application.h"
#include "MeshTest.h"

int main(int argn, char** args)
{
  lost::application::runTasklet(new MeshTest);
  
  return 0;
}

