#include "lost/platform/Platform.h"
#include "lost/lua/lua.h"
#include "lost/lua/State.h"
#include <iostream>

using namespace std;
using namespace luabind;

void greet()
{
    std::cout << "hello world!\n";
}

class Rect
{
private:
  int v;
public:
  Rect() { v=13;}
  Rect(int inVal) { v=inVal;}
  
  int val() { return v; }
};


class NumberPrinter {
  public:
    NumberPrinter(int number) :
      m_number(number)
    {
      cout << "NumberPrinter constructor" << endl;
    }

    NumberPrinter(const NumberPrinter& inN)
    {
      cout << "NumberPrinter copy constructor" << endl;
      m_number = inN.m_number;
    }

    ~NumberPrinter()
    {
      cout << "NumberPrinter destructor" << endl;
    }

    void print() {
      cout << m_number << endl;
    }

  private:
    int m_number;
};
int main (int argc, char * const argv[]) {
  try
  {
    lost::lua::State state;
    state.openLibs();
    
    // perform luabind stuff
    open(state); // luabind::open requires a lua_State*, but we can cast lost::lua::State to lua_State*, so it just works
    module(state, "testing")
    [
        def("greet", &greet),
        class_<Rect>("Rect")
          .def(constructor<>())
          .def(constructor<int>())
          .def("val", &Rect::val)
               
    ];    

    module(state) [
      luabind::class_<NumberPrinter>("NumberPrinter")
        .def(luabind::constructor<int>())
        .def("print", &NumberPrinter::print)
    ];

    state.doFile(lost::platform::buildResourcePath("test.lua"));



    NumberPrinter p(13131314);
    NumberPrinter p2(348576);

    // calls test0r:print()
    globals(state)["test0r"] = &p;
    object test0r = globals(state)["test0r"];
    object test0rprint = test0r["print"];
    call_function<void>(test0rprint, test0r); // test0r is the 'self' parameter for the method call

    // also calls test0r:print()
    call_function<void>(globals(state)["test0r"]["print"], globals(state)["test0r"]);

    // execute the test script in the same interpreter
    state.doString("print('test0ring');test0r:print();print('ok');");

    // test if we can copy an object into the lua interpreter, but then only extract a pointer to it later on
    globals(state)["np2"] = p2; // this copies the object
    // we can extract a copy or, in this case, only a pointer to the object residing in the interpreter
    NumberPrinter* p2p = object_cast<NumberPrinter*>(globals(state)["np2"]);
    p2p->print();

    return 0;
  }
  catch(exception& ex)
  {
    cout << "error: "<<ex.what() << endl;
    return 1;
  }
}
