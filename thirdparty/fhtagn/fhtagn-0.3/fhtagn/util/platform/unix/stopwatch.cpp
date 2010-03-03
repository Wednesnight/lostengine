/**
 * $Id: stopwatch.cpp 319 2009-06-08 22:17:47Z unwesen $
 *
 * This file is part of the Fhtagn! C++ Library.
 * Copyright (C) 2009 Jens Finkhaeuser <unwesen@users.sourceforge.net>.
 *
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

#include <fhtagn/util/stopwatch.h>

#include <errno.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

#include <stdexcept>

namespace fhtagn {
namespace util {

stopwatch::times_t
stopwatch::get_absolute_times()
{
  // Initialize wall time
  struct ::timeval tv;
  int res = ::gettimeofday(&tv, NULL);
  if (-1 == res) {
    throw std::runtime_error(::strerror(errno));
  }
  usec_t wall_time = (static_cast<usec_t>(tv.tv_sec) * 1000000) + tv.tv_usec;

  // Initialize system & user time
  struct ::rusage usg;
  res = ::getrusage(RUSAGE_SELF, &usg);
  if (-1 == res) {
    throw std::runtime_error(::strerror(errno));
  }
  usec_t sys_time = (static_cast<usec_t>(usg.ru_stime.tv_sec) * 1000000)
    + usg.ru_stime.tv_usec;
  usec_t user_time = (static_cast<usec_t>(usg.ru_utime.tv_sec) * 1000000)
    + usg.ru_utime.tv_usec;

  return boost::make_tuple(wall_time, sys_time, user_time);
}

}} // namespace fhtagn::util
