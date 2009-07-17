#include "Filt3rz.h"

int main(int argn, char** args)
{
  lost::application::runTasklet(new Filt3rz);  
  return 0;
}
