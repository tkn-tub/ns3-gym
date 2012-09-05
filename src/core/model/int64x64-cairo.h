#include "ns3/core-config.h"
#if !defined(INT64X64_CAIRO_H) && defined (INT64X64_USE_CAIRO) && !defined(PYTHON_SCAN)
#define INT64X64_CAIRO_H

#include <stdint.h>
#include <cmath>

#include "cairo-wideint-private.h"

#ifdef __i386__
// this assembly code does not appear to work right yet.
#define noInt64x64_CAIRO_ASM 1
#endif

namespace ns3 {

class int64x64_t
{
public:
  inline int64x64_t ()
  {
    _v.hi = 0;
    _v.lo = 0;
  }
  inline int64x64_t (double value)
  {
#define HPCAIRO_MAX_64 18446744073709551615.0
    double fhi = std::floor (value);
    int64_t hi = lround (fhi);
    uint64_t lo = (uint64_t)((value - fhi) * HPCAIRO_MAX_64);
    _v.hi = hi;
    _v.lo = lo;
#undef HPCAIRO_MAX_64
  }
  inline int64x64_t (int v)
  {
    _v.hi = v;
    _v.lo = 0;
  }
  inline int64x64_t (long int v)
  {
    _v.hi = v;
    _v.lo = 0;
  }
  inline int64x64_t (long long int v)
  {
    _v.hi = v;
    _v.lo = 0;
  }
  inline int64x64_t (unsigned int v)
  {
    _v.hi = v;
    _v.lo = 0;
  }
  inline int64x64_t (unsigned long int v)
  {
    _v.hi = v;
    _v.lo = 0;
  }
  inline int64x64_t (unsigned long long int v)
  {
    _v.hi = v;
    _v.lo = 0;
  }
  inline int64x64_t (int64_t hi, uint64_t lo)
  {
    _v.hi = hi;
    _v.lo = lo;
  }

  inline int64x64_t (const int64x64_t &o)
    : _v (o._v) {}
  inline int64x64_t &operator = (const int64x64_t &o)
  {
    _v = o._v;
    return *this;
  }

  inline double GetDouble (void) const
  {
#define HPCAIRO_MAX_64 18446744073709551615.0
    bool is_negative = IsNegative ();
    cairo_int128_t value = is_negative ? _cairo_int128_negate (_v) : _v;
    double flo = value.lo;
    flo /= HPCAIRO_MAX_64;
    double retval = value.hi;
    retval += flo;
    retval = is_negative ? -retval : retval;
    return retval;
#undef HPCAIRO_MAX_64
  }
  inline int64_t GetHigh (void) const
  {
    return (int64_t)_v.hi;
  }
  inline uint64_t GetLow (void) const
  {
    return _v.lo;
  }

  void MulByInvert (const int64x64_t &o);

