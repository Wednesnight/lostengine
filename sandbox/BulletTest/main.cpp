#include "lost/application/Runner.h"
#include "BulletTest.h"

int main(int argc, char *argv[])
{
  return lost::application::runTasklet(argc, argv, new BulletTest);
}
