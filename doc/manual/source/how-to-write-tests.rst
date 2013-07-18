.. include:: replace.txt
.. highlight:: cpp

How to write tests
------------------

A primary goal of the ns-3 project is to help users to improve the 
validity and credibility of their results.  There are many elements
to obtaining valid models and simulations, and testing is a major
component.  If you contribute models or examples to ns-3, you may
be asked to contribute test code.  Models that you contribute will be
used for many years by other people, who probably have no idea upon
first glance whether the model is correct.  The test code that you
write for your model will help to avoid future regressions in
the output and will aid future users in understanding the verification
and bounds of applicability of your models.

There are many ways to verify the correctness of a model's implementation.
In this section,
we hope to cover some common cases that can be used as a guide to
writing new tests.

Sample TestSuite skeleton
*************************

When starting from scratch (i.e. not adding a TestCase to an existing
TestSuite), these things need to be decided up front:

* What the test suite will be called 
* What type of test it will be (Build Verification Test, Unit Test,
  System Test, or Performance Test)
* Where the test code will live (either in an existing ns-3 module or
  separately in src/test/ directory).  You will have to edit the wscript
  file in that directory to compile your new code, if it is a new file.

A program called ``src/create-module.py`` is a good starting point.
This program can be invoked such as ``create-module.py router`` for
a hypothetical new module called ``router``.  Once you do this, you
will see a ``router`` directory, and a ``test/router-test-suite.cc`` 
test suite.  This file can be a starting point for your initial test.
This is a working test suite, although the actual tests performed are
trivial.  Copy it over to your module's test directory, and do a global
substitution of "Router" in that file for something pertaining to
the model that you want to test.  You can also edit things such as a
more descriptive test case name. 

You also need to add a block into your wscript to get this test to
compile:

.. sourcecode:: python

    module_test.source = [
        'test/router-test-suite.cc',
        ]

Before you actually start making this do useful things, it may help
to try to run the skeleton.  Make sure that ns-3 has been configured with
the "--enable-tests" option.  Let's assume that your new test suite
is called "router" such as here:

::

  RouterTestSuite::RouterTestSuite ()
    : TestSuite ("router", UNIT)

Try this command:

.. sourcecode:: bash

  $ ./test.py -s router

Output such as below should be produced:

.. sourcecode:: text

  PASS: TestSuite router
  1 of 1 tests passed (1 passed, 0 skipped, 0 failed, 0 crashed, 0 valgrind errors)

See src/lte/test/test-lte-antenna.cc for a worked example.

Test macros
***********

There are a number of macros available for checking test program
output with expected output.  These macros are defined in 
``src/core/model/test.h``.

The main set of macros that are used include the following:

::

  NS_TEST_ASSERT_MSG_EQ(actual, limit, msg)
  NS_TEST_ASSERT_MSG_NE(actual, limit, msg)
  NS_TEST_ASSERT_MSG_LT(actual, limit, msg)
  NS_TEST_ASSERT_MSG_GT(actual, limit, msg)
  NS_TEST_ASSERT_MSG_EQ_TOL(actual, limit, tol, msg)  

The first argument ``actual`` is the value under test, the second value
``limit`` is the expected value (or the value to test against), and the
last argument ``msg`` is the error message to print out if the test fails.

The first four macros above test for equality, inequality, less than, or
greater than, respectively.  The fifth macro above tests for equality,
but within a certain tolerance.  This variant is useful when testing
floating point numbers for equality against a limit, where you want to
avoid a test failure due to rounding errors.

Finally, there are variants of the above where the keyword ``ASSERT``
is replaced by ``EXPECT``.  These variants are designed specially for
use in methods (especially callbacks) returning void.  Reserve their
use for callbacks that you use in your test programs; otherwise,
use the ``ASSERT`` variants.

How to add an example program to the test suite
***********************************************

One can "smoke test" that examples compile and run successfully
to completion (without memory leaks) using the ``examples-to-run.py``
script located in your module's test directory.  Briefly, by including
an instance of this file in your test directory, you can cause the
test runner to execute the examples listed.  It is usually best to make 
sure that you select examples that have reasonably short run times so as
to not bog down the tests.  See the example in ``src/lte/test/`` 
directory.

Testing for boolean outcomes
****************************

Testing outcomes when randomness is involved
********************************************

Testing output data against a known distribution
************************************************

Providing non-trivial input vectors of data
*******************************************

Storing and referencing non-trivial output data
***********************************************

Presenting your output test data
********************************

