#include "lost/common/Logger.h"
#include "lost/al/al.h"
#include "lost/al/Device.h"
#include "lost/al/Context.h"
#include "lost/al/Listener.h"
#include "lost/al/Source.h"
#include "lost/al/Buffer.h"
#include <boost/shared_ptr.hpp>

#include "stb_vorbis.h"

using namespace lost::common;
using namespace lost::al;
using namespace lost::math;
using namespace std;
using namespace boost;

int main (int argc, char* const argv[]) {
  try
  {
    LogLevel( log_all );

    ALDEBUG; // clear last error
    
    Device::logAllDeviceIds();
    DOUT("default device: "<<Device::defaultDeviceId());

    shared_ptr<Context> context(new Context());    
    context->makeCurrent();

/*    DOUT("dopplerFactor: "<<Context::dopplerFactor());
    DOUT("speed of sound: "<<Context::speedOfSound());
    DOUT("distance model: "<<Context::distanceModel());
    
    DOUT("version: "<<Context::version());
    DOUT("renderer: "<<Context::renderer());
    DOUT("vendor: "<<Context::vendor());
    DOUT("extensions: " <<Context::extensions());
    
    Context::distanceModel(AL_NONE);
    
    Listener::position(Vec3(1,2,3));
    Listener::velocity(Vec3(4,5,6));
    Listener::gain(.75);
    Listener::orientation(Vec3(7,8,9), Vec3(0,1,0));
    
    DOUT("pos: "<<Listener::position());
    DOUT("velocity: "<<Listener::velocity());
    DOUT("gain: "<<Listener::gain());
    Vec3 at, up;
    Listener::getOrientation(at, up);
    DOUT("orientation at: "<<at<<" up: "<<up);*/
    context->process();
//    context->suspend();
    
    int error = 0;
    string filename = "wannabill.ogg";
    stb_vorbis* oggfile = stb_vorbis_open_filename(filename.c_str(), &error, NULL);
    if(oggfile)
    {
      stb_vorbis_info info = stb_vorbis_get_info(oggfile);
      stb_vorbis_close(oggfile);
      DOUT("ok: "<<info.sample_rate);
      ifstream file(filename.c_str());
      ostringstream os;
      os << file.rdbuf();
      string data = os.str();
      short* decodedSamples;
      int channels;
      int numSamples = stb_vorbis_decode_memory((unsigned char*)(data.c_str()), data.size(), &channels, &decodedSamples);
      DOUT("num decoded samples: "<<numSamples);
      DOUT("channels: "<<channels);
      Source src;
      shared_ptr<Buffer> buffer(new Buffer);
      buffer->bufferData((channels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
                         decodedSamples,
                         numSamples*2,
                         info.sample_rate);
      src.queue(buffer);
      src.play();
      // wait for the buffer to finish playing
      while(src.processedBuffers() != src.queuedBuffers())
      {
        DOUT("processed: "<<src.processedBuffers());
        DOUT("queued: "<<src.queuedBuffers());
        sleep(1);
      }
      src.unqueue(buffer);
      src.stop();
      free(decodedSamples);
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
