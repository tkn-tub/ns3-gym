/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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


#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/lte-module.h>


NS_LOG_COMPONENT_DEFINE ("LteRrcTest");

namespace ns3 {

class LteRrcConnectionEstablishmentTestCase : public TestCase
{
public:

  /** 
   * 
   * 
   * \param nUes number of UEs in the test
   * \param tc connection time base value for all UEs in ms
   * \param tConnIncrPerUe additional connection time increment for each UE index (0...nUes-1) in ms
   * \param delayDiscStart expected duration to perform connection establishment in ms
   * 
   */
  LteRrcConnectionEstablishmentTestCase (uint32_t nUes, uint32_t nBearers, uint32_t tc, uint32_t tConnIncrPerUe, uint32_t delayDiscStart, bool useIdealRrc);
  
private:
  static std::string BuildNameString (uint32_t nUes, uint32_t nBearers, uint32_t tc, uint32_t tConnIncrPerUe, uint32_t delayDiscStart, bool useIdealRrc);
  virtual void DoRun (void);
  void Connect (Ptr<NetDevice> ueDevice, Ptr<NetDevice> enbDevice);
  void CheckConnected (Ptr<NetDevice> ueDevice, Ptr<NetDevice> enbDevice);


  uint32_t m_nUes; // number of UEs in the test
  uint32_t m_nBearers; // number of UEs in the test
  uint32_t m_tConnBase;  // connection time base value for all UEs in ms
  uint32_t m_tConnIncrPerUe; // additional connection time increment for each UE index (0...nUes-1) in ms
  uint32_t m_delayConnEnd;  // expected duration to perform connection establishment in ms
  uint32_t m_delayDiscStart;  // delay between connection completed and disconnection request in ms
  uint32_t m_delayDiscEnd; // expected duration to complete disconnection in ms
  bool     m_useIdealRrc;
  Ptr<LteHelper> m_lteHelper;

};


std::string LteRrcConnectionEstablishmentTestCase::BuildNameString (uint32_t nUes, uint32_t nBearers, uint32_t tConnBase, uint32_t tConnIncrPerUe, uint32_t delayDiscStart, bool useIdealRrc)
{
  std::ostringstream oss;
  oss << "nUes=" << nUes 
      << "nBearers=" << nBearers 
      << ", tConnBase=" << tConnBase
      << ", tConnIncrPerUe=" << tConnIncrPerUe
      << ", delayDiscStart=" << delayDiscStart;
  if (useIdealRrc)
    {
      oss << ", ideal RRC";
    }
  else
    {
      oss << ", real RRC";
    }  
  return oss.str ();
}

LteRrcConnectionEstablishmentTestCase::LteRrcConnectionEstablishmentTestCase (uint32_t nUes, uint32_t nBearers, uint32_t tConnBase, uint32_t tConnIncrPerUe, uint32_t delayDiscStart, bool useIdealRrc)
  : TestCase (BuildNameString (nUes, nBearers, tConnBase, tConnIncrPerUe, delayDiscStart, useIdealRrc)),
    m_nUes (nUes),
    m_nBearers (nBearers),
    m_tConnBase (tConnBase),
    m_tConnIncrPerUe (tConnIncrPerUe),
    m_delayConnEnd (140+nUes*8/4), // includes: time to receive system information, time for Random Access (RACH preamble, RAR response), time to send and receive RRC connection request+setup+completed. Value should be slightly higher than T300 in TS 36.331
    m_delayDiscStart (delayDiscStart),
    m_delayDiscEnd (10),
    m_useIdealRrc (useIdealRrc)
{
}

void
LteRrcConnectionEstablishmentTestCase::DoRun ()
{
  // normal code
  
  m_lteHelper = CreateObject<LteHelper> ();
  m_lteHelper->SetAttribute ("UseIdealRrc", BooleanValue (m_useIdealRrc));

  NodeContainer enbNodes;
  enbNodes.Create (1);
  NodeContainer ueNodes;
  ueNodes.Create (m_nUes);

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueNodes);

  NetDeviceContainer enbDevs;
  enbDevs = m_lteHelper->InstallEnbDevice (enbNodes);

  NetDeviceContainer ueDevs;
  ueDevs = m_lteHelper->InstallUeDevice (ueNodes);

  // custom code used for testing purposes
  // instead of lteHelper->Attach () and lteHelper->ActivateXxx

