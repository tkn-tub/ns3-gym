## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
import os.path
import build

ns3 = build.Ns3()
ns3.build_dir = 'build-dir'
ns3.version = '0.0.1'
ns3.name = 'ns3'
ns3.doxygen_config = os.path.join('doc', 'doxygen.conf')
ns3.add_extra_dist(os.path.join('doc', 'main.txt'))


#
# The Core module
#
core = build.Ns3Module('core', 'src/core')
ns3.add(core)
core.add_sources([
    'reference-list-test.cc',
    'callback-test.cc',
    'ptr.cc',
    'test.cc'
    ])
env = Environment()
if env['PLATFORM'] == 'posix' or env['PLATFORM'] == 'darwin':
    core.add_external_dep('pthread')
    core.add_sources([
        'unix-system-wall-clock-ms.cc',
        'unix-system-file.cc'
        ])
elif env['PLATFORM'] == 'win32':
    core.add_sources([
        'win32-system-wall-clock-ms.cc',
        'win32-system-file.cc'
        ])
core.add_inst_headers([
    'system-file.h',
    'system-wall-clock-ms.h',
    'reference-list.h',
    'callback.h',
    'ptr.h',
    'test.h'
    ])


#
# The Simu module
#
simu = build.Ns3Module('simulator', 'src/simulator')
ns3.add(simu)
simu.add_dep('core')
simu.add_external_dep('m')
simu.add_sources([
    'high-precision.cc',
    'time.cc',
    'event-id.cc',
    'scheduler.cc',
    'scheduler-factory.cc',
    'scheduler-list.cc',
    'scheduler-heap.cc',
    'scheduler-map.cc',
    'event-impl.cc',
    'simulator.cc',
    ])
simu.add_headers([
    'scheduler-heap.h',
    'scheduler-map.h',
    'scheduler-list.h'
    ])
simu.add_inst_headers([
    'high-precision.h',
    'nstime.h',
    'event-id.h',
    'event-impl.h',
    'simulator.h',
    'scheduler.h',
    'scheduler-factory.h',
    ])
high_precision_as_double = ARGUMENTS.get('high-precision-as-double', 'n')
if high_precision_as_double == 'y':
    simu.add_inst_header ('high-precision-double.h')
    simu.add_source ('high-precision-double.cc')
else:
    simu.add_inst_headers ([
        'high-precision-128.h',
        'cairo-wideint-private.h'
        ])
    simu.add_sources ([
        'high-precision-128.cc',
        'cairo-wideint.c',
        ])

def config_simulator (env, config):
    retval = []
    high_precision_as_double = ARGUMENTS.get('high-precision-as-double', 'n')
    if high_precision_as_double == 'y':
        retval.append ('#define USE_HIGH_PRECISION_DOUBLE 1')
    else:
        retval.append ('#undef USE_HIGH_PRECISION_DOUBLE')
    if config.CheckCHeader ('stdint.h') == 1:
        retval.append ('#define HAVE_STDINT_H 1')
    elif config.CheckCHeader ('inttypes.h') == 1:
        retval.append ('#define HAVE_INTTYPES_H 1')
    elif config.CheckCHeader ('sys/inttypes.h') == 1:
        retval.append ('#define HAVE_SYS_INT_TYPES_H 1')
    return retval
simu.add_config (config_simulator)
    

#
# The Common module
#
common = build.Ns3Module('common', 'src/common')
common.add_deps(['core', 'simulator'])
ns3.add(common)
common.add_sources([
    'buffer.cc',
    'header.cc',
    'trailer.cc',
    'packet.cc',
    'tags.cc',
    'pcap-writer.cc',
    'trace-container.cc',
    'variable-tracer-test.cc',
    'stream-tracer-test.cc',
    ])
common.add_inst_headers([
    'buffer.h',
    'header.h',
    'trailer.h',
    'tags.h',
    'packet.h',
    'ui-variable-tracer.h',
    'si-variable-tracer.h',
    'f-variable-tracer.h',
    'callback-tracer.h',
    'stream-tracer.h',
    'trace-container.h',
    'pcap-writer.h',
    ])

