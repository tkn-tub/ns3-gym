/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
#include "ns3/event.h"
#include "ns3/event.tcc"
#include <iostream>

using namespace ns3;

class MyModel {
public:
	void deal_with_event (double event_value);
};

void
MyModel::deal_with_event (double value)
{
	std::cout << "Member method received event." << std::endl;
}

static void 
random_function (void)
{
	std::cout << "Function received event." << std::endl;
}


int main (int argc, char *argv[])
{
	Event ev;
	// create event to forward to random_function
	ev = make_event (&random_function);
	// set cancel bit to on
	ev.cancel ();
	// try to invoke the random_function through the event.
	// This does nothing since cancel bit is on.
	ev ();
	MyModel model;
	// create event to forward to MyModel::deal_with_event
	// on the class instance "model".
	ev = make_event (&MyModel::deal_with_event, &model, 10.0);
	// invoke member method through the event.
	ev ();
}
