#! /usr/bin/env python
import sys
from optparse import OptionParser
import os


WSCRIPT_TEMPLATE = '''# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module(%(MODULE)r, ['core'])
    module.source = [
        'model/%(MODULE)s.cc',
        'helper/%(MODULE)s-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('%(MODULE)s')
    module_test.source = [
        'test/%(MODULE)s-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = %(MODULE)r
    headers.source = [
        'model/%(MODULE)s.h',
        'helper/%(MODULE)s-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

'''



MODEL_CC_TEMPLATE = '''/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "%(MODULE)s.h"

namespace ns3 {

/* ... */


}

'''



MODEL_H_TEMPLATE = '''/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef %(INCLUDE_GUARD)s
#define %(INCLUDE_GUARD)s

namespace ns3 {

/* ... */

}

#endif /* %(INCLUDE_GUARD)s */

'''



HELPER_CC_TEMPLATE = '''/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "%(MODULE)s-helper.h"

namespace ns3 {

/* ... */


}

'''



HELPER_H_TEMPLATE = '''/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef %(INCLUDE_GUARD)s
#define %(INCLUDE_GUARD)s

#include "ns3/%(MODULE)s.h"

namespace ns3 {

/* ... */

}

#endif /* %(INCLUDE_GUARD)s */

'''


EXAMPLES_WSCRIPT_TEMPLATE = '''# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    obj = bld.create_ns3_program('%(MODULE)s-example', [%(MODULE)r])
    obj.source = '%(MODULE)s-example.cc'

'''

EXAMPLE_CC_TEMPLATE = '''/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/%(MODULE)s-helper.h"

using namespace ns3;


int 
main (int argc, char *argv[])
{
  bool verbose = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell application to log if true", verbose);

  cmd.Parse (argc,argv);

  /* ... */

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}


'''


TEST_CC_TEMPLATE = '''/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

// Include a header file from your module to test.
#include "ns3/%(MODULE)s.h"

// An essential include is test.h
#include "ns3/test.h"

// Do not put your test classes in namespace ns3.  You may find it useful
// to use the using directive to access the ns3 namespace directly
using namespace ns3;

// This is an example TestCase.
class %(CAPITALIZED)sTestCase1 : public TestCase
{
public:
  %(CAPITALIZED)sTestCase1 ();
  virtual ~%(CAPITALIZED)sTestCase1 ();

private:
  virtual void DoRun (void);
};

// Add some help text to this case to describe what it is intended to test
%(CAPITALIZED)sTestCase1::%(CAPITALIZED)sTestCase1 ()
  : TestCase ("%(CAPITALIZED)s test case (does nothing)")
{
}

// This destructor does nothing but we include it as a reminder that
// the test case should clean up after itself
%(CAPITALIZED)sTestCase1::~%(CAPITALIZED)sTestCase1 ()
{
}

//
// This method is the pure virtual method from class TestCase that every
// TestCase must implement
//
void
%(CAPITALIZED)sTestCase1::DoRun (void)
{
  // A wide variety of test macros are available in src/core/test.h
  NS_TEST_ASSERT_MSG_EQ (true, true, "true doesn't equal true for some reason");
  // Use this one for floating point comparisons
  NS_TEST_ASSERT_MSG_EQ_TOL (0.01, 0.01, 0.001, "Numbers are not equal within tolerance");
}

// The TestSuite class names the TestSuite, identifies what type of TestSuite,
// and enables the TestCases to be run.  Typically, only the constructor for
// this class must be defined
//
class %(CAPITALIZED)sTestSuite : public TestSuite
{
public:
  %(CAPITALIZED)sTestSuite ();
};

%(CAPITALIZED)sTestSuite::%(CAPITALIZED)sTestSuite ()
  : TestSuite ("%(MODULE)s", UNIT)
{
  // TestDuration for TestCase can be QUICK, EXTENSIVE or TAKES_FOREVER
  AddTestCase (new %(CAPITALIZED)sTestCase1, TestCase::QUICK);
}

// Do not forget to allocate an instance of this TestSuite
static %(CAPITALIZED)sTestSuite %(COMPOUND)sTestSuite;

'''


