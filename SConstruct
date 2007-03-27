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
    'debug.cc',
    'assert.cc',
    'ptr.cc',
    'object.cc',
    'test.cc',
    'random-variable.cc',
    'rng-stream.cc',
    'object-container.cc',
    ])
env = Environment()
if env['PLATFORM'] == 'posix' or env['PLATFORM'] == 'darwin' or env['PLATFORM'] == 'cygwin':
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
    'object.h',
    'debug.h',
    'assert.h',
    'fatal-error.h',
    'test.h',
    'random-variable.h',
    'rng-stream.h',
    'object-container.h'
    ])

def config_core (env, config):
    retval = []
    # XXX This check is primitive but it should be
    # good enough for now.
    if config.CheckCHeader ('stdlib.h') == 1:
        retval.append ('#define HAVE_STDLIB_H 1')
        retval.append ('#define HAVE_GETENV 1')
    else:
        retval.append ('#undef HAVE_STDLIB_H')
        retval.append ('#undef HAVE_GETENV')
    return retval
core.add_config (config_core)

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
    'trace-writer.cc',
    'variable-tracer-test.cc',
    'trace-context.cc',
    'trace-resolver.cc',
    'callback-trace-source.cc',
    'empty-trace-resolver.cc',
    'composite-trace-resolver.cc',
    'trace-root.cc',
    'data-rate.cc',
    ])
common.add_headers ([
    ])
common.add_inst_headers([
    'buffer.h',
    'header.h',
    'trailer.h',
    'tags.h',
    'packet.h',
    'uv-trace-source.h',
    'sv-trace-source.h',
    'fv-trace-source.h',
    'trace-writer.h',
    'pcap-writer.h',
    'callback-trace-source.h',
    'trace-context.h',
    'trace-resolver.h',
    'empty-trace-resolver.h',
    'composite-trace-resolver.h',
    'array-trace-resolver.h',
    'trace-root.h',
    'terminal-trace-resolver.h',
    'smartvector.h',
    'smartset.h',
    'data-rate.h',
    ])

node = build.Ns3Module ('node', 'src/node')
ns3.add (node)
node.add_deps (['core', 'common', 'simulator'])
node.add_sources ([
    'node.cc',
    'l3-demux.cc',
    'l3-protocol.cc',
    'ipv4-l4-demux.cc',
    'ipv4-l4-protocol.cc',
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
    'datagram-socket.cc',
    'udp.cc',
    'arp-header.cc',
    'application.cc',
    'onoff-application.cc',
    'arp-cache.cc',
    'arp-ipv4-interface.cc',
    'arp.cc',
    'ipv4-loopback-interface.cc',
    'llc-snap-header.cc',
    'header-utils.cc',
    'net-device-list.cc',
    'queue.cc',
    'drop-tail.cc',
    'channel.cc',
    'node-list.cc',
    ])
node.add_headers ([
    'ipv4-header.h',
    'udp-header.h',
    'ipv4-checksum.h',
    'udp.h',
    'ipv4-l4-protocol.h',
    'application.h',
    'onoff-application.h',
    'arp-header.h',
    'arp-cache-cache.h',
    'arp.h',
    'ipv4-loopback-interface.h',
    'l3-demux.h',
    'ipv4-l4-demux.h',
    'net-device-list.h',
    'header-utils.h',
    'protocol.h',
    'queue.h',
    ])
node.add_inst_headers ([
    'node.h',
    'internet-node.h',
    'datagram-socket.h',
    'ipv4-address.h',
    'net-device.h',
    'net-device-list.h',
    'arp-ipv4-interface.h',
    'ipv4-interface.h',
    'mac-address.h',
    'ipv4.h',
    'l3-protocol.h',
    'ipv4-route.h',
    'queue.h',
    'drop-tail.h',
    'llc-snap-header.h',
    'arp-header.h',
    'ipv4-header.h',
    'udp-header.h',
    'channel.h',
    'node-list.h',
    'application.h',
    'onoff-application.h',
    ])

p2p = build.Ns3Module ('p2p', 'src/devices/p2p')
#ns3.add (p2p)
p2p.add_deps (['node'])
p2p.add_sources ([
    'p2p-net-device.cc',
    'p2p-channel.cc',
    ])
p2p.add_inst_headers ([
    'p2p-net-device.h',
    'p2p-channel.h',
    ])

p2p = build.Ns3Module ('p2p', 'src/devices/p2p')
ns3.add (p2p)
p2p.add_deps (['node'])
p2p.add_sources ([
    'p2p-net-device.cc',
    'p2p-channel.cc',
    'p2p-topology.cc',
    ])
p2p.add_headers ([
    'propagator.h',
    ])
p2p.add_inst_headers ([
    'p2p-net-device.h',
    'p2p-channel.h',
    'p2p-topology.h',
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
sample_debug = build.Ns3Module('sample-debug', 'samples')
sample_debug.set_executable()
ns3.add(sample_debug)
sample_debug.add_dep('core')
sample_debug.add_source('main-debug.cc')
sample_debug.add_source('main-debug-other.cc')

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
#ns3.add(sample_trace)
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

sample_p2p_net_device_if = build.Ns3Module ('sample-p2p-net-device-if', 'samples')
sample_p2p_net_device_if.set_executable ()
ns3.add (sample_p2p_net_device_if)
sample_p2p_net_device_if.add_deps (['common', 'node', 'p2p'])
sample_p2p_net_device_if.add_source ('main-p2p-net-device-if.cc')

sample_simple = build.Ns3Module('sample-simple', 'samples')
sample_simple.set_executable()
ns3.add(sample_simple)
sample_simple.add_deps(['core', 'simulator', 'node'])
sample_simple.add_source('main-simple.cc')

sample_sp2p = build.Ns3Module('sample-simple-p2p', 'samples')
sample_sp2p.set_executable()
#n3.add(sample_sp2p)
sample_sp2p.add_deps(['core', 'simulator', 'node', 'p2p'])
sample_sp2p.add_source('main-simple-p2p.cc')

# examples
example_simple_p2p = build.Ns3Module('simple-p2p', 'examples')
example_simple_p2p.set_executable()
ns3.add(example_simple_p2p)
example_simple_p2p.add_deps(['core', 'simulator', 'node', 'p2p'])
example_simple_p2p.add_source('simple-p2p.cc')

ns3.generate_dependencies()
