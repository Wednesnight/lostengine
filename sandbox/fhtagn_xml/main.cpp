#include <iostream>

#include "fhtagn/xml/parser.h"
#include "fhtagn/xml/defaulthandler.h"

using namespace std;

int main(int argc, char *argv[])
{
  fhtagn::xml::parser<fhtagn::xml::defaulthandler> parser;
  fhtagn::xml::defaulthandler                      handler;

  parser.parse( "<test><test2></test2></test>", handler );

  return EXIT_SUCCESS;
}
