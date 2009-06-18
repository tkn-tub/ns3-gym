/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */

#include "flame-protocol.h"
#include "flame-rtable.h"
#include "flame-header.h"
#include "ns3/llc-snap-header.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/mesh-point-device.h"
#include "ns3/wifi-net-device.h"
#include "ns3/mesh-point-device.h"
#include "ns3/mesh-wifi-interface-mac.h"
#include "ns3/random-variable.h"

namespace ns3 {
namespace flame {
//-----------------------------------------------------------------------------
// FlameTag
//-----------------------------------------------------------------------------
NS_OBJECT_ENSURE_REGISTERED (FlameTag);

TypeId
FlameTag::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::flame::FlameTag")
     .SetParent<Tag> ()
     .AddConstructor<FlameTag> ();
   return tid;
}

TypeId
FlameTag::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
FlameTag::GetSerializedSize () const
{
  return 6;
}

void
FlameTag::Serialize (TagBuffer i) const
{
  uint8_t buf[6];
  address.CopyTo (buf);
  for (int j = 0; j < 6; j ++)
    i.WriteU8 (buf[j]);
}

void
FlameTag::Deserialize (TagBuffer i)
{
  uint8_t buf[6];
  for (int j = 0; j < 6; j ++)
    buf[j] = i.ReadU8 ();
  address.CopyFrom (buf);
}

void
FlameTag::Print (std::ostream &os) const
{
  os << "address = " << address;
}

//-----------------------------------------------------------------------------
// FlameTag
//-----------------------------------------------------------------------------
TypeId
FlameProtocol::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::flame::FlameProtocol")
    .SetParent<MeshL2RoutingProtocol> ()
    .AddConstructor<FlameProtocol> ()
    .AddAttribute ("BroadcastInterval", "How often we must send broadcast packets",
        TimeValue (Seconds (5)),
        MakeTimeAccessor (&FlameProtocol::m_broadcastInterval),
        MakeTimeChecker ()
        );
  return tid;
}
FlameProtocol::FlameProtocol () :
  m_broadcastInterval (Seconds (5)),
  m_lastBroadcast (Simulator::Now ()),
  m_myLastSeqno (0)
{
}
FlameProtocol::~FlameProtocol ()
{
}
void
FlameProtocol::DoDispose ()
{
}
bool
FlameProtocol::RequestRoute (uint32_t  sourceIface, const Mac48Address source, const Mac48Address destination,
      Ptr<const Packet> const_packet, uint16_t  protocolType, RouteReplyCallback  routeReply)
{
  Ptr<Packet> packet = const_packet->Copy ();
  if (source == m_address)
  {
    //Packet from upper layer!
    if(destination == Mac48Address::GetBroadcast ())
    {
      //Broadcast always is forwarded as broadcast!
      FlameHeader flameHdr;
      FlameTag tag (Mac48Address::GetBroadcast ());
      flameHdr.AddCost (0);
      flameHdr.SetSeqno (m_myLastSeqno ++);
      flameHdr.SetProtocol (protocolType);
      flameHdr.SetOrigDst (destination);
      flameHdr.SetOrigSrc (source);
      packet->AddHeader (flameHdr);
      packet->AddPacketTag (tag);
      routeReply (true, packet, source, destination, FLAME_PORT, FlameRtable::INTERFACE_ANY);
    }
    else
      NS_FATAL_ERROR ("unicast not done yet!");
  }
  else
  {
    FlameHeader flameHdr;
    packet->RemoveHeader (flameHdr); 
    //if(DropDataFrame(flameHdr.GetSeqno (), source))
    //  return false;
    if(destination == Mac48Address::GetBroadcast ())
    {
      //Broadcast always is forwarded as broadcast!
      //Broadcast was filtered in RemoveRoutingStuff, because mesh
      //point device first calss it
      FlameTag tag (Mac48Address::GetBroadcast ());
      flameHdr.AddCost (1);
      packet->AddHeader (flameHdr);
      packet->AddPacketTag (tag);
      routeReply (true, packet, source, destination, FLAME_PORT, FlameRtable::INTERFACE_ANY);
      return true;
    }
    else
    {
      if(DropDataFrame(flameHdr.GetSeqno (), source))
        return false;
      NS_FATAL_ERROR ("not done yet!");
    }
    return true;
  }
  return false;
}
bool
FlameProtocol::RemoveRoutingStuff (uint32_t fromIface, const Mac48Address source,
      const Mac48Address destination, Ptr<Packet>  packet, uint16_t&  protocolType)
{
  //Filter seqno:
  FlameHeader flameHdr;
  packet->RemoveHeader (flameHdr);
  NS_ASSERT(protocolType == FLAME_PORT);
  protocolType = flameHdr.GetProtocol ();
  if(DropDataFrame(flameHdr.GetSeqno (), source))
    return false;
  return true;
}
bool
FlameProtocol::Install (Ptr<MeshPointDevice> mp)
{
  m_mp = mp;
  std::vector<Ptr<NetDevice> > interfaces = mp->GetInterfaces ();
  for (std::vector<Ptr<NetDevice> >::const_iterator i = interfaces.begin (); i != interfaces.end(); i++)
    {
      // Checking for compatible net device
      Ptr<WifiNetDevice> wifiNetDev = (*i)->GetObject<WifiNetDevice> ();
      if (wifiNetDev == 0)
        return false;
      Ptr<MeshWifiInterfaceMac>  mac = wifiNetDev->GetMac ()->GetObject<MeshWifiInterfaceMac> ();
      if (mac == 0)
        return false;
      // Installing plugins:
      Ptr<FlameMacPlugin> flameMac = Create<FlameMacPlugin> (wifiNetDev->GetIfIndex (), this);
      m_interfaces[wifiNetDev->GetIfIndex ()] = flameMac;
      mac->InstallPlugin (flameMac);
    }
  mp->SetRoutingProtocol (this);
  // Mesh point aggregates all installed protocols
  mp->AggregateObject (this);
  m_address = Mac48Address::ConvertFrom (mp->GetAddress ());//* address;
  return true;
}
Mac48Address
FlameProtocol::GetAddress ()
{
  return m_address;
}
bool
FlameProtocol::DropDataFrame(uint16_t seqno, Mac48Address source)
{
  if(source == GetAddress ())
    return true;
  std::map<Mac48Address, uint16_t,std::less<Mac48Address> >::const_iterator i = m_lastSeqno.find (source);
  if (i == m_lastSeqno.end ())
    m_lastSeqno[source] = seqno;
  else
  {
    if (i->second >= seqno)
      return true;
    m_lastSeqno[source] = seqno;
  }
  return false;
}

} //namespace flame
} //namespace ns3
