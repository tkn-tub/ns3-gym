## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
import os.path
import build

version_file = open ('VERSION', 'r')
version = version_file.readline ()
version_file.close ()
version = version.strip ()

ns3 = build.Ns3()
ns3.build_dir = 'build-dir'
ns3.version = version
ns3.name = 'ns3'
ns3.distname = 'ns'
ns3.doxygen_config = os.path.join('doc', 'doxygen.conf')
ns3.add_extra_dist(os.path.join('doc', 'main.txt'))
ns3.add_extra_dist ('doc/architecture.pdf')
ns3.add_extra_dist ('doc/contributing.txt')
ns3.add_extra_dist ('doc/build.txt')
ns3.add_extra_dist ('doc/codingstd.txt')
ns3.add_extra_dist ('doc/mercurial.txt')
ns3.add_extra_dist ('README')
ns3.add_extra_dist ('RELEASE_NOTES')
ns3.add_extra_dist ('AUTHORS')
ns3.add_extra_dist ('VERSION')

ns3.add_extra_dist('doc/build-waf.txt')
ns3.add_extra_dist('ns3/_placeholder_')
for wscript in [
    "src/core/wscript",
    "src/node/wscript",
    "src/devices/p2p/wscript",
    "src/common/wscript",
    "src/applications/wscript",
    "src/simulator/wscript",
    "src/internet-node/wscript",
    "src/wscript",
    "utils/wscript",
    "samples/wscript",
    "examples/wscript",
    "wscript",
    ]:
    ns3.add_extra_dist(wscript)
ns3.add_extra_dist('waf')
ns3.add_extra_dist('waf.bat')



#
# The Core module
#
core = build.Ns3Module('core', 'src/core')
ns3.add(core)
core.add_sources([
    'callback-test.cc',
    'debug.cc',
    'assert.cc',
    'ptr.cc',
    'object.cc',
    'test.cc',
    'random-variable.cc',
    'rng-stream.cc',
    'uid-manager.cc',
    'default-value.cc',
    'random-variable-default-value.cc',
    'rectangle-default-value.cc',
    'command-line.cc',
    'type-name.cc',
    'component-manager.cc',
    ])
env = Environment()
if env['PLATFORM'] == 'posix' or env['PLATFORM'] == 'darwin' or env['PLATFORM'] == 'cygwin':
    core.add_external_dep('pthread')
    core.add_sources([
        'unix-system-wall-clock-ms.cc',
        ])
elif env['PLATFORM'] == 'win32':
    core.add_sources([
        'win32-system-wall-clock-ms.cc',
        ])
core.add_headers ([
    'uid-manager.h',
    'singleton.h',
])
core.add_inst_headers([
    'system-wall-clock-ms.h',
    'empty.h',
    'callback.h',
    'ptr.h',
    'object.h',
    'debug.h',
    'assert.h',
    'fatal-error.h',
    'test.h',
    'random-variable.h',
    'rng-stream.h',
    'default-value.h',
    'random-variable-default-value.h',
    'rectangle-default-value.h',
    'command-line.h',
    'type-name.h',
    'component-manager.h',
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
    'time-default-value.cc',
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
    'simulation-singleton.h',
    'time-default-value.h',
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
    'chunk.cc',
    'header.cc',
    'trailer.cc',
    'packet.cc',
    'tags.cc',
    'pcap-writer.cc',
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
    'chunk.h',
    'header.h',
    'trailer.h',
    'tags.h',
    'packet.h',
    'uv-trace-source.h',
    'sv-trace-source.h',
    'fv-trace-source.h',
    'pcap-writer.h',
    'callback-trace-source.h',
    'trace-context.h',
    'trace-resolver.h',
    'empty-trace-resolver.h',
    'composite-trace-resolver.h',
    'array-trace-resolver.h',
    'trace-root.h',
    'terminal-trace-resolver.h',
    'data-rate.h',
    ])

