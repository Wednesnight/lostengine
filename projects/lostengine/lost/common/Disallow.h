#ifndef LOST_COMMON_DISALLOW_H
#define LOST_COMMON_DISALLOW_H

// from here : http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml?showone=Copy_Constructors#Copy_Constructors
// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// use like this:
/*
class Foo {
 public:
  Foo(int f);
  ~Foo();

 private:
  DISALLOW_COPY_AND_ASSIGN(Foo);
};
*/

#endif