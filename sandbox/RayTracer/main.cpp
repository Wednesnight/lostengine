#include "lost/application/Runner.h"
#include "RayTracer.h"

int main(int argc, char *argv[]) {
  return lost::application::runTasklet(argc, argv, new RayTracer);
}
