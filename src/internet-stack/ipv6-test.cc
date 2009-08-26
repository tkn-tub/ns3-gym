/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Strasbourg University
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
 *
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#ifdef RUN_SELF_TESTS

#include "ns3/simulator.h"
#include "ns3/test.h"
#include "ns3/log.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/node.h"
#include "ns3/csma-net-device.h"

#include "icmpv6-l4-protocol.h"
#include "ipv6-interface.h"

namespace ns3
{

/**
 * \class Ipv6L3ProtocolTest
 * \brief Ipv6L3Protocol unit tests.
 */
class Ipv6L3ProtocolTest : public Test
{
  public:
    /**
     * \brief Constructor.
     */
    Ipv6L3ProtocolTest ();

    /**
     * \brief Destructor.
     */
    virtual ~Ipv6L3ProtocolTest ();

    /**
     * \brief Run unit tests for this class.
     * \return true if all tests have passed, false otherwise
     */
    virtual bool RunTests ();
};

Ipv6L3ProtocolTest::Ipv6L3ProtocolTest () : Test ("Ipv6L3Protocol")
{
}

Ipv6L3ProtocolTest::~Ipv6L3ProtocolTest ()
{
}

bool Ipv6L3ProtocolTest::RunTests ()
{
  bool result = true;
  Ptr<Node> node = CreateObject<Node> ();
  Ptr<Ipv6L3Protocol> ipv6 = CreateObject<Ipv6L3Protocol> ();
  Ptr<Icmpv6L4Protocol> icmpv6 = CreateObject<Icmpv6L4Protocol> ();
  Ptr<Ipv6Interface> interface = CreateObject<Ipv6Interface> ();
  Ptr<Ipv6Interface> interface2 = CreateObject<Ipv6Interface> ();
  Ptr<CsmaNetDevice> device = CreateObject<CsmaNetDevice> ();
  Ptr<CsmaNetDevice> device2 = CreateObject<CsmaNetDevice> ();
  uint32_t index = 0;

  /* init */
  node->AggregateObject (ipv6);
  node->AggregateObject (icmpv6);
  ipv6->Insert (icmpv6);

  /* first real interface (loopback is also installed) */
  node->AddDevice (device);
  interface->SetDevice (device);
  interface->SetNode (node);
  index = ipv6->AddIpv6Interface (interface);
  NS_TEST_ASSERT_EQUAL (index, 1);

  /* second interface */
  node->AddDevice (device2);
  interface2->SetDevice (device2);
  interface2->SetNode (node);
  index = ipv6->AddIpv6Interface (interface2);
  NS_TEST_ASSERT_EQUAL (index, 2);

  Ipv6InterfaceAddress ifaceAddr = interface->GetLinkLocalAddress ();
  NS_TEST_ASSERT_EQUAL (ifaceAddr.GetAddress ().IsLinkLocal (), true);

  interface->SetUp ();
  NS_TEST_ASSERT_EQUAL (interface->GetNAddresses (), 1); /* interface has always a link-local address */

  interface2->SetUp ();

  Ipv6InterfaceAddress ifaceAddr1 = Ipv6InterfaceAddress ("2001:1234:5678:9000::1", Ipv6Prefix (64));
  interface->AddAddress (ifaceAddr1);
  Ipv6InterfaceAddress ifaceAddr2 = Ipv6InterfaceAddress ("2001:ffff:5678:9000::1", Ipv6Prefix (64));
  interface->AddAddress (ifaceAddr2);

  Ipv6InterfaceAddress ifaceAddr3 = Ipv6InterfaceAddress ("2001:ffff:5678:9001::2", Ipv6Prefix (64));
  interface2->AddAddress (ifaceAddr3);

  uint32_t num = interface->GetNAddresses ();
  NS_TEST_ASSERT_EQUAL (num, 3); /* 2 global addresses + link-local ones */

  num = interface2->GetNAddresses ();
  NS_TEST_ASSERT_EQUAL (num, 2); /* 1 global addresses + link-local ones */

  interface->RemoveAddress (2);
  num = interface->GetNAddresses ();
  NS_TEST_ASSERT_EQUAL (num, 2);

  Ipv6InterfaceAddress output = interface->GetAddress (1);
  NS_TEST_ASSERT_EQUAL (ifaceAddr1, output);

  index = ipv6->GetInterfaceForPrefix ("2001:1234:5678:9000::0", Ipv6Prefix (64));
  NS_TEST_ASSERT_EQUAL (index, 1); /* link-local address is always index 0 */

  index = ipv6->GetInterfaceForAddress ("2001:ffff:5678:9001::2");
  NS_TEST_ASSERT_EQUAL (index, 2);

  index = ipv6->GetInterfaceForAddress ("2001:ffff:5678:9000::1"); /* address we just remove */
  NS_TEST_ASSERT_EQUAL (index, (uint32_t)-1);

  return result;
}

static Ipv6L3ProtocolTest gIpv6L3ProtocolTest;

} /* namespace ns3 */

#endif /* RUN_SELF_TESTS */

