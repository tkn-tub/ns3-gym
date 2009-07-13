/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Pavel Boyko <boyko@iitp.ru>
 */

#ifndef STATS_H_
#define STATS_H_
#include <cmath>
#include <iostream>
#include <limits>

/// Simple average, min, max and std. deviation calculator
template <typename T = double>
class Average
{
public:
  Average () : size(0), min (std::numeric_limits<T>::max ()), max (0), avg (0), avg2 (0) {}

  /// Add new value
  void Update (T const & x)
  {
    min = std::min (x, min);
    max = std::max (x, max);
    avg = (size * avg + x) / (size + 1);
    avg2 = (size * avg2 + x * x) / (size + 1);
    size++;
  }
  /// Reset statistics
  void Reset ()
  {
    size = 0;
    min = std::numeric_limits<T>::max ();
    max = 0;
    avg = 0;
    avg2 = 0;
  }

  ///\name Access results
  //\{
  uint32_t Count () const { return size; }
  T        Min   () const { return min; }
  T        Max   () const { return max; }
  double   Avg   () const { return avg; }
  double   Err   () const { return sqrt ((avg2 - avg*avg)/(size - 1)); }
  //\}

private:
  uint32_t size;
  T      min, max;
  double avg, avg2;
};

/// Print avg (err) [min, max]
template <typename T>
std::ostream & operator<< (std::ostream & os, Average<T> const & x)
{
  if (x.Count () != 0)
    os << x.Avg () << " (" << x.Err () << ") [" << x.Min () << ", " << x.Max () << "]";
  else
    os << "NA"; // not avaliable
  return os;
}

#endif /* STATS_H_ */
