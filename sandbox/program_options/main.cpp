#include <iostream>
#include <iterator>
#include "config.hpp"

using namespace std;
using namespace lostbase;

int main(int argc, char *argv[])
{
  try
  {
    configuration l_config;

    l_config.add_variable( "help", "produce help message" );
    l_config("huelp", "mehr hilfe")
            ("asd", "jkl")
            ("löläölöä", "köl");
    l_config.add_variable( "compression", po::value<int>(), "set compression level" );

    l_config.load_commandline( argc, argv );

    if (l_config.isset( "help" ))
    {
      l_config.print();
      return 1;
    }

    if (l_config.isset( "compression" ))
    {
      cout << "Compression level was set to " 
           << l_config.get_variable( "compression" ).as<int>() << ".\n";
    } else {
      cout << "Compression level was not set.\n";
    }
  }
  catch(exception& e) {
    cerr << "error: " << e.what() << "\n";
    return 1;
  }
  catch(...) {
    cerr << "Exception of unknown type!\n";
  }

  return EXIT_SUCCESS;
}
