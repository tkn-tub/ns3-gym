import os.path
import build

ns3 = build.Ns3 ()
ns3.build_dir = 'build-dir'
ns3.version = '0.0.1'
ns3.name = 'ns3'
ns3.doxygen_config = os.path.join ('doc', 'doxygen.conf')
ns3.add_extra_dist (os.path.join ('doc', 'main.txt'))


#
# The Core module
#
core = build.Ns3Module ('core', 'src/core')
ns3.add (core)
core.add_sources ([
        'reference-list-test.cc',
        'callback-test.cc',
        'test.cc'
	])
env = Environment ()
if env['PLATFORM'] == 'posix' or env['PLATFORM'] == 'darwin':
	core.add_external_dep ('pthread')
	core.add_sources ([
		'unix-system-wall-clock-ms.cc',
		'unix-system-file.cc'
		])
elif env['PLATFORM'] == 'win32':
	core.add_sources ([
		'win32-system-wall-clock-ms.cc',
		'win32-system-file.cc'
		])
core.add_inst_headers ([
	'system-file.h',
        'system-wall-clock-ms.h',
        'reference-list.h',
        'callback.h',
        'test.h'
	])


#
# The Simu module
#
simu = build.Ns3Module ('simulator', 'src/simulator')
ns3.add (simu)
simu.add_dep ('core')
simu.add_sources ([
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
simu.add_headers ([
	'scheduler-heap.h',
	'scheduler-map.h',
	'scheduler-list.h'
	])
simu.add_inst_headers ([
	'time.h',
	'event-id.h',
	'event-impl.h',
	'simulator.h',
	'scheduler.h',
	'scheduler-factory.h',
	])

#
# The Common module
#
common = build.Ns3Module ('common', 'src/common')
common.add_deps (['core', 'simulator'])
ns3.add (common)
common.add_sources ([
	'buffer.cc',
	'header.cc',
	'packet.cc',
	'tags.cc',
	'pcap-writer.cc',
	'trace-container.cc',
	'variable-tracer-test.cc',
	'stream-tracer-test.cc',
	])
common.add_inst_headers ([
	'buffer.h',
	'header.h',
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


# utils
run_tests = build.Ns3Module ('run-tests', 'utils')
ns3.add (run_tests)
run_tests.set_executable ()
run_tests.add_deps (['core', 'simulator', 'common'])
run_tests.add_source ('run-tests.cc')

bench_packets = build.Ns3Module ('bench-packets', 'utils')
#ns3.add (bench_packets)
bench_packets.set_executable ()
bench_packets.add_dep ('core')
bench_packets.add_source ('bench-packets.cc')

bench_simu = build.Ns3Module ('bench-simulator', 'utils')
ns3.add (bench_simu)
bench_simu.set_executable ()
bench_simu.add_dep ('simulator')
bench_simu.add_source ('bench-simulator.cc')


# samples
sample_callback = build.Ns3Module ('sample-callback', 'samples')
sample_callback.set_executable ()
ns3.add (sample_callback)
sample_callback.add_dep ('core')
sample_callback.add_source ('main-callback.cc')

sample_trace = build.Ns3Module ('sample-trace', 'samples')
ns3.add (sample_trace)
sample_trace.add_dep ('common')
sample_trace.set_executable ()
sample_trace.add_source ('main-trace.cc')

sample_simu = build.Ns3Module ('sample-simulator', 'samples')
ns3.add (sample_simu)
sample_simu.set_executable ()
sample_simu.add_dep ('simulator')
sample_simu.add_source ('main-simulator.cc')

sample_packet = build.Ns3Module ('sample-packet', 'samples')
ns3.add (sample_packet)
sample_packet.set_executable ()
sample_packet.add_dep ('common')
sample_packet.add_source ('main-packet.cc')

sample_test = build.Ns3Module ('sample-test', 'samples')
sample_test.set_executable ()
ns3.add (sample_test)
sample_test.add_dep ('core')
sample_test.add_source ('main-test.cc')


ns3.generate_dependencies ()



