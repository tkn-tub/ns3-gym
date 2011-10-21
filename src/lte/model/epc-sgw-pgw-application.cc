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

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EpcSgwPgwApplication");


/////////////////////////
// EnbInfo
/////////////////////////


EpcSgwPgwApplication::EnbInfo::EnbInfo ()
  :  m_teidCounter (0)
{
  NS_LOG_FUNCTION (this);
}

uint32_t
EpcSgwPgwApplication::EnbInfo::AddBearer (Ptr<LteTft> tft)
{
  NS_LOG_FUNCTION (this << tft);
  return m_tftClassifier.Add (tft);
}

uint32_t
EpcSgwPgwApplication::EnbInfo::Classify (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
  // we hardcode DOWNLINK direction since the PGW is espected to
  // classify only downlink packets (uplink packets will go to the
  // internet without any classification). 
  return m_tftClassifier.Classify (p, LteTft::DOWNLINK);
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

  

EpcSgwPgwApplication::EpcSgwPgwApplication (const Ptr<VirtualNetDevice> tunDevice, const Ptr<Socket> s1uSocket)
  : m_s1uSocket (s1uSocket),
    m_tunDevice (tunDevice),
    m_gtpuUdpPort (2152) // fixed by the standard
{
  NS_LOG_FUNCTION (this << tunDevice << s1uSocket);
  m_s1uSocket->SetRecvCallback (MakeCallback (&EpcSgwPgwApplication::RecvFromS1uSocket, this));
}

  
EpcSgwPgwApplication::~EpcSgwPgwApplication ()
{
  NS_LOG_FUNCTION_NOARGS ();
}


uint32_t 
EpcSgwPgwApplication::ActivateS1Bearer (Ipv4Address ueAddr, Ipv4Address enbAddr, Ptr<LteTft> tft)
{
  NS_LOG_FUNCTION (this << ueAddr << enbAddr << tft);
  // side effect: add entry if not exists
  m_ueAddrEnbAddrMap[ueAddr] = enbAddr;

  // side effect: create new EnbInfo if it does not exist
  uint32_t teid = m_enbInfoMap[enbAddr].AddBearer (tft);
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

  // find corresponding eNB address
  std::map<Ipv4Address, Ipv4Address>::iterator it1 = m_ueAddrEnbAddrMap.find (ueAddr);
  if (it1 == m_ueAddrEnbAddrMap.end ())
    {        
      NS_LOG_WARN ("could not find corresponding eNB for UE address " << ueAddr) ;
    }
  else
    {
      Ipv4Address enbAddr = it1->second;
      // lookup into TFT classifier for that eNB
      std::map<Ipv4Address, EnbInfo>::iterator it2 = m_enbInfoMap.find (enbAddr);
      NS_ASSERT (it2 != m_enbInfoMap.end ());
      uint32_t teid = it2->second.Classify (packet);   
      if (teid == 0)
        {
          NS_LOG_WARN ("no matching TEID for this packet");                   
        }
      else
        {
          SendToS1uSocket (packet, enbAddr, teid);
        }
    }
  // there is no reason why we should notify the Gi TUN
  // VirtualNetDevice that he failed to send the packet 
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
