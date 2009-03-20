#include "lost/font/Model.h"

using namespace lost::gl;

namespace lost
{
  namespace font
  {
    Model::Model()
    {
      renderState = State::create(Texture2D::create(true), Blend::create(true), BlendFunc::create(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA),
                                  DepthTest::create(false), AlphaTest::create(false), NormalArray::create(false), VertexArray::create(true), TextureArray::create(true));
        
      vertexCount = 0;
      indexCount = 0;
    }

    Model::~Model()
    {
    }

    void Model::render(const boost::shared_ptr<Canvas>& canvas)
    {
      // don't do anything if model is empty
      if((vertexCount==0) || (indexCount==0))
        return;
      canvas->context->pushState(renderState);
      
      texture->bind();
      glVertexPointer(2,GL_FLOAT,0,vertices.get()); GLDEBUG;
      glTexCoordPointer(2,GL_FLOAT,0,texcoords.get()); GLDEBUG;
      glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, indices.get()); GLDEBUG;

      canvas->context->popState();
    }
  }
}
