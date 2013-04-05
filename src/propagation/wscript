## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def build(bld):
    module = bld.create_ns3_module('propagation', ['network', 'mobility'])
    module.includes = '.'
    module.source = [
        'model/propagation-delay-model.cc',
        'model/propagation-loss-model.cc',
        'model/jakes-propagation-loss-model.cc',
        'model/jakes-process.cc',
        'model/cost231-propagation-loss-model.cc',
        'model/okumura-hata-propagation-loss-model.cc',
        'model/itu-r-1411-los-propagation-loss-model.cc',
        'model/itu-r-1411-nlos-over-rooftop-propagation-loss-model.cc',
        'model/kun-2600-mhz-propagation-loss-model.cc',
        ]

    module_test = bld.create_ns3_module_test_library('propagation')
    module_test.source = [
        'test/propagation-loss-model-test-suite.cc',
        'test/okumura-hata-test-suite.cc',
        'test/itu-r-1411-los-test-suite.cc',
        'test/kun-2600-mhz-test-suite.cc',
        'test/itu-r-1411-nlos-over-rooftop-test-suite.cc',
        ]

    headers = bld(features='ns3header')
    headers.module = 'propagation'
    headers.source = [
        'model/propagation-delay-model.h',
        'model/propagation-loss-model.h',
        'model/jakes-propagation-loss-model.h',
        'model/jakes-process.h',
        'model/propagation-cache.h',
        'model/cost231-propagation-loss-model.h',
        'model/propagation-environment.h',
        'model/okumura-hata-propagation-loss-model.h',
        'model/itu-r-1411-los-propagation-loss-model.h',
        'model/itu-r-1411-nlos-over-rooftop-propagation-loss-model.h',
        'model/kun-2600-mhz-propagation-loss-model.h',
        ]

    if (bld.env['ENABLE_EXAMPLES']):
        bld.recurse('examples')

    bld.ns3_python_bindings()
