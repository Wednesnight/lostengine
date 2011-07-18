#ifndef SLUB_EXCEPTION_H
#define SLUB_EXCEPTION_H

#include <stdexcept>

namespace slub {

  class NotFoundException : public std::exception {

    std::string message;
    
  public:

    NotFoundException(const std::string& message, const std::string& name) throw()
    : message(message + name) {}
    
    virtual ~NotFoundException() throw() {}
    
    const char* what() const throw() {
      return message.c_str();
    }
    
  };

  class ConstructorNotFoundException : public NotFoundException {

  public:
    
    ConstructorNotFoundException(const std::string& className) throw()
    : NotFoundException("No matching constructor found: ", className) {}

  };

  class MethodNotFoundException : public NotFoundException {
    
  public:
    
    MethodNotFoundException(const std::string& methodName) throw()
    : NotFoundException("No matching method found: ", methodName) {}
    
  };

  class FieldNotFoundException : public NotFoundException {
    
  public:
    
    FieldNotFoundException(const std::string& fieldName) throw()
    : NotFoundException("No matching field found: ", fieldName) {}
    
  };

  class OperatorNotFoundException : public NotFoundException {
    
  public:
    
    OperatorNotFoundException(const std::string& fieldName) throw()
    : NotFoundException("No matching operator found: ", fieldName) {}
    
  };
  
  class OverloadNotFoundException : public NotFoundException {
    
  public:
    
    OverloadNotFoundException(const std::string& overload) throw()
    : NotFoundException("No matching overload found: ", overload) {}
    
  };

  class BaseClassNotFoundException : public NotFoundException {
    
  public:
    
    BaseClassNotFoundException() throw()
    : NotFoundException("Base class was not registered", "") {}
    
  };
  
}

#endif
