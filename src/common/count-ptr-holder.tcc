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

#ifndef COUNT_PTR_HOLDER_TCC
#define COUNT_PTR_HOLDER_TCC

namespace yans {

template <typename T>
class CountPtrHolder {
public:
	typedef T *real_type;

	CountPtrHolder (T *env) 
		: m_env (env) {
		m_env->ref ();
	}
	~CountPtrHolder () {
		if (m_env != 0) {
			m_env->unref ();
		}
	}
	CountPtrHolder (CountPtrHolder const&o) {
		m_env = o.m_env;
		m_env->ref ();
	}
	T *remove (void) {
		T *env = m_env;
		m_env = 0;
		return env;
	}
private:
	CountPtrHolder ();
	CountPtrHolder &operator = (CountPtrHolder const& o);
	T *m_env;
};

template<typename T>
CountPtrHolder<T>
make_count_ptr_holder (T *t) {
	return CountPtrHolder<T> (t);
}

template<typename T>
CountPtrHolder<T const>
make_const_count_ptr_holder (T *t) {
	return CountPtrHolder<T const> (t);
}

}; // namespace yans

#endif /* REF_HOLDER_TCC */
