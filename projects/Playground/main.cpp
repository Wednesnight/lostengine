#include <iostream>
#include <stdexcept>
#include "lost/application/Application.h"
#include "Controller.h"

int main(int argn, char** args)
{
  return lost::application::runTasklet(new Controller);
}
