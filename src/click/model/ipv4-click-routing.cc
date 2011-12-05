/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Lalith Suresh
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
 * Authors: Lalith Suresh <suresh.lalith@gmail.com>
 */


#ifdef NS3_CLICK

#include "ns3/node.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-click-protocol.h"

#include "ipv4-click-routing.h"
#include <string>
#include <map>

#include <cstdlib>
#include <cstdarg>

NS_LOG_COMPONENT_DEFINE ("Ipv4ClickRouting");

namespace ns3 {

// Values from nsclick ExtRouter implementation
#define INTERFACE_ID_KERNELTAP 0
#define INTERFACE_ID_FIRST 1
#define INTERFACE_ID_FIRST_DROP 33

NS_OBJECT_ENSURE_REGISTERED (Ipv4ClickRouting);

std::map < simclick_node_t *, Ptr<Ipv4ClickRouting> > Ipv4ClickRouting::m_clickInstanceFromSimNode;

TypeId
Ipv4ClickRouting::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ipv4ClickRouting")
    .SetParent<Ipv4RoutingProtocol> ()
    .AddConstructor<Ipv4ClickRouting> ()
  ;

  return tid;
}

Ipv4ClickRouting::Ipv4ClickRouting ()
  : m_nonDefaultName (false),
    m_ipv4 (0)
{
}

Ipv4ClickRouting::~Ipv4ClickRouting ()
{
}

void
Ipv4ClickRouting::DoStart ()
{
  uint32_t id = m_ipv4->GetObject<Node> ()->GetId ();

  if (!m_nonDefaultName)
    {
      std::stringstream name;
      name << "Node" << id;
      m_nodeName = name.str ();
    }

  m_simNode = new simclick_node_t;
  timerclear (&m_simNode->curtime);

  AddSimNodeToClickMapping ();

  NS_ASSERT (m_clickFile.length () > 0);

  // Even though simclick_click_create() will halt programme execution
  // if it is unable to initialise a Click router, we play safe
  if (simclick_click_create (m_simNode, m_clickFile.c_str ()) >= 0)
    {
      NS_LOG_DEBUG (m_nodeName << " has initialised a Click Router");
      m_clickInitialised = true;
    }
  else
    {
      NS_LOG_DEBUG ("Click Router Initialisation failed for " << m_nodeName);
      m_clickInitialised = false;
    }

  NS_ASSERT (m_clickInitialised == true);
  simclick_click_run (m_simNode);
}

void
Ipv4ClickRouting::SetIpv4 (Ptr<Ipv4> ipv4)
{
  m_ipv4 = ipv4;
}

void
Ipv4ClickRouting::DoDispose ()
{
  m_ipv4 = 0;
  delete m_simNode;
  Ipv4RoutingProtocol::DoDispose ();
}

void
Ipv4ClickRouting::SetClickFile (std::string clickfile)
{
  m_clickFile = clickfile;
}

void
Ipv4ClickRouting::SetClickRoutingTableElement (std::string name)
{
  m_clickRoutingTableElement = name;
}

void
Ipv4ClickRouting::SetNodeName (std::string name)
{
  m_nodeName = name;
  m_nonDefaultName = true;
}

std::string
Ipv4ClickRouting::GetNodeName ()
{
  return m_nodeName;
}

int
Ipv4ClickRouting::GetInterfaceId (const char *ifname)
{
  int retval = -1;

  // The below hard coding of interface names follows the
  // same approach as used in the original nsclick code for
  // ns-2. The interface names map directly to what is to
  // be used in the Click configuration files.
  // Thus eth0 will refer to the first network device of
  // the node, and is to be named so in the Click graph.
  // This function is called by Click during the intialisation
  // phase of the Click graph, during which it tries to map
  // interface IDs to interface names. The return value
  // corresponds to the interface ID that Click will use.

  // Tap/tun devices refer to the kernel devices
  if (strstr (ifname, "tap") || strstr (ifname, "tun"))
    {
      retval = 0;
    }
  else if (const char *devname = strstr (ifname, "eth"))
    {
      while (*devname && !isdigit ((unsigned char) *devname))
        {
          devname++;
        }

      if (*devname)
        {
          retval = atoi (devname) + INTERFACE_ID_FIRST;
        }
    }
  else if (const char *devname = strstr (ifname, "drop"))
    {
      while (*devname && !isdigit ((unsigned char) *devname))
        {
          devname++;
        }
      if (*devname)
        {
          retval = atoi (devname) + INTERFACE_ID_FIRST_DROP;
        }
    }

  // This protects against a possible inconsistency of having
  // more interfaces defined in the Click graph
  // for a Click node than are defined for it in
  // the simulation script
  if (retval >= (int) m_ipv4->GetNInterfaces ())
    {
      return -1;
    }

  return retval;
}

