#ifndef GL_STATE_H
#define GL_STATE_H

#include <map>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include "Param.h"

struct State
{
private:
  std::map<GLenum, SharedParam> params;
public:
  State(const SharedParam& p1)
  {
  }
  State(const SharedParam& p1, const SharedParam& p2)
  {
  }
  State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3)
  {
  }
  State(const SharedParam& p1, const SharedParam& p2, const SharedParam& p3, const SharedParam& p4)
  {
  }

  void param(const SharedParam& inParam)
  {
    if (params.find(inParam->which) != params.end()) throw std::runtime_error("duplicate state param");
    params[inParam->which] = inParam;
  }
};

#endif
