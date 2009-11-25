/**
 * $Id: version.cpp 170 2008-08-17 16:11:39Z unwesen $
 *
 * Copyright (C) 2007,2008 the authors.
 *
 * Author: Henning Pfeiffer <slashgod@users.sourceforge.net>
 * Author: Jens Finkhaeuser <unwesen@users.sourceforge.net>
 *
 * This file is part of the Fhtagn! C++ Library, and may be distributed under
 * the following license terms:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 **/

#include <fhtagn/fhtagn-config.h>
#include <fhtagn/version.h>

#define FHTAGN_STRINGIFY(n) FHTAGN_STRINGIFY_HELPER(n)
#define FHTAGN_STRINGIFY_HELPER(n) #n

/**
 * For (temporary) compatibility with autotools build
 **/
#if defined(PACKAGE_MAJOR)
#define FHTAGN_MAJOR PACKAGE_MAJOR
#endif

#if defined(PACKAGE_MINOR)
#define FHTAGN_MINOR PACKAGE_MINOR
#endif

#if defined(PACKAGE_MAJOR) && defined(PACKAGE_MINOR)
#define FHTAGN_VERSION PACKAGE_MAJOR.PACKAGE_MINOR
#endif

/**
 * end of autotools compatibility
 **/

namespace fhtagn {


std::pair<uint16_t, uint16_t>
version()
{
    return std::make_pair<uint16_t, uint16_t>(FHTAGN_MAJOR, FHTAGN_MINOR);
}



char const * const copyright_string = "Fhtagn! " FHTAGN_STRINGIFY(FHTAGN_VERSION)
    " - Copyright (c) 2007,2008 by the authors.\n"
    "This version of Fhtagn! is released under the Boost Software License\n"
    "Version 1.0 from August 17th, 2003.";


char const * const license_string =
    "Boost Software License - Version 1.0 - August 17th, 2003\n"
    "\n"
    "Permission is hereby granted, free of charge, to any person or organization\n"
    "obtaining a copy of the software and accompanying documentation covered by\n"
    "this license (the \"Software\") to use, reproduce, display, distribute,\n"
    "execute, and transmit the Software, and to prepare derivative works of the\n"
    "Software, and to permit third-parties to whom the Software is furnished to\n"
    "do so, all subject to the following:\n"
    "\n"
    "The copyright notices in the Software and this entire statement, including\n"
    "the above license grant, this restriction and the following disclaimer,\n"
    "must be included in all copies of the Software, in whole or in part, and\n"
    "all derivative works of the Software, unless such copies or derivative\n"
    "works are solely in the form of machine-executable object code generated by\n"
    "a source language processor.\n"
    "\n"
    "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
    "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
    "FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT\n"
    "SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE\n"
    "FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,\n"
    "ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER\n"
    "DEALINGS IN THE SOFTWARE.";

} // namespace fhtagn