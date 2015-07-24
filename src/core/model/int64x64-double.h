/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 INRIA
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
 */

#include "ns3/core-config.h"
#if !defined(INT64X64_DOUBLE_H) && (defined (INT64X64_USE_DOUBLE) || defined(PYTHON_SCAN))
#define INT64X64_DOUBLE_H

#include <stdint.h>
#include <cmath>  // pow
#include <utility>  // pair

/**
 * \file
 * \ingroup highprec
 * Declaration and implementation of the ns3::int64x64_t type
 * using the double type.
 */

namespace ns3 {

/**
 * \internal
 * The implementation documented here uses native long double.
 */
class int64x64_t
{
  /// Mask for fraction part
  static const uint64_t    HP_MASK_LO = 0xffffffffffffffffULL;
  /**
   * Floating point value of HP_MASK_LO + 1
   * We really want:
   * \code
   *   static const long double HP_MAX_64 = std:pow (2.0L, 64);
   * \endcode
   * but we can't call functions in const definitions,
   * We could make this a static and initialize in int64x64-double.cc or
   * int64x64.cc, but this requires handling static initialization order
   * when most of the implementation is inline.  Instead, we resort to
   * this define.
   */
#define HP_MAX_64    (std::pow (2.0L, 64))

public:
  /**
   * Type tag for the underlying implementation.
   *
   * A few testcases are are sensitive to implementation,
   * specifically the double implementation.  To handle this,
   * we expose the underlying implementation type here.
   */
  enum impl_type {
    int128_impl,  //!< Native int128_t implementation.
    cairo_impl,   //!< cairo wideint implementation
    ld_impl,      //!< long double implementation
  };

  /// Type tag for this implementation.
  static const enum impl_type implementation = ld_impl;

  /// Default constructor
  inline int64x64_t ()
    : _v (0) {}
  /**@{*/
  /**
   * Construct from a floating point value.
   *
   * \param [in] v Floating value to represent
   */
  inline int64x64_t (double v)
    : _v (v) {}
  inline int64x64_t (long double v)
    : _v (v) {}
  /**@}*/

  /**@{*/
  /**
   * Construct from an integral type.
   *
   * \param [in] v Integer value to represent
   */
  inline int64x64_t (int v)
    : _v (v) {}
  inline int64x64_t (long int v)
    : _v (v) {}
  inline int64x64_t (long long int v)
    : _v (v) {}
  inline int64x64_t (unsigned int v)
    : _v (v) {}
  inline int64x64_t (unsigned long int v)
    : _v (v) {}
  inline int64x64_t (unsigned long long int v)
    : _v (v) {}
  /**@}*/
  /**
   * Construct from explicit high and low values.
   *
   * \param [in] hi Integer portion.
   * \param [in] lo Fractional portion, already scaled to HP_MAX_64.
   */
  explicit inline int64x64_t (int64_t hi, uint64_t lo)
  {
    const bool negative = hi < 0;
    const long double fhi = negative ? -hi : hi;
    const long double flo = lo / HP_MAX_64;
    _v = negative ? - fhi : fhi;
    _v += flo;
    // _v = negative ? -_v : _v;
  }

  /**
   * Copy constructor.
   *
   * \param [in] o Value to copy.
   */
  inline int64x64_t (const int64x64_t & o)
    : _v (o._v) {}
  /**
   * Assignment.
   *
   * \param [in] o Value to assign to this int64x64_t.
   */
  inline int64x64_t & operator = (const int64x64_t & o)
  {
    _v = o._v;
    return *this;
  }

  /**
   * Get this value as a double.
   *
   * \return This value in floating form.
   */
  inline double GetDouble (void) const
  {
    return (double)_v;
  }
private:
  /**
   * Get the high and low portions of this value.
   *
   * \return A pair of the high and low words
   */
  std::pair<int64_t, uint64_t> GetHighLow (void) const
    {
    const bool negative = _v < 0;
    const long double v = negative ? -_v : _v;

    long double fhi;
    long double flo = std::modf (v, &fhi);
    // Add 0.5 to round, which improves the last count
    // This breaks these tests:
    //   TestSuite devices-mesh-dot11s-regression
    //   TestSuite devices-mesh-flame-regression
    //   TestSuite routing-aodv-regression
    //   TestSuite routing-olsr-regression
    // Setting round = 0; breaks:
    //   TestSuite int64x64
    const long double round = 0.5;
    flo = flo * HP_MAX_64 + round;
    int64_t  hi = fhi;
    uint64_t lo = flo;
    if (flo >= HP_MAX_64)
      {
	// conversion to uint64 rolled over
	++hi;
      }
    if (negative)
      {
	lo = ~lo;
	hi = ~hi;
	if (++lo == 0)
	  {
	    ++hi;
	  }
      }
    return std::make_pair (hi, lo);
    }
public:
  /**
   * Get the integer portion.
   *
   * \return The integer portion of this value.
   */
  inline int64_t GetHigh (void) const
  {
    return GetHighLow ().first;
  }
  /**
   * Get the fractional portion of this value, unscaled.
   *
   * \return The fractional portion, unscaled, as an integer.
   */
  inline uint64_t GetLow (void) const
  {
    return GetHighLow ().second;
  }