node = build.Ns3Module ('node', 'src/node')
ns3.add (node)
node.add_deps (['core', 'common', 'simulator'])
node.add_sources ([
    'node.cc',
    'ipv4-address.cc',
    'net-device.cc',
    'mac-address.cc',
    'llc-snap-header.cc',
    'ipv4-route.cc',
    'queue.cc',
    'drop-tail-queue.cc',
    'channel.cc',
    'node-list.cc',
    'socket.cc',
    'socket-factory.cc',
    'udp.cc',
    'ipv4.cc',
    'application.cc',
    'mobility-model.cc',
    'mobility-model-notifier.cc',
    'static-mobility-model.cc',
    'static-speed-mobility-model.cc',
    'grid-topology.cc',
    'random-topology.cc',
    'random-walk-mobility-model.cc',
    'random-mobility-model.cc',
    'random-direction-mobility-model.cc',
    'hierarchical-mobility-model.cc',
    'ns2-mobility-file-topology.cc',
    'mobility-model-helper.cc',
    'position-2d.cc',
    'position.cc',
    'random-position.cc',
    ])
node.add_inst_headers ([
    'node.h',
    'ipv4-address.h',
    'net-device.h',
    'mac-address.h',
    'ipv4-route.h',
    'queue.h',
    'drop-tail-queue.h',
    'llc-snap-header.h',
    'channel.h',
    'node-list.h',
    'socket.h',
    'socket-factory.h',
    'udp.h',
    'ipv4.h',
    'application.h',
    'mobility-model.h',
    'mobility-model-notifier.h',
    'static-mobility-model.h',
    'static-speed-mobility-model.h',
    'grid-topology.h',
    'random-topology.h',
    'random-walk-mobility-model.h',
    'random-mobility-model.h',
    'random-direction-mobility-model.h',
    'hierarchical-mobility-model.h',
    'ns2-mobility-file-topology.h',
    'mobility-model-helper.h',
    'position-2d.h',
    'position.h',
    'random-position.h',
    ])

applications = build.Ns3Module ('applications', 'src/applications')
ns3.add (applications)
applications.add_deps (['node'])
applications.add_sources ([
    'onoff-application.cc',
])
applications.add_inst_headers ([
    'onoff-application.h',
])

inode = build.Ns3Module ('internet-node', 'src/internet-node')
ns3.add (inode)
inode.add_deps (['node'])
inode.add_sources ([
    'internet-node.cc',
    'l3-demux.cc',
    'l3-protocol.cc',
    'ipv4-l4-demux.cc',
    'ipv4-l4-protocol.cc',
    'ipv4-header.cc',
    'udp-header.cc',
    'ipv4-checksum.cc',
    'ipv4-interface.cc',
    'ipv4-l3-protocol.cc',
    'ipv4-end-point.cc',
    'udp-l4-protocol.cc',
    'arp-header.cc',
    'arp-cache.cc',
    'arp-ipv4-interface.cc',
    'arp-l3-protocol.cc',
    'ipv4-loopback-interface.cc',
    'header-utils.cc',
    'udp-socket.cc',
    'ipv4-end-point-demux.cc',
    'arp-private.cc',
    'ipv4-impl.cc',
    'ipv4-private.cc',
    'ascii-trace.cc',
    'pcap-trace.cc',
    'udp-impl.cc',
])
inode.add_headers ([
    'ipv4-header.h',
    'udp-header.h',
    'ipv4-checksum.h',
    'arp-header.h',
    'arp-cache.h',
    'arp-l3-protocol.h',
    'ipv4-loopback-interface.h',
    'l3-demux.h',
    'header-utils.h',
    'arp-ipv4-interface.h',
    'udp-socket.h',
    'udp-l4-protocol.h',
    'arp-private.h',
    'ipv4-impl.h',
    'ipv4-private.h',
    'ipv4-l3-protocol.h',
    'l3-protocol.h',
    'ipv4-l4-protocol.h',
    'ipv4-l4-demux.h',
    'ipv4-end-point-demux.h',
    'ipv4-end-point.h',
    'ipv4-header.h',
    'udp-header.h',
    'ipv4-interface.h',
    'sgi-hashmap.h',
    'udp-impl.h',
])
inode.add_inst_headers ([
    'internet-node.h',
    'ascii-trace.h',
    'pcap-trace.h',
])



