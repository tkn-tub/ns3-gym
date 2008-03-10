#ifndef TRACED_VALUE_H
#define TRACED_VALUE_H

#include "traced-callback.h"
#include "integer.h"
#include "uinteger.h"
#include "boolean.h"
#include "double.h"
#include "enum.h"

#define TRACED_VALUE_DEBUG(x)

namespace ns3 {

/**
 * \brief trace classes with value semantics
 *
 * If you want to trace the change of value of a class or
 * primitive type which have value semantics (they _must_
 * support operator !=), you can wrap them in an instance of
 * this template: this instance will behave just like
 * the original class (if it did not export any special method),
 * and will define Connect/Disconnect methods to work
 * with an ns3::TraceSourceAccessor.
 */
template <typename T>
class TracedValue
{
public:
  TracedValue ()
    : m_v () {}
  TracedValue (const TracedValue &o)
    : m_v (o.m_v) {}
  TracedValue (const T &v)
    : m_v (v) {}
  operator T () const {
    return m_v;
  }
  TracedValue &operator = (const TracedValue &o) {
    TRACED_VALUE_DEBUG ("x=");
    Set (o.m_v);
    return *this;
  }
  TracedValue (const Integer &value) 
    : m_v (value.Get ()) {}
  operator Integer () const {
    return Integer (m_v);
  }
  TracedValue (const Uinteger &value)
    : m_v (value.Get ()) {}
  operator Uinteger () const {
    return Uinteger (m_v);
  }
  TracedValue (const Boolean &value)
    : m_v (value.Get ()) {}
  operator Boolean () const {
    return Boolean (m_v);
  }
  TracedValue (const Enum &value)
    : m_v (value.Get ()) {}
  operator Enum () const {
    return Enum (m_v);
  }
  void Connect (const CallbackBase &cb) {
    m_cb.Connect (cb);
  }
  void ConnectWithContext (const CallbackBase &cb, std::string path) {
    m_cb.ConnectWithContext (cb, path);
  }
  void Disconnect (const CallbackBase &cb) {
    m_cb.Disconnect (cb);
  }
  void DisconnectWithContext (const CallbackBase &cb, std::string path) {
    m_cb.DisconnectWithContext (cb, path);
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
  TracedValue &operator++ () {
    TRACED_VALUE_DEBUG ("++x");
    T tmp = Get ();
    ++tmp;
    Set (tmp);
    return *this;
  }
  TracedValue &operator-- () {
    TRACED_VALUE_DEBUG ("--x");
    T tmp = Get ();
    --tmp;
    Set (tmp);
    return *this;
  }
  TracedValue operator++ (int) {
    TRACED_VALUE_DEBUG ("x++");
    TracedValue old (*this);
    T tmp = Get ();
    tmp++;
    Set (tmp);
    return old;
  }
  TracedValue operator-- (int) {
    TRACED_VALUE_DEBUG ("x--");
    TracedValue old (*this);
    T tmp = Get ();
    tmp--;
    Set (tmp);
    return old;
  }
private:
  T m_v;
  TracedCallback<T,T> m_cb;
};

template <typename T, typename U>
bool operator == (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x==x");
  return lhs.Get () == rhs.Get ();
}
template <typename T, typename U>
bool operator == (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x==");
  return lhs.Get () == rhs;
}
template <typename T, typename U>
bool operator == (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG ("==x");
  return lhs == rhs.Get ();
}

template <typename T, typename U>
bool operator != (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x!=x");
  return lhs.Get () != rhs.Get ();
}
template <typename T, typename U>
bool operator != (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x!=");
  return lhs.Get () != rhs;
}
template <typename T, typename U>
bool operator != (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG ("!=x");
  return lhs != rhs.Get ();
}

template <typename T, typename U>
bool operator <= (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x<=x");
  return lhs.Get () <= rhs.Get ();
}
template <typename T, typename U>
bool operator <= (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x<=");
  return lhs.Get () <= rhs;
}
template <typename T, typename U>
bool operator <= (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG ("<=x");
  return lhs <= rhs.Get ();
}
template <typename T, typename U>
bool operator >= (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x>=x");
  return lhs.Get () >= rhs.Get ();
}
template <typename T, typename U>
bool operator >= (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x>=");
  return lhs.Get () >= rhs;
}
template <typename T, typename U>
bool operator >= (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG (">=x");
  return lhs >= rhs.Get ();
}

template <typename T, typename U>
bool operator < (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x<x");
  return lhs.Get () < rhs.Get ();
}
template <typename T, typename U>
bool operator < (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x<");
  return lhs.Get () < rhs;
}
template <typename T, typename U>
bool operator < (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG ("<x");
  return lhs < rhs.Get ();
}
template <typename T, typename U>
bool operator > (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x>x");
  return lhs.Get () > rhs.Get ();
}
template <typename T, typename U>
bool operator > (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x>");
  return lhs.Get () > rhs;
}
template <typename T, typename U>
bool operator > (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG (">x");
  return lhs > rhs.Get ();
}
template <typename T, typename U>
TracedValue<T> &operator += (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x+=");
  T tmp = lhs.Get ();
  tmp += rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> &operator -= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x-=");
  T tmp = lhs.Get ();
  tmp -= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> &operator *= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x*=");
  T tmp = lhs.Get ();
  tmp *= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> &operator /= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x/=");
  T tmp = lhs.Get ();
  tmp /= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> &operator %= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x%=");
  T tmp = lhs.Get ();
  tmp %= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> &operator <<= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x<<=");
  T tmp = lhs.Get ();
  tmp <<= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> &operator >>= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x>>=");
  T tmp = lhs.Get ();
  tmp >>= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> &operator &= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x&=");
  T tmp = lhs.Get ();
  tmp &= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> &operator |= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x|=");
  T tmp = lhs.Get ();
  tmp |= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> &operator ^= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x^=");
  T tmp = lhs.Get ();
  tmp ^= rhs;
  lhs.Set (tmp);
  return lhs;
}
template <typename T, typename U>
TracedValue<T> operator + (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x+x");
  return TracedValue<T> (lhs.Get () + rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator + (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x+");
  return TracedValue<T> (lhs.Get () + rhs);
}
template <typename T, typename U>
TracedValue<T> operator + (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("+x");
  return TracedValue<T> (lhs + rhs.Get ());
}

template <typename T, typename U>
TracedValue<T> operator - (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x-x");
  return TracedValue<T> (lhs.Get () - rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator - (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x-");
  return TracedValue<T> (lhs.Get () - rhs);
}
template <typename T, typename U>
TracedValue<T> operator - (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("-x");
  return TracedValue<T> (lhs - rhs.Get ());
}

template <typename T, typename U>
TracedValue<T> operator * (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x*x");
  return TracedValue<T> (lhs.Get () * rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator * (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x*");
  return TracedValue<T> (lhs.Get () * rhs);
}
template <typename T, typename U>
TracedValue<T> operator * (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("*x");
  return TracedValue<T> (lhs - rhs.Get ());
}

template <typename T, typename U>
TracedValue<T> operator / (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x/x");
  return TracedValue<T> (lhs.Get () / rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator / (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x/");
  return TracedValue<T> (lhs.Get () / rhs);
}
template <typename T, typename U>
TracedValue<T> operator / (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("/x");
  return TracedValue<T> (lhs / rhs.Get ());
}

template <typename T, typename U>
TracedValue<T> operator % (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x%x");
  return TracedValue<T> (lhs.Get () % rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator % (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x%");
  return TracedValue<T> (lhs.Get () % rhs);
}
template <typename T, typename U>
TracedValue<T> operator % (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("%x");
  return TracedValue<T> (lhs % rhs.Get ());
}

template <typename T, typename U>
TracedValue<T> operator ^ (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x^x");
  return TracedValue<T> (lhs.Get () ^ rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator ^ (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x^");
  return TracedValue<T> (lhs.Get () ^ rhs);
}
template <typename T, typename U>
TracedValue<T> operator ^ (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("^x");
  return TracedValue<T> (lhs ^ rhs.Get ());
}

template <typename T, typename U>
TracedValue<T> operator | (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x|x");
  return TracedValue<T> (lhs.Get () | rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator | (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x|");
  return TracedValue<T> (lhs.Get () | rhs);
}
template <typename T, typename U>
TracedValue<T> operator | (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("|x");
  return TracedValue<T> (lhs | rhs.Get ());
}

template <typename T, typename U>
TracedValue<T> operator & (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x&x");
  return TracedValue<T> (lhs.Get () & rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator & (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x&");
  return TracedValue<T> (lhs.Get () & rhs);
}
template <typename T, typename U>
TracedValue<T> operator & (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("&x");
  return TracedValue<T> (lhs & rhs.Get ());
}

template <typename T, typename U>
TracedValue<T> operator << (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x<<x");
  return TracedValue<T> (lhs.Get () << rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator << (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x<<");
  return TracedValue<T> (lhs.Get () << rhs);
}
template <typename T, typename U>
TracedValue<T> operator << (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("<<x");
  return TracedValue<T> (lhs << rhs.Get ());
}

template <typename T, typename U>
TracedValue<T> operator >> (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x>>x");
  return TracedValue<T> (lhs.Get () >> rhs.Get ());
}
template <typename T, typename U>
TracedValue<T> operator >> (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x>>");
  return TracedValue<T> (lhs.Get () >> rhs);
}
template <typename T, typename U>
TracedValue<T> operator >> (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG (">>x");
  return TracedValue<T> (lhs >> rhs.Get ());
}


template <typename T>
TracedValue<T> operator + (const TracedValue<T> &lhs) {
  TRACED_VALUE_DEBUG ("(+x)");
  return TracedValue<T> (+lhs.Get ());
}
template <typename T>
TracedValue<T> operator - (const TracedValue<T> &lhs) {
  TRACED_VALUE_DEBUG ("(-x)");
  return TracedValue<T> (-lhs.Get ());
}
template <typename T>
TracedValue<T> operator ~ (const TracedValue<T> &lhs) {
  TRACED_VALUE_DEBUG ("(~x)");
  return TracedValue<T> (~lhs.Get ());
}
template <typename T>
TracedValue<T> operator ! (const TracedValue<T> &lhs) {
  TRACED_VALUE_DEBUG ("(!x)");
  return TracedValue<T> (!lhs.Get ());
}


} // namespace ns3

#endif /* TRACED_VALUE_H */
