/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008-2009 Strasbourg University
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

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/node.h"
#include "ns3/net-device.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv6.h"

#include "ipv6-address-helper.h"

namespace ns3 
{

NS_LOG_COMPONENT_DEFINE ("Ipv6AddressHelper");

Ipv6AddressHelper::Ipv6AddressHelper ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_network = Ipv6Address ("2001::");
  m_prefix = Ipv6Prefix (64);
}

Ipv6Address Ipv6AddressHelper::NewAddress (Address addr)
{
  NS_LOG_FUNCTION (this << addr);

  switch (addr.GetLength ())
    {
    case 6:
      return Ipv6Address::MakeAutoconfiguredAddress (Mac48Address::ConvertFrom (addr), m_network);
    default:
      return Ipv6Address ("::");
    }
  /* never reached */
  return Ipv6Address ("::");
}

void Ipv6AddressHelper::NewNetwork (Ipv6Address network, Ipv6Prefix prefix)
{
  NS_LOG_FUNCTION (this << network << prefix);

  m_network = network;
  m_prefix = prefix;
}

Ipv6InterfaceContainer Ipv6AddressHelper::Assign (const NetDeviceContainer &c)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv6InterfaceContainer retval;

  for (uint32_t i = 0; i < c.GetN (); ++i) 
    {
      Ptr<NetDevice> device = c.Get (i);

      Ptr<Node> node = device->GetNode ();
      NS_ASSERT_MSG (node, "Ipv6AddressHelper::Allocate (): Bad node");

      Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
      NS_ASSERT_MSG (ipv6, "Ipv6AddressHelper::Allocate (): Bad ipv6");
      int32_t ifIndex = 0;

      ifIndex = ipv6->GetInterfaceForDevice (device);
      if (ifIndex == -1)
        {
          ifIndex = ipv6->AddInterface (device);
        }
      NS_ASSERT_MSG (ifIndex >= 0, "Ipv6AddressHelper::Allocate (): "
                     "Interface index not found");

      Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (NewAddress (device->GetAddress ()), m_prefix);
      ipv6->SetMetric (ifIndex, 1);
      ipv6->SetUp (ifIndex);
      ipv6->AddAddress (ifIndex, ipv6Addr);

      retval.Add (ipv6, ifIndex);
    }
  return retval;
}

Ipv6InterfaceContainer Ipv6AddressHelper::Assign (const NetDeviceContainer &c, std::vector<bool> withConfiguration)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv6InterfaceContainer retval;
  for (uint32_t i = 0; i < c.GetN (); ++i) 
    {
      Ptr<NetDevice> device = c.Get (i);

      Ptr<Node> node = device->GetNode ();
      NS_ASSERT_MSG (node, "Ipv6AddressHelper::Allocate (): Bad node");

      Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
      NS_ASSERT_MSG (ipv6, "Ipv6AddressHelper::Allocate (): Bad ipv6");

      int32_t ifIndex = ipv6->GetInterfaceForDevice (device);
      if (ifIndex == -1)
        {
          ifIndex = ipv6->AddInterface (device);
        }
      NS_ASSERT_MSG (ifIndex >= 0, "Ipv6AddressHelper::Allocate (): "
                     "Interface index not found");

      ipv6->SetMetric (ifIndex, 1);
      ipv6->SetUp (ifIndex);

      if (withConfiguration.at (i))
        {
          Ipv6InterfaceAddress ipv6Addr = Ipv6InterfaceAddress (NewAddress (device->GetAddress ()), m_prefix);
          ipv6->AddAddress (ifIndex, ipv6Addr);
        }

      retval.Add (ipv6, ifIndex);
    }
  return retval;
}

Ipv6InterfaceContainer Ipv6AddressHelper::AssignWithoutAddress (const NetDeviceContainer &c)
{
  NS_LOG_FUNCTION_NOARGS ();
  Ipv6InterfaceContainer retval;
  for (uint32_t i = 0; i < c.GetN (); ++i) 
    {
      Ptr<NetDevice> device = c.Get (i);

      Ptr<Node> node = device->GetNode ();
      NS_ASSERT_MSG (node, "Ipv6AddressHelper::Allocate (): Bad node");

      Ptr<Ipv6> ipv6 = node->GetObject<Ipv6> ();
      NS_ASSERT_MSG (ipv6, "Ipv6AddressHelper::Allocate (): Bad ipv6");

      int32_t ifIndex = ipv6->GetInterfaceForDevice (device);
      if (ifIndex == -1)
        {
          ifIndex = ipv6->AddInterface (device);
        }
      NS_ASSERT_MSG (ifIndex >= 0, "Ipv6AddressHelper::Allocate (): "
                     "Interface index not found");

      ipv6->SetMetric (ifIndex, 1);
      ipv6->SetUp (ifIndex);

      retval.Add (ipv6, ifIndex);
    }
  return retval;
}

} /* namespace ns3 */

