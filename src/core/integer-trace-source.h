/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2008 INRIA
 * All rights reserved.
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

#ifndef INTEGER_TRACE_SOURCE_H
#define INTEGER_TRACE_SOURCE_H

#include <stdint.h>
#include "event-trace-source.h"
#include "integer.h"

namespace ns3 {

class IntegerTraceSourceBase
{
public:
  typedef EventTraceSource<int64_t, int64_t> ChangeNotifyCallback;

  IntegerTraceSourceBase () {}
  IntegerTraceSourceBase (const IntegerTraceSourceBase &o) {}
  IntegerTraceSourceBase &operator = (const IntegerTraceSourceBase &o) {
    return *this;
  }

  void Connect (const CallbackBase & callback) {
    m_callback.AddCallback (callback);
  }
  void Disconnect (const CallbackBase & callback) {
    m_callback.RemoveCallback (callback);
  }
protected:
  void Notify (int64_t oldVal, int64_t newVal) {
    if (oldVal != newVal) 
      {
        m_callback (oldVal, newVal);
      }
  }
private:
  ChangeNotifyCallback m_callback;
};

#if 0
template <typename T>
class UIntegerTraceSource;
#endif


template <typename T>
class IntegerTraceSource : public IntegerTraceSourceBase {
public:
  IntegerTraceSource ()
      : m_var (0)
  {}
  IntegerTraceSource (T const &var) 
      : m_var (var)
  {}

  IntegerTraceSource &operator = (IntegerTraceSource const &o) {
      Set (o.Get ());
      return *this;
  }
  template <typename TT>
  IntegerTraceSource &operator = (IntegerTraceSource<TT> const &o) {
      Set (o.Get ());
      return *this;
  }
#if 0
  template <typename TT>
  IntegerTraceSource &operator = (UintegerTraceSource<TT> const &o) {
      Set (o.Get ());
      return *this;
  }
#endif
  IntegerTraceSource &operator++ () {
      Set (Get () + 1);
      return *this;
  }
  IntegerTraceSource &operator-- () {
      Set (Get () - 1);
      return *this;
  }
  IntegerTraceSource operator++ (int) {
      IntegerTraceSource old (*this);
      ++*this;
      return old;
  }
  IntegerTraceSource operator-- (int) {
      IntegerTraceSource old (*this);
      --*this;
      return old;
  }
  operator T () const {
      return Get ();
  }
  void Set (T var) {
      Notify (m_var, var);
      m_var = var;
  }
  T Get (void) const {
      return m_var;
  }

  IntegerTraceSource (Integer value) 
    : m_var (value.Get ()) {}
  operator Integer () const {
    return Integer (m_var);
  }

private:
  T m_var;
};

template <typename T>
IntegerTraceSource<T> &operator += (IntegerTraceSource<T> &lhs, IntegerTraceSource<T> const &rhs) {
  lhs.Set (lhs.Get () + rhs.Get ());
  return lhs;
}
template <typename T>
IntegerTraceSource<T> &operator -= (IntegerTraceSource<T> &lhs, IntegerTraceSource<T> const &rhs) {
  lhs.Set (lhs.Get () - rhs.Get ());
  return lhs;
}
template <typename T>
IntegerTraceSource<T> &operator *= (IntegerTraceSource<T> &lhs, IntegerTraceSource<T> const &rhs) {
  lhs.Set (lhs.Get () * rhs.Get ());
  return lhs;
}
template <typename T>
IntegerTraceSource<T> &operator /= (IntegerTraceSource<T> &lhs, IntegerTraceSource<T> const &rhs) {
  lhs.Set (lhs.Get () / rhs.Get ());
  return lhs;
}
template <typename T>
IntegerTraceSource<T> &operator <<= (IntegerTraceSource<T> &lhs, IntegerTraceSource<T> const &rhs) {
  lhs.Set (lhs.Get () << rhs.Get ());
  return lhs;
}
template <typename T>
IntegerTraceSource<T> &operator >>= (IntegerTraceSource<T> &lhs, IntegerTraceSource<T> const &rhs) {
  lhs.Set (lhs.Get () >> rhs.Get ());
  return lhs;
}
template <typename T>
IntegerTraceSource<T> &operator &= (IntegerTraceSource<T> &lhs, IntegerTraceSource<T> const &rhs) {
  lhs.Set (lhs.Get () & rhs.Get ());
  return lhs;
}
template <typename T>
IntegerTraceSource<T> &operator |= (IntegerTraceSource<T> &lhs, IntegerTraceSource<T> const &rhs) {
  lhs.Set (lhs.Get () | rhs.Get ());
  return lhs;
}
template <typename T>
IntegerTraceSource<T> &operator ^= (IntegerTraceSource<T> &lhs, IntegerTraceSource<T> const &rhs) {
  lhs.Set (lhs.Get () ^ rhs.Get ());
  return lhs;
}


template <typename T, typename U>
IntegerTraceSource<T> &operator += (IntegerTraceSource<T> &lhs, U const &rhs) {
  lhs.Set (lhs.Get () + rhs);
  return lhs;
}
template <typename T, typename U>
IntegerTraceSource<T> &operator -= (IntegerTraceSource<T> &lhs, U const &rhs) {
  lhs.Set (lhs.Get () - rhs);
  return lhs;
}
template <typename T, typename U>
IntegerTraceSource<T> &operator *= (IntegerTraceSource<T> &lhs, U const &rhs) {
  lhs.Set (lhs.Get () * rhs);
  return lhs;
}
template <typename T, typename U>
IntegerTraceSource<T> &operator /= (IntegerTraceSource<T> &lhs, U const &rhs) {
  lhs.Set (lhs.Get () / rhs);
  return lhs;
}
template <typename T, typename U>
IntegerTraceSource<T> &operator <<= (IntegerTraceSource<T> &lhs, U const &rhs) {
  lhs.Set (lhs.Get () << rhs);
  return lhs;
}
template <typename T, typename U>
IntegerTraceSource<T> &operator >>= (IntegerTraceSource<T> &lhs, U const &rhs) {
  lhs.Set (lhs.Get () >> rhs);
  return lhs;
}
template <typename T, typename U>
IntegerTraceSource<T> &operator &= (IntegerTraceSource<T> &lhs, U const &rhs) {
  lhs.Set (lhs.Get () & rhs);
  return lhs;
}
template <typename T, typename U>
IntegerTraceSource<T> &operator |= (IntegerTraceSource<T> &lhs, U const &rhs) {
  lhs.Set (lhs.Get () | rhs);
  return lhs;
}
template <typename T, typename U>
IntegerTraceSource<T> &operator ^= (IntegerTraceSource<T> &lhs, U const &rhs) {
  lhs.Set (lhs.Get () ^ rhs);
  return lhs;
}

} // namespace ns3

#endif /* INTEGER_TRACE_SOURCE_H */
