/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
#include "ns3/trace-container.h"
#include "ns3/ui-traced-variable.tcc"
#include "ns3/callback-tracer.h"
#include "ns3/trace-stream.h"
#include "ns3/pcap-writer.h"
#include "ns3/packet.h"
#include <iostream>

using namespace ns3;

CallbackTracer<Packet> a;
UiTracedVariable<unsigned short> b;
TraceStream c;
CallbackTracer<double, int> d;

void
register_all_trace_sources (TraceContainer *container)
{
	container->register_callback ("source-a", &a);
	container->register_ui_variable ("source-b", &b);
	container->register_stream ("source-c", &c);
	container->register_callback ("source-d", &d);
}
void
generate_trace_events (void)
{
	// log en empty packet
	a (Packet ());
	b = 10;
	b += 100;
	b += 50;
	b = (unsigned short) -20;
	c << "this is a simple test b=" << b << std::endl;
	d (3.1415, 3);
}

void
variable_event (uint64_t old, uint64_t cur)
{}

void
callback_event (double a, int b)
{}


int main (int argc, char *argv[])
{
	TraceContainer traces;
	register_all_trace_sources (&traces);
	PcapWriter pcap;
	pcap.open ("trace-test.log");
	pcap.write_header_ethernet ();
	traces.set_callback ("source-a", 
			     make_callback (&PcapWriter::write_packet, &pcap));
	traces.set_ui_variable_callback ("source-b", make_callback (&variable_event));
	traces.set_stream ("source-c", &std::cout);
	traces.set_callback ("source-d", make_callback (&callback_event));
	generate_trace_events ();
	return 0;
}
