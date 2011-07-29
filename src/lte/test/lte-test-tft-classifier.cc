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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */



#include "ns3/test.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/udp-l4-protocol.h"
#include "ns3/tcp-l4-protocol.h"

#include "ns3/lte-tft-classifier.h"

#include <iomanip>

NS_LOG_COMPONENT_DEFINE ("LteTestTftClassifier");

namespace ns3 {

class LteTftClassifierTestCase : public TestCase
{
public:
  LteTftClassifierTestCase (Ptr<LteTftClassifier> c,
                            LteTft::Direction d,
                            Ipv4Address sa, 
                            Ipv4Address da, 
                            uint16_t sp,
                            uint16_t dp,
                            uint8_t tos,
                            uint32_t tftId);
  virtual ~LteTftClassifierTestCase ();

private:
  
  Ptr<LteTftClassifier> m_c;
  LteTft::Direction m_d;
  uint8_t m_tftId;
  Ipv4Header m_ipHeader;
  UdpHeader m_udpHeader;
  TcpHeader m_tcpHeader;

  virtual void DoRun (void);
};

LteTftClassifierTestCase::LteTftClassifierTestCase (Ptr<LteTftClassifier> c,
                                                    LteTft::Direction d,
                                                    Ipv4Address sa, 
                                                    Ipv4Address da, 
                                                    uint16_t sp,
                                                    uint16_t dp,
                                                    uint8_t tos,
                                                    uint32_t tftId)
  : TestCase (""),
    m_c (c),
    m_d (d),    
    m_tftId (tftId)
{
  std::ostringstream oss;
  oss << c;
  switch (d)
    {
    case LteTft::DOWNLINK:
      oss << " DL, ";
      break;
    case LteTft::UPLINK:
      oss << " UL, ";
      break;
    default:
      oss << " BD, ";
      break;
    }
  oss << ", sa = " << sa
      << ", da = " << da
      << ", sp = " << sp
      << ", dp = " << dp
      << ", tos = 0x" << std::hex << tos
      << " --> tftId = " << tftId;  
  SetName (oss.str ());

  NS_LOG_FUNCTION (this << oss.str ());

  
  m_ipHeader.SetSource (sa);
  m_ipHeader.SetDestination (da);
  m_ipHeader.SetTos (tos);  


  m_udpHeader.SetSourcePort (sp);
  m_udpHeader.SetDestinationPort (dp);

  
}

LteTftClassifierTestCase::~LteTftClassifierTestCase ()
{
}

void 
LteTftClassifierTestCase::DoRun (void)
{
  ns3::PacketMetadata::Enable ();

  Ptr<Packet> udpPacket = Create<Packet> ();
  m_ipHeader.SetProtocol (UdpL4Protocol::PROT_NUMBER);
  udpPacket->AddHeader (m_udpHeader);
  udpPacket->AddHeader (m_ipHeader);
  NS_LOG_LOGIC (this << *udpPacket);
  uint32_t obtainedTftId = m_c ->Classify (udpPacket, m_d);
  NS_TEST_ASSERT_MSG_EQ (obtainedTftId, m_tftId, "bad classification of UDP packet");
}





/**
 * Test the calculation of carrier frequency based on EARFCN
 */
class LteTftClassifierTestSuite : public TestSuite
{
public:
  LteTftClassifierTestSuite ();
};

static LteTftClassifierTestSuite g_lteTftClassifierTestSuite;

LteTftClassifierTestSuite::LteTftClassifierTestSuite ()
  : TestSuite ("lte-tft-classifier", UNIT)
{
  NS_LOG_FUNCTION (this);
  
  Ptr<LteTftClassifier> c1 = Create<LteTftClassifier> ();
  
  Ptr<LteTft> tft1_1 = Create<LteTft> ();

  LteTft::PacketFilter pf1_1_1;
  pf1_1_1.remoteAddress.Set ("1.0.0.0");
  pf1_1_1.localAddress.Set ("2.0.0.0");
  pf1_1_1.remoteMask.Set (0xFF000000);
  pf1_1_1.localMask.Set (0xFF000000);
  tft1_1->Add (pf1_1_1);

  c1->Add (tft1_1);

  AddTestCase (new LteTftClassifierTestCase (c1, LteTft::UPLINK, Ipv4Address ("2.2.3.4"), Ipv4Address ("1.12.0.132"), 4, 1234, 0, 1));
  AddTestCase (new LteTftClassifierTestCase (c1, LteTft::UPLINK, Ipv4Address ("6.2.3.4"), Ipv4Address ("1.12.0.132"), 4, 1234, 0, 0));
                
}


} // namespace ns3
