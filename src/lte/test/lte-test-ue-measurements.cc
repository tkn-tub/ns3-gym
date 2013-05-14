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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Marco Miozzo <mmiozzo@cttc.es>
 */

#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include <ns3/enum.h>
#include "ns3/boolean.h"
#include "ns3/mobility-helper.h"
#include "ns3/lte-helper.h"
#include "ns3/ff-mac-scheduler.h"

#include "ns3/lte-enb-phy.h"
#include "ns3/lte-enb-net-device.h"

#include "ns3/lte-ue-phy.h"
#include "ns3/lte-ue-net-device.h"
#include <ns3/lte-enb-rrc.h>

#include "lte-test-ue-measurements.h"

#include "lte-test-sinr-chunk-processor.h"
#include <ns3/lte-common.h>

NS_LOG_COMPONENT_DEFINE ("LteUeMeasurementsTest");

namespace ns3 {


void
ReportUeMeasurementsCallback (LteUeMeasurementsTestCase *testcase, std::string path,
                             uint16_t rnti, uint16_t cellId, double rsrp, double rsrq, bool servingCell)
{
  testcase->ReportUeMeasurements (rnti, cellId, rsrp, rsrq, servingCell);
}

void
RecvMeasurementReportCallback (LteUeMeasurementsTestCase *testcase, std::string path,
                             uint64_t imsi, uint16_t rnti, uint16_t cellId, LteRrcSap::MeasurementReport meas)
{
  testcase->RecvMeasurementReport (imsi, rnti, cellId, meas);
}


/**
 * TestSuite
 */

LteUeMeasurementsTestSuite::LteUeMeasurementsTestSuite ()
  : TestSuite ("lte-ue-measurements", SYSTEM)
{

  AddTestCase (new LteUeMeasurementsTestCase ("d1=10, d2=10000",  10.000000, 10000.000000, -53.739702, -113.739702, -3.010305, -63.010305), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=20, d2=10000",  20.000000, 10000.000000, -59.760302, -113.739702, -3.010319, -56.989719), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=50, d2=10000",  50.000000, 10000.000000, -67.719102, -113.739702, -3.010421, -49.031021), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=100, d2=10000",  100.000000, 10000.000000, -73.739702, -113.739702, -3.010783, -43.010783), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=200, d2=10000",  200.000000, 10000.000000, -79.760302, -113.739702, -3.012232, -36.991632), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=100, d2=10000",  100.000000, 10000.000000, -73.739702, -113.739702, -3.010783, -43.010783), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=200, d2=10000",  200.000000, 10000.000000, -79.760302, -113.739702, -3.012232, -36.991632), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=500, d2=10000",  500.000000, 10000.000000, -87.719102, -113.739702, -3.022359, -29.042959), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=1000, d2=10000",  1000.000000, 10000.000000, -93.739702, -113.739702, -3.058336, -23.058336), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=2000, d2=10000",  2000.000000, 10000.000000, -99.760302, -113.739702, -3.199337, -17.178738), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=5000, d2=10000",  5000.000000, 10000.000000, -107.719102, -113.739702, -4.075793, -10.096393), TestCase::QUICK);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=10000, d2=10000",  10000.000000, 10000.000000, -113.739702, -113.739702, -6.257687, -6.257687), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=20000, d2=10000",  20000.000000, 10000.000000, -119.760302, -113.739702, -10.373365, -4.352765), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=50000, d2=10000",  50000.000000, 10000.000000, -127.719102, -113.739702, -17.605046, -3.625645), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=100000, d2=10000",  100000.000000, 10000.000000, -133.739702, -113.739702, -23.511071, -3.511071), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=200000, d2=10000",  200000.000000, 10000.000000, -139.760302, -113.739702, -29.502549, -3.481949), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=500000, d2=10000",  500000.000000, 10000.000000, -147.719102, -113.739702, -37.453160, -3.473760), TestCase::EXTENSIVE);
  AddTestCase (new LteUeMeasurementsTestCase ("d1=1000000, d2=10000",  1000000.000000, 10000.000000, -153.739702, -113.739702, -43.472589, -3.472589), TestCase::EXTENSIVE);


}

static LteUeMeasurementsTestSuite lteUeMeasurementsTestSuite;


/**
 * TestCase
 */

