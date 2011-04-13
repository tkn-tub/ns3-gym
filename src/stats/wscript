## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def configure(conf):
   conf.env['SQLITE_STATS'] = conf.check(lib='sqlite3', define_name='SQLITE3', uselib='SQLITE3')
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
        ]
    headers = bld.new_task_gen('ns3header')
    headers.module = 'stats'
    headers.source = [
        'model/data-calculator.h',
        'model/packet-data-calculators.h',
        'model/time-data-calculators.h',
        'model/basic-data-calculators.h',
        'model/data-output-interface.h',
        'model/omnet-data-output.h',
        'model/data-collector.h',
        ]

    if bld.env['SQLITE_STATS']:
        headers.source.append('model/sqlite-data-output.h')
        obj.source.append('model/sqlite-data-output.cc')
        obj.uselib = 'SQLITE3'

    bld.ns3_python_bindings()
