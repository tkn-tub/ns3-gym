/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.cat>
 *         Nicola Baldo <nbaldo@cttc.cat>
 */


#include "epc-sgw-pgw-application.h"
#include "ns3/log.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4.h"
#include "ns3/inet-socket-address.h"
#include "ns3/epc-gtpu-header.h"
#include "ns3/abort.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcSgwPgwApplication");


/////////////////////////
// UeInfo
/////////////////////////


EpcSgwPgwApplication::UeInfo::UeInfo ()
{
  NS_LOG_FUNCTION (this);
}

void
EpcSgwPgwApplication::UeInfo::AddBearer (Ptr<EpcTft> tft, uint32_t teid)
{
  NS_LOG_FUNCTION (this << tft << teid);
  return m_tftClassifier.Add (tft, teid);
}

uint32_t
EpcSgwPgwApplication::UeInfo::Classify (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  // we hardcode DOWNLINK direction since the PGW is espected to
  // classify only downlink packets (uplink packets will go to the
  // internet without any classification). 
  return m_tftClassifier.Classify (p, EpcTft::DOWNLINK);
}

Ipv4Address 
EpcSgwPgwApplication::UeInfo::GetEnbAddr ()
{
  return m_enbAddr;
}

void
EpcSgwPgwApplication::UeInfo::SetEnbAddr (Ipv4Address enbAddr)
{
  m_enbAddr = enbAddr;
}

/////////////////////////
// EpcSgwPgwApplication
/////////////////////////


TypeId
EpcSgwPgwApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcSgwPgwApplication")
    .SetParent<Object> ();
  return tid;
}

void
EpcSgwPgwApplication::DoDispose ()
{
  m_s1uSocket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  m_s1uSocket = 0;
}

  

EpcSgwPgwApplication::EpcSgwPgwApplication (const Ptr<VirtualNetDevice> tunDevice, const Ptr<Socket> s1uSocket)
  : m_s1uSocket (s1uSocket),
    m_tunDevice (tunDevice),
    m_gtpuUdpPort (2152), // fixed by the standard
    m_teidCount (0)
{
  NS_LOG_FUNCTION (this << tunDevice << s1uSocket);
  m_s1uSocket->SetRecvCallback (MakeCallback (&EpcSgwPgwApplication::RecvFromS1uSocket, this));
}

  
EpcSgwPgwApplication::~EpcSgwPgwApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
}


uint32_t 
EpcSgwPgwApplication::ActivateS1Bearer (Ipv4Address ueAddr, Ipv4Address enbAddr, Ptr<EpcTft> tft)
{
  NS_LOG_FUNCTION (this << ueAddr << enbAddr << tft);

  // simple sanity check. If you ever need more than 4M teids
  // throughout your simulation, you'll need to implement a smarter teid
  // management algorithm. 
  NS_ABORT_IF (m_teidCount == 0xFFFFFFFF);
  uint32_t teid = ++m_teidCount;  

  std::map<Ipv4Address, UeInfo>::iterator it = m_ueInfoMap.find (ueAddr);
  if (it == m_ueInfoMap.end ())
    {
      // UE unknown, creating new entry
      std::pair<std::map<Ipv4Address, UeInfo>::iterator, bool> ret;
      ret = m_ueInfoMap.insert (std::pair <Ipv4Address, UeInfo> (ueAddr, UeInfo ()));
      it = ret.first;
      it->second.SetEnbAddr (enbAddr);
    }
        
  it->second.AddBearer (tft, teid);
  return teid;
}

bool
EpcSgwPgwApplication::RecvFromTunDevice (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (this << source << dest << packet << packet->GetSize ());

  // get IP address of UE
  Ptr<Packet> pCopy = packet->Copy ();
  Ipv4Header ipv4Header;
  pCopy->RemoveHeader (ipv4Header);
  Ipv4Address ueAddr =  ipv4Header.GetDestination ();
  NS_LOG_LOGIC ("packet addressed to UE " << ueAddr);

  // find corresponding UeInfo address
  std::map<Ipv4Address, UeInfo>::iterator it = m_ueInfoMap.find (ueAddr);
  if (it == m_ueInfoMap.end ())
    {        
      NS_LOG_WARN ("unknown UE address " << ueAddr) ;
    }
  else
    {
      Ipv4Address enbAddr = it->second.GetEnbAddr ();      
      uint32_t teid = it->second.Classify (packet);   
      if (teid == 0)
        {
          NS_LOG_WARN ("no matching bearer for this packet");                   
        }
      else
        {
          SendToS1uSocket (packet, enbAddr, teid);
        }
    }
  // there is no reason why we should notify the TUN
  // VirtualNetDevice that he failed to send the packet: if we receive
  // any bogus packet, it will just be silently discarded.
  const bool succeeded = true;
  return succeeded;
}

void 
EpcSgwPgwApplication::RecvFromS1uSocket (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);  
  NS_ASSERT (socket == m_s1uSocket);
  Ptr<Packet> packet = socket->Recv ();
  GtpuHeader gtpu;
  packet->RemoveHeader (gtpu);
  uint32_t teid = gtpu.GetTeid ();

  // workaround for bug 231 https://www.nsnam.org/bugzilla/show_bug.cgi?id=231
  SocketAddressTag tag;
  packet->RemovePacketTag (tag);

  SendToTunDevice (packet, teid);
}

void 
EpcSgwPgwApplication::SendToTunDevice (Ptr<Packet> packet, uint32_t teid)
{
  NS_LOG_FUNCTION (this << packet << teid);
  NS_LOG_LOGIC (" packet size: " << packet->GetSize () << " bytes");
  m_tunDevice->Receive (packet, 0x0800, m_tunDevice->GetAddress (), m_tunDevice->GetAddress (), NetDevice::PACKET_HOST);
}

void 
EpcSgwPgwApplication::SendToS1uSocket (Ptr<Packet> packet, Ipv4Address enbAddr, uint32_t teid)
{
  NS_LOG_FUNCTION (this << packet << enbAddr << teid);

  GtpuHeader gtpu;
  gtpu.SetTeid (teid);
  // From 3GPP TS 29.281 v10.0.0 Section 5.1
  // Length of the payload + the non obligatory GTP-U header
  gtpu.SetLength (packet->GetSize () + gtpu.GetSerializedSize () - 8);  
  packet->AddHeader (gtpu);
  uint32_t flags = 0;
  m_s1uSocket->SendTo (packet, flags, InetSocketAddress(enbAddr, m_gtpuUdpPort));
}

}; // namespace ns3
