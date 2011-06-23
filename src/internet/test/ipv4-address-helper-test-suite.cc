/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 University of Washington
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/test.h"
#include "ns3/ipv4-address-generator.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/simulator.h"

namespace ns3 {

class NetworkAllocatorHelperTestCase : public TestCase
{
public:
  NetworkAllocatorHelperTestCase ();
private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

NetworkAllocatorHelperTestCase::NetworkAllocatorHelperTestCase ()
  : TestCase ("Make sure the network allocator part is working on some common network prefixes.")
{
}

void
NetworkAllocatorHelperTestCase::DoTeardown (void)
{
  Ipv4AddressGenerator::Reset ();
  Simulator::Destroy ();
}
void
NetworkAllocatorHelperTestCase::DoRun (void)
{
  Ipv4Address address;
  Ipv4Address network;
  Ipv4AddressHelper h;

  h.SetBase ("1.0.0.0", "255.0.0.0");
  network = h.NewNetwork ();
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("2.0.0.0"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("2.0.0.1"), "XXX");

  h.SetBase ("0.1.0.0", "255.255.0.0");
  network = h.NewNetwork ();
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.2.0.0"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.2.0.1"), "XXX");

  h.SetBase ("0.0.1.0", "255.255.255.0");
  network = h.NewNetwork ();
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.0.2.0"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.2.1"), "XXX");
}

class AddressAllocatorHelperTestCase : public TestCase
{
public:
  AddressAllocatorHelperTestCase ();
private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

AddressAllocatorHelperTestCase::AddressAllocatorHelperTestCase ()
  : TestCase ("Make sure the address allocator part is working")
{
}

void
AddressAllocatorHelperTestCase::DoTeardown (void)
{
  Ipv4AddressGenerator::Reset ();
  Simulator::Destroy ();
}

void
AddressAllocatorHelperTestCase::DoRun (void)
{
  Ipv4Address network;
  Ipv4Address address;
  Ipv4AddressHelper h;

  h.SetBase ("1.0.0.0", "255.0.0.0", "0.0.0.3");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("1.0.0.3"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("1.0.0.4"), "XXX");

  h.SetBase ("0.1.0.0", "255.255.0.0", "0.0.0.3");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.1.0.3"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.1.0.4"), "XXX");

  h.SetBase ("0.0.1.0", "255.255.255.0", "0.0.0.3");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.1.3"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.1.4"), "XXX");
}

class ResetAllocatorHelperTestCase : public TestCase
{
public:
  ResetAllocatorHelperTestCase ();
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

ResetAllocatorHelperTestCase::ResetAllocatorHelperTestCase ()
  : TestCase ("Make sure the reset to base behavior is working")
{
}

void
ResetAllocatorHelperTestCase::DoRun (void)
{
  Ipv4Address network;
  Ipv4Address address;
  Ipv4AddressHelper h;

  //
  // We're going to use some of the same addresses allocated above, 
  // so reset the Ipv4AddressGenerator to make it forget we did.
  //

  h.SetBase ("1.0.0.0", "255.0.0.0", "0.0.0.3");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("1.0.0.3"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("1.0.0.4"), "XXX");
  network = h.NewNetwork ();
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("2.0.0.0"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("2.0.0.3"), "XXX");

  h.SetBase ("0.1.0.0", "255.255.0.0", "0.0.0.3");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.1.0.3"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.1.0.4"), "XXX");
  network = h.NewNetwork ();
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.2.0.0"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.2.0.3"), "XXX");

  h.SetBase ("0.0.1.0", "255.255.255.0", "0.0.0.3");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.1.3"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.1.4"), "XXX");
  network = h.NewNetwork ();
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.0.2.0"), "XXX");
  address = h.NewAddress ();
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.2.3"), "XXX");
}

void
ResetAllocatorHelperTestCase::DoTeardown (void)
{
  Ipv4AddressGenerator::Reset ();
  Simulator::Destroy ();
}

static class Ipv4AddressHelperTestSuite : public TestSuite
{
public:
  Ipv4AddressHelperTestSuite ()
    : TestSuite ("ipv4-address-helper", UNIT) 
  {
    AddTestCase (new NetworkAllocatorHelperTestCase ());
    AddTestCase (new AddressAllocatorHelperTestCase ());
    AddTestCase (new ResetAllocatorHelperTestCase ());
  }
} g_ipv4AddressHelperTestSuite;

} // namespace ns3

