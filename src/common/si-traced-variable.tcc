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

#ifndef SI_TRACED_VARIABLE_TCC
#define SI_TRACED_VARIABLE_TCC

#include "yans/callback.h"
#include <stdint.h>

namespace yans {

class SiTracedVariableBase {
public:
	typedef Callback<void,int64_t, int64_t> ChangeNotifyCallback;

	SiTracedVariableBase () {}
	SiTracedVariableBase (SiTracedVariableBase const &o) {}
	SiTracedVariableBase &operator = (SiTracedVariableBase const &o) {
		return *this;
	}

	~SiTracedVariableBase () {}

	void set_callback(ChangeNotifyCallback callback) {
		m_callback = callback;
	}
protected:
	void notify (int64_t old_val, int64_t new_val) {
		if (old_val != new_val && !m_callback.is_null ()) {
			m_callback (old_val, new_val);
		}
	}
private:
	ChangeNotifyCallback m_callback;
};

template <typename T>
class UiTracedVariable;


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
 * yans::UiTracedVariable<int32_t> :
 \code
 #include <stdint.h>
 #include "yans/si-traced-variable.tcc"

 yans::SiTracedVariable<uint16_t> var;
 \endcode
 * and you would use it like any other variable of type int32_t:
 \code
 var += 12;
 var = 10;
 var = -10;
 \endcode
 */
template <typename T>
class SiTracedVariable : public SiTracedVariableBase {
public:
	SiTracedVariable ()
		: m_var (0)
	{}
	SiTracedVariable (T const &var) 
		: m_var (var)
	{}

	SiTracedVariable &operator = (SiTracedVariable const &o) {
		assign (o.get ());
		return *this;
	}
	template <typename TT>
	SiTracedVariable &operator = (SiTracedVariable<TT> const &o) {
		assign (o.get ());
		return *this;
	}
	template <typename TT>
	SiTracedVariable &operator = (UiTracedVariable<TT> const &o) {
		assign (o.get ());
		return *this;
	}
	SiTracedVariable &operator++ () {
		assign (get () + 1);
		return *this;
	}
	SiTracedVariable &operator-- () {
		assign (get () - 1);
		return *this;
	}
	SiTracedVariable operator++ (int) {
		SiTracedVariable old (*this);
		++*this;
		return old;
	}
	SiTracedVariable operator-- (int) {
		SiTracedVariable old (*this);
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
SiTracedVariable<T> &operator += (SiTracedVariable<T> &lhs, SiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () + rhs.get ());
	return lhs;
}
template <typename T>
SiTracedVariable<T> &operator -= (SiTracedVariable<T> &lhs, SiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () - rhs.get ());
	return lhs;
}
template <typename T>
SiTracedVariable<T> &operator *= (SiTracedVariable<T> &lhs, SiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () * rhs.get ());
	return lhs;
}
template <typename T>
SiTracedVariable<T> &operator /= (SiTracedVariable<T> &lhs, SiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () / rhs.get ());
	return lhs;
}
template <typename T>
SiTracedVariable<T> &operator <<= (SiTracedVariable<T> &lhs, SiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () << rhs.get ());
	return lhs;
}
template <typename T>
SiTracedVariable<T> &operator >>= (SiTracedVariable<T> &lhs, SiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () >> rhs.get ());
	return lhs;
}
template <typename T>
SiTracedVariable<T> &operator &= (SiTracedVariable<T> &lhs, SiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () & rhs.get ());
	return lhs;
}
template <typename T>
SiTracedVariable<T> &operator |= (SiTracedVariable<T> &lhs, SiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () | rhs.get ());
	return lhs;
}
template <typename T>
SiTracedVariable<T> &operator ^= (SiTracedVariable<T> &lhs, SiTracedVariable<T> const &rhs) {
	lhs.assign (lhs.get () ^ rhs.get ());
	return lhs;
}


template <typename T, typename U>
SiTracedVariable<T> &operator += (SiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () + rhs);
	return lhs;
}
template <typename T, typename U>
SiTracedVariable<T> &operator -= (SiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () - rhs);
	return lhs;
}
template <typename T, typename U>
SiTracedVariable<T> &operator *= (SiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () * rhs);
	return lhs;
}
template <typename T, typename U>
SiTracedVariable<T> &operator /= (SiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () / rhs);
	return lhs;
}
template <typename T, typename U>
SiTracedVariable<T> &operator <<= (SiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () << rhs);
	return lhs;
}
template <typename T, typename U>
SiTracedVariable<T> &operator >>= (SiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () >> rhs);
	return lhs;
}
template <typename T, typename U>
SiTracedVariable<T> &operator &= (SiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () & rhs);
	return lhs;
}
template <typename T, typename U>
SiTracedVariable<T> &operator |= (SiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () | rhs);
	return lhs;
}
template <typename T, typename U>
SiTracedVariable<T> &operator ^= (SiTracedVariable<T> &lhs, U const &rhs) {
	lhs.assign (lhs.get () ^ rhs);
	return lhs;
}

}; // namespace yans

#endif /* TRACED_VARIABLE_TCC */
