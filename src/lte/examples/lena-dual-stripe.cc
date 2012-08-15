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
#include <ns3/internet-module.h>
#include <ns3/lte-module.h>
#include <ns3/config-store-module.h>
#include <ns3/buildings-module.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/applications-module.h>
#include <ns3/log.h>
#include <iomanip>
#include <string>
#include <vector>

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
  Ptr<UniformRandomVariable> m_xMinVar;
  Ptr<UniformRandomVariable> m_yMinVar;
  
};

FemtocellBlockAllocator::FemtocellBlockAllocator (Box area, uint32_t nApartmentsX, uint32_t nFloors)
  : m_area (area),
    m_nApartmentsX (nApartmentsX),
    m_nFloors (nFloors),
    m_xSize (nApartmentsX*10 + 20),
    m_ySize (70)
{
    m_xMinVar = CreateObject<UniformRandomVariable> ();
    m_xMinVar->SetAttribute ("Min", DoubleValue (area.xMin));
    m_xMinVar->SetAttribute ("Max", DoubleValue (area.xMax - m_xSize));
    m_yMinVar = CreateObject<UniformRandomVariable> ();
    m_yMinVar->SetAttribute ("Min", DoubleValue (area.yMin));
    m_yMinVar->SetAttribute ("Max", DoubleValue (area.yMax - m_ySize));
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
      box.xMin = m_xMinVar->GetValue ();
      box.xMax = box.xMin + m_xSize;
      box.yMin = m_yMinVar->GetValue ();
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

void 
PrintGnuplottableBuildingListToFile (std::string filename)
{
  std::ofstream outFile;
  outFile.open (filename.c_str ());
  if (!outFile.is_open ())
    {
      NS_LOG_ERROR ("Can't open file " << filename);
      return;
    }
  uint32_t index = 0;
  for (BuildingList::Iterator it = BuildingList::Begin (); it != BuildingList::End (); ++it)
    {
      ++index;
      Box box = (*it)->GetBoundaries ();
      outFile << "set object " << index
              << " rect from " << box.xMin  << "," << box.yMin  
              << " to "   << box.xMax  << "," << box.yMax
              << " front fs empty "
              << std::endl;
    }
}

void 
PrintGnuplottableUeListToFile (std::string filename)
{
  std::ofstream outFile;
  outFile.open (filename.c_str ());
  if (!outFile.is_open ())
    {
      NS_LOG_ERROR ("Can't open file " << filename);
      return;
    }
  for (NodeList::Iterator it = NodeList::Begin (); it != NodeList::End (); ++it)
    {
      Ptr<Node> node = *it;
      int nDevs = node->GetNDevices ();
      for (int j = 0; j < nDevs; j++)
        {
          Ptr<LteUeNetDevice> uedev = node->GetDevice (j)->GetObject <LteUeNetDevice> ();
          if (uedev)
            {
              Vector pos = node->GetObject<MobilityModel> ()->GetPosition ();
              outFile << "set label \"" << uedev->GetImsi ()
                      << "\" at "<< pos.x << "," << pos.y << " left font \"Helvetica,4\" textcolor rgb \"grey\" front point pt 1 ps 0.3 lc rgb \"grey\" offset 0,0"  
                      << std::endl;
            }
        }
    }
}

void 
PrintGnuplottableEnbListToFile (std::string filename)
{
  std::ofstream outFile;
  outFile.open (filename.c_str ());
  if (!outFile.is_open ())
    {
      NS_LOG_ERROR ("Can't open file " << filename);
      return;
    }
  for (NodeList::Iterator it = NodeList::Begin (); it != NodeList::End (); ++it)
    {
      Ptr<Node> node = *it;
      int nDevs = node->GetNDevices ();
      for (int j = 0; j < nDevs; j++)
        {
          Ptr<LteEnbNetDevice> enbdev = node->GetDevice (j)->GetObject <LteEnbNetDevice> ();
          if (enbdev)
            {
              Vector pos = node->GetObject<MobilityModel> ()->GetPosition ();
              outFile << "set label \"" << enbdev->GetCellId ()
                      << "\" at "<< pos.x << "," << pos.y << " left font \"Helvetica,4\" textcolor rgb \"white\" front  point pt 2 ps 0.3 lc rgb \"white\" offset 0,0"  
                      << std::endl;
            }
        }
    }
}

int
main (int argc, char *argv[])
{
  // change some default attributes so that they are reasonable for
  // this scenario, but do this before processing command line
  // arguments, so that the user is allowed to override these settings 
  Config::SetDefault ("ns3::UdpClient::Interval", TimeValue (MilliSeconds(1)));
  Config::SetDefault ("ns3::UdpClient::MaxPackets", UintegerValue(1000000));
  
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
  uint16_t macroEnbBandwidth = 25;
  uint16_t homeEnbBandwidth = 25;
  double simTime = 0.25;
  bool epc = false;
  bool epcDl = true;
  bool epcUl = true;
  bool useUdp = true;
  bool generateRem = false;
  std::string fadingFileTrace = "";
  
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
  cmd.AddValue ("epc", "if true, will setup the EPC to simulate an end-to-end topology;"
                "if false, only the LTE radio access will be simulated.", epc);
  cmd.AddValue ("epcDl", "if true, will activate data flows in the downlink when EPC is being used. "
                "If false, downlink flows won't be activated. "
                "If EPC is not used, this parameter will be ignored.", epcDl);
  cmd.AddValue ("epcUl", "if true, will activate data flows in the uplink when EPC is being used. "
                "If false, uplink flows won't be activated. "
                "If EPC is not used, this parameter will be ignored.", epcUl);
  cmd.AddValue ("useUdp", "if true, the UdpClient application will be used. "
                "Otherwise, the BulkSend application will be used over a TCP connection. "
                "If EPC is not used, this parameter will be ignored.", useUdp);
  cmd.AddValue ("fadingTrace", "The path of the fading trace (by default any fading trace is loadedm which implies that fading is not considered)", fadingFileTrace);


  cmd.Parse (argc, argv);

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  cmd.Parse (argc, argv);

  Box macroUeBox;

  if (nMacroEnbSites > 0)
    {
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

      macroUeBox = Box (-areaMarginFactor*interSiteDistance, 
                        (nMacroEnbSitesX + areaMarginFactor)*interSiteDistance, 
                        -areaMarginFactor*interSiteDistance, 
                        (nMacroEnbSitesY -1)*interSiteDistance*sqrt(0.75) + areaMarginFactor*interSiteDistance,
                        1.0, 2.0);
    }
  else
    {
      // still need the box to place femtocell blocks
      macroUeBox = Box (0, 150, 0, 150, 1.0, 2.0);
    }
  
  FemtocellBlockAllocator blockAllocator (macroUeBox, nApartmentsX, nFloors);
  blockAllocator.Create (nBlocks);


  uint32_t nHomeEnbs = round (4 * nApartmentsX * nBlocks * nFloors * homeEnbDeploymentRatio * homeEnbActivationRatio);
  NS_LOG_LOGIC ("nHomeEnbs = " << nHomeEnbs);
  uint32_t nHomeUes = round (nHomeEnbs * homeUesHomeEnbRatio);
  NS_LOG_LOGIC ("nHomeUes = " << nHomeUes);
  double macroUeAreaSize = (macroUeBox.xMax - macroUeBox.xMin) * (macroUeBox.yMax - macroUeBox.yMin);
  uint32_t nMacroUes = round (macroUeAreaSize * macroUeDensity) ;
  NS_LOG_LOGIC ("nMacroUes = " << nMacroUes << " (density=" << macroUeDensity << ")");
  
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
  
  if (!fadingFileTrace.empty ())
    {
      lteHelper->SetAttribute ("FadingModel", StringValue ("ns3::TraceFadingLossModel"));
      lteHelper->SetFadingModelAttribute("TraceFilename", StringValue (fadingFileTrace));
    }

  Ptr<EpcHelper> epcHelper;
  if (epc)
    {
      NS_LOG_LOGIC ("enabling EPC");
      epcHelper = CreateObject<EpcHelper> ();
      lteHelper->SetEpcHelper (epcHelper);      
    }

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
  Ptr<UniformRandomVariable> xVal = CreateObject<UniformRandomVariable> ();
  xVal->SetAttribute ("Min", DoubleValue (macroUeBox.xMin));
  xVal->SetAttribute ("Max", DoubleValue (macroUeBox.xMax));
  positionAlloc->SetAttribute ("X", PointerValue (xVal));
  Ptr<UniformRandomVariable> yVal = CreateObject<UniformRandomVariable> ();
  yVal->SetAttribute ("Min", DoubleValue (macroUeBox.yMin));
  yVal->SetAttribute ("Max", DoubleValue (macroUeBox.yMax));
  positionAlloc->SetAttribute ("Y", PointerValue (yVal));
  Ptr<UniformRandomVariable> zVal = CreateObject<UniformRandomVariable> ();
  zVal->SetAttribute ("Min", DoubleValue (macroUeBox.zMin));
  zVal->SetAttribute ("Max", DoubleValue (macroUeBox.zMax));
  positionAlloc->SetAttribute ("Z", PointerValue (zVal));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (macroUes);
  NetDeviceContainer macroUeDevs = lteHelper->InstallUeDevice (macroUes);
  lteHelper->AttachToClosestEnb (macroUeDevs, macroEnbDevs);


  // home UEs located in the same apartment in which there are the Home eNBs
  positionAlloc = CreateObject<SameRoomPositionAllocator> (homeEnbs);
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (homeUes);
  NetDeviceContainer homeUeDevs = lteHelper->InstallUeDevice (homeUes);

  NetDeviceContainer::Iterator ueDevIt;
  NetDeviceContainer::Iterator enbDevIt = homeEnbDevs.Begin ();
  // attach explicitly each home UE to its home eNB
  for (ueDevIt = homeUeDevs.Begin (); 
       ueDevIt != homeUeDevs.End ();
       ++ueDevIt, ++enbDevIt)
    {
      // this because of the order in which SameRoomPositionAllocator
      // will place the UEs
      if (enbDevIt == homeEnbDevs.End ())
        {
          enbDevIt = homeEnbDevs.Begin ();
        }
      lteHelper->Attach (*ueDevIt, *enbDevIt);
    }


  if (epc)
    {
      NS_LOG_LOGIC ("setting up internet, remote host and applications");
  
      // Create a single RemoteHost
      NodeContainer remoteHostContainer;
      remoteHostContainer.Create (1);
      Ptr<Node> remoteHost = remoteHostContainer.Get (0);
      InternetStackHelper internet;
      internet.Install (remoteHostContainer);

      // Create the Internet
      PointToPointHelper p2ph;
      p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
      p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
      p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
      Ptr<Node> pgw = epcHelper->GetPgwNode ();
      NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
      Ipv4AddressHelper ipv4h;
      ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
      Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
      // in this container, interface 0 is the pgw, 1 is the remoteHost
      Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);

      Ipv4StaticRoutingHelper ipv4RoutingHelper;
      Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
      remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

      // for internetworking purposes, consider together home UEs and macro UEs
      NodeContainer ues;
      ues.Add (homeUes);
      ues.Add (macroUes);
      NetDeviceContainer ueDevs;
      ueDevs.Add (homeUeDevs);
      ueDevs.Add (macroUeDevs);      

      // Install the IP stack on the UEs      
      internet.Install (ues);
      Ipv4InterfaceContainer ueIpIfaces;
      ueIpIfaces = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueDevs));
      
      // Install and start applications on UEs and remote host
      uint16_t dlPort = 10000;
      uint16_t ulPort = 20000;

      ApplicationContainer clientApps;
      ApplicationContainer serverApps;
      for (uint32_t u = 0; u < ues.GetN (); ++u)
        {
          Ptr<Node> ue = ues.Get (u);
          // Set the default gateway for the UE
          Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
          ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
          ++dlPort;
          ++ulPort;
          if (useUdp)
            {              
              if (epcDl)
                {
                  NS_LOG_LOGIC ("installing UDP DL app for UE " << u);
                  UdpClientHelper dlClientHelper (ueIpIfaces.GetAddress (u), dlPort);
                  clientApps.Add (dlClientHelper.Install (remoteHost));
                  PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory", 
                                                       InetSocketAddress (Ipv4Address::GetAny (), dlPort));
                  serverApps.Add (dlPacketSinkHelper.Install (ue));
                }
              if (epcUl)
                {      
                  NS_LOG_LOGIC ("installing UDP UL app for UE " << u);
                  UdpClientHelper ulClientHelper (remoteHostAddr, ulPort);
                  clientApps.Add (ulClientHelper.Install (ue));
                  PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory", 
                                                       InetSocketAddress (Ipv4Address::GetAny (), ulPort));
                  serverApps.Add (ulPacketSinkHelper.Install (remoteHost));  
                }            
            }                    
          else // use TCP
            {
              if (epcDl)
                {
                  NS_LOG_LOGIC ("installing TCP DL app for UE " << u);
                  BulkSendHelper dlClientHelper ("ns3::TcpSocketFactory",
                                                 InetSocketAddress (ueIpIfaces.GetAddress (u), dlPort));
                  dlClientHelper.SetAttribute ("MaxBytes", UintegerValue (0));
                  clientApps.Add (dlClientHelper.Install (remoteHost));
                  PacketSinkHelper dlPacketSinkHelper ("ns3::TcpSocketFactory", 
                                                       InetSocketAddress (Ipv4Address::GetAny (), dlPort));
                  serverApps.Add (dlPacketSinkHelper.Install (ue));
                }
              if (epcUl)
                {     
                  NS_LOG_LOGIC ("installing TCP UL app for UE " << u);              
                  BulkSendHelper ulClientHelper ("ns3::TcpSocketFactory",
                                                 InetSocketAddress (remoteHostAddr, ulPort));
                  ulClientHelper.SetAttribute ("MaxBytes", UintegerValue (0));                  
                  clientApps.Add (ulClientHelper.Install (ue));
                  PacketSinkHelper ulPacketSinkHelper ("ns3::TcpSocketFactory", 
                                                       InetSocketAddress (Ipv4Address::GetAny (), ulPort));
                  serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
                }
            }
        }
      serverApps.Start (Seconds (0.0));
      clientApps.Start (Seconds (0.0));

    } // end if (epc)


 
  // activate bearer for all UEs
  enum EpsBearer::Qci q = EpsBearer::NGBR_VIDEO_TCP_DEFAULT;
  EpsBearer bearer (q);
  lteHelper->ActivateEpsBearer (homeUeDevs, bearer, EpcTft::Default ());  
  lteHelper->ActivateEpsBearer (macroUeDevs, bearer, EpcTft::Default ());



  BuildingsHelper::MakeMobilityModelConsistent ();

  Ptr<RadioEnvironmentMapHelper> remHelper;
  if (generateRem)
    {
      PrintGnuplottableBuildingListToFile ("buildings.txt");
      PrintGnuplottableEnbListToFile ("enbs.txt");
      PrintGnuplottableUeListToFile ("ues.txt");

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

  lteHelper->EnableMacTraces ();
  lteHelper->EnableRlcTraces ();
  if (epc)
    {
      lteHelper->EnablePdcpTraces ();
    }

  Simulator::Run ();

  //GtkConfigStore config;
  //config.ConfigureAttributes ();

  lteHelper = 0;
  Simulator::Destroy ();
  return 0;
}
