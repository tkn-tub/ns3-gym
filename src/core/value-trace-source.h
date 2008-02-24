#ifndef VALUE_TRACE_SOURCE_H
#define VALUE_TRACE_SOURCE_H

#include "event-trace-source.h"
#include "integer.h"
#include "uinteger.h"
#include "boolean.h"
#include "double.h"
#include "enum.h"

#define TRACE(x)

namespace ns3 {

template <typename T>
class ValueTraceSource
{
public:
  ValueTraceSource ()
    : m_v () {}
  ValueTraceSource (const ValueTraceSource &o)
    : m_v (o.m_v) {}
  ValueTraceSource (const T &v)
    : m_v (v) {}
  operator T () const {
    return m_v;
  }
  ValueTraceSource &operator = (const ValueTraceSource &o) {
    TRACE ("x=");
    Set (o.m_v);
    return *this;
  }
  ValueTraceSource (const Integer &value) 
    : m_v (value.Get ()) {}
  operator Integer () const {
    return Integer (m_v);
  }
  ValueTraceSource (const Uinteger &value)
    : m_v (value.Get ()) {}
  operator Uinteger () const {
    return Uinteger (m_v);
  }
  ValueTraceSource (const Boolean &value)
    : m_v (value.Get ()) {}
  operator Boolean () const {
    return Boolean (m_v);
  }
  ValueTraceSource (const Enum &value)
    : m_v (value.Get ()) {}
  operator Enum () const {
    return Enum (m_v);
  }
  void Connect (const CallbackBase &cb) {
    m_cb.Connect (cb);
  }
  void Disconnect (const CallbackBase &cb) {
    m_cb.Disconnect (cb);
  }
  void Set (const T &v) {
    if (m_v != v)
      {
	m_cb (m_v, v);
	m_v = v;
      }
  }
  T Get (void) const {
    return m_v;
  }
  ValueTraceSource &operator++ () {
    TRACE ("++x");
    T tmp = Get ();
    ++tmp;
    Set (tmp);
    return *this;
  }
  ValueTraceSource &operator-- () {
    TRACE ("--x");
    T tmp = Get ();
    --tmp;
    Set (tmp);
    return *this;
  }
  ValueTraceSource operator++ (int) {
    TRACE ("x++");
    ValueTraceSource old (*this);
    T tmp = Get ();
    tmp++;
    Set (tmp);
    return old;
  }
  ValueTraceSource operator-- (int) {
    TRACE ("x--");
    ValueTraceSource old (*this);
    T tmp = Get ();
    tmp--;
    Set (tmp);
    return old;
  }
private:
  T m_v;
  EventTraceSource<T,T> m_cb;
};

template <typename T, typename U>
bool operator == (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs)
{
  TRACE ("x==x");
  return lhs.Get () == rhs.Get ();
}
template <typename T, typename U>
bool operator == (const ValueTraceSource<T> &lhs, const U &rhs)
{
  TRACE ("x==");
  return lhs.Get () == rhs;
}
template <typename T, typename U>
bool operator == (const U &lhs, const ValueTraceSource<T> &rhs)
{
  TRACE ("==x");
  return lhs == rhs.Get ();
}

template <typename T, typename U>
bool operator != (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs)
{
  TRACE ("x!=x");
  return lhs.Get () != rhs.Get ();
}
template <typename T, typename U>
bool operator != (const ValueTraceSource<T> &lhs, const U &rhs)
{
  TRACE ("x!=");
  return lhs.Get () != rhs;
}
template <typename T, typename U>
bool operator != (const U &lhs, const ValueTraceSource<T> &rhs)
{
  TRACE ("!=x");
  return lhs != rhs.Get ();
}

template <typename T, typename U>
bool operator <= (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs)
{
  TRACE ("x<=x");
  return lhs.Get () <= rhs.Get ();
}
template <typename T, typename U>
bool operator <= (const ValueTraceSource<T> &lhs, const U &rhs)
{
  TRACE ("x<=");
  return lhs.Get () <= rhs;
}
template <typename T, typename U>
bool operator <= (const U &lhs, const ValueTraceSource<T> &rhs)
{
  TRACE ("<=x");
  return lhs <= rhs.Get ();
}
template <typename T, typename U>
bool operator >= (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs)
{
  TRACE ("x>=x");
  return lhs.Get () >= rhs.Get ();
}
template <typename T, typename U>
bool operator >= (const ValueTraceSource<T> &lhs, const U &rhs)
{
  TRACE ("x>=");
  return lhs.Get () >= rhs;
}
template <typename T, typename U>
bool operator >= (const U &lhs, const ValueTraceSource<T> &rhs)
{
  TRACE (">=x");
  return lhs >= rhs.Get ();
}

template <typename T, typename U>
bool operator < (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs)
{
  TRACE ("x<x");
  return lhs.Get () < rhs.Get ();
}
template <typename T, typename U>
bool operator < (const ValueTraceSource<T> &lhs, const U &rhs)
{
  TRACE ("x<");
  return lhs.Get () < rhs;
}
template <typename T, typename U>
bool operator < (const U &lhs, const ValueTraceSource<T> &rhs)
{
  TRACE ("<x");
  return lhs < rhs.Get ();
}
template <typename T, typename U>
bool operator > (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs)
{
  TRACE ("x>x");
  return lhs.Get () > rhs.Get ();
}
template <typename T, typename U>
bool operator > (const ValueTraceSource<T> &lhs, const U &rhs)
{
  TRACE ("x>");
  return lhs.Get () > rhs;
}
template <typename T, typename U>
bool operator > (const U &lhs, const ValueTraceSource<T> &rhs)
{
  TRACE (">x");
  return lhs > rhs.Get ();
}
template <typename T, typename U>
ValueTraceSource<T> &operator += (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x+=");
  T tmp = lhs.Get ();
  tmp += rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> &operator -= (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x-=");
  T tmp = lhs.Get ();
  tmp -= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> &operator *= (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x*=");
  T tmp = lhs.Get ();
  tmp *= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> &operator /= (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x/=");
  T tmp = lhs.Get ();
  tmp /= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> &operator %= (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x%=");
  T tmp = lhs.Get ();
  tmp %= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> &operator <<= (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x<<=");
  T tmp = lhs.Get ();
  tmp <<= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> &operator >>= (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x>>=");
  T tmp = lhs.Get ();
  tmp >>= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> &operator &= (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x&=");
  T tmp = lhs.Get ();
  tmp &= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> &operator |= (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x|=");
  T tmp = lhs.Get ();
  tmp |= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> &operator ^= (ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x^=");
  T tmp = lhs.Get ();
  tmp ^= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
ValueTraceSource<T> operator + (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x+x");
  return ValueTraceSource<T> (lhs.Get () + rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator + (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x+");
  return ValueTraceSource<T> (lhs.Get () + rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator + (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE ("+x");
  return ValueTraceSource<T> (lhs + rhs.Get ());
}

template <typename T, typename U>
ValueTraceSource<T> operator - (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x-x");
  return ValueTraceSource<T> (lhs.Get () - rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator - (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x-");
  return ValueTraceSource<T> (lhs.Get () - rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator - (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE ("-x");
  return ValueTraceSource<T> (lhs - rhs.Get ());
}

template <typename T, typename U>
ValueTraceSource<T> operator * (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x*x");
  return ValueTraceSource<T> (lhs.Get () * rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator * (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x*");
  return ValueTraceSource<T> (lhs.Get () * rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator * (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE ("*x");
  return ValueTraceSource<T> (lhs - rhs.Get ());
}

template <typename T, typename U>
ValueTraceSource<T> operator / (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x/x");
  return ValueTraceSource<T> (lhs.Get () / rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator / (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x/");
  return ValueTraceSource<T> (lhs.Get () / rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator / (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE ("/x");
  return ValueTraceSource<T> (lhs / rhs.Get ());
}

template <typename T, typename U>
ValueTraceSource<T> operator % (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x%x");
  return ValueTraceSource<T> (lhs.Get () % rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator % (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x%");
  return ValueTraceSource<T> (lhs.Get () % rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator % (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE ("%x");
  return ValueTraceSource<T> (lhs % rhs.Get ());
}

template <typename T, typename U>
ValueTraceSource<T> operator ^ (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x^x");
  return ValueTraceSource<T> (lhs.Get () ^ rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator ^ (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x^");
  return ValueTraceSource<T> (lhs.Get () ^ rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator ^ (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE ("^x");
  return ValueTraceSource<T> (lhs ^ rhs.Get ());
}

template <typename T, typename U>
ValueTraceSource<T> operator | (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x|x");
  return ValueTraceSource<T> (lhs.Get () | rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator | (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x|");
  return ValueTraceSource<T> (lhs.Get () | rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator | (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE ("|x");
  return ValueTraceSource<T> (lhs | rhs.Get ());
}

template <typename T, typename U>
ValueTraceSource<T> operator & (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x&x");
  return ValueTraceSource<T> (lhs.Get () & rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator & (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x&");
  return ValueTraceSource<T> (lhs.Get () & rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator & (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE ("&x");
  return ValueTraceSource<T> (lhs & rhs.Get ());
}

template <typename T, typename U>
ValueTraceSource<T> operator << (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x<<x");
  return ValueTraceSource<T> (lhs.Get () << rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator << (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x<<");
  return ValueTraceSource<T> (lhs.Get () << rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator << (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE ("<<x");
  return ValueTraceSource<T> (lhs << rhs.Get ());
}

template <typename T, typename U>
ValueTraceSource<T> operator >> (const ValueTraceSource<T> &lhs, const ValueTraceSource<U> &rhs) {
  TRACE ("x>>x");
  return ValueTraceSource<T> (lhs.Get () >> rhs.Get ());
}
template <typename T, typename U>
ValueTraceSource<T> operator >> (const ValueTraceSource<T> &lhs, const U &rhs) {
  TRACE ("x>>");
  return ValueTraceSource<T> (lhs.Get () >> rhs);
}
template <typename T, typename U>
ValueTraceSource<T> operator >> (const U &lhs, const ValueTraceSource<T> &rhs) {
  TRACE (">>x");
  return ValueTraceSource<T> (lhs >> rhs.Get ());
}


template <typename T>
ValueTraceSource<T> operator + (const ValueTraceSource<T> &lhs) {
  TRACE ("(+x)");
  return ValueTraceSource<T> (+lhs.Get ());
}
template <typename T>
ValueTraceSource<T> operator - (const ValueTraceSource<T> &lhs) {
  TRACE ("(-x)");
  return ValueTraceSource<T> (-lhs.Get ());
}
template <typename T>
ValueTraceSource<T> operator ~ (const ValueTraceSource<T> &lhs) {
  TRACE ("(~x)");
  return ValueTraceSource<T> (~lhs.Get ());
}
template <typename T>
ValueTraceSource<T> operator ! (const ValueTraceSource<T> &lhs) {
  TRACE ("(!x)");
  return ValueTraceSource<T> (!lhs.Get ());
}


} // namespace ns3

#endif /* VALUE_TRACE_SOURCE_H */