bool
Ipv4ClickRouting::IsInterfaceReady (int ifid)
{
  if (ifid >= 0 && ifid < (int) m_ipv4->GetNInterfaces ())
    {
      return true;
    }
  else
    {
      return false;
    }
}

std::string
Ipv4ClickRouting::GetIpAddressFromInterfaceId (int ifid)
{
  std::stringstream addr;
  m_ipv4->GetAddress (ifid, 0).GetLocal ().Print (addr);

  return addr.str ();
}

std::string
Ipv4ClickRouting::GetIpPrefixFromInterfaceId (int ifid)
{
  std::stringstream addr;
  m_ipv4->GetAddress (ifid, 0).GetMask ().Print (addr);

  return addr.str ();
}

std::string
Ipv4ClickRouting::GetMacAddressFromInterfaceId (int ifid)
{
  std::stringstream addr;

  Ptr<NetDevice> device = m_ipv4->GetNetDevice (ifid);
  Address devAddr = device->GetAddress ();
  addr << Mac48Address::ConvertFrom (devAddr);

  return addr.str ();
}

void
Ipv4ClickRouting::AddSimNodeToClickMapping ()
{
  m_clickInstanceFromSimNode.insert (std::make_pair (m_simNode, this));
}

Ptr<Ipv4ClickRouting>
Ipv4ClickRouting::GetClickInstanceFromSimNode (simclick_node_t *simnode)
{
  return m_clickInstanceFromSimNode[simnode];
}

void
Ipv4ClickRouting::RunClickEvent ()
{
  m_simNode->curtime.tv_sec = Simulator::Now ().GetSeconds ();
  m_simNode->curtime.tv_usec = Simulator::Now ().GetMicroSeconds () % 1000000;
  NS_LOG_DEBUG ("RunClickEvent at " << m_simNode->curtime.tv_sec << " " << 
                                       m_simNode->curtime.tv_usec << " " << Simulator::Now ());
  simclick_click_run (m_simNode);
}

void
Ipv4ClickRouting::HandleScheduleFromClick (const struct timeval *when)
{
  NS_LOG_DEBUG ("HandleScheduleFromClick at " << when->tv_sec << " " << when->tv_usec << " " << Simulator::Now ());

  Time simtime  = Time::FromInteger(when->tv_sec, Time::S) + Time::FromInteger(when->tv_usec, Time::US);
  Time simdelay = simtime - Simulator::Now();

  Simulator::Schedule (simdelay, &Ipv4ClickRouting::RunClickEvent, this);
}

void
Ipv4ClickRouting::HandlePacketFromClick (int ifid, int ptype, const unsigned char* data, int len)
{
  NS_LOG_DEBUG ("HandlePacketFromClick");

  // Figure out packet's destination here:
  // If ifid == 0, then the packet's going up
  // else, the packet's going down
  if (ifid == 0)
    {
      NS_LOG_DEBUG ("Incoming packet from tap0. Sending Packet up the stack.");
      Ptr<Ipv4L3ClickProtocol> ipv4l3 = DynamicCast<Ipv4L3ClickProtocol> (m_ipv4);

      Ptr<Packet> p = Create<Packet> (data, len);

      Ipv4Header ipHeader;
      p->RemoveHeader (ipHeader);

      ipv4l3->LocalDeliver (p, ipHeader, (uint32_t) ifid);
    }
  else if (ifid)
    {
      NS_LOG_DEBUG ("Incoming packet from eth" << ifid - 1 << " of type " << ptype << ". Sending packet down the stack.");

      Ptr<Packet> p = Create<Packet> (data, len);

      DynamicCast<Ipv4L3ClickProtocol> (m_ipv4)->SendDown (p, ifid);
    }
}

