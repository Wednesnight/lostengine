#include "lost/common/Logger.h"
#include "lost/al/al.h"
#include "lost/al/Device.h"
#include "lost/al/Context.h"
#include <boost/shared_ptr.hpp>

#include "stb_vorbis.h"

using namespace lost::common;
using namespace lost::al;
using namespace std;
using namespace boost;

int main (int argc, char* const argv[]) {
  try
  {
    LogLevel( log_all );

    ALDEBUG; // clear last error
    
    Device::logAllDeviceIds();
    shared_ptr<Context> context(new Context());
    
    context->makeCurrent();
    context->process();
    context->suspend();
    
    int error = 0;
    string filename = "wannabill.ogg";
    stb_vorbis* oggfile = stb_vorbis_open_filename(filename.c_str(), &error, NULL);
    if(oggfile)
    {
      stb_vorbis_info info = stb_vorbis_get_info(oggfile);
      stb_vorbis_close(oggfile);
      DOUT("ok: "<<info.sample_rate);
    }
    else
    {
      EOUT("couldn't open file '"<<filename<<"'");
    }
    
    return 0;
  }
  catch(std::exception& ex)
  {
    EOUT("caught execption: "<<ex.what());
    return 1;
  }
}
