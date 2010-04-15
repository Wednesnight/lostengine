#ifndef LOST_MESH_SPHERE_H
#define LOST_MESH_SPHERE_H

#include "lost/mesh/forward.h"
#include "lost/mesh/Mesh.h"

namespace lost
{
  namespace mesh
  {

    struct Sphere : Mesh
    {
      Sphere(float radius = 1.0f, uint8_t subdivisions = 16);
      virtual ~Sphere() {}

      static SpherePtr create(float radius = 1.0f, uint8_t subdivisions = 16)
      {
        return SpherePtr(new Sphere(radius, subdivisions));
      }

      void updateRadius(float radius);  // sets new radius, recalculates mesh, uses all other cached values
      void updateSubdivisions(uint8_t subdivisions);  // sets new subdivision count (that is number of rows / points per row),
                                                      // recalculates mesh, uses all other cached values

    private:
      float   radius;
      uint8_t subdivisions; // affects LOD for sphere, that is number of rows / points per row

      void update();
    };

  }
}

#endif
