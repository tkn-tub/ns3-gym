/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011, 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 */

#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/double.h"
#include "ns3/test.h"
#include "ns3/mobility-helper.h"
#include "ns3/lte-helper.h"

#include "ns3/lte-ue-phy.h"
#include "ns3/lte-ue-net-device.h"
#include "ns3/lte-enb-phy.h"
#include "ns3/lte-enb-net-device.h"

#include "lte-test-sinr-chunk-processor.h"

NS_LOG_COMPONENT_DEFINE ("LteAntennaTest");

namespace ns3 {



class LteEnbAntennaTestCase : public TestCase
{
public:
  static std::string BuildNameString (double orientationDegrees, double beamwidthDegrees, double x, double y);
  LteEnbAntennaTestCase (double orientationDegrees, double beamwidthDegrees, double x, double y, double antennaGainDb);
  LteEnbAntennaTestCase ();
  virtual ~LteEnbAntennaTestCase ();

private:
  virtual void DoRun (void);

  double m_orientationDegrees;
  double m_beamwidthDegrees;
  double m_x;
  double m_y;
  double m_antennaGainDb;
};




std::string LteEnbAntennaTestCase::BuildNameString (double orientationDegrees, double beamwidthDegrees, double x, double y)
{
  std::ostringstream oss;
  oss <<  "o=" << orientationDegrees
      << ", bw=" << beamwidthDegrees  
      << ", x=" << x 
      << ", y=" << y;
  return oss.str ();
}


LteEnbAntennaTestCase::LteEnbAntennaTestCase (double orientationDegrees, double beamwidthDegrees, double x, double y, double antennaGainDb)
  : TestCase (BuildNameString (orientationDegrees, beamwidthDegrees, x, y)),
    m_orientationDegrees (orientationDegrees),
    m_beamwidthDegrees (beamwidthDegrees),
    m_x (x),
    m_y (y),
    m_antennaGainDb (antennaGainDb)
{
  NS_LOG_FUNCTION (this);
}

LteEnbAntennaTestCase::~LteEnbAntennaTestCase ()
{
}

void
LteEnbAntennaTestCase::DoRun (void)
{
  
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();

  // use 0dB Pathloss, since we are testing only the antenna gain
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::ConstantSpectrumPropagationLossModel"));
  lteHelper->SetPathlossModelAttribute ("Loss", DoubleValue (0.0));

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);
  NodeContainer allNodes = NodeContainer ( enbNodes, ueNodes );

  // Install Mobility Model
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));  // eNB
  positionAlloc->Add (Vector (m_x, m_y, 0.0));  // UE
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (allNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
  lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");
  lteHelper->SetEnbAntennaModelType ("ns3::CosineAntennaModel");
  lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (m_orientationDegrees));
  lteHelper->SetEnbAntennaModelAttribute ("Beamwidth",   DoubleValue (m_beamwidthDegrees));
  lteHelper->SetEnbAntennaModelAttribute ("MaxGain",     DoubleValue (0.0));

  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  // Attach a UE to a eNB
  lteHelper->Attach (ueDevs, enbDevs.Get (0));

  // Activate the default EPS bearer
  enum EpsBearer::Qci q = EpsBearer::NGBR_VIDEO_TCP_DEFAULT;
  EpsBearer bearer (q);
  lteHelper->ActivateEpsBearer (ueDevs, bearer, EpcTft::Default ());

  // Use testing chunk processor in the PHY layer
  // It will be used to test that the SNR is as intended
  Ptr<LtePhy> uePhy = ueDevs.Get (0)->GetObject<LteUeNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testDlSinr = Create<LteTestSinrChunkProcessor> (uePhy);
  uePhy->GetDownlinkSpectrumPhy ()->AddSinrChunkProcessor (testDlSinr);

  Ptr<LtePhy> enbphy = enbDevs.Get (0)->GetObject<LteEnbNetDevice> ()->GetPhy ()->GetObject<LtePhy> ();
  Ptr<LteTestSinrChunkProcessor> testUlSinr = Create<LteTestSinrChunkProcessor> (enbphy);
  enbphy->GetUplinkSpectrumPhy ()->AddSinrChunkProcessor (testUlSinr);


  Simulator::Stop (Seconds (0.020));
  Simulator::Run ();


  const double enbTxPowerDbm = 30; // default eNB TX power over whole bandwdith
  const double ueTxPowerDbm  = 10; // default UE TX power over whole bandwdith
  const double ktDbm = -174;    // reference LTE noise PSD
  const double noisePowerDbm = ktDbm + 10 * log10 (25 * 180000); // corresponds to kT*bandwidth in linear units
  const double ueNoiseFigureDb = 9.0; // default UE noise figure
  const double enbNoiseFigureDb = 5.0; // default eNB noise figure

  double calculatedSinrDbDl = -INFINITY;
  if (testDlSinr->GetSinr () != 0)
    {
      calculatedSinrDbDl = 10.0 * log10 (testDlSinr->GetSinr ()->operator[] (0));
    }
  double calculatedSinrDbUl = -INFINITY;
  if (testUlSinr->GetSinr () != 0)
    {
      calculatedSinrDbUl = 10.0 * log10 (testUlSinr->GetSinr ()->operator[] (0));
    }
  
  // remember that propagation loss is 0dB
  double calculatedAntennaGainDbDl = - (enbTxPowerDbm - calculatedSinrDbDl - noisePowerDbm - ueNoiseFigureDb);
  double calculatedAntennaGainDbUl = - (ueTxPowerDbm - calculatedSinrDbUl - noisePowerDbm - enbNoiseFigureDb);
  double tolerance = (m_antennaGainDb != 0) ? abs (m_antennaGainDb)*0.001 : 0.001;
  NS_TEST_ASSERT_MSG_EQ_TOL (calculatedAntennaGainDbDl, m_antennaGainDb, tolerance, "Wrong DL antenna gain!");
  NS_TEST_ASSERT_MSG_EQ_TOL (calculatedAntennaGainDbUl, m_antennaGainDb, tolerance, "Wrong UL antenna gain!");
  
  Simulator::Destroy ();
}


