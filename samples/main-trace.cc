/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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
RegisterAllTraceSources (TraceContainer *container)
{
    container->RegisterCallback ("source-a", &a);
    container->RegisterUiVariable ("source-b", &b);
    container->RegisterStream ("source-c", &c);
    container->RegisterCallback ("source-d", &d);
}
void
GenerateTraceEvents (void)
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
VariableEvent (uint64_t old, uint64_t cur)
{}

void
CallbackEvent (double a, int b)
{}


int main (int argc, char *argv[])
{
    TraceContainer traces;
    RegisterAllTraceSources (&traces);
    PcapWriter pcap;
    pcap.Open ("trace-test.log");
    pcap.WriteHeaderEthernet ();
    traces.SetCallback ("source-a", 
                        MakeCallback (&PcapWriter::WritePacket, &pcap));
    traces.SetUiVariableCallback ("source-b", MakeCallback (&VariableEvent));
    traces.SetStream ("source-c", &std::cout);
    traces.SetCallback ("source-d", MakeCallback (&CallbackEvent));
    GenerateTraceEvents ();
    return 0;
}
