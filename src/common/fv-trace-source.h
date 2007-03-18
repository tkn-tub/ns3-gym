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

#ifndef F_VARIABLE_TRACER_H
#define F_VARIABLE_TRACER_H

#include "callback-trace-source.h"
#include <stdint.h>

namespace ns3 {

class FVTraceSourceBase {
public:
  typedef CallbackTraceSource<double, double> ChangeNotifyCallback;

  FVTraceSourceBase () {}
  FVTraceSourceBase (FVTraceSourceBase const &o) {}
  FVTraceSourceBase &operator = (FVTraceSourceBase const &o) {
      return *this;
  }

  ~FVTraceSourceBase () {}

  void AddCallback (CallbackBase const & callback, TraceContext const & context) {
    m_callback.AddCallback (callback, context);
  }
  void RemoveCallback (CallbackBase const & callback) {
    m_callback.RemoveCallback (callback);
  }
protected:
  void notify (double oldVal, double newVal) {
      if (oldVal != newVal) 
        {
          m_callback (oldVal, newVal);
        }
  }
private:
  ChangeNotifyCallback m_callback;
};

template <typename T>
class FVTraceSource : public FVTraceSourceBase 
{
public:
};

}; // namespace ns3

#endif /* F_VARIABLE_TRACER_H */