void
Ipv4ClickRouting::SendPacketToClick (int ifid, int ptype, const unsigned char* data, int len)
{
  NS_LOG_FUNCTION (this << ifid);
  m_simNode->curtime.tv_sec = Simulator::Now ().GetSeconds ();
  m_simNode->curtime.tv_usec = Simulator::Now ().GetMicroSeconds () % 1000000;

  // Since packets in ns-3 don't have global Packet ID's and Flow ID's, we
  // feed dummy values into pinfo. This avoids the need to make changes in the Click code
  simclick_simpacketinfo pinfo;
  pinfo.id = 0;
  pinfo.fid = 0;

  simclick_click_send (m_simNode,ifid,ptype,data,len,&pinfo);
}

void
Ipv4ClickRouting::Send (Ptr<Packet> p, Ipv4Address src, Ipv4Address dst)
{
  uint32_t ifid;

  // Find out which interface holds the src address of the packet...
  for (ifid = 0; ifid < m_ipv4->GetNInterfaces (); ifid++)
    {
      Ipv4Address addr = m_ipv4->GetAddress (ifid, 0).GetLocal ();

      if (addr == src)
        {
          break;
        }
    }

  int len = p->GetSize ();
  uint8_t *buf = new uint8_t [len];
  p->CopyData (buf, len);

  // ... and send the packet on the corresponding Click interface.
  SendPacketToClick (0, SIMCLICK_PTYPE_IP, buf, len);

  delete [] buf;
}

void
Ipv4ClickRouting::Receive (Ptr<Packet> p, Mac48Address receiverAddr, Mac48Address dest)
{
  NS_LOG_FUNCTION (this << p << receiverAddr << dest);

  uint32_t ifid;

  // Find out which device this packet was received from...
  for (ifid = 0; ifid < m_ipv4->GetNInterfaces (); ifid++)
    {
      Ptr<NetDevice> device = m_ipv4->GetNetDevice (ifid);

      if (Mac48Address::ConvertFrom (device->GetAddress ()) == receiverAddr)
        {
          break;
        }
    }

  int len = p->GetSize ();
  uint8_t *buf = new uint8_t [len];
  p->CopyData (buf, len);

  // ... and send the packet to the corresponding Click interface
  SendPacketToClick (ifid, SIMCLICK_PTYPE_ETHER, buf, len);

  delete [] buf;
}

std::string
Ipv4ClickRouting::ReadHandler (std::string elementName, std::string handlerName)
{
  std::string s = simclick_click_read_handler (m_simNode, elementName.c_str (), handlerName.c_str (), 0, 0);
  return s;
}

int
Ipv4ClickRouting::WriteHandler (std::string elementName, std::string handlerName, std::string writeString)
{
  int r = simclick_click_write_handler (m_simNode, elementName.c_str (), handlerName.c_str (), writeString.c_str ());

  // Note: There are probably use-cases for returning
  // a write handler's error code, so don't assert.
  // For example, the 'add' handler for IPRouteTable
  // type elements fails if the route to be added
  // already exists.

  return r;
}

void
Ipv4ClickRouting::SetPromisc (int ifid)
{
  Ptr<Ipv4L3ClickProtocol> ipv4l3 = DynamicCast<Ipv4L3ClickProtocol> (m_ipv4);
  NS_ASSERT(ipv4l3);
  ipv4l3->SetPromisc (ifid);
}