  uint32_t i = 0;
  uint32_t tmax = 0;
  for (NetDeviceContainer::Iterator it = ueDevs.Begin (); it != ueDevs.End (); ++it)
    {
      Ptr<NetDevice> ueDevice = *it;
      Ptr<NetDevice> enbDevice = enbDevs.Get (0);      
      Ptr<LteUeNetDevice> ueLteDevice = ueDevice->GetObject<LteUeNetDevice> ();
      Ptr<EpcUeNas> ueNas = ueLteDevice->GetNas ();
      
      uint32_t tc = m_tConnBase + m_tConnIncrPerUe * i; // time connection start
      uint32_t tcc = tc + m_delayConnEnd; // time check connection completed;
      uint32_t td =  tcc + m_delayDiscStart; // time disconnect start
      uint32_t tcd = td + m_delayDiscEnd; // time check disconnection completed
      tmax = std::max (tmax, tcd);
      
      // trick to resolve overloading
      //void (LteHelper::* overloadedAttachFunctionPointer) (Ptr<NetDevice>, Ptr<NetDevice>) = &LteHelper::Attach;      
      //Simulator::Schedule (MilliSeconds (tc), overloadedAttachFunctionPointer, lteHelper, *it, enbDevice);
      Simulator::Schedule (MilliSeconds (tc), &LteRrcConnectionEstablishmentTestCase::Connect, this, ueDevice, enbDevice);

      Simulator::Schedule (MilliSeconds (tcc), &LteRrcConnectionEstablishmentTestCase::CheckConnected, this, *it, enbDevice);

      // disconnection not supported yet

      ++i;
    }


  Simulator::Stop (MilliSeconds (tmax+1));

  Simulator::Run ();

  Simulator::Destroy ();

}

void 
LteRrcConnectionEstablishmentTestCase::Connect (Ptr<NetDevice> ueDevice, Ptr<NetDevice> enbDevice)
{
  m_lteHelper->Attach (ueDevice, enbDevice);

  for (uint32_t b = 0; b < m_nBearers; ++b)
    {
      enum EpsBearer::Qci q = EpsBearer::NGBR_VIDEO_TCP_DEFAULT;
      EpsBearer bearer (q);
      m_lteHelper->ActivateDataRadioBearer (ueDevice, bearer);
    }
}
void 
LteRrcConnectionEstablishmentTestCase::CheckConnected (Ptr<NetDevice> ueDevice, Ptr<NetDevice> enbDevice)
{
  Ptr<LteUeNetDevice> ueLteDevice = ueDevice->GetObject<LteUeNetDevice> ();
  Ptr<LteUeRrc> ueRrc = ueLteDevice->GetRrc ();
  NS_TEST_ASSERT_MSG_EQ (ueRrc->GetState (), LteUeRrc::CONNECTED_NORMALLY, "Wrong LteUeRrc state!");


  Ptr<LteEnbNetDevice> enbLteDevice = enbDevice->GetObject<LteEnbNetDevice> ();
  Ptr<LteEnbRrc> enbRrc = enbLteDevice->GetRrc ();
  uint16_t rnti = ueRrc->GetRnti ();
  Ptr<UeManager> ueManager = enbRrc->GetUeManager (rnti);  
  NS_TEST_ASSERT_MSG_NE (ueManager, 0, "RNTI " << rnti << " not found in eNB");

  NS_TEST_ASSERT_MSG_EQ (ueManager->GetState (), UeManager::CONNECTED_NORMALLY, "Wrong UeManager state!");

  uint16_t ueCellId = ueRrc->GetCellId ();
  uint16_t enbCellId = enbLteDevice->GetCellId ();
  uint16_t ueImsi = ueLteDevice->GetImsi ();
  uint16_t enbImsi = ueManager->GetImsi ();
  uint8_t ueDlBandwidth = ueRrc->GetDlBandwidth ();
  uint8_t enbDlBandwidth = enbLteDevice->GetDlBandwidth ();
  uint8_t ueUlBandwidth = ueRrc->GetUlBandwidth ();
  uint8_t enbUlBandwidth = enbLteDevice->GetUlBandwidth ();
  uint8_t ueDlEarfcn = ueRrc->GetDlEarfcn ();
  uint8_t enbDlEarfcn = enbLteDevice->GetDlEarfcn ();
  uint8_t ueUlEarfcn = ueRrc->GetUlEarfcn ();
  uint8_t enbUlEarfcn = enbLteDevice->GetUlEarfcn ();


  NS_TEST_ASSERT_MSG_EQ (ueCellId, enbCellId, "inconsistent CellId");
  NS_TEST_ASSERT_MSG_EQ (ueImsi, enbImsi, "inconsistent Imsi");
  NS_TEST_ASSERT_MSG_EQ (ueDlBandwidth, enbDlBandwidth, "inconsistent DlBandwidth");
  NS_TEST_ASSERT_MSG_EQ (ueUlBandwidth, enbUlBandwidth, "inconsistent UlBandwidth");
  NS_TEST_ASSERT_MSG_EQ (ueDlEarfcn, enbDlEarfcn, "inconsistent DlEarfcn");
  NS_TEST_ASSERT_MSG_EQ (ueUlEarfcn, enbUlEarfcn, "inconsistent UlEarfcn");

  ObjectMapValue enbDataRadioBearerMapValue;
  ueManager->GetAttribute ("DataRadioBearerMap", enbDataRadioBearerMapValue);
  NS_TEST_ASSERT_MSG_EQ (enbDataRadioBearerMapValue.GetN (), m_nBearers, "wrong num bearers at eNB");  

  ObjectMapValue ueDataRadioBearerMapValue;
  ueRrc->GetAttribute ("DataRadioBearerMap", ueDataRadioBearerMapValue);
  NS_TEST_ASSERT_MSG_EQ (ueDataRadioBearerMapValue.GetN (), m_nBearers, "wrong num bearers at UE"); 

  ObjectMapValue::Iterator enbBearerIt = enbDataRadioBearerMapValue.Begin ();
  ObjectMapValue::Iterator ueBearerIt = ueDataRadioBearerMapValue.Begin ();
  while (enbBearerIt != enbDataRadioBearerMapValue.End () &&
         ueBearerIt != ueDataRadioBearerMapValue.End ())
    {
      Ptr<LteDataRadioBearerInfo> enbDrbInfo = enbBearerIt->second->GetObject<LteDataRadioBearerInfo> ();
      Ptr<LteDataRadioBearerInfo> ueDrbInfo = ueBearerIt->second->GetObject<LteDataRadioBearerInfo> ();
      //NS_TEST_ASSERT_MSG_EQ (enbDrbInfo->m_epsBearer, ueDrbInfo->m_epsBearer, "epsBearer differs");
      NS_TEST_ASSERT_MSG_EQ ((uint32_t) enbDrbInfo->m_epsBearerIdentity, (uint32_t) ueDrbInfo->m_epsBearerIdentity, "epsBearerIdentity differs");
      NS_TEST_ASSERT_MSG_EQ ((uint32_t) enbDrbInfo->m_drbIdentity, (uint32_t) ueDrbInfo->m_drbIdentity, "drbIdentity differs");
      //NS_TEST_ASSERT_MSG_EQ (enbDrbInfo->m_rlcConfig, ueDrbInfo->m_rlcConfig, "rlcConfig differs");
      NS_TEST_ASSERT_MSG_EQ ((uint32_t) enbDrbInfo->m_logicalChannelIdentity, (uint32_t) ueDrbInfo->m_logicalChannelIdentity, "logicalChannelIdentity differs");
      //NS_TEST_ASSERT_MSG_EQ (enbDrbInfo->m_logicalChannelConfig, ueDrbInfo->m_logicalChannelConfig, "logicalChannelConfig differs");
 
      ++enbBearerIt;
      ++ueBearerIt;
    }
  NS_ASSERT_MSG (enbBearerIt == enbDataRadioBearerMapValue.End (), "too many bearers at eNB");
  NS_ASSERT_MSG (ueBearerIt == ueDataRadioBearerMapValue.End (), "too many bearers at UE");
  
}



