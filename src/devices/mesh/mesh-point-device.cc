/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 *         Pavel Boyko <boyko@iitp.ru>
 */


#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/mesh-point-device.h"
#include "ns3/wifi-net-device.h"
#include "ns3/mesh-wifi-interface-mac.h"

NS_LOG_COMPONENT_DEFINE ("MeshPointDevice");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (MeshPointDevice);

TypeId
MeshPointDevice::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::MeshPointDevice")
                      .SetParent<NetDevice> ()
                      .AddConstructor<MeshPointDevice> ()
                      .AddAttribute ("RoutingProtocol", "The mesh routing protocol used by this mesh point.",
                          PointerValue (),
                          MakePointerAccessor (&MeshPointDevice::GetRoutingProtocol,
                                               &MeshPointDevice::SetRoutingProtocol),
                          MakePointerChecker<MeshL2RoutingProtocol> ())
                      ;
  return tid;
}

MeshPointDevice::MeshPointDevice () : m_ifIndex (0), m_mtu(1500)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_channel = CreateObject<BridgeChannel> ();
}

MeshPointDevice::~MeshPointDevice ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
MeshPointDevice::DoDispose ()
{
  NS_LOG_FUNCTION_NOARGS ();
  for (std::vector< Ptr<NetDevice> >::iterator iter = m_ifaces.begin (); iter != m_ifaces.end (); iter++)
    *iter = 0;
  m_ifaces.clear ();
  m_node = 0;
  NetDevice::DoDispose ();

}

//-----------------------------------------------------------------------------
// NetDevice interface implementation
//-----------------------------------------------------------------------------

void
MeshPointDevice::ReceiveFromDevice (Ptr<NetDevice> incomingPort, Ptr<const Packet> packet, uint16_t protocol,
                                       Address const &src, Address const &dst, PacketType packetType)
{
  NS_LOG_FUNCTION_NOARGS ();
  NS_LOG_DEBUG ("UID is " << packet->GetUid ());
  const Mac48Address src48 = Mac48Address::ConvertFrom (src);
  const Mac48Address dst48 = Mac48Address::ConvertFrom (dst);
  NS_LOG_DEBUG ("SRC="<<src48<<", DST = "<<dst48<<", I am: "<<m_address);
  if (!m_promiscRxCallback.IsNull ())
    m_promiscRxCallback (this, packet, protocol, src, dst, packetType);
  if(dst48.IsGroup ())
  {
    Forward (incomingPort, packet, protocol, src48, dst48);
    m_rxCallback (this, packet->Copy (), protocol, src);
    return;
  }
  if(dst48 == m_address)
    m_rxCallback (this, packet, protocol, src);
  else
    Forward (incomingPort, packet->Copy (), protocol, src48, dst48);
}

void
MeshPointDevice::Forward (Ptr<NetDevice> inport, Ptr<const Packet> packet,
                             uint16_t protocol, const Mac48Address src, const Mac48Address dst)
{
  // pass through routing protocol
  m_requestRoute (inport->GetIfIndex(), src, dst, packet, protocol, m_myResponse);
}

void
MeshPointDevice::SetName (const std::string name)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_name = name;
}

std::string
MeshPointDevice::GetName () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_name;
}

void
MeshPointDevice::SetIfIndex (const uint32_t index)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_ifIndex = index;
}

uint32_t
MeshPointDevice::GetIfIndex () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_ifIndex;
}

Ptr<Channel>
MeshPointDevice::GetChannel () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_channel;
}

Address
MeshPointDevice::GetAddress () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_address;
}

bool
MeshPointDevice::SetMtu (const uint16_t mtu)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_mtu = mtu;
  return true;
}

uint16_t
MeshPointDevice::GetMtu () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_mtu;
}

bool
MeshPointDevice::IsLinkUp () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

void
MeshPointDevice::SetLinkChangeCallback (Callback<void> callback)
{
  // do nothing
}

bool
MeshPointDevice::IsBroadcast () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address
MeshPointDevice::GetBroadcast () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return Mac48Address ("ff:ff:ff:ff:ff:ff");
}

bool
MeshPointDevice::IsMulticast () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

Address
MeshPointDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_LOG_FUNCTION (this << multicastGroup);
  Mac48Address multicast = Mac48Address::GetMulticast (multicastGroup);
  return multicast;
}

bool
MeshPointDevice::IsPointToPoint () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return false;
}

bool
MeshPointDevice::IsBridge () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return false;
}


bool
MeshPointDevice::Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  const Mac48Address dst48 = Mac48Address::ConvertFrom (dest);
  NS_LOG_DEBUG("SEND:, DST = "<<dst48<<", I am: "<<m_address);
  return m_requestRoute (m_ifIndex, m_address, dst48, packet, protocolNumber, m_myResponse);
}

