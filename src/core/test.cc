/* -*- Mode:NS3; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifdef RUN_SELF_TESTS
#include <iostream>

namespace ns3 {

TestManager *
TestManager::Get (void)
{
    static TestManager manager;
    return &manager;
}

TestManager::TestManager ()
    : m_verbose (false)
{}

TestManager::~TestManager ()
{
    TestsI i = m_tests.begin ();
    while (i != m_tests.end ()) {
        delete (*i).second;
        i = m_tests.erase (i);
    }
}
void
TestManager::Add (Test *test, char const *name)
{
    Get ()->m_tests.push_back (std::make_pair (test, new std::string (name)));
}
void
TestManager::EnableVerbose (void)
{
    Get ()->m_verbose = true;
}
std::ostream &
TestManager::Failure (void)
{
    return std::cerr;
}
bool 
TestManager::RunTests (void)
{
    return Get ()->RealRunTests ();
}
bool 
TestManager::RealRunTests (void)
{
    bool isSuccess = true;
    for (TestsCI i = m_tests.begin (); i != m_tests.end (); i++) {
        std::string *testName = (*i).second;
        if (!(*i).first->RunTests ()) {
            isSuccess = false;
            if (m_verbose) {
                std::cerr << "FAIL " << *testName << std::endl;
            }
        } else {
            if (m_verbose) {
                std::cerr << "PASS "<<*testName << std::endl;
            }
        }
    }
    if (!isSuccess) {
        std::cerr << "FAIL" << std::endl;
    }
    return isSuccess;
}

Test::Test (char const *name)
{
    TestManager::Add (this, name);
}

Test::~Test ()
{}

std::ostream &
Test::Failure (void)
{
    return TestManager::Failure ();
}

}; // namespace ns3

#endif /* RUN_SELF_TESTS */
