/**
 * $Id: stopwatch.h 319 2009-06-08 22:17:47Z unwesen $
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
#ifndef FHTAGN_UTIL_STOPWATCH_H
#define FHTAGN_UTIL_STOPWATCH_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <fhtagn/fhtagn.h>

#include <boost/tuple/tuple.hpp>

namespace fhtagn {
namespace util {

/**
 * Simple stopwatch class.
 *
 * Note that any of the below functions can throw a std::runtime_error if and
 * one of the underlying system calls fails. This is not to be expected, though.
 **/
class stopwatch
{
public:
  /**
   * We expect int64_t to exist here. Failing that, we very definitely expect
   * int32_t to exist. That may not be utterly portable, but it's still fairly
   * reasonable for utility code.
   **/
#if HAVE_BOOST__INT64_T
  typedef boost::int64_t usec_t;
#else
  typedef boost::int32_t usec_t;
#endif

  /**
   * The times the stopwatch returns are a wall time, system time and user time
   * (in that order), all in microseconds.
   **/
  typedef boost::tuple<usec_t, usec_t, usec_t>  times_t;

  /**
   * The constructor retrieves and stores the current absolute wall, system and
   * user times.
   **/
  stopwatch();

  /**
   * Returns the current absolute wall, system and user times.
   **/
  times_t get_absolute_times();

  /**
   * Resets the stored absolute wall, system and user times with current values.
   * This affects the times returned by get_cumulative_times()
   **/
  void reset();

  /**
   * Returns the the difference between the stored wall, system and user times
   * and the current values. Also stores the current values, so subsequent calls
   * to get_times() return the difference between these calls.
   **/
  times_t get_times();

  /**
   * Returns the difference in time between either the construction of this
   * object or the last reset() call.
   **/
  times_t get_cumulative_times();

private:
  times_t m_start;
  times_t m_checkpoint;
};


}} // namespace fhtagn::util

#endif // guard
