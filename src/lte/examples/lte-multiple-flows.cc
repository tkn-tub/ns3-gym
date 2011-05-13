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

  Ptr<UeNetDevice> ue[nbUE];
  for (int i = 0; i < nbUE; i++)
    {
      ue[i] = ueDevs.Get (i)->GetObject<UeNetDevice> ();
      lte.RegisterUeToTheEnb (ue[i], enb);
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
      ue[i]->GetPhy ()->SetDownlinkSubChannels (dlSubChannels);
      ue[i]->GetPhy ()->SetUplinkSubChannels (ulSubChannels);
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

      lte.AddMobility (ue[i]->GetPhy (), ueMobility);

      lte.AddDownlinkChannelRealization (enbMobility, ueMobility, ue[i]->GetPhy ());
    }




  //******* FLOW N° 1 *********
  UdpServerHelper udpServer_1;
  ApplicationContainer serverApp_1;
  UdpClientHelper udpClient_1;
  ApplicationContainer clientApp_1;

  udpServer_1 = UdpServerHelper (100);
  serverApp_1 = udpServer_1.Install (ueNodes.Get (0));
  serverApp_1.Start (Seconds (0.02));
  serverApp_1.Stop (Seconds (2));

  udpClient_1 = UdpClientHelper (UEinterfaces.GetAddress (0), 100);
  udpClient_1.SetAttribute ("MaxPackets", UintegerValue (1200));
  udpClient_1.SetAttribute ("Interval", TimeValue (Seconds (0.12)));
  udpClient_1.SetAttribute ("PacketSize", UintegerValue (800));
  clientApp_1 = udpClient_1.Install (enbNodes.Get (0));
  clientApp_1.Start (Seconds (0.01));
  clientApp_1.Stop (Seconds (2));

  Ptr<RadioBearerInstance> bearer_1 = CreateObject<RadioBearerInstance> ();
  bearer_1->SetBearerDirection (RadioBearerInstance::DIRECTION_TYPE_DL);
  bearer_1->SetBearerType (RadioBearerInstance::BEARER_TYPE_DRB);
  IpcsClassifierRecord *ipcs_1 = new IpcsClassifierRecord (UEinterfaces.GetAddress (0),
                                                           "255.255.255.0",
                                                           ENBinterface.GetAddress (0),
                                                           "255.255.255.0",
                                                           100, 100, 0, 10000, 17, 1);
  bearer_1->SetIpcsClassifierRecord (ipcs_1);
  enb->GetRrcEntity ()->AddDownlinkNgbrBearer (bearer_1);





  //******* FLOW N° 2 *********
  UdpServerHelper udpServer_2;
  ApplicationContainer serverApp_2;
  UdpClientHelper udpClient_2;
  ApplicationContainer clientApp_2;

  udpServer_2 = UdpServerHelper (100);
  serverApp_2 = udpServer_2.Install (ueNodes.Get (1));
  serverApp_2.Start (Seconds (0.02));
  serverApp_2.Stop (Seconds (2));

  udpClient_2 = UdpClientHelper (UEinterfaces.GetAddress (1), 100);
  udpClient_2.SetAttribute ("MaxPackets", UintegerValue (1200));
  udpClient_2.SetAttribute ("Interval", TimeValue (Seconds (0.12)));
  udpClient_2.SetAttribute ("PacketSize", UintegerValue (800));
  clientApp_2 = udpClient_2.Install (enbNodes.Get (0));
  clientApp_2.Start (Seconds (0.011));
  clientApp_2.Stop (Seconds (2));

  Ptr<RadioBearerInstance> bearer_2 = CreateObject<RadioBearerInstance> ();
  bearer_2->SetBearerDirection (RadioBearerInstance::DIRECTION_TYPE_DL);
  bearer_2->SetBearerType (RadioBearerInstance::BEARER_TYPE_DRB);
  IpcsClassifierRecord *ipcs_2 = new IpcsClassifierRecord (UEinterfaces.GetAddress (1),
                                                           "255.255.255.0",
                                                           ENBinterface.GetAddress (0),
                                                           "255.255.255.0",
                                                           100, 100, 0, 10000, 17, 1);
  bearer_2->SetIpcsClassifierRecord (ipcs_2);
  enb->GetRrcEntity ()->AddDownlinkNgbrBearer (bearer_2);




  //******* FLOW N° 3 *********
  UdpServerHelper udpServer_3;
  ApplicationContainer serverApp_3;
  UdpClientHelper udpClient_3;
  ApplicationContainer clientApp_3;

  udpServer_3 = UdpServerHelper (100);
  serverApp_3 = udpServer_3.Install (ueNodes.Get (2));
  serverApp_3.Start (Seconds (0.02));
  serverApp_3.Stop (Seconds (2));

  udpClient_3 = UdpClientHelper (UEinterfaces.GetAddress (2), 100);
  udpClient_3.SetAttribute ("MaxPackets", UintegerValue (1200));
  udpClient_3.SetAttribute ("Interval", TimeValue (Seconds (0.12)));
  udpClient_3.SetAttribute ("PacketSize", UintegerValue (800));
  clientApp_3 = udpClient_2.Install (enbNodes.Get (0));
  clientApp_3.Start (Seconds (0.011));
  clientApp_3.Stop (Seconds (2));

  Ptr<RadioBearerInstance> bearer_3 = CreateObject<RadioBearerInstance> ();
  bearer_3->SetBearerDirection (RadioBearerInstance::DIRECTION_TYPE_DL);
  bearer_3->SetBearerType (RadioBearerInstance::BEARER_TYPE_DRB);
  IpcsClassifierRecord *ipcs_3 = new IpcsClassifierRecord (UEinterfaces.GetAddress (2),
                                                           "255.255.255.0",
                                                           ENBinterface.GetAddress (0),
                                                           "255.255.255.0",
                                                           100, 100, 0, 10000, 17, 1);
  bearer_3->SetIpcsClassifierRecord (ipcs_3);
  enb->GetRrcEntity ()->AddDownlinkNgbrBearer (bearer_3);









  std::cout << "Starting simulation....." << std::endl;
  Simulator::Stop (Seconds (2.0));


  Simulator::Run ();

  Simulator::Destroy ();

  delete ipcs_1;
  delete ipcs_2;
  delete ipcs_3;

  std::cout << "Done." << std::endl;

  return 0;
}
