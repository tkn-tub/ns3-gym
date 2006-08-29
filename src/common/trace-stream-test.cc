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
#include "trace-stream.h"
#include "yans/test.h"
#include <iostream>

#ifdef RUN_SELF_TESTS

namespace {

class TestTraceStream : public yans::Test {
public:
	TestTraceStream ();
	virtual bool run_tests (void);
};

static TestTraceStream g_test_stream;

TestTraceStream::TestTraceStream ()
	: Test ("TraceStream")
{}

bool
TestTraceStream::run_tests (void)
{
	bool ok = true;
	yans::TraceStream trace;
	//trace.set_stream (&std::cout);
	trace << 1;
	trace << " X ";
	trace << 1.0;
	trace << std::endl;
	trace << "test ";
	trace << 1 << " test";
	trace << "test "
	      << 1.0 << " "
	      << 0xdeadbead
	      << std::endl;
	trace << "0x" << std::hex 
	      << 0xdeadbeaf 
	      << std::dec << " "
	      << 0xdeadbeaf
	      << std::endl;
	return ok;
}


}; // namespace yans

#endif /* RUN_SELF_TESTS */
