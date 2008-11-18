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
  LogLevel( log_all );
  DOUT("check 1 2");

  ALDEBUG; // clear last error
  
  vector<string> ids = Device::allDeviceIds();
  DOUT("found "<<ids.size()<<" devices");
  for(int i=0; i<ids.size(); ++i)
  {
    DOUT("Device "<<i<<" : '"<<ids[i]<<"'");
  }
  
  shared_ptr<Device> device(new Device(ids[0]));
  shared_ptr<Context> context(new Context(device));
  
  context->makeCurrent();
  context->process();
  context->suspend();
  
  int error = 0;
  stb_vorbis* oggfile = stb_vorbis_open_filename("sketch008.ogg", &error, NULL);
  stb_vorbis_info info = stb_vorbis_get_info(oggfile);
  stb_vorbis_close(oggfile);
  
  DOUT("ok: "<<info.sample_rate);
  
  return 0;
}
