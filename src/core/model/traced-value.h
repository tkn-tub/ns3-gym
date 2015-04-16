/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef TRACED_VALUE_H
#define TRACED_VALUE_H

#include "traced-callback.h"
#include "integer.h"
#include "uinteger.h"
#include "boolean.h"
#include "double.h"
#include "enum.h"

/**
 * \file
 * \ingroup tracing
 * ns3::TracedValue declaration and template implementation.
 */


/**
 * Logging macro for TracedValue.
 *
 * No NS_LOG_... here.  When logging is needed use something like
 * \code
 *   #define TRACED_VALUE_DEBUG(x)                  \
 *     std::cout << __FILE__ << ":" << __FUNCTION__    \
 *               << "(" << __LINE__ << ") "           \
 *               << x << std::endl
 * \endcode
*/
#define TRACED_VALUE_DEBUG(x)

namespace ns3 {

/**
 * \ingroup core
 * \defgroup tracing Tracing
 * \brief Publish/subscribe tools to collect and report changes to any
 *        values used by the various model components.
 *
 * Additional callback function signatures defined elsewhere:
 *   - Time::TracedValueCallback
 *   - ns3::SequenceNumber32TracedValueCallback
 */

/**
 * \ingroup tracing
 *
 * \brief Trace classes with value semantics
 *
 * If you want to trace the change of value of a class or
 * primitive type which have value semantics (they _must_
 * support operator !=), you can wrap them in an instance of
 * this template.  This instance will behave just like
 * the original class (if it did not export any special method),
 * and will define Connect/DisconnectWithoutContext methods to work
 * with MakeTraceSourceAccessor.
 *
 * \tparam T The type of the underlying value being traced.
 */
template <typename T>
class TracedValue
{
public:
  /** Default constructor. */
  TracedValue ()
    : m_v () {}
  /**
   * Copy constructor.
   * \param o The value to copy.
   */
  TracedValue (const TracedValue &o)
    : m_v (o.m_v) {}
  /**
   * Construct from an explicit variable.
   * \param v The variable to trace.
   */
  TracedValue (const T &v)
    : m_v (v) {}
  /**
   * Cast to the underlying type.
   * \returns The underlying value.
   */
  operator T () const {
    return m_v;
  }
  /**
   * Assignment.
   * \param o The value to assign to this instance.
   * \return This TracedValue.
   */
  TracedValue &operator = (const TracedValue &o) {
    TRACED_VALUE_DEBUG ("x=");
    Set (o.m_v);
    return *this;
  }
  /**
   * Copy from a TracedValue of a compatable type.
   * \tparam U The underlying type of the other TracedValue.
   * \param other The other TracedValuet to copy.
   */
  template <typename U>
  TracedValue (const TracedValue<U> &other)
    : m_v (other.m_v)
  {}
  /**
   * Copy from a variable type compatible with this underlying type.
   * \tparam U Type of the other variable.
   * \param other The other variable to copy.
   */
  template <typename U>
  TracedValue (const U &other)
    : m_v ((T)other)
  {}
  /**
   * Connect a Callback (without context.)
   *
   * \param cb The callback to connect.
   */
  void ConnectWithoutContext (const CallbackBase &cb) {
    m_cb.ConnectWithoutContext (cb);
  }
  /**
   * Connect a Callback with a context string.
   *
   * The context string will be provided as the first argument to the
   * Callback function.
   *
   * \param cb The Callback to connect to the target trace source.
   * \param path The context to bind to the user callback.
   */
  void Connect (const CallbackBase &cb, std::string path) {
    m_cb.Connect (cb, path);
  }
  /**
   * Disconnect a Callback which was connected without context.
   *
   * \param cb The Callback to disconnect.
   */
  void DisconnectWithoutContext (const CallbackBase &cb) {
    m_cb.DisconnectWithoutContext (cb);
  }
  /**
   * Disconnect a Callback which was connected with context.
   *
   * \param cb The Callback to disconnect.
   * \param path The context to bind to the user callback.
   */
  void Disconnect (const CallbackBase &cb, std::string path) {
    m_cb.Disconnect (cb, path);
  }
  /**
   * Set the value of the underlying variable.
   *
   * If the new value differs from the old, the Callback will be invoked.
   * \param v The new value.
   */
  void Set (const T &v) {
    if (m_v != v)
      {
        m_cb (m_v, v);
        m_v = v;
      }
  }
  /**
   * Get the underlying value.
   * \returns The value.
   */
  T Get (void) const {
    return m_v;
  }
  /**
   * Pre/post- increment/decrement operator.
   *
   * This invokes the Callback.
   * \returns This TracedValue.
   */
  /**@{*/
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
  /**@}*/

