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

#ifndef SV_TRACE_SOURCE_H
#define SV_TRACE_SOURCE_H

#include "callback-trace-source.h"
#include "trace-source.h"
#include <stdint.h>

namespace ns3 {

class SVTraceSourceBase : public TraceSource {
public:
  typedef CallbackTraceSource<int64_t, int64_t> ChangeNotifyCallback;

  SVTraceSourceBase () {}
  SVTraceSourceBase (SVTraceSourceBase const &o) {}
  SVTraceSourceBase &operator = (SVTraceSourceBase const &o) {
      return *this;
  }

  virtual ~SVTraceSourceBase () {}

  void AddCallback (CallbackBase const & callback, TraceContext const & context) {
    m_callback.AddCallback (callback, context);
  }
  void RemoveCallback (CallbackBase const & callback) {
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

template <typename T>
class UVTraceSource;


/**
 * \brief trace variables of type "signed integer"
 * \ingroup lowleveltracing
 *
 * This template class implements a POD type: it
 * behaves like any other variable of type "signed integer"
 * except that it also reports any changes to its
 * value with its internal callback.
 *
 * To instantiate a 32-bit signed variable (to store
 * a TCP counter for example), you would create a variable of type
 * ns3::UVTraceSource<int32_t> :
 \code
 #include <stdint.h>
 #include "sv-trace-source.h"

 ns3::SVTraceSource<uint16_t> var;
 \endcode
 * and you would use it like any other variable of type int32_t:
 \code
 var += 12;
 var = 10;
 var = -10;
 \endcode
 */
template <typename T>
class SVTraceSource : public SVTraceSourceBase {
public:
  SVTraceSource ()
      : m_var (0)
  {}
  SVTraceSource (T const &var) 
      : m_var (var)
  {}

  SVTraceSource &operator = (SVTraceSource const &o) {
      Assign (o.Get ());
      return *this;
  }
  template <typename TT>
  SVTraceSource &operator = (SVTraceSource<TT> const &o) {
      Assign (o.Get ());
      return *this;
  }
  template <typename TT>
  SVTraceSource &operator = (UVTraceSource<TT> const &o) {
      Assign (o.Get ());
      return *this;
  }
  SVTraceSource &operator++ () {
      Assign (Get () + 1);
      return *this;
  }
  SVTraceSource &operator-- () {
      Assign (Get () - 1);
      return *this;
  }
  SVTraceSource operator++ (int) {
      SVTraceSource old (*this);
      ++*this;
      return old;
  }
  SVTraceSource operator-- (int) {
      SVTraceSource old (*this);
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
SVTraceSource<T> &operator += (SVTraceSource<T> &lhs, SVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () + rhs.Get ());
  return lhs;
}
template <typename T>
SVTraceSource<T> &operator -= (SVTraceSource<T> &lhs, SVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () - rhs.Get ());
  return lhs;
}
template <typename T>
SVTraceSource<T> &operator *= (SVTraceSource<T> &lhs, SVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () * rhs.Get ());
  return lhs;
}
template <typename T>
SVTraceSource<T> &operator /= (SVTraceSource<T> &lhs, SVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () / rhs.Get ());
  return lhs;
}
template <typename T>
SVTraceSource<T> &operator <<= (SVTraceSource<T> &lhs, SVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () << rhs.Get ());
  return lhs;
}
template <typename T>
SVTraceSource<T> &operator >>= (SVTraceSource<T> &lhs, SVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () >> rhs.Get ());
  return lhs;
}
template <typename T>
SVTraceSource<T> &operator &= (SVTraceSource<T> &lhs, SVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () & rhs.Get ());
  return lhs;
}
template <typename T>
SVTraceSource<T> &operator |= (SVTraceSource<T> &lhs, SVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () | rhs.Get ());
  return lhs;
}
template <typename T>
SVTraceSource<T> &operator ^= (SVTraceSource<T> &lhs, SVTraceSource<T> const &rhs) {
  lhs.Assign (lhs.Get () ^ rhs.Get ());
  return lhs;
}


template <typename T, typename U>
SVTraceSource<T> &operator += (SVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () + rhs);
  return lhs;
}
template <typename T, typename U>
SVTraceSource<T> &operator -= (SVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () - rhs);
  return lhs;
}
template <typename T, typename U>
SVTraceSource<T> &operator *= (SVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () * rhs);
  return lhs;
}
template <typename T, typename U>
SVTraceSource<T> &operator /= (SVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () / rhs);
  return lhs;
}
template <typename T, typename U>
SVTraceSource<T> &operator <<= (SVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () << rhs);
  return lhs;
}
template <typename T, typename U>
SVTraceSource<T> &operator >>= (SVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () >> rhs);
  return lhs;
}
template <typename T, typename U>
SVTraceSource<T> &operator &= (SVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () & rhs);
  return lhs;
}
template <typename T, typename U>
SVTraceSource<T> &operator |= (SVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () | rhs);
  return lhs;
}
template <typename T, typename U>
SVTraceSource<T> &operator ^= (SVTraceSource<T> &lhs, U const &rhs) {
  lhs.Assign (lhs.Get () ^ rhs);
  return lhs;
}

}; // namespace ns3

#endif /* SV_TRACE_SOURCE_H */
