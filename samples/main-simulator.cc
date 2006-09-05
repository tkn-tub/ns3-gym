/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
#include "ns3/simulator.h"
#include "ns3/time.h"
#include <iostream>

using namespace ns3;

class MyModel {
public:
	void start (void);
private:
	void dealWithEvent (double eventValue);
};

void 
MyModel::start (void)
{
	Simulator::schedule (Time::relS (10.0), 
			     &MyModel::dealWithEvent, 
			     this, Simulator::now ().s ());
}
void
MyModel::dealWithEvent (double value)
{
	std::cout << "Member method received event at " << Simulator::now ().s () << 
		"s started at " << value << "s" << std::endl;
}

static void 
random_function (MyModel *model)
{
	std::cout << "random function received event at " << 
		Simulator::now ().s () << "s" << std::endl;
	model->start ();
}


int main (int argc, char *argv[])
{
	MyModel model;

	Simulator::schedule (Time::absS (10.0), &random_function, &model);

	Simulator::run ();

	Simulator::destroy ();
}
