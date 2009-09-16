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
  : UiTasklet(WindowParams("GuiroTest", Rect(200,200,800,600)))
  {
  }

  void printRG(NodePtr node, std::string prefix = "")
  {
    for (std::list<NodePtr>::iterator idx = node->children.begin(); idx != node->children.end(); ++idx)
    {
      DOUT(prefix << (*idx)->name);
      printRG((*idx), prefix +"  ");
    }
  }

  bool startup()
  {
    bool result = UiTasklet::startup();
    if (result)
    {
      screen = lua->globals["GuiroTestController"]["controls"]["screen"]["rootNode"];
      printRG(screen);
    }
    return result;
  }

  bool main()
  {
    bool result = UiTasklet::main();
    if (result)
    {
      if (screen)
      {
        screen->process(window->context);
      }
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
