/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
#include "yans/event.h"
#include "yans/event.tcc"
#include "yans/simulator.h"
#include <iostream>

using namespace yans;

class MyModel {
public:
	void start (void);
private:
	void deal_with_event (double event_value);
};

void 
MyModel::start (void)
{
	Simulator::schedule_rel_s (10.0, make_event (&MyModel::deal_with_event, 
						  this, Simulator::now_s ()));
}
void
MyModel::deal_with_event (double value)
{
	std::cout << "Member method received event at " << Simulator::now_s () << " started at " << value << std::endl;
}

static void 
random_function (MyModel *model)
{
	std::cout << "random function received event at " << Simulator::now_s () << std::endl;
	model->start ();
}


int main (int argc, char *argv[])
{
	MyModel model;

	Simulator::schedule_rel_s (10.0, make_event (&random_function, 
						     &model));

	Simulator::run ();

	Simulator::destroy ();
}
