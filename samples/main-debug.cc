/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/debug.h"
#include "ns3/assert.h"

NS_DEBUG_COMPONENT_DEFINE ("MyComponentA");

// declare other function
namespace foo {
void OneFunction (void);
}

int main (int argc, int argv)
{
  NS_DEBUG ("nargc="<<argc);

  foo::OneFunction ();

  NS_DEBUG ("other debug output");

  int a;
  a = 0;

  NS_ASSERT (a == 0);
  NS_ASSERT_MSG (a == 0, "my msg");
  NS_ASSERT (a != 0);
  NS_ASSERT_MSG (a != 0, "my 2 msg");
}
