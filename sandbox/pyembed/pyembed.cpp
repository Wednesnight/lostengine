#include <boost/python.hpp>

using namespace boost::python;

class CppClass {
public:
  int getNum() {
    return 7;
  }
};

int main( int argc, char ** argv ) {
  try {
    Py_Initialize();

    object main_module((
      handle<>(borrowed(PyImport_AddModule("__main__")))));

    object main_namespace = main_module.attr("__dict__");

	main_namespace["CppClass"] = class_<CppClass>("CppClass")
								.def("getNum",&CppClass::getNum);
							   
    handle<> ignored(( PyRun_String( "print \"Hello, World\"\ncpp = CppClass()\nprint cpp.getNum()\n",
                                     Py_file_input,
                                     main_namespace.ptr(),
                                     main_namespace.ptr() ) ));
  } catch( error_already_set ) {
    PyErr_Print();
  }
}
