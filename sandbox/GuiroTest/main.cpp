#include "lost/application/Application.h"
#include "lost/application/UiTasklet.h"
#include "lost/rg/Node.h"

using namespace lost;
using namespace lost::application;
using namespace lost::math;
using namespace lost::rg;

struct GuiroTest : public UiTasklet
{
  NodePtr screen;

  GuiroTest()
  : UiTasklet(WindowParams("GuiroTest", Rect(50,200,640,480)))
  {
  }

  bool startup()
  {
    bool result = UiTasklet::startup();
    if (result)
    {
      screen = lua->globals["GuiroTestController"]["controls"]["screen"]["renderNode"];
    }
    return result;
  }

  bool main()
  {
    bool result = UiTasklet::main();
    if (result)
    {
      if (screen) screen->process(window->context);
      window->context->swapBuffers();
    }
    return result;
  }
};

int main(int argn, char** args)
{
  runTasklet(new GuiroTest);
  return 0;
}
