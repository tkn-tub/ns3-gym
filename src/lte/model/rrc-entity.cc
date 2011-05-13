/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */


#include "rrc-entity.h"
#include <ns3/log.h>
#include <ns3/pointer.h>
#include <ns3/packet.h>
#include "ns3/ipcs-classifier-record.h"
#include "radio-bearer-instance.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/llc-snap-header.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"
#include "lte-mac-header.h"

NS_LOG_COMPONENT_DEFINE ("RrcEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (RrcEntity);

TypeId RrcEntity::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RrcEntity")
    .SetParent<Object> ()
  ;
  return tid;
}


RrcEntity::RrcEntity ()
{
  NS_LOG_FUNCTION (this); 
  m_downlinkGbrBearersContainer = new BearersContainer ();
  m_downlinkNgbrBearersContainer = new BearersContainer ();
  m_uplinkGbrBearersContainer = new BearersContainer ();
  m_uplinkNgbrBearersContainer = new BearersContainer ();

  m_defaultBearer = CreateObject<RadioBearerInstance> ();
}


RrcEntity::~RrcEntity ()
{
  NS_LOG_FUNCTION (this);
}

void
RrcEntity::DoDispose ()
{
  NS_LOG_FUNCTION (this);

  // RadioBearerInstance has a ref to RlcEntity 
  // which has a ref to RrcEntity 
  // which has a ref to RadioBearerInstance
  // so we have to break this ref cycle by manually Disposing all RadioBearerInstances

  DisposeAllElements (m_downlinkGbrBearersContainer);
  DisposeAllElements (m_downlinkNgbrBearersContainer);
  DisposeAllElements (m_uplinkGbrBearersContainer);
  DisposeAllElements (m_uplinkNgbrBearersContainer);

  delete m_downlinkGbrBearersContainer;
  delete m_downlinkNgbrBearersContainer;
  delete m_uplinkGbrBearersContainer;
  delete m_uplinkNgbrBearersContainer;

  m_defaultBearer->Dispose ();
  m_defaultBearer = 0;
  Object::DoDispose ();
}

void 
RrcEntity::DisposeAllElements (BearersContainer *c)
{
  for (BearersContainer::iterator it = c->begin (); it != c->end (); ++it)
    {
      (*it)->Dispose ();
    }
}


RrcEntity::BearersContainer*
RrcEntity::GetDownlinkGbrBearers (void) const
{
  NS_LOG_FUNCTION (this);
  return m_downlinkGbrBearersContainer;
}


RrcEntity::BearersContainer*
RrcEntity::GetDownlinkNgbrBearers (void) const
{
  NS_LOG_FUNCTION (this);
  return m_downlinkNgbrBearersContainer;
}


RrcEntity::BearersContainer*
RrcEntity::GetUplinkGbrBearers (void) const
{
  NS_LOG_FUNCTION (this);
  return m_uplinkGbrBearersContainer;
}


RrcEntity::BearersContainer*
RrcEntity::GetUplinkNgbrBearers (void) const
{
  NS_LOG_FUNCTION (this);
  return m_uplinkNgbrBearersContainer;
}


void
RrcEntity::AddDownlinkGbrBearer (Ptr<RadioBearerInstance> bearer)
{
  NS_LOG_FUNCTION (this);
  m_downlinkGbrBearersContainer->push_back (bearer);
}


void
RrcEntity::AddDownlinkNgbrBearer (Ptr<RadioBearerInstance> bearer)
{
  NS_LOG_FUNCTION (this);
  m_downlinkNgbrBearersContainer->push_back (bearer);
}


void
RrcEntity::AddUplinkGbrBearer (Ptr<RadioBearerInstance> bearer)
{
  NS_LOG_FUNCTION (this);
  m_uplinkGbrBearersContainer->push_back (bearer);
}


void
RrcEntity::AddUplinkNgbrBearer (Ptr<RadioBearerInstance> bearer)
{
  NS_LOG_FUNCTION (this);
  m_uplinkNgbrBearersContainer->push_back (bearer);
}


