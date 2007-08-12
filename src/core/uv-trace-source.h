/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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

#ifndef UV_TRACE_SOURCE_H
#define UV_TRACE_SOURCE_H

#include "callback-trace-source.h"
#include "trace-source.h"
#include <stdint.h>

namespace ns3 {

class UVTraceSourceBase : public TraceSource {
public:
  typedef CallbackTraceSource<uint64_t, uint64_t> ChangeNotifyCallback;

  UVTraceSourceBase ()
      : m_callback () {}
  /* We don't want to copy the base callback. Only setCallback on
   * a specific instance will do something to it. */
  UVTraceSourceBase (UVTraceSourceBase const &o) 
      : m_callback () {}
  UVTraceSourceBase &operator = (UVTraceSourceBase const &o) {
      return *this;
  }
  ~UVTraceSourceBase () {}

  virtual void AddCallback (CallbackBase const & callback, TraceContext const & context) {
    m_callback.AddCallback (callback, context);
  }
  virtual void RemoveCallback (CallbackBase const & callback) {
    m_callback.RemoveCallback (callback);
  }

protected:
  void Notify (uint64_t oldVal, uint64_t newVal) {
      if (oldVal != newVal) 
        {
          m_callback (oldVal, newVal);
        }
  }
private:
  ChangeNotifyCallback m_callback;
};

template <typename T>
class SVTraceSource;


/**
 * \brief trace variables of type "unsigned integer"
 * \ingroup lowleveltracing
 *
 * This template class implements a POD type: it
 * behaves like any other variable of type "unsigned integer"
 * except that it also reports any changes to its
 * value with its internal callback.
 *
 * To instantiate a 32-bit unsigned variable (to store
 * a TCP counter for example), you would create a variable of type
 * ns3::UVTraceSource<uint32_t> :
 \code
 #include <stdint.h>
 #include "uv-trace-source.h"

 ns3::UVTraceSource<uint32_t> var;
 \endcode
 * and you would use it like any other variable of type uint32_t:
 \code
 var += 12;
 var = 10;
 \endcode
 */
template <typename T>
class UVTraceSource : public UVTraceSourceBase {
public:
  UVTraceSource ()
      : m_var ()
  {}
  UVTraceSource (T const &var) 
      : m_var (var)
  {}

  UVTraceSource &operator = (UVTraceSource const &o) {
      Assign (o.Get ());
      return *this;
  }
  template <typename TT>
  UVTraceSource &operator = (UVTraceSource<TT> const &o) {
      Assign (o.Get ());
      return *this;
  }
  template <typename TT>
  UVTraceSource &operator = (SVTraceSource<TT> const &o) {
      Assign (o.Get ());
      return *this;
  }
  UVTraceSource &operator++ () {
      Assign (Get () + 1);
      return *this;
  }
  UVTraceSource &operator-- () {
      Assign (Get () - 1);
      return *this;
  }
  UVTraceSource operator++ (int) {
      UVTraceSource old (*this);
      ++*this;
      return old;
  }
  UVTraceSource operator-- (int) {
      UVTraceSource old (*this);
      --*this;
      return old;
  }
  operator T () const {
      return Get ();
  }


  void Assign (T var) {
      Notify (m_var, var);
      m_var = var;
  }
  T Get (void) const {
      return m_var;
  }

private:
  T m_var;
};

template <typename T>
UVTraceSource<T> &operator += (UVTraceSource<T> &lhs, UVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () + rhs.Get ());
  return lhs;
}
template <typename T>
UVTraceSource<T> &operator -= (UVTraceSource<T> &lhs, UVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () - rhs.Get ());
  return lhs;
}
template <typename T>
UVTraceSource<T> &operator *= (UVTraceSource<T> &lhs, UVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () * rhs.Get ());
  return lhs;
}
template <typename T>
UVTraceSource<T> &operator /= (UVTraceSource<T> &lhs, UVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () / rhs.Get ());
  return lhs;
}
template <typename T>
UVTraceSource<T> &operator <<= (UVTraceSource<T> &lhs, UVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () << rhs.Get ());
  return lhs;
}
template <typename T>
UVTraceSource<T> &operator >>= (UVTraceSource<T> &lhs, UVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () >> rhs.Get ());
  return lhs;
}
template <typename T>
UVTraceSource<T> &operator &= (UVTraceSource<T> &lhs, UVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () & rhs.Get ());
  return lhs;
}
template <typename T>
UVTraceSource<T> &operator |= (UVTraceSource<T> &lhs, UVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () | rhs.Get ());
  return lhs;
}
template <typename T>
UVTraceSource<T> &operator ^= (UVTraceSource<T> &lhs, UVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () ^ rhs.Get ());
  return lhs;
}


template <typename T, typename U>
UVTraceSource<T> &operator += (UVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () + rhs);
  return lhs;
}
template <typename T, typename U>
UVTraceSource<T> &operator -= (UVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () - rhs);
  return lhs;
}
template <typename T, typename U>
UVTraceSource<T> &operator *= (UVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () * rhs);
  return lhs;
}
template <typename T, typename U>
UVTraceSource<T> &operator /= (UVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () / rhs);
  return lhs;
}
template <typename T, typename U>
UVTraceSource<T> &operator <<= (UVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () << rhs);
  return lhs;
}
template <typename T, typename U>
UVTraceSource<T> &operator >>= (UVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () >> rhs);
  return lhs;
}
template <typename T, typename U>
UVTraceSource<T> &operator &= (UVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () & rhs);
  return lhs;
}
template <typename T, typename U>
UVTraceSource<T> &operator |= (UVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () | rhs);
  return lhs;
}
template <typename T, typename U>
UVTraceSource<T> &operator ^= (UVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () ^ rhs);
  return lhs;
}

}; // namespace ns3

#endif /* UV_TRACE_SOURCE_H */