  /**
   * Multiply this value by a Q0.128 value, presumably representing an inverse,
   * completing a division operation.
   *
   * \param [in] o The inverse operand.
   *
   * \note There is no difference between Q64.64 and Q0.128 in this implementation,
   * so this function is a simple multiply.
   */
  inline void MulByInvert (const int64x64_t & o)
  {
    _v *= o._v;
  }

  /**
   * Compute the inverse of an integer value.
   *
   * \param [in] v The value to compute the inverse of.
   * \return The inverse.
   */
  static inline int64x64_t Invert (uint64_t v)
  {
    int64x64_t tmp ((long double)1 / v);
    return tmp;
  }

private:
  friend bool         operator == (const int64x64_t & lhs, const int64x64_t & rhs);

  friend bool         operator <  (const int64x64_t & lhs, const int64x64_t & rhs);
  friend bool         operator >  (const int64x64_t & lhs, const int64x64_t & rhs);
  
  friend int64x64_t & operator += (      int64x64_t & lhs, const int64x64_t & rhs);
  friend int64x64_t & operator -= (      int64x64_t & lhs, const int64x64_t & rhs);
  friend int64x64_t & operator *= (      int64x64_t & lhs, const int64x64_t & rhs);
  friend int64x64_t & operator /= (      int64x64_t & lhs, const int64x64_t & rhs);

  friend int64x64_t   operator -  (const int64x64_t & lhs);
  friend int64x64_t   operator !  (const int64x64_t & lhs);

  long double _v;  //!< The Q64.64 value.

};  // class int64x64_t


/**
 * \ingroup highprec
 * Equality operator.
 */
inline bool operator == (const int64x64_t & lhs, const int64x64_t & rhs)
{
  return lhs._v == rhs._v;
}
/**
 * \ingroup highprec
 * Less than operator
 */
inline bool operator < (const int64x64_t & lhs, const int64x64_t & rhs)
{
  return lhs._v < rhs._v;
}
/**
 * \ingroup highprec
 * Greater operator
 */
inline bool operator > (const int64x64_t & lhs, const int64x64_t & rhs)
{
  return lhs._v > rhs._v;
}

/**
 * \ingroup highprec
 * Compound addition operator
 */
inline int64x64_t & operator += (int64x64_t & lhs, const int64x64_t & rhs)
{
  lhs._v += rhs._v;
  return lhs;
}
/**
 * \ingroup highprec
 * Compound subtraction operator
 */
inline int64x64_t & operator -= (int64x64_t & lhs, const int64x64_t & rhs)
{
  lhs._v -= rhs._v;
  return lhs;
}
/**
 * \ingroup highprec
 * Compound multiplication operator
 */
inline int64x64_t & operator *= (int64x64_t & lhs, const int64x64_t & rhs)
{
  lhs._v *= rhs._v;
  return lhs;
}
/**
 * \ingroup highprec
 * Compound division operator
 */
inline int64x64_t & operator /= (int64x64_t & lhs, const int64x64_t & rhs)
{
  lhs._v /= rhs._v;
  return lhs;
}

/**
 * \ingroup highprec
 * Unary plus operator
 */
inline int64x64_t operator + (const int64x64_t & lhs)
{
  return lhs;
}
/**
 * \ingroup highprec
 * Unary negation operator (change sign operator)
 */
inline int64x64_t operator - (const int64x64_t & lhs)
{
  return int64x64_t (-lhs._v);
}
/**
 * \ingroup highprec
 * Logical not operator
 */
inline int64x64_t operator ! (const int64x64_t & lhs)
{
  return int64x64_t (!lhs._v);
}


} // namespace ns3

#endif /* INT64X64_DOUBLE_H */