Ptr<RadioBearerInstance>
RrcEntity::Classify (Ptr<Packet> p) const
{

  NS_LOG_FUNCTION (this);

  Ptr<Packet> C_Packet = p->Copy ();

  LteMacHeader header;
  C_Packet->RemoveHeader (header);
  NS_LOG_LOGIC ("packet " << header.GetSource () << " --> " << header.GetDestination ());

  LlcSnapHeader llc;
  C_Packet->RemoveHeader (llc);


  Ipv4Header ipv4Header;
  C_Packet->RemoveHeader (ipv4Header);
  Ipv4Address source_address = ipv4Header.GetSource ();
  Ipv4Address dest_address = ipv4Header.GetDestination ();
  uint8_t protocol = ipv4Header.GetProtocol ();

  uint16_t sourcePort = 0;
  uint16_t destPort = 0;
  if (protocol == UdpL4Protocol::PROT_NUMBER)
    {
      UdpHeader udpHeader;
      C_Packet->RemoveHeader (udpHeader);
      sourcePort = udpHeader.GetSourcePort ();
      destPort = udpHeader.GetDestinationPort ();
    }
  else if (protocol == TcpL4Protocol::PROT_NUMBER)
    {
      TcpHeader tcpHeader;
      C_Packet->RemoveHeader (tcpHeader);
      sourcePort = tcpHeader.GetSourcePort ();
      destPort = tcpHeader.GetDestinationPort ();
    }
  else
    {
      NS_LOG_INFO ("\t\t\tUnknown protocol: " << protocol);
      return 0;
      return 0;
    }

  NS_LOG_INFO ("Classifing packet: src_addr=" << source_address << " dst_addr="
                                              << dest_address << " src_port=" << sourcePort << " dst_port="
                                              << destPort << " proto=" << (uint16_t) protocol);

  //now it is possible to classify the packet!
  std::vector< Ptr<RadioBearerInstance> >::iterator it;
  for (it = m_downlinkGbrBearersContainer->begin (); it != m_downlinkGbrBearersContainer->end (); it++)
    {
      NS_LOG_INFO ("Check for downlinkGbrBearersContainer");
      Ptr<RadioBearerInstance> bearer = (*it);
      if (bearer->GetIpcsClassifierRecord ()->CheckMatch (source_address, dest_address, sourcePort, destPort, protocol))
        {
          return bearer;
        }
    }

  for (it = m_downlinkNgbrBearersContainer->begin (); it != m_downlinkNgbrBearersContainer->end (); it++)
    {
      NS_LOG_INFO ("Check for downlinkNGbrBearersContainer");
      Ptr<RadioBearerInstance> bearer = (*it);
      if (bearer->GetIpcsClassifierRecord ()->CheckMatch (source_address, dest_address, sourcePort, destPort, protocol))
        {
          return bearer;
        }
    }

  for (it = m_uplinkGbrBearersContainer->begin (); it != m_uplinkGbrBearersContainer->end (); it++)
    {
      NS_LOG_INFO ("Check for ullinkGbrBearersContainer");
      Ptr<RadioBearerInstance> bearer = (*it);
      if (bearer->GetIpcsClassifierRecord ()->CheckMatch (source_address, dest_address, sourcePort, destPort, protocol))
        {
          return bearer;
        }
    }

  for (it = m_uplinkNgbrBearersContainer->begin (); it != m_uplinkNgbrBearersContainer->end (); it++)
    {
      NS_LOG_INFO ("Check for ullinkNgbrBearersContainer");
      Ptr<RadioBearerInstance> bearer = (*it);
      if (bearer->GetIpcsClassifierRecord ()->CheckMatch (source_address, dest_address, sourcePort, destPort, protocol))
        {
          return bearer;
        }
    }

  NS_LOG_INFO ("\t\t\tError during the packet classification"); 
  return 0;
}


Ptr<RadioBearerInstance>
RrcEntity::GetDefaultBearer (void)
{
  NS_LOG_FUNCTION (this);
  return m_defaultBearer;
}


} // namespace ns3
