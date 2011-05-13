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
#include "ns3/simulation-singleton.h"

namespace ns3 {

class NetworkNumberAllocatorTestCase : public TestCase
{
public:
  NetworkNumberAllocatorTestCase ();
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

NetworkNumberAllocatorTestCase::NetworkNumberAllocatorTestCase ()
  : TestCase ("Make sure the network number allocator is working on some of network prefixes.")
{
}
void
NetworkNumberAllocatorTestCase::DoTeardown (void)
{
  Ipv4AddressGenerator::Reset ();
}
void
NetworkNumberAllocatorTestCase::DoRun (void)
{
  Ipv4Address network;
  Ipv4AddressGenerator::Init (Ipv4Address ("1.0.0.0"), Ipv4Mask ("255.0.0.0"),
                              Ipv4Address ("0.0.0.0"));
  network = Ipv4AddressGenerator::GetNetwork (Ipv4Mask ("255.0.0.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("1.0.0.0"), "XXX");
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.0.0.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("2.0.0.0"), "XXX");

  Ipv4AddressGenerator::Init (Ipv4Address ("0.1.0.0"),
                              Ipv4Mask ("255.255.0.0"), Ipv4Address ("0.0.0.0"));
  network = Ipv4AddressGenerator::GetNetwork (Ipv4Mask ("255.255.0.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.1.0.0"), "XXX");
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.0.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.2.0.0"), "XXX");

  Ipv4AddressGenerator::Init (Ipv4Address ("0.0.1.0"),
                              Ipv4Mask ("255.255.255.0"), Ipv4Address ("0.0.0.0"));
  network = Ipv4AddressGenerator::GetNetwork (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.0.1.0"), "XXX");
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.0.2.0"), "XXX");

  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.0.0.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("3.0.0.0"), "XXX");
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.0.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.3.0.0"), "XXX");
  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.0.3.0"), "XXX");
}

class AddressAllocatorTestCase : public TestCase
{
public:
  AddressAllocatorTestCase ();
private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

AddressAllocatorTestCase::AddressAllocatorTestCase ()
  : TestCase ("Sanity check on allocation of addresses")
{
}

void
AddressAllocatorTestCase::DoRun (void)
{
  Ipv4Address address;

  Ipv4AddressGenerator::Init (Ipv4Address ("1.0.0.0"), Ipv4Mask ("255.0.0.0"),
                              Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("1.0.0.3"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("1.0.0.4"), "XXX");

  Ipv4AddressGenerator::Init (Ipv4Address ("0.1.0.0"),
                              Ipv4Mask ("255.255.0.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.1.0.3"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.1.0.4"), "XXX");

  Ipv4AddressGenerator::Init (Ipv4Address ("0.0.1.0"),
                              Ipv4Mask ("255.255.255.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.1.3"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.1.4"), "XXX");
}

void 
AddressAllocatorTestCase::DoTeardown (void)
{
  Ipv4AddressGenerator::Reset ();
  Simulator::Destroy ();
}


class NetworkAndAddressTestCase : public TestCase
{
public:
  NetworkAndAddressTestCase ();
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

NetworkAndAddressTestCase::NetworkAndAddressTestCase ()
  : TestCase ("Make sure Network and address allocation play together.")
{
}

void
NetworkAndAddressTestCase::DoTeardown (void)
{
  Ipv4AddressGenerator::Reset ();
  Simulator::Destroy ();
}

void
NetworkAndAddressTestCase::DoRun (void)
{
  Ipv4Address address;
  Ipv4Address network;

  Ipv4AddressGenerator::Init (Ipv4Address ("3.0.0.0"), Ipv4Mask ("255.0.0.0"),
                              Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("3.0.0.3"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("3.0.0.4"), "XXX");

  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.0.0.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("4.0.0.0"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.0.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("4.0.0.5"), "XXX");

  Ipv4AddressGenerator::Init (Ipv4Address ("0.3.0.0"), 
                              Ipv4Mask ("255.255.0.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.3.0.3"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.3.0.4"), "XXX");

  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.0.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.4.0.0"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.0.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.4.0.5"), "XXX");

  Ipv4AddressGenerator::Init (Ipv4Address ("0.0.3.0"),
                              Ipv4Mask ("255.255.255.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.3.3"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.3.4"), "XXX");

  network = Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (network, Ipv4Address ("0.0.4.0"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("0.0.4.5"), "XXX");
}

class ExampleAddressGeneratorTestCase : public TestCase
{
public:
  ExampleAddressGeneratorTestCase ();
private:
  virtual void DoRun (void);
  virtual void DoTeardown (void);
};

ExampleAddressGeneratorTestCase::ExampleAddressGeneratorTestCase ()
  : TestCase ("A quick kindof-semi-almost-real example")
{
}

void
ExampleAddressGeneratorTestCase::DoTeardown (void)
{
  Ipv4AddressGenerator::Reset ();
}

void
ExampleAddressGeneratorTestCase::DoRun (void)
{
  Ipv4Address address;
  //
  // First, initialize our /24 network to 192.168.0.0 and begin 
  // allocating with ip address 0.0.0.3 out of that prefix.
  //
  Ipv4AddressGenerator::Init (Ipv4Address ("192.168.0.0"),
                              Ipv4Mask ("255.255.255.0"), Ipv4Address ("0.0.0.3"));
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("192.168.0.3"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("192.168.0.4"), "XXX");
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("192.168.0.5"), "XXX");
  //
  // Allocate the next network out of our /24 network (this should be 
  // 192.168.1.0) and begin allocating with IP address 0.0.0.3 out of that
  // prefix.
  //
  Ipv4AddressGenerator::NextNetwork (Ipv4Mask ("255.255.255.0"));
  Ipv4AddressGenerator::InitAddress (Ipv4Address ("0.0.0.3"),
                                     Ipv4Mask ("255.255.255.0"));
  //
  // The first address we should get is the previous numbers ORed together, which
  // is 192.168.1.3, of course.
  //
  address = Ipv4AddressGenerator::NextAddress (Ipv4Mask ("255.255.255.0"));
  NS_TEST_EXPECT_MSG_EQ (address, Ipv4Address ("192.168.1.3"), "XXX");
}

class AddressCollisionTestCase : public TestCase
{
public:
  AddressCollisionTestCase ();
private:
  void DoRun (void);
  void DoTeardown (void);
};

AddressCollisionTestCase::AddressCollisionTestCase ()
  : TestCase ("Make sure that the address collision logic works.")
{
}

void
AddressCollisionTestCase::DoTeardown (void)
{
  Ipv4AddressGenerator::Reset ();
  Simulator::Destroy ();
}
void
AddressCollisionTestCase::DoRun (void)
{
  Ipv4AddressGenerator::AddAllocated ("0.0.0.5");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.10");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.15");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.20");

  Ipv4AddressGenerator::AddAllocated ("0.0.0.4");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.3");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.2");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.1");

  Ipv4AddressGenerator::AddAllocated ("0.0.0.6");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.7");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.8");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.9");

  Ipv4AddressGenerator::AddAllocated ("0.0.0.11");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.12");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.13");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.14");

  Ipv4AddressGenerator::AddAllocated ("0.0.0.19");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.18");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.17");
  Ipv4AddressGenerator::AddAllocated ("0.0.0.16");

  Ipv4AddressGenerator::TestMode ();
  bool added = Ipv4AddressGenerator::AddAllocated ("0.0.0.21");
  NS_TEST_EXPECT_MSG_EQ (added, true, "XXX");

  added = Ipv4AddressGenerator::AddAllocated ("0.0.0.4");
  NS_TEST_EXPECT_MSG_EQ (added, false, "XXX");

  added = Ipv4AddressGenerator::AddAllocated ("0.0.0.9");
  NS_TEST_EXPECT_MSG_EQ (added, false, "XXX");

  added = Ipv4AddressGenerator::AddAllocated ("0.0.0.16");
  NS_TEST_EXPECT_MSG_EQ (added, false, "XXX");

  added = Ipv4AddressGenerator::AddAllocated ("0.0.0.21");
  NS_TEST_EXPECT_MSG_EQ (added, false, "XXX");
}


static class Ipv4AddressGeneratorTestSuite : public TestSuite
{
public:
  Ipv4AddressGeneratorTestSuite ()
    : TestSuite ("ipv4-address-generator")
  {
    AddTestCase (new NetworkNumberAllocatorTestCase ());
    AddTestCase (new AddressAllocatorTestCase ());
    AddTestCase (new NetworkAndAddressTestCase ());
    AddTestCase (new ExampleAddressGeneratorTestCase ());
    AddTestCase (new AddressCollisionTestCase ());
  }
} g_ipv4AddressGeneratorTestSuite;

} // namespace ns3

