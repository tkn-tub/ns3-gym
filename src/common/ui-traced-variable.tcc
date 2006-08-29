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

#ifndef UI_TRACED_VARIABLE_TCC
#define UI_TRACED_VARIABLE_TCC

#include "ns3/callback.h"
#include <stdint.h>

namespace yans {

class UiTracedVariableBase {
public:
	typedef Callback<void, uint64_t, uint64_t> ChangeNotifyCallback;

	UiTracedVariableBase ()
		: m_callback () {}
	/* We don't want to copy the base callback. Only set_callback on
	 * a specific instance will do something to it. */
	UiTracedVariableBase (UiTracedVariableBase const &o) 
		: m_callback () {}
	UiTracedVariableBase &operator = (UiTracedVariableBase const &o) {
		return *this;
	}
	~UiTracedVariableBase () {}

	void set_callback(ChangeNotifyCallback callback) {
		m_callback = callback;
	}
protected:
	void notify (uint64_t old_val, uint64_t new_val) {
		if (old_val != new_val && !m_callback.is_null ()) {
			m_callback (old_val, new_val);
		}
	}
private:
	ChangeNotifyCallback m_callback;
};

template <typename T>
class SiTracedVariable;


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
 * yans::UiTracedVariable<uint32_t> :
 \code
 #include <stdint.h>
 #include "ns3/ui-traced-variable.tcc"

 yans::UiTracedVariable<uint32_t> var;
 \endcode
 * and you would use it like any other variable of type uint32_t:
 \code
 var += 12;
 var = 10;
 \endcode
 */
template <typename T>
class UiTracedVariable : public UiTracedVariableBase {
public:
	UiTracedVariable ()
		: m_var ()
	{}
	UiTracedVariable (T const &var) 
		: m_var (var)
	{}

	UiTracedVariable &operator = (UiTracedVariable const &o) {
		assign (o.get ());
		return *this;
	}
	template <typename TT>
	UiTracedVariable &operator = (UiTracedVariable<TT> const &o) {
		assign (o.get ());
		return *this;
	}
	template <typename TT>
	UiTracedVariable &operator = (SiTracedVariable<TT> const &o) {
		assign (o.get ());
		return *this;
	}
	UiTracedVariable &operator++ () {
		assign (get () + 1);
		return *this;
	}
	UiTracedVariable &operator-- () {
		assign (get () - 1);
		return *this;
	}
	UiTracedVariable operator++ (int) {
		UiTracedVariable old (*this);
		++*this;
		return old;
	}
	UiTracedVariable operator-- (int) {
		UiTracedVariable old (*this);
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
UiTracedVariable<T> &operator += (UiTracedVariable<T> &lhs, UiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () + rhs.get ());
	return lhs;
}
template <typename T>
UiTracedVariable<T> &operator -= (UiTracedVariable<T> &lhs, UiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () - rhs.get ());
	return lhs;
}
template <typename T>
UiTracedVariable<T> &operator *= (UiTracedVariable<T> &lhs, UiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () * rhs.get ());
	return lhs;
}
template <typename T>
UiTracedVariable<T> &operator /= (UiTracedVariable<T> &lhs, UiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () / rhs.get ());
	return lhs;
}
template <typename T>
UiTracedVariable<T> &operator <<= (UiTracedVariable<T> &lhs, UiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () << rhs.get ());
	return lhs;
}
template <typename T>
UiTracedVariable<T> &operator >>= (UiTracedVariable<T> &lhs, UiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () >> rhs.get ());
	return lhs;
}
template <typename T>
UiTracedVariable<T> &operator &= (UiTracedVariable<T> &lhs, UiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () & rhs.get ());
	return lhs;
}
template <typename T>
UiTracedVariable<T> &operator |= (UiTracedVariable<T> &lhs, UiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () | rhs.get ());
	return lhs;
}
template <typename T>
UiTracedVariable<T> &operator ^= (UiTracedVariable<T> &lhs, UiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () ^ rhs.get ());
	return lhs;
}


template <typename T, typename U>
UiTracedVariable<T> &operator += (UiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () + rhs);
	return lhs;
}
template <typename T, typename U>
UiTracedVariable<T> &operator -= (UiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () - rhs);
	return lhs;
}
template <typename T, typename U>
UiTracedVariable<T> &operator *= (UiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () * rhs);
	return lhs;
}
template <typename T, typename U>
UiTracedVariable<T> &operator /= (UiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () / rhs);
	return lhs;
}
template <typename T, typename U>
UiTracedVariable<T> &operator <<= (UiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () << rhs);
	return lhs;
}
template <typename T, typename U>
UiTracedVariable<T> &operator >>= (UiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () >> rhs);
	return lhs;
}
template <typename T, typename U>
UiTracedVariable<T> &operator &= (UiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () & rhs);
	return lhs;
}
template <typename T, typename U>
UiTracedVariable<T> &operator |= (UiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () | rhs);
	return lhs;
}
template <typename T, typename U>
UiTracedVariable<T> &operator ^= (UiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () ^ rhs);
	return lhs;
}

}; // namespace yans

#endif /* UI_TRACED_VARIABLE_TCC */
