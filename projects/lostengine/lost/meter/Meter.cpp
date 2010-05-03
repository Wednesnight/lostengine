#include "lost/meter/Meter.h"

#include "lost/event/EventDispatcher.h"
#include "lost/mesh/Mesh.h"
#include <boost/bind.hpp>
#include "lost/event/Receive.h"
#include "lost/math/Vec2.h"
#include "lost/common/Color.h"
#include "lost/math/Matrix.h"
#include "lost/math/Vec3.h"

using namespace boost;
using namespace lost::common;
using namespace lost::event;
using namespace lost::math;
using namespace lost::mesh;

namespace lost
{
  namespace meter
  {

    Meter::Meter()
    {
      init();
    }

    Meter::Meter(const EventDispatcherPtr& dispatcher)
    {
      init();
      dispatcher->addEventListener(MeterEvent::METER_UPDATE(), receive<MeterEvent>(bind(&Meter::updateByEvent, this, _1)));
    }

    void Meter::init()
    {
      bounds = Rect(0,0,100,100);
      // percentage meter as default
      minMax = Vec2(0.0f, 100.0f);
      numEntries = 100;
      adjustRange = true;

      mesh.reset(new Mesh2D());
      mesh->indexBuffer->drawMode = GL_LINE_STRIP;
      vertices = NULL;
      updateMesh();
    }

    void Meter::updateMesh()
    {
      if (vertices != NULL) delete vertices;

      vertexSizeInBytes = sizeof(Mesh2D::VertexType);
      vertexIndex = 0;
      vertexOffset = bounds.width / numEntries;
      valueStep = bounds.height / (minMax.max - minMax.min);

      vertices = new Mesh2D::VertexType[numEntries];
      for (unsigned int idx = 0; idx < numEntries; idx++)
      {
        vertices[idx].x = 0.0f;
        vertices[idx].y = 0.0f;
      }

      mesh->vertices(true);
      mesh->vertexBuffer->bindBufferData(vertices, numEntries);
      mesh->material->color = whiteColor;
      mesh->transform = MatrixTranslation(Vec3(bounds.x, bounds.y, 0));
    }

    void Meter::updateByEvent(MeterEventPtr event)
    {
      update(event->value);
    }

    void Meter::update(float value)
    {
      if (adjustRange) updateRange(value, value);
      vertices[vertexIndex].x = fmin(vertexIndex*vertexOffset, bounds.width);
      vertices[vertexIndex].y = fmin(valueStep*(value-minMax.min), bounds.height);
      vertexIndex++;

      if (vertexIndex < numEntries)
      {
        mesh->vertexBuffer->bindBufferSubData(0, vertexSizeInBytes*(numEntries-vertexIndex), &vertices[vertexIndex]);
        mesh->vertexBuffer->bindBufferSubData(vertexSizeInBytes*(numEntries-vertexIndex), vertexSizeInBytes*vertexIndex, vertices);
      }
      else
      {
        mesh->vertexBuffer->bindBufferSubData(0, vertexSizeInBytes*numEntries, vertices);
        vertexIndex = 0;
      }
    }

    void Meter::updateRange(float min, float max)
    {
      if (min < minMax.min || max > minMax.max)
      {
        minMax = Vec2(fmin(minMax.min, min), fmax(minMax.max, max));
        float newValueStep = bounds.height / (minMax.max - minMax.min);
        for (unsigned int idx = 0; idx < numEntries; idx++)
        {
          float value = vertices[vertexIndex].x / valueStep;
          vertices[vertexIndex].y = fmin(newValueStep*value, bounds.height);
        }
        valueStep = newValueStep;
      }
    }
    
  }
}
