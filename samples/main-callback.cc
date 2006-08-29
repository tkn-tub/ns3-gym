/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
#include "yans/callback.h"
#include <cassert>
#include <iostream>

using namespace yans;

static double 
cb_one (double a, double b)
{
	std::cout << "invoke cb_one a=" << a << ", b=" << b << std::endl;
	return a;
}

class MyCb {
public:
	int cb_two (double a) {
		std::cout << "invoke cb_two a=" << a << std::endl;
		return -5;
	}
};


int main (int argc, char *argv[])
{
	// return type: double
	// first arg type: double
	// second arg type: double
	Callback<double, double, double> one;
	// build callback instance which points to cb_one function
	one = make_callback (&cb_one);
	// this is not a null callback
	assert (!one.is_null ());
	// invoke cb_one function through callback instance
	double ret_one;
	ret_one = one (10.0, 20.0);

	// return type: int
	// first arg type: double
	Callback<int, double> two;
	MyCb cb;
	// build callback instance which points to MyCb::cb_two
	two = make_callback (&MyCb::cb_two, &cb);
	// this is not a null callback
	assert (!two.is_null ());
	// invoke MyCb::cb_two through callback instance
	int ret_two;
	ret_two = two (10.0);	

	two = make_null_callback<int, double> ();
	// invoking a null callback is just like
	// invoking a null function pointer:
	// it will crash.
	//int ret_two_null = two (20.0);
	assert (two.is_null ());

	return 0;
}