node = build.Ns3Module ('node', 'src/node')
ns3.add (node)
node.add_deps (['core'])
node.add_sources ([
    'node.cc',
    'l3-demux.cc',
    'l3-protocol.cc',
    'ipv4-l3-protocol.cc',
    'ipv4-l4-demux.cc',
    'ipv4-l4-protocol.cc',
    'udp-ipv4-l4-protocol.cc',
    'ipv4-address.cc',
    'internet-node.cc',
    'net-device.cc',
    'mac-address.cc',
    'ipv4-header.cc',
    'udp-header.cc',
    'ipv4-checksum.cc',
    'ipv4-route.cc',
    'ipv4-interface.cc',
    'ipv4.cc',
    'ipv4-end-point.cc',
    'udp-end-point.cc',
    'udp-socket.cc',
    'udp.cc',
    'arp-header.cc',
    'arp-l3-protocol.cc',
    'arp-cache.cc',
    'arp-ipv4-interface.cc',
    'arp.cc',
    ])
node.add_headers ([
    'ipv4-address.h',
    'ipv4-header.h',
    'udp-header.h',
    'ipv4-checksum.h',
    'ipv4.h',
    'udp.h',
    'ipv4-l4-protocol.h',
    'udp-ipv4-l4-protocol.h',
    'ipv4-l3-protocol.h',
    'arp-l3-protocol.h',
    'arp-header.h',
    'arp-cache-cache.h',
    'arp-ipv4-interface.h',
    'arp.h',
    ])
node.add_inst_headers ([
    'node.h',
    'l3-demux.h',
    'l3-protocol.h',
    'ipv4-l4-demux.h',
    'net-device-list.h',
    'internet-node.h',
    'net-device.h',
    'mac-address.h',
    'ipv4-route.h',
    'ipv4-interface.h',
    'udp-socket.h',
    ])


# utils
run_tests = build.Ns3Module('run-tests', 'utils')
ns3.add(run_tests)
run_tests.set_executable()
run_tests.add_deps(['core', 'simulator', 'common'])
run_tests.add_source('run-tests.cc')

bench_packets = build.Ns3Module('bench-packets', 'utils')
#ns3.add(bench_packets)
bench_packets.set_executable()
bench_packets.add_dep('core')
bench_packets.add_source('bench-packets.cc')

bench_simu = build.Ns3Module('bench-simulator', 'utils')
ns3.add(bench_simu)
bench_simu.set_executable()
bench_simu.add_dep('simulator')
bench_simu.add_source('bench-simulator.cc')

replay_simu = build.Ns3Module('replay-simulation', 'utils')
ns3.add(replay_simu)
replay_simu.set_executable()
replay_simu.add_dep('simulator')
replay_simu.add_source('replay-simulation.cc')


# samples
sample_callback = build.Ns3Module('sample-callback', 'samples')
sample_callback.set_executable()
ns3.add(sample_callback)
sample_callback.add_dep('core')
sample_callback.add_source('main-callback.cc')

sample_ptr = build.Ns3Module('sample-ptr', 'samples')
sample_ptr.set_executable()
ns3.add(sample_ptr)
sample_ptr.add_dep('core')
sample_ptr.add_source('main-ptr.cc')

sample_trace = build.Ns3Module('sample-trace', 'samples')
ns3.add(sample_trace)
sample_trace.add_dep('common')
sample_trace.set_executable()
sample_trace.add_source('main-trace.cc')

sample_simu = build.Ns3Module('sample-simulator', 'samples')
ns3.add(sample_simu)
sample_simu.set_executable()
sample_simu.add_dep('simulator')
sample_simu.add_source('main-simulator.cc')

sample_packet = build.Ns3Module('sample-packet', 'samples')
ns3.add(sample_packet)
sample_packet.set_executable()
sample_packet.add_dep('common')
sample_packet.add_source('main-packet.cc')

sample_test = build.Ns3Module('sample-test', 'samples')
sample_test.set_executable()
ns3.add(sample_test)
sample_test.add_dep('core')
sample_test.add_source('main-test.cc')


ns3.generate_dependencies()
