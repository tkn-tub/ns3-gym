/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/callback.h"
#include "ns3/assert.h"
#include <iostream>

using namespace ns3;

static double 
CbOne (double a, double b)
{
  std::cout << "invoke cbOne a=" << a << ", b=" << b << std::endl;
  return a;
}

class MyCb {
public:
  int CbTwo (double a) {
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
  one = MakeCallback (&CbOne);
  // this is not a null callback
  NS_ASSERT (!one.IsNull ());
  // invoke cbOne function through callback instance
  double retOne;
  retOne = one (10.0, 20.0);
  // cast retOne to void, to suppress variable ‘retOne’ set but
  // not used compiler warning
  (void) retOne; 

  // return type: int
  // first arg type: double
  Callback<int, double> two;
  MyCb cb;
  // build callback instance which points to MyCb::cbTwo
  two = MakeCallback (&MyCb::CbTwo, &cb);
  // this is not a null callback
  NS_ASSERT (!two.IsNull ());
  // invoke MyCb::cbTwo through callback instance
  int retTwo;
  retTwo = two (10.0);
  // cast retTwo to void, to suppress variable ‘retTwo’ set but
  // not used compiler warning
  (void) retTwo;
  two = MakeNullCallback<int, double> ();
  // invoking a null callback is just like
  // invoking a null function pointer:
  // it will crash.
  //int retTwoNull = two (20.0);
  NS_ASSERT (two.IsNull ());

#if 0
  // The below type mismatch between CbOne() and callback two will fail to 
  // compile if enabled in this program.
  two = MakeCallback (&CbOne);
#endif

#if 0
  // This is a slightly different example, in which the code will compile
  // but because callbacks are type-safe, will cause a fatal error at runtime 
  // (the difference here is that Assign() is called instead of operator=)
  Callback<void, float> three;
  three.Assign (MakeCallback (&CbOne));
#endif

  return 0;
}
