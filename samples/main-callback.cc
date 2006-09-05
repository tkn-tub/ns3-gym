/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
#include "ns3/callback.h"
#include <cassert>
#include <iostream>

using namespace ns3;

static double 
cbOne (double a, double b)
{
	std::cout << "invoke cbOne a=" << a << ", b=" << b << std::endl;
	return a;
}

class MyCb {
public:
	int cbTwo (double a) {
		std::cout << "invoke cbTwo a=" << a << std::endl;
		return -5;
	}
};


int main (int argc, char *argv[])
{
	// return type: double
	// first arg type: double
	// second arg type: double
	Callback<double, double, double> one;
	// build callback instance which points to cbOne function
	one = makeCallback (&cbOne);
	// this is not a null callback
	assert (!one.isNull ());
	// invoke cbOne function through callback instance
	double retOne;
	retOne = one (10.0, 20.0);

	// return type: int
	// first arg type: double
	Callback<int, double> two;
	MyCb cb;
	// build callback instance which points to MyCb::cbTwo
	two = makeCallback (&MyCb::cbTwo, &cb);
	// this is not a null callback
	assert (!two.isNull ());
	// invoke MyCb::cbTwo through callback instance
	int retTwo;
	retTwo = two (10.0);	

	two = makeNullCallback<int, double> ();
	// invoking a null callback is just like
	// invoking a null function pointer:
	// it will crash.
	//int retTwoNull = two (20.0);
	assert (two.isNull ());

	return 0;
}
