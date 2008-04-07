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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/ipv4.h"
#include "ns3/ipv4-address-generator.h"
#include "ipv4-address-helper.h"

NS_LOG_COMPONENT_DEFINE("Ipv4AddressHelper");

namespace ns3 {

Ipv4AddressHelper::Ipv4AddressHelper () 
{
  NS_LOG_FUNCTION;

//
// Set the default values to an illegal state.  Do this so the client is 
// forced to think at least briefly about what addresses get used and what
// is going on here.
//
  m_network = 0xffffffff;
  m_mask = 0;
  m_address = 0xffffffff;
  m_base = 0xffffffff;
  m_shift = 0xffffffff;
  m_max = 0xffffffff;
}

  void
Ipv4AddressHelper::SetBase (
  const Ipv4Address network, 
  const Ipv4Mask mask,
  const Ipv4Address address)
{
  NS_LOG_FUNCTION;

  m_network = network.GetHostOrder ();
  m_mask = mask.GetHostOrder ();
  m_base = m_address = address.GetHostOrder ();

//
// Some quick reasonableness testing.
//
  NS_ASSERT_MSG((m_network & ~m_mask) == 0,
    "Ipv4AddressHelper::SetBase(): Inconsistent network and mask");

  NS_ASSERT_MSG((m_address & m_mask) == 0,
    "Ipv4AddressHelper::SetBase(): Inconsistent address and mask");

//
// Figure out how much to shift network numbers to get them aligned, and what
// the maximum allowed address is with respect to the current mask.
//
  m_shift = NumAddressBits (m_mask);
  m_max = (1 << m_shift) - 2;

  NS_ASSERT_MSG(m_shift <= 32,
    "Ipv4AddressHelper::SetBase(): Unreasonable address length");

//
// Shift the network down into the normalized position.
//
  m_network >>= m_shift;

  NS_LOG_LOGIC ("m_network == " << m_network);
  NS_LOG_LOGIC ("m_mask == " << m_mask);
  NS_LOG_LOGIC ("m_address == " << m_address);
}

  Ipv4Address
Ipv4AddressHelper::NewAddress (void)
{
//
// The way this is expected to be used is that an address and network number
// are initialized, and then NewAddress() is called repeatedly to allocate and
// get new addresses on a given subnet.  The client will expect that the first
// address she gets back is the one she used to initialize the generator with.
// This implies that this operation is a post-increment.
//
  NS_ASSERT_MSG (m_address <= m_max,
    "Ipv4AddressHelper::NewAddress(): Address overflow");

  Ipv4Address addr ((m_network << m_shift) | m_address);
  ++m_address;
//
// The Ipv4AddressGenerator allows us to keep track of the addresses we have
// allocated and will assert if we accidentally generate a duplicate.  This
// avoids some really hard to debug problems.
//
  Ipv4AddressGenerator::AddAllocated (addr);
  return addr;
}

  Ipv4Address
Ipv4AddressHelper::NewNetwork (void)
{
  NS_LOG_FUNCTION;
  ++m_network;
  m_address = m_base;
  return Ipv4Address (m_network << m_shift);
}

Ipv4InterfaceContainer
Ipv4AddressHelper::Assign (const NetDeviceContainer &c)
{
  NS_LOG_FUNCTION;
  Ipv4InterfaceContainer retval;
  for (uint32_t i = 0; i < c.GetN (); ++i) {
    Ptr<NetDevice> device = c.Get (i);

    Ptr<Node> node = device->GetNode ();
    NS_ASSERT_MSG (node, "Ipv4AddressHelper::Allocate(): Bad node");

    Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
    NS_ASSERT_MSG (ipv4, "Ipv4AddressHelper::Allocate(): Bad ipv4");

    int32_t ifIndex = ipv4->FindInterfaceForDevice (device);
    if (ifIndex == -1)
      {
        ifIndex = ipv4->AddInterface (device);
      }
    NS_ASSERT_MSG (ifIndex >= 0, "Ipv4AddressHelper::Allocate(): "
      "Interface index not found");

    ipv4->SetAddress (ifIndex, NewAddress ());
    ipv4->SetNetworkMask (ifIndex, m_mask);
    ipv4->SetMetric (ifIndex, 1);
    ipv4->SetUp (ifIndex);
    retval.Add (ipv4, ifIndex);
  }
  return retval;
}

const uint32_t N_BITS = 32;

  uint32_t
Ipv4AddressHelper::NumAddressBits (uint32_t maskbits) const
{
  NS_LOG_FUNCTION;
  for (uint32_t i = 0; i < N_BITS; ++i)
    {
      if (maskbits & 1)
        {
          NS_LOG_LOGIC ("NumAddressBits -> " << i);
          return i;
        }
      maskbits >>= 1;
    }

  NS_ASSERT_MSG(false, "Ipv4AddressHelper::NumAddressBits(): Bad Mask");
  return 0;
}

}; // namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class AddressHelperTest : public Test
{
public:
  AddressHelperTest ();
  virtual bool RunTests (void);
};

static AddressHelperTest g_addressHelperTest;

AddressHelperTest::AddressHelperTest ()
  : Test ("AddressHelper")
{
}

  bool
AddressHelperTest::RunTests (void)
{
  bool result = true;
  Ipv4Address network;
  Ipv4Address address;

//
// Make sure the network allocator part is working on some common network
// prefixes.
//
  Ipv4AddressHelper h;

  h.SetBase ("1.0.0.0", "255.0.0.0");
  network = h.NewNetwork();
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("2.0.0.0"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("2.0.0.1"));

  h.SetBase ("0.1.0.0", "255.255.0.0");
  network = h.NewNetwork();
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.2.0.0"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.2.0.1"));

  h.SetBase ("0.0.1.0", "255.255.255.0");
  network = h.NewNetwork();
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.0.2.0"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.2.1"));

//
// Make sure the address allocator part is working.
//
  h.SetBase ("1.0.0.0", "255.0.0.0", "0.0.0.3");
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("1.0.0.3"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("1.0.0.4"));

  h.SetBase ("0.1.0.0", "255.255.0.0", "0.0.0.3");
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.1.0.3"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.1.0.4"));

  h.SetBase ("0.0.1.0", "255.255.255.0", "0.0.0.3");
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.1.3"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.1.4"));

//
// Make sure the reset to base behavior is working.  We're going to use some
// of the same addresses allocated above, so reset the Ipv4AddressGenerator
// to make it forget we did.
//
  Ipv4AddressGenerator::Reset ();

  h.SetBase ("1.0.0.0", "255.0.0.0", "0.0.0.3");
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("1.0.0.3"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("1.0.0.4"));
  network = h.NewNetwork();
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("2.0.0.0"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("2.0.0.3"));

  h.SetBase ("0.1.0.0", "255.255.0.0", "0.0.0.3");
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.1.0.3"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.1.0.4"));
  network = h.NewNetwork();
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.2.0.0"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.2.0.3"));

  h.SetBase ("0.0.1.0", "255.255.255.0", "0.0.0.3");
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.1.3"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.1.4"));
  network = h.NewNetwork();
  NS_TEST_ASSERT_EQUAL (network, Ipv4Address ("0.0.2.0"));
  address = h.NewAddress();
  NS_TEST_ASSERT_EQUAL (address, Ipv4Address ("0.0.2.3"));

  return true;
}

} // namespace ns3

#endif /* RUN_SELF_TEST */
