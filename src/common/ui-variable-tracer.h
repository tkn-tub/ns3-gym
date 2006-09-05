/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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

#ifndef UI_VARIABLE_TRACER_H
#define UI_VARIABLE_TRACER_H

#include "ns3/callback.h"
#include <stdint.h>

namespace ns3 {

class UiVariableTracerBase {
public:
	typedef Callback<void, uint64_t, uint64_t> ChangeNotifyCallback;

	UiVariableTracerBase ()
		: m_callback () {}
	/* We don't want to copy the base callback. Only setCallback on
	 * a specific instance will do something to it. */
	UiVariableTracerBase (UiVariableTracerBase const &o) 
		: m_callback () {}
	UiVariableTracerBase &operator = (UiVariableTracerBase const &o) {
		return *this;
	}
	~UiVariableTracerBase () {}

	void setCallback(ChangeNotifyCallback callback) {
		m_callback = callback;
	}
protected:
	void notify (uint64_t oldVal, uint64_t newVal) {
		if (oldVal != newVal && !m_callback.isNull ()) {
			m_callback (oldVal, newVal);
		}
	}
private:
	ChangeNotifyCallback m_callback;
};

template <typename T>
class SiVariableTracer;


/**
 * \brief trace variables of type "unsigned integer"
 *
 * This template class implements a POD type: it
 * behaves like any other variable of type "unsigned integer"
 * except that it also reports any changes to its
 * value with its internal callback.
 *
 * To instantiate a 32-bit unsigned variable (to store
 * a TCP counter for example), you would create a variable of type
 * ns3::UiVariableTracer<uint32_t> :
 \code
 #include <stdint.h>
 #include "ns3/ui-traced-variable.tcc"

 ns3::UiVariableTracer<uint32_t> var;
 \endcode
 * and you would use it like any other variable of type uint32_t:
 \code
 var += 12;
 var = 10;
 \endcode
 */
template <typename T>
class UiVariableTracer : public UiVariableTracerBase {
public:
	UiVariableTracer ()
		: m_var ()
	{}
	UiVariableTracer (T const &var) 
		: m_var (var)
	{}

	UiVariableTracer &operator = (UiVariableTracer const &o) {
		assign (o.get ());
		return *this;
	}
	template <typename TT>
	UiVariableTracer &operator = (UiVariableTracer<TT> const &o) {
		assign (o.get ());
		return *this;
	}
	template <typename TT>
	UiVariableTracer &operator = (SiVariableTracer<TT> const &o) {
		assign (o.get ());
		return *this;
	}
	UiVariableTracer &operator++ () {
		assign (get () + 1);
		return *this;
	}
	UiVariableTracer &operator-- () {
		assign (get () - 1);
		return *this;
	}
	UiVariableTracer operator++ (int) {
		UiVariableTracer old (*this);
		++*this;
		return old;
	}
	UiVariableTracer operator-- (int) {
		UiVariableTracer old (*this);
		--*this;
		return old;
	}
	operator T () const {
		return get ();
	}


	void assign (T var) {
		notify (m_var, var);
		m_var = var;
	}
        T get (void) const {
		return m_var;
	}

private:
	T m_var;
};

template <typename T>
UiVariableTracer<T> &operator += (UiVariableTracer<T> &lhs, UiVariableTracer<T> const &rhs) {
	lhs.assign (lhs.get () + rhs.get ());
	return lhs;
}
template <typename T>
UiVariableTracer<T> &operator -= (UiVariableTracer<T> &lhs, UiVariableTracer<T> const &rhs) {
	lhs.assign (lhs.get () - rhs.get ());
	return lhs;
}
template <typename T>
UiVariableTracer<T> &operator *= (UiVariableTracer<T> &lhs, UiVariableTracer<T> const &rhs) {
	lhs.assign (lhs.get () * rhs.get ());
	return lhs;
}
template <typename T>
UiVariableTracer<T> &operator /= (UiVariableTracer<T> &lhs, UiVariableTracer<T> const &rhs) {
	lhs.assign (lhs.get () / rhs.get ());
	return lhs;
}
template <typename T>
UiVariableTracer<T> &operator <<= (UiVariableTracer<T> &lhs, UiVariableTracer<T> const &rhs) {
	lhs.assign (lhs.get () << rhs.get ());
	return lhs;
}
template <typename T>
UiVariableTracer<T> &operator >>= (UiVariableTracer<T> &lhs, UiVariableTracer<T> const &rhs) {
	lhs.assign (lhs.get () >> rhs.get ());
	return lhs;
}
template <typename T>
UiVariableTracer<T> &operator &= (UiVariableTracer<T> &lhs, UiVariableTracer<T> const &rhs) {
	lhs.assign (lhs.get () & rhs.get ());
	return lhs;
}
template <typename T>
UiVariableTracer<T> &operator |= (UiVariableTracer<T> &lhs, UiVariableTracer<T> const &rhs) {
	lhs.assign (lhs.get () | rhs.get ());
	return lhs;
}
template <typename T>
UiVariableTracer<T> &operator ^= (UiVariableTracer<T> &lhs, UiVariableTracer<T> const &rhs) {
	lhs.assign (lhs.get () ^ rhs.get ());
	return lhs;
}


template <typename T, typename U>
UiVariableTracer<T> &operator += (UiVariableTracer<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () + rhs);
	return lhs;
}
template <typename T, typename U>
UiVariableTracer<T> &operator -= (UiVariableTracer<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () - rhs);
	return lhs;
}
template <typename T, typename U>
UiVariableTracer<T> &operator *= (UiVariableTracer<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () * rhs);
	return lhs;
}
template <typename T, typename U>
UiVariableTracer<T> &operator /= (UiVariableTracer<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () / rhs);
	return lhs;
}
template <typename T, typename U>
UiVariableTracer<T> &operator <<= (UiVariableTracer<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () << rhs);
	return lhs;
}
template <typename T, typename U>
UiVariableTracer<T> &operator >>= (UiVariableTracer<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () >> rhs);
	return lhs;
}
template <typename T, typename U>
UiVariableTracer<T> &operator &= (UiVariableTracer<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () & rhs);
	return lhs;
}
template <typename T, typename U>
UiVariableTracer<T> &operator |= (UiVariableTracer<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () | rhs);
	return lhs;
}
template <typename T, typename U>
UiVariableTracer<T> &operator ^= (UiVariableTracer<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () ^ rhs);
	return lhs;
}

}; // namespace ns3

#endif /* UI_VARIABLE_TRACER_H */
