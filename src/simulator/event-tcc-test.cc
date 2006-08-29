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
#include "event.tcc"
#include "ns3/test.h"

#ifdef RUN_SELF_TESTS

#define ENSURE(got,expected) \
if (got != expected) { \
	g_error = true; \
}

namespace {
bool g_error = false;

void null_cb (void)
{}
void one_cb (int a)
{
	ENSURE (a, 1);
}
void two_cb (int a,int b)
{
	ENSURE (a, 1);
	ENSURE (b, 2);
}
void three_cb (int a,int b,int c)
{
	ENSURE (a, 1);
	ENSURE (b, 2);
	ENSURE (c, 3);
}
void four_cb (int a,int b,int c,int d)
{
	ENSURE (a, 1);
	ENSURE (b, 2);
	ENSURE (c, 3);
	ENSURE (d, 4);
}
void five_cb (int a,int b,int c,int d,int e)
{
	ENSURE (a, 1);
	ENSURE (b, 2);
	ENSURE (c, 3);
	ENSURE (d, 4);
	ENSURE (e, 5);
}

};

namespace ns3 {
class EventTest : public Test {
public:
	EventTest ();
	virtual bool run_tests (void);
};

EventTest::EventTest ()
	: Test ("Event")
{}

bool 
EventTest::run_tests (void)
{
	Event ev;

	ev = ns3::make_event (&null_cb);
	ev ();
	ev = ns3::make_event (&one_cb, 1);
	ev ();
	ev = ns3::make_event (&two_cb, 1, 2);
	ev ();
	ev = ns3::make_event (&three_cb, 1, 2, 3);
	ev ();
	ev = ns3::make_event (&four_cb, 1, 2, 3, 4);
	ev ();
	ev = ns3::make_event (&five_cb, 1, 2, 3, 4, 5);
	ev ();

	if (g_error) {
		return false;
	}
	return true;
}

static EventTest g_test;

};



#endif /* RUN_SELF_TESTS */
