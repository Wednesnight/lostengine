#define BOOST_SPIRIT_DEBUG
#ifndef BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT
#define BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT 2
#endif
#include <iostream>
#include <boost/spirit/core.hpp>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include "Grammar.h"

using namespace std;
using namespace boost;
using namespace boost::spirit;

int main (int argc, char * const argv[]) {
  try
  {
    string filename = "test.lua";
    ifstream file(filename.c_str());
    if(!file) throw runtime_error("couldn't open file: "+filename);
    ostringstream os;
    os << file.rdbuf();
    file.close();
    string data = os.str();
    cout << "loaded:" << endl;
    cout << os.str() << endl << endl;
    lost::lua::Grammar luagrammar;
    cout << parse(data.c_str(), luagrammar >> end_p, space_p).full; // requires end_p here due to boost docs, change of consumation of whitespaces at end of file
    
  }
  catch(exception& ex)
  {
    cerr << "caught exception: " << typeid(ex).name() << " reason: " << ex.what() << endl;
    return 1;
  }
  
  return 0;
}
