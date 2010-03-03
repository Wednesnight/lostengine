/**
 * $Id: version.cpp 325 2009-06-09 07:29:38Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
 * Author: Henning Pfeiffer <slashgod@users.sourceforge.net>
 * Author: Jens Finkhaeuser <unwesen@users.sourceforge.net>
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


std::pair<boost::uint16_t, boost::uint16_t>
version()
{
    return std::make_pair<boost::uint16_t, boost::uint16_t>(FHTAGN_MAJOR, FHTAGN_MINOR);
}



char const * const copyright_string = "Fhtagn! " FHTAGN_STRINGIFY(FHTAGN_VERSION)
    " - Copyright (c) 2009 by Jens Finkhaeuser.\n"
    "This program comes with ABSOLUTELY NO WARRANTY. For licensing details see\n"
    "the file COPYING included in the source distribution.\n";


char const * const license_string =
    "This program is licensed as free software for personal, educational or\n"
    "other non-commerical uses: you can redistribute it and/or modify it\n"
    "under the terms of the GNU General Public License as published by the\n"
    "Free Software Foundation, either version 3 of the License, or (at your\n"
    "option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU General Public License\n"
    "along with this program.  If not, see <http://www.gnu.org/licenses/>.\n"
    "\n"
    "Alternatively, licenses for commercial purposes are available as well.\n"
    "Please send your enquiries to the copyright holder's address above.";


} // namespace fhtagn
