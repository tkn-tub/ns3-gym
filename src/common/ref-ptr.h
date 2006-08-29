/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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

#ifndef REF_PTR_H
#define REF_PTR_H

namespace yans {

template <typename T>
class RefPtr {
private:
	T *m_env;
public:
	RefPtr () 
		: m_env (0) 
	{}

	RefPtr (T *env) 
		: m_env (env) 
	{}
	RefPtr (RefPtr const&o) 
		: m_env (o.m_env)
	{
		if (m_env != 0) {
			m_env->m_count++;
		}
	}
	// allow conversions from T to T const.
	template <typename U>
	RefPtr (RefPtr<U> const &o)
		: m_env (o.peek ())
	{
		if (m_env != 0) {
			m_env->m_count++;
		}
	}
	~RefPtr () 
	{
		if (m_env != 0) {
			m_env->m_count--;
			if (m_env->m_count == 0) {
				m_env->destroy ();
			}
		}
	}
	RefPtr &operator = (RefPtr const& o) 
	{
		if (o.m_env != 0) {
			o.m_env->m_count++;
		}
		if (m_env != 0) {
			m_env->m_count--;
			if (m_env->m_count == 0) {
				m_env->destroy ();
			}
		}
		m_env = o.m_env;
		return *this;
	}
	T *operator -> () 
	{
		return m_env;
	}
	T *operator -> () const
	{
		return m_env;
	}
	// allow if (!sp)
	bool operator! () 
	{
		return m_env == 0;
	}
private:
	class Tester {
	private:
		void operator delete (void *);
	};
public:
	// allow if (sp)
	operator Tester * () const
	{
		if (m_env == 0) {
			return 0;
		}
		static Tester test;
		return &test;
	}
	// allow if (sp == 0)
	inline friend bool operator == (RefPtr const &lhs, T const *rhs)
	{
		return lhs.m_env == rhs;
	}
	// allow if (0 == sp)
	inline friend bool operator == (T const *lhs, RefPtr &rhs)
	{
		return lhs == rhs.m_env;
	}
	// allow if (sp != 0)
	inline friend bool operator != (RefPtr const &lhs, T const *rhs)
	{
		return lhs.m_env != rhs;
	}
	// allow if (0 != sp)
	inline friend bool operator != (T const *lhs, RefPtr &rhs)
	{
		return lhs != rhs.m_env;
	}
	
	T *peek (void) 
	{
		return m_env;
	}
	T *peek (void) const
	{
		return m_env;
	}
};

}; // namespace yans

#endif /* REF_PTR_H */
