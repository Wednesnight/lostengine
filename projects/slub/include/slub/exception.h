#ifndef SLUB_EXCEPTION_H
#define SLUB_EXCEPTION_H

#include "config.h"

#include <stdexcept>

namespace slub {

  class NotFoundException : public std::exception {

    string message;
    
  public:

    NotFoundException(const string& message, const string& name) throw()
    : message(message + name) {}
    
    virtual ~NotFoundException() throw() {}
    
    const char* what() const throw() {
      return message.c_str();
    }
    
  };

  class ConstructorNotFoundException : public NotFoundException {

  public:
    
    ConstructorNotFoundException(const string& className) throw()
    : NotFoundException("No matching constructor found: ", className) {}

  };

  class MethodNotFoundException : public NotFoundException {
    
  public:
    
    MethodNotFoundException(const string& methodName) throw()
    : NotFoundException("No matching method found: ", methodName) {}
    
  };

  class FieldNotFoundException : public NotFoundException {
    
  public:
    
    FieldNotFoundException(const string& fieldName) throw()
    : NotFoundException("No matching field found: ", fieldName) {}
    
  };

  class OperatorNotFoundException : public NotFoundException {
    
  public:
    
    OperatorNotFoundException(const string& fieldName) throw()
    : NotFoundException("No matching operator found: ", fieldName) {}
    
  };
  
  class OverloadNotFoundException : public NotFoundException {
    
  public:
    
    OverloadNotFoundException(const string& overload) throw()
    : NotFoundException("No matching overload found: ", overload) {}
    
  };

  class BaseClassNotFoundException : public NotFoundException {
    
  public:
    
    BaseClassNotFoundException() throw()
    : NotFoundException("Base class was not registered", "") {}
    
  };
  
}

#endif
