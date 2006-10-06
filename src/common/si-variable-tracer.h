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

#ifndef SI_VARIABLE_TRACER_H
#define SI_VARIABLE_TRACER_H

#include "ns3/callback.h"
#include <stdint.h>

namespace ns3 {

class SiVariableTracerBase {
public:
    typedef Callback<void,int64_t, int64_t> ChangeNotifyCallback;

    SiVariableTracerBase () {}
    SiVariableTracerBase (SiVariableTracerBase const &o) {}
    SiVariableTracerBase &operator = (SiVariableTracerBase const &o) {
        return *this;
    }

    ~SiVariableTracerBase () {}

    void SetCallback(ChangeNotifyCallback callback) {
        m_callback = callback;
    }
protected:
    void Notify (int64_t oldVal, int64_t newVal) {
        if (oldVal != newVal && !m_callback.IsNull ()) {
            m_callback (oldVal, newVal);
        }
    }
private:
    ChangeNotifyCallback m_callback;
};

template <typename T>
class UiVariableTracer;


/**
 * \brief trace variables of type "signed integer"
 *
 * This template class implements a POD type: it
 * behaves like any other variable of type "signed integer"
 * except that it also reports any changes to its
 * value with its internal callback.
 *
 * To instantiate a 32-bit signed variable (to store
 * a TCP counter for example), you would create a variable of type
 * ns3::UiVariableTracer<int32_t> :
 \code
 #include <stdint.h>
 #include "ns3/si-traced-variable.tcc"

 ns3::SiVariableTracer<uint16_t> var;
 \endcode
 * and you would use it like any other variable of type int32_t:
 \code
 var += 12;
 var = 10;
 var = -10;
 \endcode
 */
template <typename T>
class SiVariableTracer : public SiVariableTracerBase {
public:
    SiVariableTracer ()
        : m_var (0)
    {}
    SiVariableTracer (T const &var) 
        : m_var (var)
    {}

    SiVariableTracer &operator = (SiVariableTracer const &o) {
        Assign (o.Get ());
        return *this;
    }
    template <typename TT>
    SiVariableTracer &operator = (SiVariableTracer<TT> const &o) {
        Assign (o.Get ());
        return *this;
    }
    template <typename TT>
    SiVariableTracer &operator = (UiVariableTracer<TT> const &o) {
        Assign (o.Get ());
        return *this;
    }
    SiVariableTracer &operator++ () {
        Assign (Get () + 1);
        return *this;
    }
    SiVariableTracer &operator-- () {
        Assign (Get () - 1);
        return *this;
    }
    SiVariableTracer operator++ (int) {
        SiVariableTracer old (*this);
        ++*this;
        return old;
    }
    SiVariableTracer operator-- (int) {
        SiVariableTracer old (*this);
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
SiVariableTracer<T> &operator += (SiVariableTracer<T> &lhs, SiVariableTracer<T> const &rhs) {
    lhs.Assign (lhs.Get () + rhs.Get ());
    return lhs;
}
template <typename T>
SiVariableTracer<T> &operator -= (SiVariableTracer<T> &lhs, SiVariableTracer<T> const &rhs) {
    lhs.Assign (lhs.Get () - rhs.Get ());
    return lhs;
}
template <typename T>
SiVariableTracer<T> &operator *= (SiVariableTracer<T> &lhs, SiVariableTracer<T> const &rhs) {
    lhs.Assign (lhs.Get () * rhs.Get ());
    return lhs;
}
template <typename T>
SiVariableTracer<T> &operator /= (SiVariableTracer<T> &lhs, SiVariableTracer<T> const &rhs) {
    lhs.Assign (lhs.Get () / rhs.Get ());
    return lhs;
}
template <typename T>
SiVariableTracer<T> &operator <<= (SiVariableTracer<T> &lhs, SiVariableTracer<T> const &rhs) {
    lhs.Assign (lhs.Get () << rhs.Get ());
    return lhs;
}
template <typename T>
SiVariableTracer<T> &operator >>= (SiVariableTracer<T> &lhs, SiVariableTracer<T> const &rhs) {
    lhs.Assign (lhs.Get () >> rhs.Get ());
    return lhs;
}
template <typename T>
SiVariableTracer<T> &operator &= (SiVariableTracer<T> &lhs, SiVariableTracer<T> const &rhs) {
    lhs.Assign (lhs.Get () & rhs.Get ());
    return lhs;
}
template <typename T>
SiVariableTracer<T> &operator |= (SiVariableTracer<T> &lhs, SiVariableTracer<T> const &rhs) {
    lhs.Assign (lhs.Get () | rhs.Get ());
    return lhs;
}
template <typename T>
SiVariableTracer<T> &operator ^= (SiVariableTracer<T> &lhs, SiVariableTracer<T> const &rhs) {
    lhs.Assign (lhs.Get () ^ rhs.Get ());
    return lhs;
}


template <typename T, typename U>
SiVariableTracer<T> &operator += (SiVariableTracer<T> &lhs, U const &rhs) {
    lhs.Assign (lhs.Get () + rhs);
    return lhs;
}
template <typename T, typename U>
SiVariableTracer<T> &operator -= (SiVariableTracer<T> &lhs, U const &rhs) {
    lhs.Assign (lhs.Get () - rhs);
    return lhs;
}
template <typename T, typename U>
SiVariableTracer<T> &operator *= (SiVariableTracer<T> &lhs, U const &rhs) {
    lhs.Assign (lhs.Get () * rhs);
    return lhs;
}
template <typename T, typename U>
SiVariableTracer<T> &operator /= (SiVariableTracer<T> &lhs, U const &rhs) {
    lhs.Assign (lhs.Get () / rhs);
    return lhs;
}
template <typename T, typename U>
SiVariableTracer<T> &operator <<= (SiVariableTracer<T> &lhs, U const &rhs) {
    lhs.Assign (lhs.Get () << rhs);
    return lhs;
}
template <typename T, typename U>
SiVariableTracer<T> &operator >>= (SiVariableTracer<T> &lhs, U const &rhs) {
    lhs.Assign (lhs.Get () >> rhs);
    return lhs;
}
template <typename T, typename U>
SiVariableTracer<T> &operator &= (SiVariableTracer<T> &lhs, U const &rhs) {
    lhs.Assign (lhs.Get () & rhs);
    return lhs;
}
template <typename T, typename U>
SiVariableTracer<T> &operator |= (SiVariableTracer<T> &lhs, U const &rhs) {
    lhs.Assign (lhs.Get () | rhs);
    return lhs;
}
template <typename T, typename U>
SiVariableTracer<T> &operator ^= (SiVariableTracer<T> &lhs, U const &rhs) {
    lhs.Assign (lhs.Get () ^ rhs);
    return lhs;
}

}; // namespace ns3

#endif /* SI_VARIABLE_TRACER_H */
