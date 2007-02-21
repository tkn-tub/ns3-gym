/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
 * All rights reserved.
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

#include <string>
#include "ns3/debug.h"
#include "ns3/assert.h"
#include "ns3/packet.h"
#include "ns3/drop-tail.h"
#include "ns3/layer-connector.h"
#include "ns3/channel.h"

using namespace ns3;

// ===========================================================================
// Cook up a simplistic Internet Node
// ===========================================================================
class FakeInternetNode : public LayerConnectorUpper
{
public:
  FakeInternetNode ();
 ~FakeInternetNode ();

  void Doit (void);

protected:
  bool UpperDoSendUp (Packet &p);
  bool UpperDoPull (Packet &p);

  DropTailQueue m_dtqOutbound;
  DropTailQueue m_dtqInbound;
};

FakeInternetNode::FakeInternetNode ()
{
  NS_DEBUG_UNCOND("FakeInternetNode::FakeInternetNode ()");
}

FakeInternetNode::~FakeInternetNode ()
{
  NS_DEBUG_UNCOND("FakeInternetNode::~FakeInternetNode ()");
}

  void
FakeInternetNode::Doit (void)
{
  NS_DEBUG_UNCOND("FakeInternetNode::Doit ()");
  NS_DEBUG_UNCOND("FakeInternetNode::Doit (): **** Send outbound packet");
  Packet p;

  m_dtqOutbound.Enque(p);
  UpperNotify();
}

  bool
FakeInternetNode::UpperDoSendUp (Packet &p)
{
  NS_DEBUG_UNCOND("FakeInternetNode::UpperDoSendUp (" << &p << ")");

  NS_DEBUG_UNCOND("FakeInternetNode::UpperDoSendUp (): **** Receive inbound packet");
  m_dtqInbound.Enque(p);
  return m_dtqInbound.Deque(p);
}

  bool
FakeInternetNode::UpperDoPull (Packet &p)
{
  NS_DEBUG_UNCOND("FakeInternetNode::DoPull (" << &p << ")");

  return m_dtqOutbound.Deque(p);
}

// ===========================================================================
// Cook up a simplistic Physical Layer
// ===========================================================================
class FakePhysicalLayer :
  public LayerConnectorLower,
  public LayerConnectorUpper
{
public:
  FakePhysicalLayer ();
 ~FakePhysicalLayer ();

protected:
  bool LowerDoNotify (LayerConnectorUpper *upper);
  bool UpperDoSendUp (Packet &p);
  bool UpperDoPull (Packet &p);

  DropTailQueue m_dtqInbound;
  DropTailQueue m_dtqOutbound;
};

FakePhysicalLayer::FakePhysicalLayer ()
{
  NS_DEBUG_UNCOND("FakePhysicalLayer::FakePhysicalLayer ()");
}

FakePhysicalLayer::~FakePhysicalLayer ()
{
  NS_DEBUG_UNCOND("FakePhysicalLayer::~FakePhysicalLayer ()");
}

  bool
FakePhysicalLayer::LowerDoNotify (LayerConnectorUpper *upper)
{
  NS_DEBUG_UNCOND("FakePhysicalLayer::LowerDoNotify ()");

  Packet p;

  NS_DEBUG_UNCOND("FakePhysicalLayer::LowerDoNotify (): Starting pull");

  NS_ASSERT(m_upperPartner);
  m_upperPartner->UpperPull(p);

  m_dtqOutbound.Enque(p);

  NS_DEBUG_UNCOND("FakePhysicalLayer::LowerDoNotify (): Got bits,  Notify lower");

  NS_ASSERT(m_lowerPartner);
  return m_lowerPartner->LowerNotify(this);
}

  bool
FakePhysicalLayer::UpperDoSendUp (Packet &p)
{
  NS_DEBUG_UNCOND("FakePhysicalLayer::UpperDoSendUp (" << &p << ")");

  NS_ASSERT(m_upperPartner);
  return m_upperPartner->UpperSendUp(p);
}

  bool
FakePhysicalLayer::UpperDoPull (Packet &p)
{
  NS_DEBUG_UNCOND("FakePhysicalLayer::DoPull (" << &p << ")");

  return m_dtqOutbound.Deque(p);
}

// ===========================================================================
// Cook up a simplistic Channel, just to add any moby hack we feel like
// ===========================================================================
class FakeChannel : public Channel
{
public:
  FakeChannel ();
 ~FakeChannel ();
};

FakeChannel::FakeChannel ()
{
  NS_DEBUG_UNCOND("FakeChannel::FakeChannel ()");
}

FakeChannel::~FakeChannel ()
{
  NS_DEBUG_UNCOND("FakeChannel::~FakeChannel ()");
}

int main (int argc, char *argv[])
{
  NS_DEBUG_UNCOND("Channel Hackorama");

#if 0
  DebugComponentEnable("Queue");
  DebugComponentEnable("DropTailQueue");
  DebugComponentEnable("LayerConnector");
  DebugComponentEnable("Channel");
#endif

  FakeInternetNode      node1, node2, node3, node4;
  FakePhysicalLayer     phys1, phys2, phys3, phys4;
  FakeChannel           channel;
  Packet                pkt;

  // all the hassle above lets us do something very simple here

  node1.ConnectToLower(phys1);
  phys1.ConnectToUpper(node1);
  phys1.ConnectToLower(channel);
  channel.ConnectToUpper(phys1);

  node2.ConnectToLower(phys2);
  phys2.ConnectToUpper(node2);
  phys2.ConnectToLower(channel);
  channel.ConnectToUpper(phys2);

  node3.ConnectToLower(phys3);
  phys3.ConnectToUpper(node3);
  phys3.ConnectToLower(channel);
  channel.ConnectToUpper(phys3);

  node4.ConnectToLower(phys4);
  phys4.ConnectToUpper(node4);
  phys4.ConnectToLower(channel);
  channel.ConnectToUpper(phys4);

  node1.Doit();
  node2.Doit();
  node3.Doit();
  node4.Doit();

  return 0;
}
