/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
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
 * Authors: 
 *   Nicola Baldo <nbaldo@cttc.es> (the LteTftClassifier class)
 *   Giuseppe Piro <g.piro@poliba.it> (part of the code in LteTftClassifier::Classify () 
 *       which comes from RrcEntity::Classify of the GSoC 2010 LTE module)
 *
 */




#include "lte-tft-classifier.h"
#include "lte-tft.h"
#include "ns3/abort.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"

NS_LOG_COMPONENT_DEFINE ("LteTftClassifier");

namespace ns3 {

LteTftClassifier::LteTftClassifier ()
  : m_tftCount (0)
{
  NS_LOG_FUNCTION (this);
}

uint32_t 
LteTftClassifier::Add (Ptr<LteTft> tft)
{
  NS_LOG_FUNCTION (this << tft);
  // simple sanity check. If you ever need more than 4M TFTs within a same classifiers, you'll need to implement a smarter id management algorithm.
  NS_ABORT_IF (m_tftCount == 0xFFFFFFFF);
  ++m_tftCount;  
  m_tftMap[m_tftCount] = tft;
  return m_tftCount;
}

void
LteTftClassifier::Delete (uint32_t id)
{
  NS_LOG_FUNCTION (this << id);
  m_tftMap.erase (id);
}

 
uint32_t 
LteTftClassifier::Classify (Ptr<Packet> p, LteTft::Direction direction)
{
  NS_LOG_FUNCTION (this << *p << direction);

  Ptr<Packet> pCopy = p->Copy ();

  Ipv4Header ipv4Header;
  pCopy->RemoveHeader (ipv4Header);

  Ipv4Address localAddress;
  Ipv4Address remoteAddress;

  
  if (direction ==  LteTft::UPLINK)
    {
      localAddress = ipv4Header.GetSource ();
      remoteAddress = ipv4Header.GetDestination ();
    }
  else
    { 
      NS_ASSERT (direction ==  LteTft::DOWNLINK);
      remoteAddress = ipv4Header.GetSource ();
      localAddress = ipv4Header.GetDestination ();      
    }
  
  uint8_t protocol = ipv4Header.GetProtocol ();

  uint8_t tos = ipv4Header.GetTos ();

  uint16_t localPort = 0;
  uint16_t remotePort = 0;

  if (protocol == UdpL4Protocol::PROT_NUMBER)
    {
      UdpHeader udpHeader;
      pCopy->RemoveHeader (udpHeader);

      if (direction ==  LteTft::UPLINK)
	{
	  localPort = udpHeader.GetSourcePort ();
	  remotePort = udpHeader.GetDestinationPort ();
	}
      else
	{
	  remotePort = udpHeader.GetSourcePort ();
	  localPort = udpHeader.GetDestinationPort ();
	}
    }
  else if (protocol == TcpL4Protocol::PROT_NUMBER)
    {
      TcpHeader tcpHeader;
      pCopy->RemoveHeader (tcpHeader);
      if (direction ==  LteTft::UPLINK)
	{
	  localPort = tcpHeader.GetSourcePort ();
	  remotePort = tcpHeader.GetDestinationPort ();
	}
      else
	{
	  remotePort = tcpHeader.GetSourcePort ();
	  localPort = tcpHeader.GetDestinationPort ();
	}
    }
  else
    {
      NS_LOG_INFO ("Unknown protocol: " << protocol);
      return 0;  // no match
    }

  NS_LOG_INFO ("Classifing packet:"
	       << " localAddr="  << localAddress 
	       << " remoteAddr=" << remoteAddress 
	       << " localPort="  << localPort 
	       << " remotePort=" << remotePort 
	       << " tos=0x" << std::hex << tos);

  // now it is possible to classify the packet!
  std::map <uint32_t, Ptr<LteTft> >::const_iterator it;
  NS_LOG_LOGIC ("TFT MAP size: " << m_tftMap.size ());
  for (it = m_tftMap.begin (); it != m_tftMap.end (); ++it)
    {
      NS_LOG_LOGIC ("TFT id: " << it->first );
      NS_LOG_LOGIC (" Ptr<LteTft>: " << it->second);
      Ptr<LteTft> tft = it->second;         
      if (tft->Matches (direction, remoteAddress, localAddress, remotePort, localPort, tos))
        {
	  NS_LOG_LOGIC ("matches with TFT ID = " << it->first);
	  return it->first; // the id of the matching TFT
        }
    }
  NS_LOG_LOGIC ("no match");
  return 0;  // no match
}


} // namespace ns3