DOC_RST_TEMPLATE = '''Example Module Documentation
----------------------------

.. include:: replace.txt

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

This is a suggested outline for adding new module documentation to |ns3|.
See ``src/click/doc/click.rst`` for an example.

The introductory paragraph is for describing what this code is trying to
model.

For consistency (italicized formatting), please use |ns3| to refer to
ns-3 in the documentation (and likewise, |ns2| for ns-2).  These macros
are defined in the file ``replace.txt``.

Model Description
*****************

The source code for the new module lives in the directory ``src/%(MODULE)s``.

Add here a basic description of what is being modeled.

Design
======

Briefly describe the software design of the model and how it fits into 
the existing ns-3 architecture. 

Scope and Limitations
=====================

What can the model do?  What can it not do?  Please use this section to
describe the scope and limitations of the model.

References
==========

Add academic citations here, such as if you published a paper on this
model, or if readers should read a particular specification or other work.

Usage
*****

This section is principally concerned with the usage of your model, using
the public API.  Focus first on most common usage patterns, then go
into more advanced topics.

Building New Module
===================

Include this subsection only if there are special build instructions or
platform limitations.

Helpers
=======

What helper API will users typically use?  Describe it here.

Attributes
==========

What classes hold attributes, and what are the key ones worth mentioning?

Output
======

What kind of data does the model generate?  What are the key trace
sources?   What kind of logging output can be enabled?

Advanced Usage
==============

Go into further details (such as using the API outside of the helpers)
in additional sections, as needed.

Examples
========

What examples using this new code are available?  Describe them here.

Troubleshooting
===============

Add any tips for avoiding pitfalls, etc.

Validation
**********

Describe how the model has been tested/validated.  What tests run in the
test suite?  How much API and code is covered by the tests?  Again, 
references to outside published work may help here.
'''


def main(argv):
    parser = OptionParser(usage=("Usage: %prog [options] modulename\n"
                                 "Utility script to create a basic template for a new ns-3 module"))
    (options, args) = parser.parse_args()
    if len(args) != 1:
        parser.print_help()
        return 1

    modname = args[0].lower()
    if False in [word.isalnum() for word in modname.split("-")]:
        print >> sys.stderr, "Module name should only contain alphanumeric characters and dashes"
        return 2
    assert os.path.sep not in modname

    moduledir = os.path.join(os.path.dirname(__file__), modname)

    if os.path.exists(moduledir):
        print >> sys.stderr, "Module %r already exists" % (modname,)
        return 2

    print("Creating module %r, "
          "run './waf configure' to include it in the build" % (modname,))

    os.mkdir(moduledir)
    wscript = file(os.path.join(moduledir, "wscript"), "wt")
    wscript.write(WSCRIPT_TEMPLATE % dict(MODULE=modname))
    wscript.close()


    #
    # model
    # 
    modeldir = os.path.join(moduledir, "model")
    os.mkdir(modeldir)

    model_cc = file(os.path.join(moduledir, "model", "%s.cc" % modname), "wt")
    model_cc.write(MODEL_CC_TEMPLATE % dict(MODULE=modname))
    model_cc.close()

    model_h = file(os.path.join(moduledir, "model", "%s.h" % modname), "wt")
    model_h.write(MODEL_H_TEMPLATE % dict(MODULE=modname, INCLUDE_GUARD="%s_H" % (modname.replace("-", "_").upper()),))
    model_h.close()



    #
    # test
    # 
    testdir = os.path.join(moduledir, "test")
    os.mkdir(testdir)
    test_cc = file(os.path.join(moduledir, "test", "%s-test-suite.cc" % modname), "wt")
    test_cc.write(TEST_CC_TEMPLATE % dict(MODULE=modname,
                                          CAPITALIZED=''.join([word.capitalize() for word in modname.split('-')]),
                                          COMPOUND=''.join([modname.split('-')[0]] + [word.capitalize() for word in modname.split('-')[1:]]),
                                          ))
    test_cc.close()



    #
    # helper
    # 
    helperdir = os.path.join(moduledir, "helper")
    os.mkdir(helperdir)

    helper_cc = file(os.path.join(moduledir, "helper", "%s-helper.cc" % modname), "wt")
    helper_cc.write(HELPER_CC_TEMPLATE % dict(MODULE=modname))
    helper_cc.close()

    helper_h = file(os.path.join(moduledir, "helper", "%s-helper.h" % modname), "wt")
    helper_h.write(HELPER_H_TEMPLATE % dict(MODULE=modname, INCLUDE_GUARD="%s_HELPER_H" % (modname.replace("-", "_").upper()),))
    helper_h.close()

    #
    # examples
    #
    examplesdir = os.path.join(moduledir, "examples")
    os.mkdir(examplesdir)

    examples_wscript = file(os.path.join(examplesdir, "wscript"), "wt")
    examples_wscript.write(EXAMPLES_WSCRIPT_TEMPLATE % dict(MODULE=modname))
    examples_wscript.close()

    example_cc = file(os.path.join(moduledir, "examples", "%s-example.cc" % modname), "wt")
    example_cc.write(EXAMPLE_CC_TEMPLATE % dict(MODULE=modname))
    example_cc.close()

    #
    # doc
    # 
    docdir = os.path.join(moduledir, "doc")
    os.mkdir(docdir)

    doc_rst = file(os.path.join(moduledir, "doc", "%s.rst" % modname), "wt")
    doc_rst.write(DOC_RST_TEMPLATE % dict(MODULE=modname))
    doc_rst.close()


    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