LteUeMeasurementsTestCase::LteUeMeasurementsTestCase (std::string name, double d1, double d2, double rsrpDbmUe1, double rsrpDbmUe2, double rsrqDbUe1, double rsrqDbUe2)
  : TestCase (name),
    m_d1 (d1),
    m_d2 (d2),
    m_rsrpDbmUeServingCell (rsrpDbmUe1),
    m_rsrpDbmUeNeighborCell (rsrpDbmUe2),
    m_rsrqDbUeServingCell (rsrqDbUe1),
    m_rsrqDbUeNeighborCell (rsrqDbUe2)
{
  NS_LOG_INFO ("Test UE Measurements d1 = " << d1 << " m. and d2 = " << d2 << " m.");
}

LteUeMeasurementsTestCase::~LteUeMeasurementsTestCase ()
{
}

void
LteUeMeasurementsTestCase::DoRun (void)
{
  NS_LOG_INFO (this << GetName ());

  Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));
  Config::SetDefault ("ns3::LteAmc::AmcModel", EnumValue (LteAmc::PiroEW2010));
  Config::SetDefault ("ns3::LteAmc::Ber", DoubleValue (0.00005));
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::FriisSpectrumPropagationLossModel"));
  lteHelper->SetAttribute ("UseIdealRrc", BooleanValue (false));

  LogComponentEnable ("LteUeMeasurementsTest", LOG_LEVEL_ALL);
  
  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes1;
  NodeContainer ueNodes2;
  enbNodes.Create (2);
  ueNodes1.Create (1);
  ueNodes2.Create (1);
  NodeContainer allNodes = NodeContainer ( enbNodes, ueNodes1, ueNodes2);

  // the topology is the following:
  //         d2
  //  UE1-----------eNB2
  //   |             |
  // d1|             |d1
  //   |     d2      |
  //  eNB1----------UE2
  //
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));   // eNB1
  positionAlloc->Add (Vector (m_d2, m_d1, 0.0)); // eNB2
  positionAlloc->Add (Vector (0.0, m_d1, 0.0));  // UE1
  positionAlloc->Add (Vector (m_d2, 0.0, 0.0));  // UE2
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs1;
  NetDeviceContainer ueDevs2;
  lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");
  lteHelper->SetSchedulerAttribute ("UlCqiFilter", EnumValue (FfMacScheduler::PUSCH_UL_CQI));
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs1 = lteHelper->InstallUeDevice (ueNodes1);
  ueDevs2 = lteHelper->InstallUeDevice (ueNodes2);

  lteHelper->Attach (ueDevs1, enbDevs.Get (0));
  lteHelper->Attach (ueDevs2, enbDevs.Get (1));

  // Activate an EPS bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs1, bearer);
  lteHelper->ActivateDataRadioBearer (ueDevs2, bearer);

  // Use testing chunk processor in the PHY layer
  // It will be used to test that the SNR is as intended
  // we plug in two instances, one for DL and one for UL

  Ptr<LtePhy> ue1Phy = ueDevs1.Get (0)->GetObject<LteUeNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testDlSinr1 = Create<LteTestSinrChunkProcessor> (ue1Phy);
  ue1Phy->GetDownlinkSpectrumPhy ()->AddDataSinrChunkProcessor (testDlSinr1);

  Ptr<LtePhy> enb1phy = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testUlSinr1 = Create<LteTestSinrChunkProcessor> (enb1phy);
  enb1phy->GetUplinkSpectrumPhy ()->AddDataSinrChunkProcessor (testUlSinr1);

  Config::Connect ("/NodeList/2/DeviceList/0/LteUePhy/ReportUeMeasurements",
                   MakeBoundCallback (&ReportUeMeasurementsCallback, this));
  Config::Connect ("/NodeList/0/DeviceList/0/LteEnbRrc/RecvMeasurementReport",
                   MakeBoundCallback (&RecvMeasurementReportCallback, this));

  // same as above for eNB2 and UE2

  Ptr<LtePhy> ue2Phy = ueDevs2.Get (0)->GetObject<LteUeNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testDlSinr2 = Create<LteTestSinrChunkProcessor> (ue2Phy);
  ue2Phy->GetDownlinkSpectrumPhy ()->AddDataSinrChunkProcessor (testDlSinr2);

  Ptr<LtePhy> enb2phy = enbDevs.Get (1)->GetObject<LteEnbNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testUlSinr2 = Create<LteTestSinrChunkProcessor> (enb2phy);
  enb1phy->GetUplinkSpectrumPhy ()->AddDataSinrChunkProcessor (testUlSinr2);

  Config::Connect ("/NodeList/3/DeviceList/0/LteUePhy/ReportUeMeasurements",
                   MakeBoundCallback (&ReportUeMeasurementsCallback, this));
  Config::Connect ("/NodeList/1/DeviceList/0/LteEnbRrc/RecvMeasurementReport",
                   MakeBoundCallback (&RecvMeasurementReportCallback, this));

  
