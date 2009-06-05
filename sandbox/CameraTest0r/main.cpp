#include <iostream>
#include <stdexcept>
#include "lost/application/Application.h"
#include "CameraTest0r.h"

using namespace std;
using namespace lost;
using namespace lost::application;

int main(int argn, char** args)
{
  lost::application::runTasklet(new CameraTest0r);
  return 0;
}
