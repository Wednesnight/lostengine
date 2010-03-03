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

namespace fhtagn {
namespace util {

stopwatch::stopwatch()
{
  reset();
}



void
stopwatch::reset()
{
  m_start = m_checkpoint = get_absolute_times();
}



stopwatch::times_t
stopwatch::get_times()
{
  times_t current = get_absolute_times();
  times_t result = boost::make_tuple(
      current.get<0>() - m_checkpoint.get<0>(),
      current.get<1>() - m_checkpoint.get<1>(),
      current.get<2>() - m_checkpoint.get<2>());
  m_checkpoint = current;
  return result;
}



stopwatch::times_t
stopwatch::get_cumulative_times()
{
  times_t current = get_absolute_times();
  times_t result = boost::make_tuple(
      current.get<0>() - m_start.get<0>(),
      current.get<1>() - m_start.get<1>(),
      current.get<2>() - m_start.get<2>());
  return result;
}


}} // namespace fhtagn::util