bool
MeshPointDevice::SendFrom (Ptr<Packet> packet, const Address& src, const Address& dest, uint16_t protocolNumber)
{
  const Mac48Address src48 = Mac48Address::ConvertFrom (src);
  const Mac48Address dst48 = Mac48Address::ConvertFrom (dest);
  return m_requestRoute (m_ifIndex, src48, dst48, packet, protocolNumber, m_myResponse);
}

Ptr<Node>
MeshPointDevice::GetNode () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_node;
}

void
MeshPointDevice::SetNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_node = node;
}

bool
MeshPointDevice::NeedsArp () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return true;
}

void
MeshPointDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_rxCallback = cb;
}

void
MeshPointDevice::SetPromiscReceiveCallback (NetDevice::PromiscReceiveCallback cb)
{
  NS_LOG_FUNCTION_NOARGS ();
  m_promiscRxCallback = cb;
}

bool
MeshPointDevice::SupportsSendFrom () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return false; // don't allow to bridge mesh network with something else.
}

Address
MeshPointDevice::GetMulticast (Ipv6Address addr) const
{
  NS_LOG_FUNCTION (this << addr);
  return Mac48Address::GetMulticast (addr);
}

//-----------------------------------------------------------------------------
// Interfaces
//-----------------------------------------------------------------------------
uint32_t
MeshPointDevice::GetNInterfaces () const
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_ifaces.size ();
}

Ptr<NetDevice>
MeshPointDevice::GetInterface (uint32_t n) const
{
  for(std::vector< Ptr<NetDevice> >::const_iterator i = m_ifaces.begin (); i != m_ifaces.end (); i ++)
    if((*i)->GetIfIndex() == n)
      return (*i);
  
  NS_FATAL_ERROR ("Mesh point interface is not found by index");
  return 0;
}
std::vector<Ptr<NetDevice> >
MeshPointDevice::GetInterfaces () const
{
  return m_ifaces;
}
void
MeshPointDevice::AddInterface (Ptr<NetDevice> iface)
{
  NS_LOG_FUNCTION_NOARGS ();
  
  NS_ASSERT (iface != this);
  if (!Mac48Address::IsMatchingType (iface->GetAddress ()))
  {
    NS_FATAL_ERROR ("Device does not support eui 48 addresses: cannot be used as a mesh point interface.");
  }
  if (!iface->SupportsSendFrom ())
  {
    NS_FATAL_ERROR ("Device does not support SendFrom: cannot be used as a mesh point interface.");
  }
  
  // Mesh point has MAC address of it's first interface
  if (m_ifaces.empty()) 
    m_address = Mac48Address::ConvertFrom (iface->GetAddress ());
  
  const WifiNetDevice * wifiNetDev = dynamic_cast<const WifiNetDevice *> (PeekPointer (iface));
  if (wifiNetDev == 0)
    NS_FATAL_ERROR ("Device is not a WiFi NIC: cannot be used as a mesh point interface.");
      
  MeshWifiInterfaceMac * ifaceMac = dynamic_cast<MeshWifiInterfaceMac *> (PeekPointer (wifiNetDev->GetMac ()));
  if (ifaceMac == 0)
    NS_FATAL_ERROR ("WiFi device doesn't have correct MAC installed: cannot be used as a mesh point interface.");

  ifaceMac->SetMeshPointAddress (m_address);
  
  // Receive frames from this interface
  m_node->RegisterProtocolHandler (MakeCallback (&MeshPointDevice::ReceiveFromDevice, this),
                                   0, iface, /*promiscuous = */true);
  
  m_ifaces.push_back (iface);
  m_channel->AddChannel (iface->GetChannel ());
}

//-----------------------------------------------------------------------------
// Protocols
//-----------------------------------------------------------------------------

void
MeshPointDevice::SetRoutingProtocol (Ptr<MeshL2RoutingProtocol> protocol)
{
  NS_LOG_FUNCTION_NOARGS ();
  
  NS_ASSERT_MSG (PeekPointer(protocol->GetMeshPoint()) == this, "Routing protocol must be installed on mesh point to be useful.");
  
  m_routingProtocol = protocol;
  m_requestRoute = MakeCallback (&MeshL2RoutingProtocol::RequestRoute, protocol);
  m_myResponse = MakeCallback (&MeshPointDevice::DoSend, this);
}

Ptr<MeshL2RoutingProtocol>
MeshPointDevice::GetRoutingProtocol () const
{
  return m_routingProtocol;
}

void
MeshPointDevice::DoSend (bool success, Ptr<Packet> packet, Mac48Address src, Mac48Address dst, uint16_t protocol, uint32_t outIface)
{
  if (!success)
    {
      NS_LOG_DEBUG ("Resolve failed");
      return;
    }
  // Ok, now I know the route, just SendFrom
  if (outIface != 0xffffffff)
    GetInterface (outIface)->SendFrom(packet, src, dst, protocol);
  else
    for (std::vector<Ptr<NetDevice> >::iterator i = m_ifaces.begin (); i != m_ifaces.end(); i++)
      (*i) -> SendFrom (packet, src, dst, protocol);
}

} // namespace ns3
