/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/ptr.h"
#include "ns3/object.h"
#include <iostream>

using namespace ns3;

class PtrExample : public Object
{
public:
  PtrExample ();
  ~PtrExample ();
  void Method (void);
};
PtrExample::PtrExample ()
{
  std::cout << "PtrExample constructor" << std::endl;
}
PtrExample::~PtrExample()
{
  std::cout << "PtrExample destructor" << std::endl;
}
void
PtrExample::Method (void)
{
  std::cout << "PtrExample method" << std::endl;
}

static Ptr<PtrExample> g_ptr = 0;

static Ptr<PtrExample>
StorePtr (Ptr<PtrExample> p)
{
  Ptr<PtrExample> prev = g_ptr;
  g_ptr = p;
  return prev;
}

static void
ClearPtr (void)
{
  g_ptr = 0;
}



int main (int argc, char *argv[])
{
  {
    // Create a new object of type PtrExample, store it in global 
    // variable g_ptr
    Ptr<PtrExample> p = CreateObject<PtrExample> ();
    p->Method ();
    Ptr<PtrExample> prev = StorePtr (p);
    NS_ASSERT (prev == 0);
  }

  {
    // Create a new object of type PtrExample, store it in global 
    // variable g_ptr, get a hold on the previous PtrExample object.
    Ptr<PtrExample> p = CreateObject<PtrExample> ();
    Ptr<PtrExample> prev = StorePtr (p);
    // call method on object
    prev->Method ();
    // Clear the currently-stored object
    ClearPtr ();
    // get the raw pointer and release it.
    PtrExample *raw = GetPointer (prev);
    prev = 0;
    raw->Method ();
    raw->Unref ();
  }


  return 0;
}
