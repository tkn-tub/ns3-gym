.. include:: replace.txt

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
the output and will aid future users in understanding the validity
and bounds of applicability of your models.

There are many ways to test that a model is valid.  In this chapter,
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

See the file ``src/template/test/sample-test-suite.cc`` and corresponding
wscript file in that directory for a simple example, and see the directories
under ``src/test`` for more complicated examples.

*The rest of this chapter remains to be written*

How to add an example program to the test suite
***********************************************

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

