/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
#include "ns3/trace-container.h"
#include "ns3/ui-variable-tracer.h"
#include "ns3/callback-tracer.h"
#include "ns3/stream-tracer.h"
#include "ns3/pcap-writer.h"
#include "ns3/packet.h"
#include <iostream>

using namespace ns3;

CallbackTracer<Packet> a;
UiVariableTracer<unsigned short> b;
StreamTracer c;
CallbackTracer<double, int> d;

void
registerAllTraceSources (TraceContainer *container)
{
	container->registerCallback ("source-a", &a);
	container->registerUiVariable ("source-b", &b);
	container->registerStream ("source-c", &c);
	container->registerCallback ("source-d", &d);
}
void
generateTraceEvents (void)
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
variableEvent (uint64_t old, uint64_t cur)
{}

void
callbackEvent (double a, int b)
{}


int main (int argc, char *argv[])
{
	TraceContainer traces;
	registerAllTraceSources (&traces);
	PcapWriter pcap;
	pcap.open ("trace-test.log");
	pcap.writeHeaderEthernet ();
	traces.setCallback ("source-a", 
			     makeCallback (&PcapWriter::writePacket, &pcap));
	traces.setUiVariableCallback ("source-b", makeCallback (&variableEvent));
	traces.setStream ("source-c", &std::cout);
	traces.setCallback ("source-d", makeCallback (&callbackEvent));
	generateTraceEvents ();
	return 0;
}
