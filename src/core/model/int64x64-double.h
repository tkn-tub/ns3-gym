#include "ns3/core-config.h"
#if !defined(INT64X64_DOUBLE_H) && (defined (INT64X64_USE_DOUBLE) || defined(PYTHON_SCAN))
#define INT64X64_DOUBLE_H

#include <iostream>
#include <cmath>

namespace ns3 {

class int64x64_t
{
public:
  inline int64x64_t ()
    : _v (0) {}
  inline int64x64_t (double v)
    : _v (v) {}
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
  inline int64x64_t (int64_t hi, uint64_t lo)
    : _v (hi) { /** \todo add in lo? */}

  inline int64x64_t (const int64x64_t &o)
    : _v (o._v) {}
  inline int64x64_t &operator = (const int64x64_t &o)
  {
    _v = o._v;
    return *this;
  }

  inline double GetDouble (void) const
  {
    return _v;
  }
  inline int64_t GetHigh (void) const
  {
    return (int64_t)std::floor (_v);
  }
  inline uint64_t GetLow (void) const
  {
    /// \todo Generate lo?
    return 0;
  }

  inline void MulByInvert (const int64x64_t &o)
  {
    _v *= o._v;
  }

  static inline int64x64_t Invert (uint64_t v)
  {
    double d = v;
    return int64x64_t (1/d);
  }

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
  friend int64x64_t operator + (const int64x64_t &lhs, const int64x64_t &rhs);
  friend int64x64_t operator - (const int64x64_t &lhs, const int64x64_t &rhs);
  friend int64x64_t operator * (const int64x64_t &lhs, const int64x64_t &rhs);
  friend int64x64_t operator / (const int64x64_t &lhs, const int64x64_t &rhs);
  friend int64x64_t operator + (const int64x64_t &lhs);
  friend int64x64_t operator - (const int64x64_t &lhs);
  friend int64x64_t operator ! (const int64x64_t &lhs);

  double _v;
};

inline bool operator == (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs._v == rhs._v;
}

inline bool operator != (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs._v != rhs._v;
}

inline bool operator <= (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs._v <= rhs._v;
}

inline bool operator >= (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs._v >= rhs._v;
}
inline bool operator < (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs._v < rhs._v;
}
inline bool operator > (const int64x64_t &lhs, const int64x64_t &rhs)
{
  return lhs._v > rhs._v;
}
inline int64x64_t &operator += (int64x64_t &lhs, const int64x64_t &rhs)
{
  double tmp = lhs._v;
  tmp += rhs._v;
  lhs = int64x64_t (tmp);
  return lhs;
}
inline int64x64_t &operator -= (int64x64_t &lhs, const int64x64_t &rhs)
{
  double tmp = lhs._v;
  tmp -= rhs._v;
  lhs = int64x64_t (tmp);
  return lhs;
}
inline int64x64_t &operator *= (int64x64_t &lhs, const int64x64_t &rhs)
{
  double tmp = lhs._v;
  tmp *= rhs._v;
  lhs = int64x64_t (tmp);
  return lhs;
}
inline int64x64_t &operator /= (int64x64_t &lhs, const int64x64_t &rhs)
{
  double tmp = lhs._v;
  tmp /= rhs._v;
  lhs = int64x64_t (tmp);
  return lhs;
}

inline int64x64_t operator + (const int64x64_t &lhs)
{
  return lhs;
}

inline int64x64_t operator - (const int64x64_t &lhs)
{
  return int64x64_t (-lhs._v);
}

inline int64x64_t operator ! (const int64x64_t &lhs)
{
  return int64x64_t (!lhs._v);
}

} // namespace ns3

#endif /* INT64X64_DOUBLE_H */
