#ifndef LOST_MESH_FOWARD_H
#define LOST_MESH_FOWARD_H

namespace lost
{
namespace mesh
{

struct TextureManager;
typedef lost::shared_ptr<TextureManager> TextureManagerPtr;

struct Mesh; typedef lost::shared_ptr<Mesh> MeshPtr;
struct Material; typedef lost::shared_ptr<Material> MaterialPtr;

struct Disc; typedef lost::shared_ptr<Disc> DiscPtr;

}
}

#endif