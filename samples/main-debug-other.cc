/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/debug.h"

NS3_DEBUG_COMPONENT_DEFINE ("MyComponentB");

namespace foo {

void OneFunction (void)
{
  NS3_DEBUG ("OneFunction debug");
}

}; // namespace foo
