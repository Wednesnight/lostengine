#ifndef SLUB_REGISTRY_H
#define SLUB_REGISTRY_H

#include <string>

namespace slub {

  template<typename T>
  struct registry {

    static std::string typeName;

    static void registerType(const std::string& typeName) {
      registry::typeName = typeName;
    }

    static bool isRegisteredType() {
      return registry::typeName.size() > 0;
    }

    static std::string getTypeName() {
      return registry::typeName;
    }

  };

  template<typename T>
  std::string registry<T>::typeName;

}

#endif
