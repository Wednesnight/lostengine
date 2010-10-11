#include "Scene.h"
#include "Ray.h"

#include "lost/application/Tasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/ResizeEvent.h"
#include "lost/math/Vec3.h"
#include "lost/math/Vec2.h"
#include "lost/math/Rect.h"
#include "lost/bitmap/Bitmap.h"
#include "lost/common/Color.h"
#include "lost/gl/Texture.h"
#include "lost/mesh/Quad.h"
#include "lost/camera/Camera2D.h"

using namespace lost::application;
using namespace lost::bitmap;
using namespace lost::camera;
using namespace lost::common;
using namespace lost::gl;
using namespace lost::math;
using namespace lost::mesh;

struct RayTracer : public Tasklet {

public:

  RayTracer();

protected:

  bool running;
  bool needsRedraw;

  bool startup();
  bool update(double deltaSec);

  void resizeEvent(ResizeEventPtr event);
  void keyEvent(KeyEventPtr event);

  Camera2DPtr camera;
  Scene scene;

  BitmapPtr bitmap;
  TexturePtr texture;
  QuadPtr mesh;

  Rect world;
  Vec2 scale;
  Vec3 eye;

  double startTime;
  unsigned int numRays;
  unsigned int numIntersectionTests;
  unsigned int currentLine;
  Vec2 currentPosition;

  int maxDepth;
  bool showLights;

  void reset();

  void render(double timeout);

  shared_ptr<Primitive> raytrace(Ray& ray, Color& color, int depth, float rindex, float& distance);

};
