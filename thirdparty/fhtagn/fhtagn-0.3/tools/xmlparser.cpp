/**
 * $Id: xmlparser.cpp 320 2009-06-08 22:20:05Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
 * Author: Tony Kostanjsek <lobotony@users.sourceforge.net>
 *
 * This program is licensed as free software for personal, educational or
 * other non-commerical uses: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Alternatively, licenses for commercial purposes are available as well.
 * Please send your enquiries to the copyright holder's address above.
 **/
#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include "fhtagn/xml/parser.h"
#include "fhtagn/xml/defaulthandler.h"

using namespace std;
using namespace fhtagn;

int main (int argc, char * const argv[])
{
	try
	{
	
		string filename = argv[1];
		ifstream file(filename.c_str());
		if(!file) throw runtime_error("couldn't open file: '"+filename+"'");
		ostringstream os;
		os << file.rdbuf();
		cout << "----- loaded data -----" << endl;
		cout << os.str() << endl;
		cout << "----- ends ------------" << endl;

		xml::defaulthandler handler;
		xml::parser<xml::defaulthandler> parser;

		if(!parser.parse(os.str(), handler)) throw runtime_error("couldn't fully parse document");
		
		cout << " -- Ok -- " << endl;
		
		return 0;
	}
	catch(exception& ex)
	{
		cout << "caught error: " << ex.what() << endl;
		return 1;
	}
}
