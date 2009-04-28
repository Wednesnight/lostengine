#ifndef LOST_METER_METER_H
#define LOST_METER_METER_H

#include "lost/event/Event.h"
#include "lost/mesh/Mesh.h"

namespace lost
{
  namespace math { struct Vec2; struct Rect; }
  namespace event { struct EventDispatcher; typedef lost::shared_ptr<EventDispatcher> EventDispatcherPtr; }
  namespace meter
  {
    struct Meter;
    typedef lost::shared_ptr<Meter> MeterPtr;

    struct MeterEvent;
    typedef lost::shared_ptr<MeterEvent> MeterEventPtr;

    struct Meter
    {
    private:
      math::Rect   bounds;
      math::Vec2   minMax;
      unsigned int numEntries;
      bool         adjustRange;

      unsigned int              vertexSizeInBytes;
      unsigned int              vertexIndex;
      float                     vertexOffset;
      float                     valueStep;
      mesh::Mesh2D::VertexType* vertices;

      void init();
      void updateMesh();
      void updateByEvent(MeterEventPtr event);
    public:
      mesh::Mesh2DPtr mesh;

      Meter();
      Meter(const event::EventDispatcherPtr& dispatcher);

      void update(float value);
      inline void updateRange(float min, float max);
    };

    struct MeterEvent : public event::Event
    {
      static const event::Type& METER_UPDATE() { static event::Type d = "meterUpdate"; return d; }

      // meter value
      int value;

      MeterEvent(const event::Type& inType) : Event(inType) {}
      MeterEvent(const event::Type& inType, int inValue) : Event(inType), value(inValue) {}
      virtual ~MeterEvent() {}
    };
  }
}

#endif
