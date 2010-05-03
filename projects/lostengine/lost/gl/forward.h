#ifndef LOST_GL_FORWARD_H
#define LOST_GL_FORWARD_H

#include "lost/gl/gltypes.h"

namespace lost
{
namespace gl
{

struct Texture;typedef lost::shared_ptr<Texture> TexturePtr;
struct Context;typedef lost::shared_ptr<Context> ContextPtr;
struct ShaderProgram;typedef lost::shared_ptr<ShaderProgram> ShaderProgramPtr;
struct Shader;typedef lost::shared_ptr<Shader> ShaderPtr;
struct VertexShader;typedef lost::shared_ptr<VertexShader> VertexShaderPtr;
struct FragmentShader;typedef lost::shared_ptr<FragmentShader> FragmentShaderPtr;
struct FrameBuffer;typedef lost::shared_ptr<FrameBuffer> FrameBufferPtr;
struct Buffer;
struct UniformBlock;typedef lost::shared_ptr<UniformBlock> UniformBlockPtr;
struct HybridBuffer;typedef lost::shared_ptr<HybridBuffer> HybridBufferPtr;
struct HybridVertexBuffer;typedef lost::shared_ptr<HybridVertexBuffer> HybridVertexBufferPtr;
struct HybridIndexBuffer;typedef lost::shared_ptr<HybridIndexBuffer> HybridIndexBufferPtr;
}
}

#endif