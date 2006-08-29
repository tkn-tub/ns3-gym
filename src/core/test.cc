/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
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

namespace yans {

TestManager *
TestManager::get (void)
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
TestManager::add (Test *test, char const *name)
{
	get ()->m_tests.push_back (std::make_pair (test, new std::string (name)));
}
void
TestManager::enable_verbose (void)
{
	get ()->m_verbose = true;
}
std::ostream &
TestManager::failure (void)
{
	return std::cerr;
}
bool 
TestManager::run_tests (void)
{
	return get ()->real_run_tests ();
}
bool 
TestManager::real_run_tests (void)
{
	bool is_success = true;
	for (TestsCI i = m_tests.begin (); i != m_tests.end (); i++) {
		std::string *test_name = (*i).second;
		if (!(*i).first->run_tests ()) {
			is_success = false;
			if (m_verbose) {
				std::cerr << "FAIL " << *test_name << std::endl;
			}
		} else {
			if (m_verbose) {
				std::cerr << "PASS "<<*test_name << std::endl;
			}
		}
	}
	if (!is_success) {
		std::cerr << "FAIL" << std::endl;
	}
	return is_success;
}

Test::Test (char const *name)
{
	TestManager::add (this, name);
}

Test::~Test ()
{}

std::ostream &
Test::failure (void)
{
	return TestManager::failure ();
}

}; // namespace yans

#endif /* RUN_SELF_TESTS */