  static int64x64_t Invert (uint64_t v);

private:
  friend bool operator == (const int64x64_t &lhs, const int64x64_t &rhs);
  friend bool operator != (const int64x64_t &lhs, const int64x64_t &rhs);
  friend bool operator <= (const int64x64_t &lhs, const int64x64_t &rhs);
  friend bool operator >= (const int64x64_t &lhs, const int64x64_t &rhs);
  friend bool operator < (const int64x64_t &lhs, const int64x64_t &rhs);
  friend bool operator > (const int64x64_t &lhs, const int64x64_t &rhs);
  friend int64x64_t &operator += (int64x64_t &lhs, const int64x64_t &rhs);
  friend int64x64_t &operator -= (int64x64_t &lhs, const int64x64_t &rhs);
  friend int64x64_t &operator *= (int64x64_t &lhs, const int64x64_t &rhs);
  friend int64x64_t &operator /= (int64x64_t &lhs, const int64x64_t &rhs);
  friend int64x64_t operator + (const int64x64_t &lhs);
  friend int64x64_t operator - (const int64x64_t &lhs);
  friend int64x64_t operator ! (const int64x64_t &lhs);
  void Mul (const int64x64_t &o);
  void Div (const int64x64_t &o);
  static cairo_uint128_t  Umul (cairo_uint128_t a, cairo_uint128_t b);
  static cairo_uint128_t Udiv (cairo_uint128_t a, cairo_uint128_t b);
  static cairo_uint128_t UmulByInvert (cairo_uint128_t a, cairo_uint128_t b);
  inline bool IsNegative (void) const
  {
    int64_t hi = _v.hi;
    return hi < 0;
  }
  inline void Negate (void)
  {
    _v.lo = ~_v.lo;
    _v.hi = ~_v.hi;
    if (++_v.lo == 0)
      {
        ++_v.hi;
      }
  }
  inline int Compare (const int64x64_t &o) const
  {
    int status;
    int64x64_t tmp = *this;
    tmp -= o;
    status = (((int64_t)(tmp)._v.hi) < 0) ? -1 :
      (((tmp)._v.hi == 0 && (tmp)._v.lo == 0)) ? 0 : 1;
    return status;
  }
  cairo_int128_t _v;
};

inline bool operator == (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs._v.hi == rhs._v.hi && lhs._v.lo == lhs._v.lo;
}

inline bool operator != (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return !(lhs == rhs);
}

inline bool operator < (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs.Compare (rhs) < 0;
}
inline bool operator <= (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs.Compare (rhs) <= 0;
}

inline bool operator >= (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs.Compare (rhs) >= 0;
}
inline bool operator > (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs.Compare (rhs) > 0;
}
inline int64x64_t &operator += (int64x64_t &lhs, const int64x64_t &rhs)
{
#if Int64x64_CAIRO_ASM
  asm ("mov 0(%1),%%eax\n\t"
       "add %%eax,0(%0)\n\t"
       "mov 4(%1),%%eax\n\t"
       "adc %%eax,4(%0)\n\t"
       "mov 8(%1),%%eax\n\t"
       "adc %%eax,8(%0)\n\t"
       "mov 12(%1),%%eax\n\t"
       "adc %%eax,12(%0)\n\t"
       : 
       : "r" (&lhs._v), "r" (&rhs._v) 
       : "%eax", "cc");
#else
  lhs._v.hi += rhs._v.hi;
  lhs._v.lo += rhs._v.lo;
  if (lhs._v.lo < rhs._v.lo)
    {
      lhs._v.hi++;
    }
#endif
  return lhs;
}
inline int64x64_t &operator -= (int64x64_t &lhs, const int64x64_t &rhs)
{
#if Int64x64_CAIRO_ASM
  asm ("mov 0(%1),%%eax\n\t"
       "sub %%eax,0(%0)\n\t"
       "mov 4(%1),%%eax\n\t"
       "sbb %%eax,4(%0)\n\t"
       "mov 8(%1),%%eax\n\t"
       "sbb %%eax,8(%0)\n\t"
       "mov 12(%1),%%eax\n\t"
       "sbb %%eax,12(%0)\n\t"
       : 
       : "r" (&lhs._v), "r" (&rhs._v) 
       : "%eax", "cc");
#else
  lhs._v.hi -= rhs._v.hi;
  lhs._v.lo -= rhs._v.lo;
  if (lhs._v.lo > rhs._v.lo)
    {
      lhs._v.hi--;
    }
#endif
  return lhs;
}
inline int64x64_t &operator *= (int64x64_t &lhs, const int64x64_t &rhs)
{
  lhs.Mul (rhs);
  return lhs;
}
inline int64x64_t &operator /= (int64x64_t &lhs, const int64x64_t &rhs)
{
  lhs.Div (rhs);
  return lhs;
}

inline int64x64_t operator + (const int64x64_t &lhs)
{
  return lhs;
}

inline int64x64_t operator - (const int64x64_t &lhs)
{
  int64x64_t tmp = lhs;
  tmp.Negate ();
  return tmp;
}

inline int64x64_t operator ! (const int64x64_t &lhs)
{
  return (lhs._v.hi == 0 && lhs._v.lo == 0) ? int64x64_t (1, 0) : int64x64_t ();
}

} // namespace ns3

#endif /* INT64X64_CAIRO_H */
