//      +-----+    +-----+    +-----+
//      | UE0 |    | UE1 |    | UE2 |
//      +-----+    +-----+    +-----+
//     10.1.1.1   10.1.1.2   10.1.1.3
//      --------  --------    -------
//        ((*))    ((*))       ((*))
//
//                  10.1.1.4
//               +------------+
//               |eNB         | ==((*))
//               +------------+

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/internet-module.h"
#include "ns3/lte-module.h"
#include <iostream>
#include "ns3/global-route-manager.h"

NS_LOG_COMPONENT_DEFINE ("lte-device");

using namespace ns3;


int main (int argc, char *argv[])
{
  // default values
  int nbUE = 3;

  LteHelper lte;

  //lte.EnableLogComponents ();
  LogComponentEnable ("UdpClient", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpServer", LOG_LEVEL_INFO);


  // CREATE NODE CONTAINER AND CREATE LTE NODES
  NodeContainer ueNodes;
  NodeContainer enbNodes;
  ueNodes.Create (nbUE);
  enbNodes.Create (1);


  // CREATE DEVICE CONTAINER, INSTALL DEVICE TO NODE
  NetDeviceContainer ueDevs, enbDevs;
  ueDevs = lte.Install (ueNodes, LteHelper::DEVICE_TYPE_USER_EQUIPMENT);
  enbDevs = lte.Install (enbNodes, LteHelper::DEVICE_TYPE_ENODEB);



  // INSTALL INTERNET STACKS
  InternetStackHelper stack;
  stack.Install (ueNodes);
  stack.Install (enbNodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer UEinterfaces = address.Assign (ueDevs);
  Ipv4InterfaceContainer ENBinterface = address.Assign (enbDevs);



  // MANAGE LTE NET DEVICES
  Ptr<EnbNetDevice> enb;
  enb = enbDevs.Get (0)->GetObject<EnbNetDevice> ();

  std::vector<Ptr<UeNetDevice> > ue (nbUE);
  for (int i = 0; i < nbUE; i++)
    {
      ue.at (i) = ueDevs.Get (i)->GetObject<UeNetDevice> ();
      lte.RegisterUeToTheEnb (ue.at (i), enb);
    }



  // CONFIGURE DL and UL SUB CHANNELS
  // Define a list of sub channels for the downlink
  std::vector<int> dlSubChannels;
  for (int i = 0; i < 25; i++)
    {
      dlSubChannels.push_back (i);
    }
  // Define a list of sub channels for the uplink
  std::vector<int> ulSubChannels;
  for (int i = 50; i < 100; i++)
    {
      ulSubChannels.push_back (i);
    }

  enb->GetPhy ()->SetDownlinkSubChannels (dlSubChannels);
  enb->GetPhy ()->SetUplinkSubChannels (ulSubChannels);

  for (int i = 0; i < nbUE; i++)
    {
      ue.at (i)->GetPhy ()->SetDownlinkSubChannels (dlSubChannels);
      ue.at (i)->GetPhy ()->SetUplinkSubChannels (ulSubChannels);
    }


  // CONFIGURE MOBILITY
  Ptr<ConstantPositionMobilityModel> enbMobility = CreateObject<ConstantPositionMobilityModel> ();
  enbMobility->SetPosition (Vector (0.0, 0.0, 0.0));
  lte.AddMobility (enb->GetPhy (), enbMobility);

  for (int i = 0; i < nbUE; i++)
    {
      Ptr<ConstantVelocityMobilityModel> ueMobility = CreateObject<ConstantVelocityMobilityModel> ();
      ueMobility->SetPosition (Vector (30.0, 0.0, 0.0));
      ueMobility->SetVelocity (Vector (30.0, 0.0, 0.0));

      lte.AddMobility (ue.at (i)->GetPhy (), ueMobility);

      lte.AddDownlinkChannelRealization (enbMobility, ueMobility, ue.at (i)->GetPhy ());
    }








  // CONGIFURE CLIENT SERVER APPLICATION
  UdpServerHelper udpServer;
  ApplicationContainer serverApp;
  UdpClientHelper udpClient;
  ApplicationContainer clientApp;

  udpServer = UdpServerHelper (100);
  serverApp = udpServer.Install (ueNodes.Get (0));
  serverApp.Start (Seconds (0.02));
  serverApp.Stop (Seconds (2));

  udpClient = UdpClientHelper (UEinterfaces.GetAddress (0), 100);
  udpClient.SetAttribute ("MaxPackets", UintegerValue (1200));
  udpClient.SetAttribute ("Interval", TimeValue (Seconds (0.12)));
  udpClient.SetAttribute ("PacketSize", UintegerValue (800));
  clientApp = udpClient.Install (enbNodes.Get (0));
  clientApp.Start (Seconds (0.01));
  clientApp.Stop (Seconds (2));


  //CREATE RADIO BEARER
  Ptr<RadioBearerInstance> bearer = CreateObject<RadioBearerInstance> ();
  bearer->SetBearerDirection (RadioBearerInstance::DIRECTION_TYPE_DL);
  bearer->SetBearerType (RadioBearerInstance::BEARER_TYPE_DRB);

  IpcsClassifierRecord *ipcs = new IpcsClassifierRecord (UEinterfaces.GetAddress (0),
                                                         "255.255.255.0",
                                                         ENBinterface.GetAddress (0),
                                                         "255.255.255.0",
                                                         100, 100, 0, 10000, 17, 1);
  bearer->SetIpcsClassifierRecord (ipcs);

  enb->GetRrcEntity ()->AddDownlinkNgbrBearer (bearer);

  bearer = 0;









  std::cout << "Starting simulation....." << std::endl;
  Simulator::Stop (Seconds (2.0));


  Simulator::Run ();

  Simulator::Destroy ();

  delete ipcs;

  std::cout << "Done." << std::endl;

  return 0;
}
