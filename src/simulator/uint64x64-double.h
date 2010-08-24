#ifndef UINT64X64_DOUBLE_H
#define UINT64X64_DOUBLE_H

#include <iostream>
#include <math.h>

namespace ns3 {

class uint64x64_t
{
public:
  inline uint64x64_t ()
    : _v (0) {}
  explicit inline uint64x64_t (double v)
    : _v (v) {}
  explicit inline uint64x64_t (int v)
    : _v (v) {}
  explicit inline uint64x64_t (long int v)
    : _v (v) {}
  explicit inline uint64x64_t (long long int v)
    : _v (v) {}
  inline uint64x64_t (int64_t hi, uint64_t lo)
    : _v (hi) {/* XXX */}

  inline uint64x64_t (const uint64x64_t &o)
    : _v (o._v) {}
  inline uint64x64_t &operator = (const uint64x64_t &o)
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
    return (int64_t)floor (_v);
  }
  inline uint64_t GetLow (void) const
  {
    // XXX
    return 0;
  }

  inline void MulByInvert (const uint64x64_t &o)
  {
    _v *= o._v;
  }

  static inline uint64x64_t Invert (uint64_t v)
  {
    double d = v;
    return uint64x64_t (1/d);
  }

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

  double _v;
};

inline bool operator == (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return lhs._v == rhs._v;
}

inline bool operator != (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return lhs._v != rhs._v;
}

inline bool operator <= (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return lhs._v <= rhs._v;
}

inline bool operator >= (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return lhs._v >= rhs._v;
}
inline bool operator < (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return lhs._v < rhs._v;
}
inline bool operator > (const uint64x64_t &lhs, const uint64x64_t &rhs)
{
  return lhs._v > rhs._v;
}
inline uint64x64_t &operator += (uint64x64_t &lhs, const uint64x64_t &rhs)
{
  double tmp = lhs._v;
  tmp += rhs._v;
  lhs = uint64x64_t (tmp);
  return lhs;
}
inline uint64x64_t &operator -= (uint64x64_t &lhs, const uint64x64_t &rhs)
{
  double tmp = lhs._v;
  tmp -= rhs._v;
  lhs = uint64x64_t (tmp);
  return lhs;
}
inline uint64x64_t &operator *= (uint64x64_t &lhs, const uint64x64_t &rhs)
{
  double tmp = lhs._v;
  tmp *= rhs._v;
  lhs = uint64x64_t (tmp);
  return lhs;
}
inline uint64x64_t &operator /= (uint64x64_t &lhs, const uint64x64_t &rhs)
{
  double tmp = lhs._v;
  tmp /= rhs._v;
  lhs = uint64x64_t (tmp);
  return lhs;
}

inline uint64x64_t operator + (const uint64x64_t &lhs)
{
  return lhs;
}

inline uint64x64_t operator - (const uint64x64_t &lhs)
{
  return uint64x64_t (-lhs._v);
}

inline uint64x64_t operator ! (const uint64x64_t &lhs)
{
  return uint64x64_t (!lhs._v);
}

} // namespace ns3

#endif /* UINT64X64_DOUBLE_H */
