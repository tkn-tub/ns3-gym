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
#include "reference-list.h"
#include "test.h"

#ifdef RUN_SELF_TESTS

#define noREFTEST_DEBUG 1

#ifdef REFTEST_DEBUG 
#include <iostream>
#define TRACE(x) \
	std::cout << x << std::endl;
#else
#define TRACE(x)
#endif

namespace {

class A {
public:
	A () {
		TRACE ("constructor");
	}
	~A () {
		TRACE ("destructor");
	}
	void trace (void) {
		TRACE ("trace");
	}
};

class RefTest : public ns3::Test {
public:
	RefTest ();
	virtual bool runTests (void);
private:
	void test (ns3::ReferenceList<A *>);
};

RefTest::RefTest ()
	: ns3::Test ("ReferenceList")
{}

void
RefTest::test (ns3::ReferenceList<A *> a) 
{
	a->trace ();
}

bool 
RefTest::runTests (void)
{
	bool ok = true;

	{
		ns3::ReferenceList<A *> tmp;
		{
			ns3::ReferenceList<A *> a (new A ());
			
			test (a);
			tmp = a;
			test (tmp);
			a = tmp;
			test (a);
			TRACE ("leave inner scope");
		}
		test (tmp);
		TRACE ("leave outer scope");
	}

	{
		ns3::ReferenceList<A *> tmp;
	}

	{
		ns3::ReferenceList<A *> tmp (new A ());
	}

	{
		ns3::ReferenceList<A *> tmp;
		tmp.set (new A ());
	}

	{
		TRACE ("test assignement");
		ns3::ReferenceList<A *> a0 (new A ());
		ns3::ReferenceList<A *> a1 (new A ());
		a0 = a1;
	}



	return ok;
}


static RefTest gRefTest = RefTest ();

}; // namespace

#endif /* RUN_SELF_TESTS */
