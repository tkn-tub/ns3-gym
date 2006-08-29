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

#include "test.h"
#include "callback.h"
#include <stdint.h>

#ifdef RUN_SELF_TESTS

namespace yans {

static bool g_test5 = false;
static bool g_test6 = false;
static bool g_test7 = false;

void test5 (void)
{
	g_test5 = true;
}

void test6 (int)
{
	g_test6 = true;
}

int test7 (int a)
{
	g_test7 = true;
	return a;
}

class CallbackTest : public yans::Test {
private:
	bool m_test1;
	bool m_test2;
	bool m_test3;
	bool m_test4;
public:
	CallbackTest ();
	virtual bool run_tests (void);
	void reset (void);
	bool is_wrong (void);
	void test1 (void);
	int test2 (void);
	void test3 (double a);
	int test4 (double a, int b);
	void test8 (Callback<void, int> callback);
};

CallbackTest::CallbackTest ()
	: yans::Test ("Callback"),
	  m_test1 (false),
	  m_test2 (false),
	  m_test3 (false),
	  m_test4 (false)
{}

void 
CallbackTest::test1 (void)
{
	m_test1 = true;
}
int 
CallbackTest::test2 (void)
{
	m_test2 = true;
	return 2;
}
void 
CallbackTest::test3 (double a)
{
	m_test3 = true;
}
int 
CallbackTest::test4 (double a, int b)
{
	m_test4 = true;
	return 4;
}
void
CallbackTest::test8 (Callback<void,int> callback)
{
	callback (3);
}
bool
CallbackTest::is_wrong (void)
{
	if (!m_test1 ||
	    !m_test2 ||
	    !m_test3 ||
	    !m_test4 ||
	    !g_test5 ||
	    !g_test6 ||
	    !g_test7) {
		return true;
	}
	return false;
}

void
CallbackTest::reset (void)
{
	m_test1 = false;
	m_test2 = false;
	m_test3 = false;
	m_test4 = false;
	g_test5 = false;
	g_test6 = false;
	g_test7 = false;
}

  
bool 
CallbackTest::run_tests (void)
{
	bool ok = true;

	typedef yans::Callback<void> A;
	typedef yans::Callback<int> B;
	typedef yans::Callback<void, double> C;
	typedef yans::Callback<int, double, int> D;
	typedef yans::Callback<void> E;
	typedef yans::Callback<void,int> F;
	typedef yans::Callback<int,int> G;
	
	A a0 (this, &CallbackTest::test1);
	B b0;
	b0 = B (this, &CallbackTest::test2);
	C c0 = C (this, &CallbackTest::test3);
	D d0 = D (this, &CallbackTest::test4);
	E e0 = E (&test5);
	F f0 = F (&test6);
	G g0 = G (&test7);

	a0 ();
	b0 ();
	c0 (0.0);
	d0 (0.0, 1);
	e0 ();
	f0 (1);
	g0 (1);

	if (is_wrong ()) {
		ok = false;
	}

	reset ();

	A a1 = yans::make_callback (&CallbackTest::test1, this);
	B b1 = yans::make_callback (&CallbackTest::test2, this);
	C c1 = yans::make_callback (&CallbackTest::test3, this);
	D d1 = yans::make_callback (&CallbackTest::test4, this);
	E e1 = yans::make_callback (&test5);
	F f1 = yans::make_callback (&test6);
	G g1 = yans::make_callback (&test7);
	
	a1 ();
	b1 ();
	c1 (0.0);
	d1 (0.0, 1);
	e1 ();
	f1 (1);
	g1 (2);

	test8 (f1);

	Callback<void, int64_t,int64_t> a2;

	if (is_wrong ()) {
		ok = false;
	}
	return ok;
}

static CallbackTest g_callback_test;

}; // namespace

#endif /* RUN_SELF_TESTS */
