#ifndef LOST_GL_HYBRIDBUFFER_H
#define LOST_GL_HYBRIDBUFFER_H

namespace lost
{
namespace gl
{

struct HybridBuffer
{
  HybridBuffer(GLenum inGpuBufferType,
               const BufferLayout& inLayout);
  virtual ~HybridBuffer();
  
  GLenum gpuBufferType;
  bool dirty;
  HostBufferPtr hostBuffer;
  std::vector<BufferPtr> gpuBuffers;  
};

}
}

#endif