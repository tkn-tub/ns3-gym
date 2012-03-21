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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include <ns3/lte-module.h>
#include <ns3/config-store.h>
#include <ns3/buildings-module.h>
#include <ns3/radio-environment-map-helper.h>
#include <ns3/log.h>
#include <iomanip>
#include <string>
#include <vector>

// uncomment this to use GtkConfigStore
//#include <ns3/gtk-config-store.h>

// The topology of this simulation program is inspired from 
// 3GPP R4-092042, Section 4.2.1 Dual Stripe Model
// note that the term "apartments" used in that document matches with
// the term "room" used in the BuildingsMobilityModel 

using namespace ns3;


NS_LOG_COMPONENT_DEFINE ("LenaDualStripe");


bool AreOverlapping (Box a, Box b)
{
  return !((a.xMin > b.xMax) || (b.xMin > a.xMax) || (a.yMin > b.yMax) || (b.yMin > a.yMax));
}

class FemtocellBlockAllocator
{
public:
  FemtocellBlockAllocator (Box area, uint32_t nApartmentsX, uint32_t nFloors);
  void Create (uint32_t n);
  void Create ();
  
private:
  bool OverlapsWithAnyPrevious (Box);
  Box m_area;
  uint32_t m_nApartmentsX;
  uint32_t m_nFloors;
  std::list<Box> m_previousBlocks;
  double m_xSize;
  double m_ySize;
  UniformVariable m_xMinVar;
  UniformVariable m_yMinVar;
  
};

FemtocellBlockAllocator::FemtocellBlockAllocator (Box area, uint32_t nApartmentsX, uint32_t nFloors)
  : m_area (area),
    m_nApartmentsX (nApartmentsX),
    m_nFloors (nFloors),
    m_xSize (nApartmentsX*10 + 20),
    m_ySize (70),
    m_xMinVar (area.xMin, area.xMax - m_xSize),
    m_yMinVar (area.yMin, area.yMax - m_ySize)
{
 
}

void 
FemtocellBlockAllocator::Create (uint32_t n)
{
  for (uint32_t i = 0; i < n; ++i)
    {
      Create ();
    }
}

void
FemtocellBlockAllocator::Create ()
{
  Box box;
  uint32_t attempt = 0;
  do 
    {
      NS_ASSERT_MSG (attempt < 100, "Too many failed attemtps to position apartment block. Too many blocks? Too small area?");
      box.xMin = m_xMinVar.GetValue ();
      box.xMax = box.xMin + m_xSize;
      box.yMin = m_yMinVar.GetValue ();
      box.yMax = box.yMin + m_ySize;      
      ++attempt;
    }
  while (OverlapsWithAnyPrevious (box));

  NS_LOG_LOGIC ("allocated non overlapping block " << box);
  m_previousBlocks.push_back (box);
  Ptr<GridBuildingAllocator>  gridBuildingAllocator;
  gridBuildingAllocator = CreateObject<GridBuildingAllocator> ();
  gridBuildingAllocator->SetAttribute ("GridWidth", UintegerValue (1));
  gridBuildingAllocator->SetAttribute ("LengthX", DoubleValue (10*m_nApartmentsX)); 
  gridBuildingAllocator->SetAttribute ("LengthY", DoubleValue (10*2));
  gridBuildingAllocator->SetAttribute ("DeltaX", DoubleValue (10));
  gridBuildingAllocator->SetAttribute ("DeltaY", DoubleValue (10));
  gridBuildingAllocator->SetAttribute ("Height", DoubleValue (3*m_nFloors));
  gridBuildingAllocator->SetBuildingAttribute ("NRoomsX", UintegerValue (m_nApartmentsX));
  gridBuildingAllocator->SetBuildingAttribute ("NRoomsY", UintegerValue (2));
  gridBuildingAllocator->SetBuildingAttribute ("NFloors", UintegerValue (m_nFloors));
  gridBuildingAllocator->SetAttribute ("MinX", DoubleValue (box.xMin + 10));
  gridBuildingAllocator->SetAttribute ("MinY", DoubleValue (box.yMin + 10));
  gridBuildingAllocator->Create (2);
}