p2p = build.Ns3Module ('p2p', 'src/devices/p2p')
ns3.add (p2p)
p2p.add_deps (['node'])
p2p.add_sources ([
    'p2p-net-device.cc',
    'p2p-channel.cc',
    'p2p-topology.cc',
    ])
p2p.add_inst_headers ([
    'p2p-net-device.h',
    'p2p-channel.h',
    'p2p-topology.h',
    ])


# utils
mobgen = build.Ns3Module ('mobility-generator', 'utils')
ns3.add (mobgen)
mobgen.set_executable ()
mobgen.add_deps (['simulator', 'node'])
mobgen.add_source ('mobility-generator.cc')

run_tests = build.Ns3Module('run-tests', 'utils')
ns3.add(run_tests)
run_tests.set_executable()
run_tests.add_deps(['core', 'simulator', 'common'])
run_tests.add_source('run-tests.cc')

bench_object = build.Ns3Module('bench-object', 'utils')
ns3.add(bench_object)
bench_object.set_executable()
bench_object.add_deps(['core'])
bench_object.add_source('bench-object.cc')

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

sample_random_walk = build.Ns3Module('sample-random-walk', 'samples')
sample_random_walk.set_executable()
ns3.add(sample_random_walk)
sample_random_walk.add_deps(['core', 'node'])
sample_random_walk.add_source('main-random-walk.cc')

sample_grid_topology = build.Ns3Module('sample-grid-topology', 'samples')
sample_grid_topology.set_executable()
ns3.add(sample_grid_topology)
sample_grid_topology.add_deps(['core', 'internet-node'])
sample_grid_topology.add_source('main-grid-topology.cc')

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

sample_query_interface = build.Ns3Module('sample-query-interface', 'samples')
ns3.add(sample_query_interface)
sample_query_interface.add_dep('common')
sample_query_interface.set_executable()
sample_query_interface.add_source('main-query-interface.cc')

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

sample_simple = build.Ns3Module('sample-simple', 'samples')
sample_simple.set_executable()
ns3.add(sample_simple)
sample_simple.add_deps(['core', 'simulator', 'node', 'internet-node'])
sample_simple.add_source('main-simple.cc')

sample_sp2p = build.Ns3Module('sample-simple-p2p', 'samples')
sample_sp2p.set_executable()
#n3.add(sample_sp2p)
sample_sp2p.add_deps(['core', 'simulator', 'node', 'internet-node', 'p2p'])
sample_sp2p.add_source('main-simple-p2p.cc')

sample_default_value = build.Ns3Module('sample-default-value', 'samples')
sample_default_value.set_executable()
ns3.add(sample_default_value)
sample_default_value.add_deps(['core', 'simulator', 'node', 'p2p'])
sample_default_value.add_source('main-default-value.cc')

sample_object = build.Ns3Module('sample-object', 'samples')
sample_object.set_executable()
ns3.add(sample_object)
sample_object.add_deps(['core'])
sample_object.add_source('main-object.cc')

sample_component_manager = build.Ns3Module('sample-component-manager', 'samples')
sample_component_manager.set_executable()
ns3.add(sample_component_manager)
sample_component_manager.add_deps(['core'])
sample_component_manager.add_source('main-component-manager.cc')

# examples
example_simple_p2p = build.Ns3Module('simple-p2p', 'examples')
example_simple_p2p.set_executable()
ns3.add(example_simple_p2p)
example_simple_p2p.add_deps(['core', 'simulator', 'node', 'p2p', 'internet-node', 'applications'])
example_simple_p2p.add_source('simple-p2p.cc')

ns3.generate_dependencies()
