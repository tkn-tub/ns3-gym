#include "ns3/core-config.h"
#if !defined(INT64X64_DOUBLE_H) && (defined (INT64X64_USE_DOUBLE) || defined(PYTHON_SCAN))
#define INT64X64_DOUBLE_H

#include <stdint.h>
#include <cmath>  // pow


namespace ns3 {

/**
 * \ingroup core
 * \defgroup highprec High Precision Q64.64
 *
 * Functions and class for high precision Q64.64 fixed point arithmetic.
 */
  
/**
 * \ingroup highprec
 * High precision numerical type, implementing Q64.64 fixed precision.
 *
 * A Q64.64 fixed precision number consists of:
 *
 *   Bits | Function
 *   ---- | --------
 *     1  | Sign bit
 *    63  | Integer portion
 *    64  | Fractional portion
 *
 * All standard arithemetic operations are supported:
 *
 *   Category    | Operators
 *   ----------- | ---------
 *   Computation | `+`, `+=`, `-`, `-=`, `*`, `*=`, `/`, `/=`
 *   Comparison  | `==`, `!=`, `<`, `<=`, `>`, `>=`
 *   Unary       | `+`, `-`, `!`
 *
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
    int128_impl = 0,  //!< Native int128_t implementation.
    cairo_impl  = 1,  //!< cairo wideint implementation
    ld_impl     = 2   //!< long double implementation
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
   * \param [in] v floating value to represent
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
   * \param [in] v integer value to represent
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
    _v = (long double)hi + (long double)lo / HP_MAX_64;
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
  /**
   * Get the integer portion.
   *
   * \return The integer portion of this value.
   */
  inline int64_t GetHigh (void) const
  {
    return (int64_t)std::floor (_v);
  }
  /**
   * Get the fractional portion of this value, unscaled.
   *
   * \return The fractional portion, unscaled, as an integer.
   */
  inline uint64_t GetLow (void) const
  {
    long double frac = _v - std::floor (_v);
    frac = frac * HP_MASK_LO + 0.5L;
    uint64_t low = (uint64_t)frac;
    return low;
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
  friend bool         operator <= (const int64x64_t & lhs, const int64x64_t & rhs);
  friend bool         operator >  (const int64x64_t & lhs, const int64x64_t & rhs);
  friend bool         operator >= (const int64x64_t & lhs, const int64x64_t & rhs);
  
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
 * Inequality operator
 */
inline bool operator != (const int64x64_t & lhs, const int64x64_t & rhs)
{
  return !(lhs == rhs);
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
 * Less or equal operator
 */
inline bool operator <= (const int64x64_t & lhs, const int64x64_t & rhs)
{
  return lhs._v <= rhs._v;
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
 * Greater or equal operator
 */
inline bool operator >= (const int64x64_t & lhs, const int64x64_t & rhs)
{
  return lhs._v >= rhs._v;
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
