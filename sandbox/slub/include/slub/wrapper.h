#ifndef SLUB_WRAPPER_H
#define SLUB_WRAPPER_H

namespace slub {

  template<typename T>
  struct wrapper {
    T ref;
    bool gc;
  };

}

#endif
