/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/ptr.h"
#include "ns3/object.h"
#include <iostream>

using namespace ns3;

class A : public Object
{
public:
  A ();
  ~A ();
  void Method (void);
};
A::A ()
{
  std::cout << "A constructor" << std::endl;
}
A::~A()
{
  std::cout << "A destructor" << std::endl;
}
void
A::Method (void)
{
  std::cout << "A method" << std::endl;
}

static Ptr<A> g_a = 0;

static Ptr<A>
StoreA (Ptr<A> a)
{
  Ptr<A> prev = g_a;
  g_a = a;
  return prev;
}

static void
ClearA (void)
{
  g_a = 0;
}



int main (int argc, char *argv[])
{
  {
    // Create a new object of type A, store it in global 
    // variable g_a
    Ptr<A> a = Create<A> ();
    a->Method ();
    Ptr<A> prev = StoreA (a);
    NS_ASSERT (prev == 0);
  }

  {
    // Create a new object of type A, store it in global 
    // variable g_a, get a hold on the previous A object.
    Ptr<A> a = Create<A> ();
    Ptr<A> prev = StoreA (a);
    // call method on object
    prev->Method ();
    // Clear the currently-stored object
    ClearA ();
    // get the raw pointer and release it.
    A *raw = GetPointer (prev);
    prev = 0;
    raw->Method ();
    raw->Unref ();
  }


  return 0;
}
