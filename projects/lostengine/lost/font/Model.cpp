#include "lost/font/Model.h"
#include "lost/gl/Context.h"

namespace lost
{
namespace font
{
  Model::Model()
  {
    renderState.reset(new gl::State);

    renderState->texture2D = true;
    renderState->blend = true;
    renderState->blendSrc = GL_SRC_ALPHA;
    renderState->blendDest = GL_ONE_MINUS_SRC_ALPHA;
//    renderState->clearColor = blackColor;
    renderState->depthTest = false;  
    renderState->alphaTest = false;  
    renderState->normalArray = false;  
    renderState->vertexArray = true;  
    renderState->textureCoordArray = true;      
  }

  Model::~Model()
  {
  }

  void Model::render(boost::shared_ptr<gl::Context> context)
  {
    context->pushState(renderState);
    
    glVertexPointer(2,GL_FLOAT,0,vertices.get());
    glTexCoordPointer(2,GL_FLOAT,0,texcoords.get());
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, indices.get());    

    context->popState();
  }
}
}
