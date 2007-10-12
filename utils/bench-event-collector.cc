#include <iostream>
#include <vector>
#include <stdlib.h>
#include "ns3/event-collector.h"
#include "ns3/simulator.h"

using namespace ns3;

void Foo ()
{

}

int main (int argc, char *argv[])
{
    EventCollector events;

    if (argc < 3)
    {
        std::cerr << "usage: bench-event-collector NUM_EVENTS NUM_REPETITIONS" << std::endl;
        return 1;
    }
    int numEvents = atoi (argv[1]);
    int numRepetitions = atoi (argv[2]);
    
    for (int repetition = 0; repetition < numRepetitions; ++repetition)
    {
        for (int n = 0; n < numEvents; ++n)
        {
            events.Track (Simulator::Schedule (Simulator::Now (), Foo));
        }
        Simulator::Run ();
    }
}

