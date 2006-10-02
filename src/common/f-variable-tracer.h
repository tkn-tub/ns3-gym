/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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

#include "ns3/callback.h"
#include <stdint.h>

namespace ns3 {

class FVariableTracerBase {
public:
    typedef Callback<void,double, double> ChangeNotifyCallback;

    FVariableTracerBase () {}
    FVariableTracerBase (FVariableTracerBase const &o) {}
    FVariableTracerBase &operator = (FVariableTracerBase const &o) {
        return *this;
    }

    ~FVariableTracerBase () {}

    void setCallback(ChangeNotifyCallback callback) {
        m_callback = callback;
    }
protected:
    void notify (double oldVal, double newVal) {
        if (oldVal != newVal && !m_callback.isNull ()) {
            m_callback (oldVal, newVal);
        }
    }
private:
    ChangeNotifyCallback m_callback;
};


}; // namespace ns3

#endif /* F_VARIABLE_TRACER_H */
