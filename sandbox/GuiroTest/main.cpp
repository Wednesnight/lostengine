#include "lost/application/Application.h"
#include "lost/application/UiTasklet.h"

using namespace lost;
using namespace lost::application;
using namespace lost::math;

struct GuiroTest : public UiTasklet
{
  GuiroTest()
  : UiTasklet(WindowParams("GuiroTest", Rect(50,200,640,480)))
  {
  }

  bool main()
  {
    bool result = UiTasklet::main();
    if (result)
    {
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
