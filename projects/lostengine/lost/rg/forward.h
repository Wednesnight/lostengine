#ifndef LOST_RG_FORWARD_H
#define LOST_RG_FORWARD_H

namespace lost
{
namespace rg
{
struct Node; typedef lost::shared_ptr<Node> NodePtr;
struct Scissor; typedef lost::shared_ptr<Scissor> ScissorPtr;
struct ScissorRect; typedef lost::shared_ptr<ScissorRect> ScissorRectPtr;
struct Blend; typedef lost::shared_ptr<Blend> BlendPtr;
struct Camera;typedef lost::shared_ptr<Camera> CameraPtr;
struct Draw; typedef lost::shared_ptr<Draw> DrawPtr;
struct DepthTest;typedef lost::shared_ptr<DepthTest> DepthTestPtr;
struct ClearColor;typedef lost::shared_ptr<ClearColor> ClearColorPtr;
struct Clear;typedef lost::shared_ptr<Clear> ClearPtr;
struct DefaultFrameBuffer;typedef lost::shared_ptr<DefaultFrameBuffer> DefaultFrameBufferPtr;
struct FrameBuffer;typedef lost::shared_ptr<FrameBuffer> FrameBufferPtr;

}
}

#endif