  /**
   *  TracedValue Callback signature for POD.
   *
   * \param [in] oldValue original value of the traced variable
   * \param [in] newValue new value of the traced variable
   * @{
   */
  typedef void (* BoolCallback)  (const bool     oldValue, const bool     newValue);
  typedef void (* Int8Callback)  (const int8_t   oldValue, const int8_t   newValue);
  typedef void (* Uint8Callback) (const uint8_t  oldValue, const uint8_t  newValue);
  typedef void (* Int16Callback) (const int16_t  oldValue, const int16_t  newValue);
  typedef void (* Uint16Callback)(const uint16_t oldValue, const uint16_t newValue);
  typedef void (* Int32Callback) (const int32_t  oldValue, const int32_t  newValue);
  typedef void (* Uint32Callback)(const uint32_t oldValue, const uint32_t newValue);
  typedef void (* DoubleCallback)(const double   oldValue, const double   newValue);
  /**@}*/

private:
  /** The underlying value. */
  T m_v;
  /** The connected Callback. */
  TracedCallback<T,T> m_cb;
};

  
/********************************************************************
   Operator implementations for TracedValue
 ********************************************************************/

/**
 * \ingroup tracing
 */
/**@{*/
/**
 * Output streamer for TracedValue.
 *
 * The underlying value will be written to the stream.
 *
 * \tparam T The underlying type of the TracedValue.
 * \param os The output stream.
 * \param rhs The TracedValue to stream.
 * \returns The stream.
 */
template <typename T>
std::ostream& operator << (std::ostream& os, const TracedValue<T>& rhs)
{
  return os<<rhs.Get ();
}

/**
 * Boolean operator for TracedValue.
 * \tparam T The underlying type held by the left-hand argument.
 * \tparam U The underlying type held by the right-hand argument.
 * \param lhs The left-hand argument.
 * \param rhs The right-hand argument.
 * \returns The boolean result of comparing the underlying values.
 */
template <typename T, typename U>
bool operator == (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x==x");
  return lhs.Get () == rhs.Get ();
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator == (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x==");
  return lhs.Get () == rhs;
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator == (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG ("==x");
  return lhs == rhs.Get ();
}

/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator != (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x!=x");
  return lhs.Get () != rhs.Get ();
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator != (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x!=");
  return lhs.Get () != rhs;
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator != (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG ("!=x");
  return lhs != rhs.Get ();
}

/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator <= (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x<=x");
  return lhs.Get () <= rhs.Get ();
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator <= (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x<=");
  return lhs.Get () <= rhs;
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator <= (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG ("<=x");
  return lhs <= rhs.Get ();
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator >= (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x>=x");
  return lhs.Get () >= rhs.Get ();
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator >= (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x>=");
  return lhs.Get () >= rhs;
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator >= (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG (">=x");
  return lhs >= rhs.Get ();
}

/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator < (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x<x");
  return lhs.Get () < rhs.Get ();
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator < (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x<");
  return lhs.Get () < rhs;
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator < (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG ("<x");
  return lhs < rhs.Get ();
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator > (const TracedValue<T> &lhs, const TracedValue<U> &rhs)
{
  TRACED_VALUE_DEBUG ("x>x");
  return lhs.Get () > rhs.Get ();
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator > (const TracedValue<T> &lhs, const U &rhs)
{
  TRACED_VALUE_DEBUG ("x>");
  return lhs.Get () > rhs;
}
/** \copydoc operator==(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
bool operator > (const U &lhs, const TracedValue<T> &rhs)
{
  TRACED_VALUE_DEBUG (">x");
  return lhs > rhs.Get ();
}

  
/**
 * Infix arithmetic operator for TracedValue.
 *
 * This returns the arithmetic result in a new TracedValue,
 * which has no Callback connected.
 *
 * \tparam T The underlying type held by the left-hand argument.
 * \tparam U The underlying type held by the right-hand argument.
 * \param lhs The left-hand argument.
 * \param rhs The right-hand argument.
 * \returns The result of doing the operator on
 *     the underlying values.
 */
template <typename T, typename U>
TracedValue<T> operator + (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x+x");
  return TracedValue<T> (lhs.Get () + rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator + (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x+");
  return TracedValue<T> (lhs.Get () + rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator + (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("+x");
  return TracedValue<T> (lhs + rhs.Get ());
}

/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator - (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x-x");
  return TracedValue<T> (lhs.Get () - rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator - (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x-");
  return TracedValue<T> (lhs.Get () - rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator - (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("-x");
  return TracedValue<T> (lhs - rhs.Get ());
}

/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator * (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x*x");
  return TracedValue<T> (lhs.Get () * rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator * (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x*");
  return TracedValue<T> (lhs.Get () * rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator * (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("*x");
  return TracedValue<T> (lhs * rhs.Get ());
}

/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator / (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x/x");
  return TracedValue<T> (lhs.Get () / rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator / (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x/");
  return TracedValue<T> (lhs.Get () / rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator / (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("/x");
  return TracedValue<T> (lhs / rhs.Get ());
}

/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator % (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x%x");
  return TracedValue<T> (lhs.Get () % rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator % (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x%");
  return TracedValue<T> (lhs.Get () % rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator % (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("%x");
  return TracedValue<T> (lhs % rhs.Get ());
}

/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator ^ (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x^x");
  return TracedValue<T> (lhs.Get () ^ rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator ^ (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x^");
  return TracedValue<T> (lhs.Get () ^ rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator ^ (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("^x");
  return TracedValue<T> (lhs ^ rhs.Get ());
}

/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator | (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x|x");
  return TracedValue<T> (lhs.Get () | rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator | (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x|");
  return TracedValue<T> (lhs.Get () | rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator | (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("|x");
  return TracedValue<T> (lhs | rhs.Get ());
}

/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator & (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x&x");
  return TracedValue<T> (lhs.Get () & rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator & (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x&");
  return TracedValue<T> (lhs.Get () & rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator & (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("&x");
  return TracedValue<T> (lhs & rhs.Get ());
}

/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator << (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x<<x");
  return TracedValue<T> (lhs.Get () << rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator << (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x<<");
  return TracedValue<T> (lhs.Get () << rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator << (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG ("<<x");
  return TracedValue<T> (lhs << rhs.Get ());
}

/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator >> (const TracedValue<T> &lhs, const TracedValue<U> &rhs) {
  TRACED_VALUE_DEBUG ("x>>x");
  return TracedValue<T> (lhs.Get () >> rhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator >> (const TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x>>");
  return TracedValue<T> (lhs.Get () >> rhs);
}
/** \copydoc operator+(const TracedValue<T>&lhs,const TracedValue<U>&rhs) */
template <typename T, typename U>
TracedValue<T> operator >> (const U &lhs, const TracedValue<T> &rhs) {
  TRACED_VALUE_DEBUG (">>x");
  return TracedValue<T> (lhs >> rhs.Get ());
}

/**
 * Operator assignment for TracedValue.
 *
 * The result of the arithmetic operation on the underlying values
 * is assigned to the \c lhs TracedValue.  If the new value 
 * is different, the Callback will be invoked.
 *
 * \tparam T The underlying type held by the left-hand argument.
 * \tparam U The underlying type held by the right-hand argument.
 * \param lhs The left-hand argument.
 * \param rhs The right-hand argument.
 * \returns The result of doing the operator on
 *     the underlying values.
 */
template <typename T, typename U>
TracedValue<T> &operator += (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x+=");
  T tmp = lhs.Get ();
  tmp += rhs;
  lhs.Set (tmp);
  return lhs;
}
/** \copydoc operator+=(TracedValue<T>&lhs,const U&rhs) */
template <typename T, typename U>
TracedValue<T> &operator -= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x-=");
  T tmp = lhs.Get ();
  tmp -= rhs;
  lhs.Set (tmp);
  return lhs;
}
/** \copydoc operator+=(TracedValue<T>&lhs,const U&rhs) */
template <typename T, typename U>
TracedValue<T> &operator *= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x*=");
  T tmp = lhs.Get ();
  tmp *= rhs;
  lhs.Set (tmp);
  return lhs;
}
/** \copydoc operator+=(TracedValue<T>&lhs,const U&rhs) */
template <typename T, typename U>
TracedValue<T> &operator /= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x/=");
  T tmp = lhs.Get ();
  tmp /= rhs;
  lhs.Set (tmp);
  return lhs;
}
/** \copydoc operator+=(TracedValue<T>&lhs,const U&rhs) */
template <typename T, typename U>
TracedValue<T> &operator %= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x%=");
  T tmp = lhs.Get ();
  tmp %= rhs;
  lhs.Set (tmp);
  return lhs;
}
/** \copydoc operator+=(TracedValue<T>&lhs,const U&rhs) */
template <typename T, typename U>
TracedValue<T> &operator <<= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x<<=");
  T tmp = lhs.Get ();
  tmp <<= rhs;
  lhs.Set (tmp);
  return lhs;
}
/** \copydoc operator+=(TracedValue<T>&lhs,const U&rhs) */
template <typename T, typename U>
TracedValue<T> &operator >>= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x>>=");
  T tmp = lhs.Get ();
  tmp >>= rhs;
  lhs.Set (tmp);
  return lhs;
}
/** \copydoc operator+=(TracedValue<T>&lhs,const U&rhs) */
template <typename T, typename U>
TracedValue<T> &operator &= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x&=");
  T tmp = lhs.Get ();
  tmp &= rhs;
  lhs.Set (tmp);
  return lhs;
}
/** \copydoc operator+=(TracedValue<T>&lhs,const U&rhs) */
template <typename T, typename U>
TracedValue<T> &operator |= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x|=");
  T tmp = lhs.Get ();
  tmp |= rhs;
  lhs.Set (tmp);
  return lhs;
}
/** \copydoc operator+=(TracedValue<T>&lhs,const U&rhs) */
template <typename T, typename U>
TracedValue<T> &operator ^= (TracedValue<T> &lhs, const U &rhs) {
  TRACED_VALUE_DEBUG ("x^=");
  T tmp = lhs.Get ();
  tmp ^= rhs;
  lhs.Set (tmp);
  return lhs;
}
  

/**
 * Unary arithmetic operator for TracedValue.
 *
 * \tparam T The underlying type held by the TracedValue.
 * \param lhs The TracedValue.
 * \returns The result of doing the operator on
 *     the underlying values.
 */
template <typename T>
TracedValue<T> operator + (const TracedValue<T> &lhs) {
  TRACED_VALUE_DEBUG ("(+x)");
  return TracedValue<T> (+lhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs) */
template <typename T>
TracedValue<T> operator - (const TracedValue<T> &lhs) {
  TRACED_VALUE_DEBUG ("(-x)");
  return TracedValue<T> (-lhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs) */
template <typename T>
TracedValue<T> operator ~ (const TracedValue<T> &lhs) {
  TRACED_VALUE_DEBUG ("(~x)");
  return TracedValue<T> (~lhs.Get ());
}
/** \copydoc operator+(const TracedValue<T>&lhs) */
template <typename T>
TracedValue<T> operator ! (const TracedValue<T> &lhs) {
  TRACED_VALUE_DEBUG ("(!x)");
  return TracedValue<T> (!lhs.Get ());
}

/**@}*/  // \ingroup tracing

} // namespace ns3

#endif /* TRACED_VALUE_H */
