/*
Copyright (c) 2011 Tony Kostanjsek, Timo Boll

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lost/gl/HybridBuffer.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace gl
{

using namespace lost::math;

HybridBuffer::HybridBuffer()
{
  gpuBufferType = 0; // this is invalid! you MUST call init after constructing it to initialise this parameter properly.
  dirty = false;
}

HybridBuffer::~HybridBuffer()
{
}

void HybridBuffer::init(GLenum inGpuBufferType,
                        const BufferLayout& inLayout)
{
  gpuBufferType = inGpuBufferType;
  hostBuffer.reset(new HostBuffer(inLayout));

  gpuBuffer.reset(new Buffer);
  gpuBuffer->target = inGpuBufferType;

  dirty = false;  
  vertexAttributeMap[UT_position] = "position";
  vertexAttributeMap[UT_normal] = "normal";
  vertexAttributeMap[UT_texcoord0] = "texcoord0";
  vertexAttributeMap[UT_texcoord1] = "texcoord1";
  vertexAttributeMap[UT_vertexAttrib0] = "vertexAttrib0";
  vertexAttributeMap[UT_vertexAttrib1] = "vertexAttrib1";
  vertexAttributeMap[UT_vertexAttrib2] = "vertexAttrib2";
  vertexAttributeMap[UT_vertexAttrib3] = "vertexAttrib3";
}

void HybridBuffer::reset(uint32_t num)
{
  hostBuffer->reset(num);
  dirty = true;
}

void HybridBuffer::reset(common::DataPtr data)
{
  hostBuffer->reset(data);
  dirty = true;
}
  
  
void HybridBuffer::set(uint32_t idx, UsageType ut, uint8_t val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridBuffer::set(uint32_t idx, UsageType ut, uint16_t val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridBuffer::set(uint32_t idx, UsageType ut, uint32_t val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridBuffer::set(uint32_t idx, UsageType ut, float val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridBuffer::set(uint32_t idx, UsageType ut, const lost::math::Vec2& val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridBuffer::set(uint32_t idx, UsageType ut, const lost::math::Vec3& val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridBuffer::set(uint32_t idx, UsageType ut, const lost::math::Vec4& val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

void HybridBuffer::set(uint32_t idx, UsageType ut, const lost::common::Color& val)
{
  hostBuffer->set(idx, ut, val);
  dirty = true;
}

Vec2 HybridBuffer::getAsVec2(uint32_t idx, UsageType ut)
{
  return hostBuffer->getAsVec2(idx, ut);
}

Vec3 HybridBuffer::getAsVec3(uint32_t idx, UsageType ut)
{
  return hostBuffer->getAsVec3(idx, ut);
}

uint32_t HybridBuffer::getAsU32(uint32_t idx, UsageType ut)
{
  return hostBuffer->getAsU32(idx, ut);
}

uint16_t HybridBuffer::getAsU16(uint32_t idx, UsageType ut)
{
  return hostBuffer->getAsU16(idx, ut);
}

void HybridBuffer::upload()
{
  Buffer* buffer = gpuBuffer.get();
  Context::getCurrent()->bind(buffer);
  buffer->bufferData(buffer->target, 
                     hostBuffer->count*hostBuffer->layout.size(), 
                     hostBuffer->buffer,
                     GL_STATIC_DRAW); // FIXME: this parameter should probably be configurable
  dirty = false;
}

bool HybridBuffer::hasUsageType(UsageType ut)
{
  return hostBuffer->hasUsageType(ut);
}

Buffer* HybridBuffer::bufferForUsageType(UsageType ut)
{
  return gpuBuffer.get();
}

const AttributePointerConfig& HybridBuffer::pointerConfigForUsageType(UsageType ut)
{
  return hostBuffer->layout.pointerConfigForUsageType(ut);
}

}
}