bool 
FemtocellBlockAllocator::OverlapsWithAnyPrevious (Box box)
{
  for (std::list<Box>::iterator it = m_previousBlocks.begin (); it != m_previousBlocks.end (); ++it)
    {
      if (AreOverlapping (*it, box))
        {
          return true;
        }
    }
  return false;
}

int
main (int argc, char *argv[])
{
  // scenario parameters
  uint32_t nBlocks = 10;
  uint32_t nApartmentsX = 10;
  uint32_t nFloors = 1;
  uint32_t nMacroEnbSites = 3;
  uint32_t nMacroEnbSitesX = 1;
  double interSiteDistance = 500;
  double areaMarginFactor = 0.5; 
  double macroUeDensity = 0.0001;
  double homeEnbDeploymentRatio = 0.2;
  double homeEnbActivationRatio = 0.5;
  double homeUesHomeEnbRatio = 1;
  double macroEnbTxPowerDbm = 46.0;
  double homeEnbTxPowerDbm = 20.0;
  uint16_t macroEnbDlEarfcn = 100;
  uint16_t homeEnbDlEarfcn = 100;
  uint8_t macroEnbBandwidth = 25;
  uint8_t homeEnbBandwidth = 25;
  double simTime = 0.01;
  bool generateRem = false;
  
  CommandLine cmd;
  cmd.AddValue ("nBlocks", "Number of femtocell blocks", nBlocks);
  cmd.AddValue ("nApartmentsX", "Number of apartments along the X axis in a femtocell block", nApartmentsX);
  cmd.AddValue ("nFloors", "Number of floors", nFloors);
  cmd.AddValue ("nMacroEnbSites", "How many macro sites there are", nMacroEnbSites);
  cmd.AddValue ("nMacroEnbSitesX", 
                "(minimum) number of sites along the X-axis of the hex grid", nMacroEnbSitesX);
  cmd.AddValue ("interSiteDistance", "min distance between two nearby macro cell sites", interSiteDistance);
  cmd.AddValue ("areaMarginFactor", "how much the UE area extends outside the macrocell grid, "
                "expressed as fraction of the interSiteDistance", areaMarginFactor);
  cmd.AddValue ("macroUeDensity", "How many macrocell UEs there are per square meter", macroUeDensity);
  cmd.AddValue ("homeEnbDeploymentRatio", 
                "The HeNB deployment ratio as per 3GPP R4-092042", homeEnbDeploymentRatio);
  cmd.AddValue ("homeEnbActivationRatio", 
                "The HeNB activation ratio as per 3GPP R4-092042", homeEnbActivationRatio);
  cmd.AddValue ("homeUesHomeEnbRatio", 
                "How many (on average) home UEs per HeNB there are in the simulation", 
                homeUesHomeEnbRatio);
  cmd.AddValue ("macroEnbTxPowerDbm", "TX power [dBm] used by macro eNBs", macroEnbTxPowerDbm);
  cmd.AddValue ("homeEnbTxPowerDbm", "TX power [dBm] used by HeNBs", homeEnbTxPowerDbm);
  cmd.AddValue ("macroEnbDlEarfcn", "DL EARFCN used by macro eNBs", macroEnbDlEarfcn);
  cmd.AddValue ("homeEnbDlEarfcn", "DL EARFCN used by HeNBs", homeEnbDlEarfcn);
  cmd.AddValue ("macroEnbBandwidth", "bandwdith [num RBs] used by macro eNBs", macroEnbBandwidth);
  cmd.AddValue ("homeEnbBandwidth", "bandwdith [num RBs] used by HeNBs", homeEnbBandwidth);
  cmd.AddValue ("simTime", "Total duration of the simulation [s]", simTime);
  cmd.AddValue ("generateRem", "if true, will generate a REM and then abort the simulation;"
                "if false, will run the simulation normally (without generating any REM)", generateRem);

  cmd.Parse (argc, argv);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  cmd.Parse (argc, argv);

  uint32_t currentSite = nMacroEnbSites -1;
  uint32_t biRowIndex = (currentSite / (nMacroEnbSitesX + nMacroEnbSitesX + 1));
  uint32_t biRowRemainder = currentSite % (nMacroEnbSitesX + nMacroEnbSitesX + 1);
  uint32_t rowIndex = biRowIndex*2 + 1;
  if (biRowRemainder >= nMacroEnbSitesX)
    {
      ++rowIndex;
    }
  uint32_t nMacroEnbSitesY = rowIndex;
  NS_LOG_LOGIC ("nMacroEnbSitesY = " << nMacroEnbSitesY);

  Box macroUeBox (-areaMarginFactor*interSiteDistance, 
                  (nMacroEnbSitesX + areaMarginFactor)*interSiteDistance, 
                  -areaMarginFactor*interSiteDistance, 
                  (nMacroEnbSitesY -1)*interSiteDistance*sqrt(0.75) + areaMarginFactor*interSiteDistance,
                  1.0, 2.0);
  
  FemtocellBlockAllocator blockAllocator (macroUeBox, nApartmentsX, nFloors);
  blockAllocator.Create (nBlocks);


  uint32_t nHomeEnbs = round (4 * nApartmentsX * nBlocks * nFloors * homeEnbDeploymentRatio * homeEnbActivationRatio);
  NS_LOG_LOGIC ("nHomeEnbs = " << nHomeEnbs);
  uint32_t nHomeUes = round (nHomeEnbs * homeUesHomeEnbRatio);
  NS_LOG_LOGIC ("nHomeUes = " << nHomeUes);
  double macroUeAreaSize = (macroUeBox.xMax - macroUeBox.xMin) * (macroUeBox.yMax - macroUeBox.yMin);
  uint32_t nMacroUes = round (macroUeAreaSize * macroUeDensity) ;
  NS_LOG_LOGIC ("nMacroUes = " << nMacroUes);
  
  NodeContainer homeEnbs;
  homeEnbs.Create (nHomeEnbs);
  NodeContainer macroEnbs;
  macroEnbs.Create (3 * nMacroEnbSites);
  NodeContainer homeUes;
  homeUes.Create (nHomeUes);
  NodeContainer macroUes;
  macroUes.Create (nMacroUes);

  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::BuildingsMobilityModel");


  Ptr <LteHelper> lteHelper = CreateObject<LteHelper> ();
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::HybridBuildingsPropagationLossModel"));
  lteHelper->SetPathlossModelAttribute ("ShadowSigmaExtWalls", DoubleValue (0));
  lteHelper->SetPathlossModelAttribute ("ShadowSigmaOutdoor", DoubleValue (1));
  lteHelper->SetPathlossModelAttribute ("ShadowSigmaIndoor", DoubleValue (1.5));
  // use always LOS model
  lteHelper->SetPathlossModelAttribute ("Los2NlosThr", DoubleValue (1e6));
  lteHelper->SetSpectrumChannelType ("ns3::MultiModelSpectrumChannel");



  // Macro eNBs in 3-sector hex grid
  
  mobility.Install (macroEnbs);
  Ptr<LteHexGridEnbTopologyHelper> lteHexGridEnbTopologyHelper = CreateObject<LteHexGridEnbTopologyHelper> ();
  lteHexGridEnbTopologyHelper->SetLteHelper (lteHelper);
  lteHexGridEnbTopologyHelper->SetAttribute ("InterSiteDistance", DoubleValue (interSiteDistance));
  lteHexGridEnbTopologyHelper->SetAttribute ("MinX", DoubleValue (interSiteDistance/2));
  lteHexGridEnbTopologyHelper->SetAttribute ("GridWidth", UintegerValue (nMacroEnbSitesX));
  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (macroEnbTxPowerDbm));
  lteHelper->SetEnbAntennaModelType ("ns3::ParabolicAntennaModel");
  lteHelper->SetEnbAntennaModelAttribute ("Beamwidth",   DoubleValue (70));
  lteHelper->SetEnbAntennaModelAttribute ("MaxAttenuation",     DoubleValue (20.0));
  lteHelper->SetEnbDeviceAttribute ("DlEarfcn", UintegerValue (macroEnbDlEarfcn));
  lteHelper->SetEnbDeviceAttribute ("UlEarfcn", UintegerValue (macroEnbDlEarfcn + 18000));
  lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (macroEnbBandwidth));
  lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (macroEnbBandwidth));
  NetDeviceContainer macroEnbDevs = lteHexGridEnbTopologyHelper->SetPositionAndInstallEnbDevice (macroEnbs);

  
  // HomeEnbs randomly indoor
  
  Ptr<PositionAllocator> positionAlloc = CreateObject<RandomRoomPositionAllocator> ();
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (homeEnbs);
  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (homeEnbTxPowerDbm));
  lteHelper->SetEnbAntennaModelType ("ns3::IsotropicAntennaModel");
  lteHelper->SetEnbDeviceAttribute ("DlEarfcn", UintegerValue (homeEnbDlEarfcn));
  lteHelper->SetEnbDeviceAttribute ("UlEarfcn", UintegerValue (homeEnbDlEarfcn + 18000));
  lteHelper->SetEnbDeviceAttribute ("DlBandwidth", UintegerValue (homeEnbBandwidth));
  lteHelper->SetEnbDeviceAttribute ("UlBandwidth", UintegerValue (homeEnbBandwidth));
  NetDeviceContainer homeEnbDevs  = lteHelper->InstallEnbDevice (homeEnbs);
  

  // macro Ues 
  NS_LOG_LOGIC ("randomly allocating macro UEs in " << macroUeBox);
  positionAlloc = CreateObject<RandomBoxPositionAllocator> ();
  positionAlloc->SetAttribute ("X", RandomVariableValue (UniformVariable (macroUeBox.xMin, macroUeBox.xMax)));
  positionAlloc->SetAttribute ("Y", RandomVariableValue (UniformVariable (macroUeBox.yMin, macroUeBox.yMax)));
  positionAlloc->SetAttribute ("Z", RandomVariableValue (UniformVariable (macroUeBox.zMin, macroUeBox.zMax)));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (macroUes);
  NetDeviceContainer macroUeDevs = lteHelper->InstallUeDevice (macroUes);
  lteHelper->AttachToClosestEnb (macroUeDevs, macroEnbDevs);


  // home UEs located in the same apartment in which there are the Home eNBs
  positionAlloc = CreateObject<SameRoomPositionAllocator> (homeEnbs);
  mobility.Install (homeUes);
  NetDeviceContainer homeUeDevs = lteHelper->InstallUeDevice (homeUes);
  lteHelper->AttachToClosestEnb (homeUeDevs, homeEnbDevs);


  
  // activate bearer for all UEs
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateEpsBearer (homeUeDevs, bearer, EpcTft::Default ());  
  lteHelper->ActivateEpsBearer (macroUeDevs, bearer, EpcTft::Default ());
  


  BuildingsHelper::MakeMobilityModelConsistent ();



  Ptr<RadioEnvironmentMapHelper> remHelper;
  if (generateRem)
    {
      remHelper = CreateObject<RadioEnvironmentMapHelper> ();
      remHelper->SetAttribute ("ChannelPath", StringValue ("/ChannelList/0"));
      remHelper->SetAttribute ("OutputFile", StringValue ("lena-dual-stripe.rem"));
      remHelper->SetAttribute ("XMin", DoubleValue (macroUeBox.xMin));
      remHelper->SetAttribute ("XMax", DoubleValue (macroUeBox.xMax));
      remHelper->SetAttribute ("YMin", DoubleValue (macroUeBox.yMin));
      remHelper->SetAttribute ("YMax", DoubleValue (macroUeBox.yMax));
      remHelper->SetAttribute ("Z", DoubleValue (1.5));
      remHelper->Install ();
      // simulation will stop right after the REM has been generated
    }
  else
    {
      Simulator::Stop (Seconds (simTime));  
    }

  Simulator::Run ();

  //GtkConfigStore config;
  //config.ConfigureAttributes ();

  lteHelper = 0;
  Simulator::Destroy ();
  return 0;
}
