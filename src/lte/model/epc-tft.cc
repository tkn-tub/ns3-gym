/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */




#include "epc-tft.h"
#include "ns3/abort.h"
#include "ns3/log.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcTft")
  ;

std::ostream& operator<< (std::ostream& os, EpcTft::Direction& d)
{
 switch (d)
    {
    case EpcTft::DOWNLINK:
      os << "DOWNLINK";
      break;
    case EpcTft::UPLINK:
      os << "UPLINK";
      break;
    default:
      os << "BIDIRECTIONAL";
      break;
    }
  return os;
}


std::ostream& operator<< (std::ostream& os, EpcTft::PacketFilter& f)
{
  os << " direction: " << f.direction
     << " remoteAddress: "  << f.remoteAddress 
     << " remoteMask: "  << f.remoteMask 
     << " localAddress: "  << f.localAddress 
     << " localMask: "  << f.localMask 
     << " remotePortStart: "   << f.remotePortStart
     << " remotePortEnd: "   << f.remotePortEnd 
     << " localPortStart: "   << f.localPortStart 
     << " localPortEnd: "   << f.localPortEnd 
     << " typeOfService: 0x" << std::hex << (uint16_t) f.typeOfService << std::dec
     << " typeOfServiceMask: 0x" << std::hex << (uint16_t) f.typeOfServiceMask << std::dec;  
  return os;
}

EpcTft::PacketFilter::PacketFilter ()
  : precedence (255),
    direction (BIDIRECTIONAL),
    remoteMask ("0.0.0.0"),
    localMask ("0.0.0.0"),
    remotePortStart (0),
    remotePortEnd (65535), 
    localPortStart (0),
    localPortEnd (65535),
    typeOfService (0),  
    typeOfServiceMask (0)
{
  NS_LOG_FUNCTION (this);
}

bool 
EpcTft::PacketFilter::Matches (Direction d,
			       Ipv4Address ra, 
			       Ipv4Address la, 
			       uint16_t rp,
			       uint16_t lp,
			       uint8_t tos)
{
  NS_LOG_FUNCTION (this << d << ra << la << rp << lp << (uint16_t) tos);
  if (d & direction)
    {
      NS_LOG_LOGIC ("d matches");
      if (remoteMask.IsMatch (remoteAddress, ra))
	{
	  NS_LOG_LOGIC ("ra matches");
	  if (localMask.IsMatch (localAddress, la))
	    {
	      NS_LOG_LOGIC ("ls matches");
	      if (rp >= remotePortStart)
		{
		  NS_LOG_LOGIC ("rps matches");
		  if (rp <= remotePortEnd)
		    {
		      NS_LOG_LOGIC ("rpe matches");
		      if (lp >= localPortStart)
			{
			  NS_LOG_LOGIC ("lps matches");
			  if (lp <= localPortEnd)
			    {
			      NS_LOG_LOGIC ("lpe matches");
			      if ((tos & typeOfServiceMask) == (typeOfService & typeOfServiceMask))
				{
				  NS_LOG_LOGIC ("tos matches --> have match!");
				  return true;
				}
			    }
			}
		    }
		}
	    }
	  else
	    {
	      NS_LOG_LOGIC ("la doesn't match: la=" << la << " f.la=" << localAddress << " f.lmask=" << localMask);
	    }
	}
      else
	{
	  NS_LOG_LOGIC ("ra doesn't match: ra=" << ra << " f.ra=" << remoteAddress << " f.rmask=" << remoteMask);
	}
    }
  else
    {
      NS_LOG_LOGIC ("d doesn't match: d=0x" << std::hex << d << " f.d=0x" << std::hex << direction << std::dec);
    }
  return false;      
}


Ptr<EpcTft> 
EpcTft::Default ()
{
  Ptr<EpcTft> tft = Create<EpcTft> ();
  EpcTft::PacketFilter defaultPacketFilter;
  tft->Add (defaultPacketFilter);
  return tft;
}


EpcTft::EpcTft ()
  : m_numFilters (0)
{
  NS_LOG_FUNCTION (this);
}

uint8_t 
EpcTft::Add (PacketFilter f)
{
  NS_LOG_FUNCTION (this << f);
  NS_ABORT_IF (m_numFilters >= 16);
  
  std::list<PacketFilter>::iterator it;
  for (it = m_filters.begin ();
       (it != m_filters.end ()) && (it->precedence <= f.precedence);
       ++it)
    {
    }  
  m_filters.insert (it, f);  
  ++m_numFilters;
  return (m_numFilters - 1);
}
    
bool 
EpcTft::Matches (Direction direction,
		 Ipv4Address remoteAddress, 
		 Ipv4Address localAddress, 
		 uint16_t remotePort,
		 uint16_t localPort,
		 uint8_t typeOfService)
{
  NS_LOG_FUNCTION (this << direction << remoteAddress << localAddress << std::dec << remotePort << localPort << (uint16_t) typeOfService);
  for (std::list<PacketFilter>::iterator it = m_filters.begin ();
       it != m_filters.end ();
       ++it)
    {
      if (it->Matches (direction, remoteAddress, localAddress, remotePort, localPort, typeOfService))
	{
	  return true;
	}
    }  
  return false;
}


} // namespace ns3
