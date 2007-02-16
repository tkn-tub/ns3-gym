/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/debug.h"

NS_DEBUG_COMPONENT_DEFINE ("MyComponentB");

namespace foo {

void OneFunction (void)
{
  NS_DEBUG ("OneFunction debug");
}

}; // namespace foo
