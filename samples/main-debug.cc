/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/debug.h"

NS3_DEBUG_COMPONENT_DEFINE ("MyComponentA");

// declare other function
namespace foo {
void OneFunction (void);
}

int main (int argc, int argv)
{
  NS3_DEBUG ("nargc="<<argc);

  foo::OneFunction ();

  NS3_DEBUG ("other debug output");

  int a = 0;

  NS3_ASSERT (a == 0);
  NS3_ASSERT_MSG (a == 0, "my msg");
  NS3_ASSERT (a != 0)
  NS3_ASSERT_MSG (a != 0, "my 2 msg");
}