Ptr<Ipv4Route>
Ipv4ClickRouting::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
{
  Ptr<Ipv4Route> rtentry;

  std::stringstream addr;
  addr << "lookup ";
  header.GetDestination ().Print (addr);
  // Probe the Click Routing Table for the required IP
  // This returns a string of the form "InterfaceID GatewayAddr"
  std::string s = ReadHandler (m_clickRoutingTableElement, addr.str ());

  int pos = s.find (" ");

  int interfaceId = atoi (s.substr (0, pos).c_str ());
  Ipv4Address destination (s.substr (pos + 1).c_str ());

  if (interfaceId != -1)
    {
      rtentry = Create<Ipv4Route> ();
      rtentry->SetDestination (header.GetDestination ());
      // the source address is the interface address that matches
      // the destination address (when multiple are present on the
      // outgoing interface, one is selected via scoping rules)
      NS_ASSERT (m_ipv4);
      uint32_t numOifAddresses = m_ipv4->GetNAddresses (interfaceId);
      NS_ASSERT (numOifAddresses > 0);
      Ipv4InterfaceAddress ifAddr;
      if (numOifAddresses == 1)
        {
          ifAddr = m_ipv4->GetAddress (interfaceId, 0);
        }
      else
        {
          NS_FATAL_ERROR ("XXX Not implemented yet:  IP aliasing and Click");
        }
      rtentry->SetSource (ifAddr.GetLocal ());
      rtentry->SetGateway (destination);
      rtentry->SetOutputDevice (m_ipv4->GetNetDevice (interfaceId));
      sockerr = Socket::ERROR_NOTERROR;
      NS_LOG_DEBUG ("Found route to " << rtentry->GetDestination ()
                                      << " via nh " << rtentry->GetGateway ()
                                      << " with source addr " << rtentry->GetSource ()
                                      << " and output dev " << rtentry->GetOutputDevice ());
    }
  else
    {
      NS_LOG_DEBUG ("Click node " << m_nodeName
                                  << ": RouteOutput for dest=" << header.GetDestination ()
                                  << " No route to host");
      sockerr = Socket::ERROR_NOROUTETOHOST;
    }

  return rtentry;
}

// This method should never be called since Click handles
// forwarding directly
bool
Ipv4ClickRouting::RouteInput  (Ptr<const Packet> p, const Ipv4Header &header,
                               Ptr<const NetDevice> idev, UnicastForwardCallback ucb,
                               MulticastForwardCallback mcb, LocalDeliverCallback lcb,
                               ErrorCallback ecb)
{
  NS_FATAL_ERROR ("Click router does not have a RouteInput() interface!");
  return false;
}

void
Ipv4ClickRouting::PrintRoutingTable (Ptr<OutputStreamWrapper> stream) const
{
}

void
Ipv4ClickRouting::NotifyInterfaceUp (uint32_t i)
{
}

void
Ipv4ClickRouting::NotifyInterfaceDown (uint32_t i)
{
}

void
Ipv4ClickRouting::NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
}

void
Ipv4ClickRouting::NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address)
{
}


} // namespace ns3

static int simstrlcpy (char *buf, int len, const std::string &s)
{
  if (len)
    {
      len--;

      if ((unsigned) len > s.length ())
        {
          len = s.length ();
        }

      s.copy (buf, len);
      buf[len] = '\0';
    }
  return 0;
}

// Sends a Packet from Click to the Simulator: Defined in simclick.h. Click
// calls these methods.
int simclick_sim_send (simclick_node_t *simnode,
                       int ifid, int type, const unsigned char* data, int len,
                       simclick_simpacketinfo *pinfo)
{
  NS_LOG_DEBUG ("simclick_sim_send called at " << ns3::Simulator::Now ().GetSeconds () << ": " << ifid << " " << type << " " << data << " " << len);

  if (simnode == NULL)
    {
      return -1;
    }

  ns3::Ptr<ns3::Ipv4ClickRouting> clickInstance = ns3::Ipv4ClickRouting::GetClickInstanceFromSimNode (simnode);

  clickInstance->HandlePacketFromClick (ifid, type, data, len);

  return 0;
}

