/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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

#ifndef TEST_H
#define TEST_H

#include <list>
#include <string>
#include <utility>
#include <ostream>

#ifdef RUN_SELF_TESTS

namespace ns3 {

class TestManager;

/**
 * \brief base class for new regressions tests
 *
 * To add a new regression test, you need to:
 *    - create subclass of this abstract base class
 *    - instantiate once this subclass through a static
 *      variable.
 *
 * The following sample code shows you how to do this:
 * \include samples/main-test.cc
 */
class Test {
public:
	/**
	 * \param name the name of the test
	 */
    Test (char const *name);
    virtual ~Test ();

	/**
	 * \returns true if the test was successful, false otherwise.
	 */
    virtual bool runTests (void) = 0;

protected:
	/**
	 * \returns an output stream which base classes can write to
	 *          to return extra information on test errors.
	 */
    std::ostream &failure (void);
};

/**
 * \brief gather and run all regression tests
 */
class TestManager {
public:
	/**
	 * Enable verbose output. If you do not enable verbose output,
	 * nothing is printed on screen during the test runs.
	 */
    static void enableVerbose (void);
	/**
	 * \returns true if all tests passed, false otherwise.
	 *
	 * run all registered regression tests
	 */
    static bool runTests (void);

private:
	friend class Test;
    static void add (Test *test, char const *name);
    static std::ostream &failure (void);
    static TestManager *get (void);
    bool realRunTests (void);

    TestManager ();
    ~TestManager ();

    typedef std::list<std::pair<Test *,std::string *> > Tests;
    typedef std::list<std::pair<Test *,std::string *> >::iterator TestsI;
    typedef std::list<std::pair<Test *,std::string *> >::const_iterator TestsCI;

    Tests m_tests;
    bool m_verbose;
};
}; // namespace ns3 

#endif /* RUN_SELF_TESTS */

#endif /* TEST_H */
