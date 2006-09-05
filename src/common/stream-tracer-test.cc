/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:f -*- */
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
#include "stream-tracer.h"
#include "ns3/test.h"
#include <iostream>

#ifdef RUN_SELF_TESTS

namespace {

class TestStreamTracer : public ns3::Test {
public:
    TestStreamTracer ();
    virtual bool runTests (void);
};

static TestStreamTracer gTestStream;

TestStreamTracer::TestStreamTracer ()
    : Test ("StreamTracer")
{}

bool
TestStreamTracer::runTests (void)
{
    bool ok = true;
    ns3::StreamTracer trace;
    //trace.setStream (&std::cout);
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


}; // namespace ns3

#endif /* RUN_SELF_TESTS */
