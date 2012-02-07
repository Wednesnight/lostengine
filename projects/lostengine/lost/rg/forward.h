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

#ifndef LOST_RG_FORWARD_H
#define LOST_RG_FORWARD_H

namespace lost
{
namespace rg
{
struct Node; typedef lost::shared_ptr<Node> NodePtr;
struct Scissor; typedef lost::shared_ptr<Scissor> ScissorPtr;
struct ScissorRect; typedef lost::shared_ptr<ScissorRect> ScissorRectPtr;
struct ScissorStack; typedef lost::shared_ptr<ScissorStack> ScissorStackPtr;
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