class LteRrcTestSuite : public TestSuite
{
public:
  LteRrcTestSuite ();
};


LteRrcTestSuite::LteRrcTestSuite ()
  : TestSuite ("lte-rrc", SYSTEM)
{
  NS_LOG_FUNCTION (this);
  for (uint32_t useIdealRrc = 0; useIdealRrc <= 1; ++useIdealRrc)
    {
                                                          //         <----- all times in ms ----------------->
                                                          // nUes, nBearers, tConnBase, tConnIncrPerUe, delayDiscStart, useIdealRrc
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  1,        0,         0,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  1,        0,       100,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  1,        1,         0,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  1,        1,       100,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  1,        2,         0,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  1,        2,       100,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  2,        0,        20,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  2,        0,        20,             10,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  2,        0,        20,            100,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  2,        1,        20,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  2,        1,        20,             10,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  2,        1,        20,            100,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  2,        2,        20,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  2,        2,        20,             10,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  2,        2,        20,            100,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  3,        0,        20,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  4,        0,        20,              0,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase (  4,        0,        20,            300,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase ( 20,        0,        10,              1,             1, useIdealRrc));
      AddTestCase (new LteRrcConnectionEstablishmentTestCase ( 50,        0,         0,              0,             1, useIdealRrc));

      // // time consuming tests with a lot of UEs
      // AddTestCase (new LteRrcConnectionEstablishmentTestCase (100,        0,        10,              0,             1, useIdealRrc));
      // AddTestCase (new LteRrcConnectionEstablishmentTestCase (100,        0,        10,              1,             1, useIdealRrc));
      // AddTestCase (new LteRrcConnectionEstablishmentTestCase (200,        0,        10,              0,             1, useIdealRrc));
      // AddTestCase (new LteRrcConnectionEstablishmentTestCase (200,        0,        10,              1,             1, useIdealRrc));
    }

}

static LteRrcTestSuite g_lteRrcTestSuiteInstance;



} // namespace ns3
