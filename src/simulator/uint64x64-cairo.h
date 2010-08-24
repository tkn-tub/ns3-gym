#ifndef UINT64X64_CAIRO_H
#define UINT64X64_CAIRO_H

#include <stdint.h>
#include <math.h>
#include "cairo-wideint-private.h"

#ifdef __i386__
#define UINT64x64_CAIRO_ASM 1
#endif

namespace ns3 {

class uint64x64_t
{
public:
  inline uint64x64_t ()
  {
    _v.hi = 0;
    _v.lo = 0;
  }
  explicit inline uint64x64_t (double value)
  {
#define HPCAIRO_MAX_64 18446744073709551615.0
    double fhi = floor (value);
    int64_t hi = fhi;
    uint64_t lo = (uint64_t) ((value - fhi) * HPCAIRO_MAX_64);
    _v.hi = hi;
    _v.lo = lo;
#undef HPCAIRO_MAX_64
  }
  explicit inline uint64x64_t (int v)
  {
    _v.hi = v;
    _v.lo = 0;
  }
  explicit inline uint64x64_t (long int v)
  {
    _v.hi = v;
    _v.lo = 0;
  }
  explicit inline uint64x64_t (long long int v)
  {
    _v.hi = v;
    _v.lo = 0;
  }
  explicit inline uint64x64_t (int64_t hi, uint64_t lo)
  {
    _v.hi = hi;
    _v.lo = lo;
  }

  inline uint64x64_t (const uint64x64_t &o)
    : _v (o._v) {}
  inline uint64x64_t &operator = (const uint64x64_t &o)
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
    retval = is_negative ? -retval: retval;
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

  void MulByInvert (const uint64x64_t &o);

  static uint64x64_t Invert (uint64_t v);

private:
  friend bool operator == (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend bool operator != (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend bool operator <= (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend bool operator >= (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend bool operator < (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend bool operator > (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend uint64x64_t &operator += (uint64x64_t &lhs, const uint64x64_t &rhs);
  friend uint64x64_t &operator -= (uint64x64_t &lhs, const uint64x64_t &rhs);
  friend uint64x64_t &operator *= (uint64x64_t &lhs, const uint64x64_t &rhs);
  friend uint64x64_t &operator /= (uint64x64_t &lhs, const uint64x64_t &rhs);
  friend uint64x64_t operator + (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend uint64x64_t operator - (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend uint64x64_t operator * (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend uint64x64_t operator / (const uint64x64_t &lhs, const uint64x64_t &rhs);
  friend uint64x64_t operator + (const uint64x64_t &lhs);
  friend uint64x64_t operator - (const uint64x64_t &lhs);
  friend uint64x64_t operator ! (const uint64x64_t &lhs);
  void Mul (const uint64x64_t &o);
  void Div (const uint64x64_t &o);
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

  cairo_int128_t _v;
};

#if defined(UINT64x64_CAIRO_ASM)
#define xCMP_ls "jl"
#define xCMP_le "jle"
#define COMPARE(a,b,op)				\
  ({						\
    int status;					\
    asm ("mov 0(%1),%%eax\n\t"			\
	 "add 0(%2),%%eax\n\t"			\
	 "mov 4(%1),%%eax\n\t"			\
	 "adc 4(%2),%%eax\n\t"			\
	 "mov 8(%1),%%eax\n\t"			\
	 "adc 8(%2),%%eax\n\t"			\
	 "mov 12(%1),%%eax\n\t"			\
	 "adc 12(%2),%%eax\n\t"			\
	 x##op " 1f\n\t"			\
	 "mov $0,%0\n\t"				\
	 "ja 2f\n"				\
	 "1:\tmov $1,%0\n\t"			\
	 "2:"					\
	 : "=r" (status)			\
	 : "r" (&a._v), "r" (&b._v)		\
	 : "%eax", "cc");			\
    bool retval = status == 1;			\
    retval;					\
  })
#else
#define xCMP_ls(a) (((int64_t)(a)._v.hi) < 0)
#define xCMP_le(a) ((((int64_t)(a)._v.hi) < 0) || ((a)._v.hi == 0 && (a)._v.lo == 0))
#define COMPARE(a,b,op)							\
  ({									\
    uint64x64_t tmp = a;						\
    tmp -= b;								\
    bool result = x##op(tmp);						\
    result;								\
  }) 
#endif

inline bool operator == (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return lhs._v.hi == rhs._v.hi && lhs._v.lo == lhs._v.lo;
}

inline bool operator != (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return ! (lhs == rhs);
}

inline bool operator < (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return COMPARE(lhs,rhs,CMP_ls);
}
inline bool operator <= (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return COMPARE(lhs,rhs,CMP_le);
}

inline bool operator >= (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return !(lhs < rhs);
}
inline bool operator > (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return !(lhs <= rhs);
}
inline uint64x64_t &operator += (uint64x64_t &lhs, const uint64x64_t &rhs)
{
#if UINT64x64_CAIRO_ASM
  asm ("mov 0(%0),%%eax\n\t"
       "add 0(%1),%%eax\n\t"
       "mov %%eax,0(%0)\n\t"
       "mov 4(%0),%%eax\n\t"
       "adc 4(%1),%%eax\n\t"
       "mov %%eax,4(%0)\n\t"
       "mov 8(%0),%%eax\n\t"
       "adc 8(%1),%%eax\n\t"
       "mov %%eax,8(%0)\n\t"
       "mov 12(%0),%%eax\n\t"
       "adc 12(%1),%%eax\n\t"
       "mov %%eax,12(%0)\n\t"
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
inline uint64x64_t &operator -= (uint64x64_t &lhs, const uint64x64_t &rhs)
{
#if UINT64x64_CAIRO_ASM
  asm ("mov 0(%0),%%eax\n\t"
       "sub 0(%1),%%eax\n\t"
       "mov %%eax,0(%0)\n\t"
       "mov 4(%0),%%eax\n\t"
       "sbb 4(%1),%%eax\n\t"
       "mov %%eax,4(%0)\n\t"
       "mov 8(%0),%%eax\n\t"
       "sbb 8(%1),%%eax\n\t"
       "mov %%eax,8(%0)\n\t"
       "mov 12(%0),%%eax\n\t"
       "sbb 12(%1),%%eax\n\t"
       "mov %%eax,12(%0)\n\t"
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
inline uint64x64_t &operator *= (uint64x64_t &lhs, const uint64x64_t &rhs)
{
  lhs.Mul (rhs);
  return lhs;
}
inline uint64x64_t &operator /= (uint64x64_t &lhs, const uint64x64_t &rhs)
{
  lhs.Div (rhs);
  return lhs;
}

inline uint64x64_t operator + (const uint64x64_t &lhs)
{
  return lhs;
}

inline uint64x64_t operator - (const uint64x64_t &lhs)
{
  uint64x64_t tmp = lhs;
  tmp.Negate ();
  return tmp;
}

inline uint64x64_t operator ! (const uint64x64_t &lhs)
{
  return (lhs._v.hi == 0 && lhs._v.lo == 0)?uint64x64_t (1, 0):uint64x64_t ();
}

} // namespace ns3

#endif /* UINT64X64_CAIRO_H */
