/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
#include "ns3/simulator.h"
#include "ns3/time.h"
#include <iostream>

using namespace ns3;

class MyModel {
public:
	void start (void);
private:
	void deal_with_event (double event_value);
};

void 
MyModel::start (void)
{
	Simulator::schedule (RelTimeS (10.0), 
			     &MyModel::deal_with_event, 
			     this, Simulator::now ().s ());
}
void
MyModel::deal_with_event (double value)
{
	std::cout << "Member method received event at " << Simulator::now ().s () << " started at " << value << std::endl;
}

static void 
random_function (MyModel *model)
{
	std::cout << "random function received event at " << Simulator::now ().s () << std::endl;
	model->start ();
}


int main (int argc, char *argv[])
{
	MyModel model;

	Simulator::schedule (AbsTimeS (10.0), &random_function, &model);

	Simulator::run ();

	Simulator::destroy ();
}