// need to allow for RRC connection establishment + SRS
  Simulator::Stop (Seconds (0.800));
  Simulator::Run ();

  Simulator::Destroy ();

}


void
LteUeMeasurementsTestCase::ReportUeMeasurements (uint16_t rnti, uint16_t cellId, double rsrp, double rsrq, bool servingCell)
{
  // need to allow for RRC connection establishment + CQI feedback reception + UE measurements filtering (200 ms)
  if (Simulator::Now () > MilliSeconds (400))
    {
      if (servingCell)
        {
          NS_LOG_DEBUG ("UE serving cellId " << cellId << " Rxed RSRP " << rsrp << " thr " << m_rsrpDbmUeServingCell << " RSRQ " << rsrq << " thr " << m_rsrqDbUeServingCell);
          NS_TEST_ASSERT_MSG_EQ_TOL (m_rsrpDbmUeServingCell, rsrp, 0.2, "Wrong RSRP UE 1");
          NS_TEST_ASSERT_MSG_EQ_TOL (m_rsrqDbUeServingCell, rsrq, 0.2 , "Wrong RSRQ UE 1");
        }
      else
        {
          NS_LOG_DEBUG ("UE neighbor cellId " << cellId << " Rxed RSRP " << rsrp << " thr " << m_rsrpDbmUeNeighborCell << " RSRQ " << rsrq << " thr " << m_rsrqDbUeNeighborCell);
          NS_TEST_ASSERT_MSG_EQ_TOL (m_rsrpDbmUeNeighborCell, rsrp, 0.2 , "Wrong RSRP UE 2");
          NS_TEST_ASSERT_MSG_EQ_TOL (m_rsrqDbUeNeighborCell, rsrq, 0.2 , "Wrong RSRQ UE ");
        }
    }
}


void
LteUeMeasurementsTestCase::RecvMeasurementReport (uint64_t imsi, uint16_t cellId, uint16_t rnti, LteRrcSap::MeasurementReport meas)
{
  // need to allow for RRC connection establishment + CQI feedback reception + UE measurements filtering (200 ms)
  if (Simulator::Now () > MilliSeconds (400))
    {
      if (cellId == imsi)
        {
          NS_LOG_DEBUG ("Serving Cell: received IMSI " << imsi << " CellId " << cellId << " RNTI " << rnti  << " thr " << (uint16_t)EutranMeasurementMapping::Dbm2RsrpRange (m_rsrpDbmUeServingCell) << " RSRP " << (uint16_t)meas.measResults.rsrpResult << " RSRQ " << (uint16_t)meas.measResults.rsrqResult << " thr " << (uint16_t)EutranMeasurementMapping::Db2RsrqRange (m_rsrqDbUeServingCell));
          NS_TEST_ASSERT_MSG_EQ (meas.measResults.rsrpResult, EutranMeasurementMapping::Dbm2RsrpRange (m_rsrpDbmUeServingCell), "Wrong RSRP ");
          NS_TEST_ASSERT_MSG_EQ (meas.measResults.rsrqResult, EutranMeasurementMapping::Db2RsrqRange (m_rsrqDbUeServingCell), "Wrong RSRQ ");
        }
      else
        {
          NS_LOG_DEBUG ("Neighbor cell: received IMSI " << imsi << " CellId " << cellId << " RNTI " << rnti  << " thr " << (uint16_t)EutranMeasurementMapping::Dbm2RsrpRange (m_rsrpDbmUeNeighborCell) << " RSRP " << (uint16_t)meas.measResults.rsrpResult << " RSRQ " << (uint16_t)meas.measResults.rsrqResult << " thr " << (uint16_t)EutranMeasurementMapping::Db2RsrqRange (m_rsrqDbUeNeighborCell));
          NS_TEST_ASSERT_MSG_EQ (meas.measResults.rsrpResult, EutranMeasurementMapping::Dbm2RsrpRange (m_rsrpDbmUeNeighborCell), "Wrong RSRP ");
          NS_TEST_ASSERT_MSG_EQ (meas.measResults.rsrqResult, EutranMeasurementMapping::Db2RsrqRange (m_rsrqDbUeNeighborCell), "Wrong RSRQ ");
        }
    }
}

} // namespace ns3