class LteAntennaTestSuite : public TestSuite
{
public:
  LteAntennaTestSuite ();
};


LteAntennaTestSuite::LteAntennaTestSuite ()
  : TestSuite ("lte-antenna", SYSTEM)
{
  NS_LOG_FUNCTION (this);

  //                                      orientation beamwidth     x            y         gain 
  AddTestCase (new LteEnbAntennaTestCase (       0.0,     90.0,    1.0,          0.0,       0.0));
  AddTestCase (new LteEnbAntennaTestCase (       0.0,     90.0,    1.0,          1.0,      -3.0));
  AddTestCase (new LteEnbAntennaTestCase (       0.0,     90.0,    1.0,         -1.0,      -3.0));
  AddTestCase (new LteEnbAntennaTestCase (       0.0,     90.0,   -1.0,         -1.0,   -36.396));
  AddTestCase (new LteEnbAntennaTestCase (       0.0,     90.0,   -1.0,         -0.0,   -1414.6));
  AddTestCase (new LteEnbAntennaTestCase (       0.0,     90.0,   -1.0,          1.0,   -36.396));
  AddTestCase (new LteEnbAntennaTestCase (      45.0,     90.0,    1.0,          1.0,       0.0));
  AddTestCase (new LteEnbAntennaTestCase (     -45.0,     90.0,    1.0,         -1.0,       0.0));
  AddTestCase (new LteEnbAntennaTestCase (      90.0,     90.0,    1.0,          1.0,      -3.0));
  AddTestCase (new LteEnbAntennaTestCase (     -90.0,     90.0,    1.0,         -1.0,      -3.0)); 

  AddTestCase (new LteEnbAntennaTestCase (       0.0,    120.0,    1.0,          0.0,       0.0));
  AddTestCase (new LteEnbAntennaTestCase (       0.0,    120.0,    0.5,  sin(M_PI/3),      -3.0));
  AddTestCase (new LteEnbAntennaTestCase (       0.0,    120.0,    0.5, -sin(M_PI/3),      -3.0));
  AddTestCase (new LteEnbAntennaTestCase (       0.0,    120.0,   -1.0,         -2.0,   -13.410));
  AddTestCase (new LteEnbAntennaTestCase (       0.0,    120.0,   -1.0,          1.0,   -20.034));
  AddTestCase (new LteEnbAntennaTestCase (      60.0,    120.0,    0.5,  sin(M_PI/3),       0.0));
  AddTestCase (new LteEnbAntennaTestCase (     -60.0,    120.0,    0.5, -sin(M_PI/3),       0.0));
  AddTestCase (new LteEnbAntennaTestCase (     -60.0,    120.0,    0.5, -sin(M_PI/3),       0.0));
  AddTestCase (new LteEnbAntennaTestCase (    -120.0,    120.0,   -0.5, -sin(M_PI/3),       0.0));
  AddTestCase (new LteEnbAntennaTestCase (    -120.0,    120.0,    0.5, -sin(M_PI/3),      -3.0));
  AddTestCase (new LteEnbAntennaTestCase (    -120.0,    120.0,     -1,            0,      -3.0));
  AddTestCase (new LteEnbAntennaTestCase (    -120.0,    120.0,     -1,            2,   -15.578));
  AddTestCase (new LteEnbAntennaTestCase (    -120.0,    120.0,      1,            0,   -14.457));
  AddTestCase (new LteEnbAntennaTestCase (    -120.0,    120.0,      1,            2,   -73.154));
  AddTestCase (new LteEnbAntennaTestCase (    -120.0,    120.0,      1,         -0.1,   -12.754));


}

static LteAntennaTestSuite lteAntennaTestSuite;


} // namespace ns3

