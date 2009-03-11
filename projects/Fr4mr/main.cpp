//#include "lost/gl/gl.h"
#include "lost/platform/Platform.h"
#include "lost/common/Logger.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace lost;

#include "Controller.h"

int main (int argc, const char * argv[])
{
  MyAppController controller;
  return controller.run();
}
