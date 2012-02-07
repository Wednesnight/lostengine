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

#ifndef LOST_GL_FORWARD_H
#define LOST_GL_FORWARD_H

#include "lost/gl/gltypes.h"

namespace lost
{
namespace gl
{

struct Texture;typedef lost::shared_ptr<Texture> TexturePtr;
struct TextureAtlas;typedef lost::shared_ptr<TextureAtlas> TextureAtlasPtr;
struct Context;typedef lost::shared_ptr<Context> ContextPtr;
struct ShaderProgram;typedef lost::shared_ptr<ShaderProgram> ShaderProgramPtr;
struct Shader;typedef lost::shared_ptr<Shader> ShaderPtr;
struct VertexShader;typedef lost::shared_ptr<VertexShader> VertexShaderPtr;
struct FragmentShader;typedef lost::shared_ptr<FragmentShader> FragmentShaderPtr;
struct FrameBuffer;typedef lost::shared_ptr<FrameBuffer> FrameBufferPtr;
struct RenderBuffer;typedef lost::shared_ptr<RenderBuffer> RenderBufferPtr;
struct Buffer;typedef lost::shared_ptr<Buffer> BufferPtr;
struct UniformBlock;typedef lost::shared_ptr<UniformBlock> UniformBlockPtr;
struct HostBuffer;typedef lost::shared_ptr<HostBuffer> HostBufferPtr;
struct HybridBuffer;typedef lost::shared_ptr<HybridBuffer> HybridBufferPtr;
struct HybridVertexBuffer;typedef lost::shared_ptr<HybridVertexBuffer> HybridVertexBufferPtr;
struct HybridIndexBuffer;typedef lost::shared_ptr<HybridIndexBuffer> HybridIndexBufferPtr;
}
}

#endif
