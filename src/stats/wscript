## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def configure(conf):
    have_sqlite3 = conf.check_cfg(package='sqlite3', uselib_store='SQLITE3',
                                  args=['--cflags', '--libs'],
                                  mandatory=False)

    conf.env['SQLITE_STATS'] = have_sqlite3
    conf.report_optional_feature("SqliteDataOutput", "SQlite stats data output",
                                 conf.env['SQLITE_STATS'],
                                 "library 'sqlite3' not found")

def build(bld):
    obj = bld.create_ns3_module('stats', ['network'])
    obj.source = [
        'model/data-calculator.cc',
        'model/packet-data-calculators.cc',
        'model/time-data-calculators.cc',
        'model/data-output-interface.cc',
        'model/omnet-data-output.cc',
        'model/data-collector.cc',
        'model/gnuplot.cc',
        'helper/delay-jitter-estimation.cc',
        ]

    module_test = bld.create_ns3_module_test_library('stats')
    module_test.source = [
        'test/basic-data-calculators-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'stats'
    headers.source = [
        'model/data-calculator.h',
        'model/packet-data-calculators.h',
        'model/time-data-calculators.h',
        'model/basic-data-calculators.h',
        'model/data-output-interface.h',
        'model/omnet-data-output.h',
        'model/data-collector.h',
        'model/gnuplot.h',
        'helper/delay-jitter-estimation.h',
        ]

    if bld.env['SQLITE_STATS']:
        headers.source.append('model/sqlite-data-output.h')
        obj.source.append('model/sqlite-data-output.cc')
        obj.use.append('SQLITE3')

    if (bld.env['ENABLE_EXAMPLES']):
        bld.recurse('examples')

    bld.ns3_python_bindings()
