#include "lost/font/Model.h"
#include "lost/gl/Context.h"
#include "lost/gl/Texture.h"
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
    renderState->depthTest = false;  
    renderState->alphaTest = false;  
    renderState->normalArray = false;  
    renderState->vertexArray = true;  
    renderState->textureCoordArray = true;      
      
    vertexCount = 0;
    indexCount = 0;
      
  }

  Model::~Model()
  {
  }

  void Model::render(boost::shared_ptr<gl::Context> context)
  {
    // don't do nything if model is empty
    if((vertexCount==0) || (indexCount==0))
      return;
    context->pushState(renderState);
    
    texture->bind();
    glVertexPointer(2,GL_FLOAT,0,vertices.get());
    glTexCoordPointer(2,GL_FLOAT,0,texcoords.get());
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, indices.get());    

    context->popState();
  }
}
}
