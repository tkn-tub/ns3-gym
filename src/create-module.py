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

    headers = bld.new_task_gen(features=['ns3header'])
    headers.module = %(MODULE)r
    headers.source = [
        'model/%(MODULE)s.h',
        'helper/%(MODULE)s-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.add_subdirs('examples')

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


def main(argv):
    parser = OptionParser(usage=("Usage: %prog [options] modulename\n"
                                 "Utility script to create a basic template for a new ns-3 module"))
    (options, args) = parser.parse_args()
    if len(args) != 1:
        parser.print_help()
        return 1

    modname = args[0]
    assert os.path.sep not in modname

    moduledir = os.path.join(os.path.dirname(__file__), modname)

    if os.path.exists(moduledir):
        print >> sys.stderr, "Module %r already exists" % (modname,)
        return 2

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
    model_h.write(MODEL_H_TEMPLATE % dict(MODULE=modname, INCLUDE_GUARD="__%s_H__" % (modname.upper()),))
    model_h.close()



    #
    # helper
    # 
    helperdir = os.path.join(moduledir, "helper")
    os.mkdir(helperdir)

    helper_cc = file(os.path.join(moduledir, "helper", "%s-helper.cc" % modname), "wt")
    helper_cc.write(HELPER_CC_TEMPLATE % dict(MODULE=modname))
    helper_cc.close()

    helper_h = file(os.path.join(moduledir, "helper", "%s-helper.h" % modname), "wt")
    helper_h.write(HELPER_H_TEMPLATE % dict(MODULE=modname, INCLUDE_GUARD="__%s_HELPER_H__" % (modname.upper()),))
    helper_h.close()


    examplesdir = os.path.join(moduledir, "examples")
    os.mkdir(examplesdir)

    examples_wscript = file(os.path.join(examplesdir, "wscript"), "wt")
    examples_wscript.write(EXAMPLES_WSCRIPT_TEMPLATE % dict(MODULE=modname))
    examples_wscript.close()

    example_cc = file(os.path.join(moduledir, "examples", "%s-example.cc" % modname), "wt")
    example_cc.write(EXAMPLE_CC_TEMPLATE % dict(MODULE=modname))
    example_cc.close()


    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
