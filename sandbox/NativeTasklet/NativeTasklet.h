#ifndef NATIVETASKLET_H
#define NATIVETASKLET_H

#include "lost/application/Tasklet.h"
#include "lost/application/KeyEvent.h"
#include "lost/application/QueueEntity.h"
#include "lost/math/Vec2.h"
#include "lost/common/Color.h"
#include "lost/mesh/TextureManager.h"
#include "lost/rg/Node.h"

using namespace lost::common;
using namespace lost::math;
using namespace lost::mesh;
using namespace lost::rg;

struct ToggleClearColor : public lost::application::QueueEntity
{
  void process(const lost::application::Tasklet* sender) const;
};

struct NativeTasklet : public lost::application::Tasklet
{
  NativeTasklet();

  bool startup();
  bool update(double deltaSec);

  void keyDown(lost::application::KeyEventPtr event);

private:
  TextureManagerPtr textureManager;

  void toggleClearColor();

  NodePtr createLine(const Vec2& start, const Vec2& finish, const int width, const Color& color);
};

#endif
