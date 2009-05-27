#include <iostream>
#include <stdexcept>
#include "lost/common/Logger.h"
#include "lost/application/Application.h"
#include "MeshTest.h"

using namespace std;
using namespace lost;
using namespace lost::application;

int main(int argn, char** args)
{
  lost::application::runTasklet(new MeshTest);
  
  return 0;
}