// Click Service Methods: Defined in simclick.h
int simclick_sim_command (simclick_node_t *simnode, int cmd, ...)
{
  va_list val;
  va_start (val, cmd);

  int retval = 0;

  ns3::Ptr<ns3::Ipv4ClickRouting> clickInstance = ns3::Ipv4ClickRouting::GetClickInstanceFromSimNode (simnode);
  switch (cmd)
    {
    case SIMCLICK_VERSION:
      {
        retval = 0;
        break;
      }

    case SIMCLICK_SUPPORTS:
      {
        int othercmd = va_arg (val, int);
        retval = (othercmd >= SIMCLICK_VERSION && othercmd <= SIMCLICK_GET_NODE_ID);
        break;
      }

    case SIMCLICK_IFID_FROM_NAME:
      {
        const char *ifname = va_arg (val, const char *);

        retval = clickInstance->GetInterfaceId (ifname);

        NS_LOG_DEBUG (clickInstance->GetNodeName () << " SIMCLICK_IFID_FROM_NAME: " << ifname << " " << retval);
        break;
      }

    case SIMCLICK_IPADDR_FROM_NAME:
      {
        const char *ifname = va_arg (val, const char *);
        char *buf = va_arg (val, char *);
        int len = va_arg (val, int);

        int ifid = clickInstance->GetInterfaceId (ifname);

        if (ifid >= 0)
          {
            retval = simstrlcpy (buf, len, clickInstance->GetIpAddressFromInterfaceId (ifid));
          }
        else
          {
            retval = -1;
          }

        NS_LOG_DEBUG (clickInstance->GetNodeName () << " SIMCLICK_IPADDR_FROM_NAME: " << ifname << " " << buf << " " << len);
        break;
      }

    case SIMCLICK_IPPREFIX_FROM_NAME:
      {
        const char *ifname = va_arg (val, const char *);
        char *buf = va_arg (val, char *);
        int len = va_arg (val, int);

        int ifid = clickInstance->GetInterfaceId (ifname);

        if (ifid >= 0)
          {
            retval = simstrlcpy (buf, len, clickInstance->GetIpPrefixFromInterfaceId (ifid));
          }
        else
          {
            retval = -1;
          }

        NS_LOG_DEBUG (clickInstance->GetNodeName () << " SIMCLICK_IPPREFIX_FROM_NAME: " << ifname << " " << buf << " " << len);
        break;
      }

    case SIMCLICK_MACADDR_FROM_NAME:
      {
        const char *ifname = va_arg (val, const char *);
        char *buf = va_arg (val, char *);
        int len = va_arg (val, int);
        int ifid = clickInstance->GetInterfaceId (ifname);

        if (ifid >= 0)
          {
            retval = simstrlcpy (buf, len, clickInstance->GetMacAddressFromInterfaceId (ifid));
          }
        else
          {
            retval = -1;
          }

        NS_LOG_DEBUG (clickInstance->GetNodeName () << " SIMCLICK_MACADDR_FROM_NAME: " << ifname << " " << buf << " " << len);
        break;
      }

    case SIMCLICK_SCHEDULE:
      {
        const struct timeval *when = va_arg (val, const struct timeval *);

        clickInstance->HandleScheduleFromClick (when);

        retval = 0;
        NS_LOG_DEBUG (clickInstance->GetNodeName () << " SIMCLICK_SCHEDULE at " << when->tv_sec << "s and " << when->tv_usec << "usecs.");

        break;
      }

    case SIMCLICK_GET_NODE_NAME:
      {
        char *buf = va_arg (val, char *);
        int len = va_arg (val, int);
        retval = simstrlcpy (buf, len, clickInstance->GetNodeName ());

        NS_LOG_DEBUG (clickInstance->GetNodeName () << " SIMCLICK_GET_NODE_NAME: " << buf << " " << len);
        break;
      }

    case SIMCLICK_IF_PROMISC:
      {
        int ifid = va_arg(val, int);
        clickInstance->SetPromisc (ifid);

        retval = 0;
        NS_LOG_DEBUG (clickInstance->GetNodeName () << " SIMCLICK_IF_PROMISC: " << ifid << " " << ns3::Simulator::Now ());
        break;
      }

    case SIMCLICK_IF_READY:
      {
        int ifid = va_arg (val, int); // Commented out so that optimized build works

        // We're not using a ClickQueue, so we're always ready (for the timebeing)
        retval = clickInstance->IsInterfaceReady (ifid);

        NS_LOG_DEBUG (clickInstance->GetNodeName () << " SIMCLICK_IF_READY: " << ifid << " " << ns3::Simulator::Now ());
        break;
      }

    case SIMCLICK_TRACE:
      {
        // Used only for tracing
        NS_LOG_DEBUG (clickInstance->GetNodeName () << " Received a call for SIMCLICK_TRACE");
        break;
      }

    case SIMCLICK_GET_NODE_ID:
      {
        // Used only for tracing
        NS_LOG_DEBUG (clickInstance->GetNodeName () << " Received a call for SIMCLICK_GET_NODE_ID");
        break;
      }
    }
  return retval;
}

#endif // NS3_CLICK
