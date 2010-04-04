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
struct FrameBuffer;typedef lost::shared_ptr<FrameBuffer> FrameBufferPtr;
struct Buffer;
struct UniformBlock;typedef lost::shared_ptr<UniformBlock> UniformBlockPtr;

}
}

#endif