/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2013 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Jaume Nin <jnin@cttc.es>
 *         Nicola Baldo <nbaldo@cttc.es>
 *         Manuel Requena <manuel.requena@cttc.es>
 */

#include <ns3/emu-epc-helper.h>
#include <ns3/log.h>
#include <ns3/inet-socket-address.h>
#include <ns3/mac48-address.h>
#include <ns3/eps-bearer.h>
#include <ns3/ipv4-address.h>
#include <ns3/internet-stack-helper.h>
#include <ns3/packet-socket-helper.h>
#include <ns3/packet-socket-address.h>
#include <ns3/epc-enb-application.h>
#include <ns3/epc-sgw-pgw-application.h>
#include <ns3/emu-fd-net-device-helper.h>

#include <ns3/lte-enb-rrc.h>
#include <ns3/epc-x2.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/epc-mme.h>
#include <ns3/epc-ue-nas.h>
#include <ns3/string.h>
#include <ns3/abort.h>

#include <iomanip>
#include <iostream>

NS_LOG_COMPONENT_DEFINE ("EmuEpcHelper");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (EmuEpcHelper);


EmuEpcHelper::EmuEpcHelper () 
  : m_gtpuUdpPort (2152)  // fixed by the standard
{
  NS_LOG_FUNCTION (this);

}

EmuEpcHelper::~EmuEpcHelper ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
EmuEpcHelper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EmuEpcHelper")
    .SetParent<EpcHelper> ()
    .AddConstructor<EmuEpcHelper> ()
    .AddAttribute ("sgwDeviceName", 
                   "The name of the device used for the S1-U interface of the SGW",
                   StringValue ("veth0"),
                   MakeStringAccessor (&EmuEpcHelper::m_sgwDeviceName),
                   MakeStringChecker ())
    .AddAttribute ("enbDeviceName", 
                   "The name of the device used for the S1-U interface of the eNB",
                   StringValue ("veth1"),
                   MakeStringAccessor (&EmuEpcHelper::m_enbDeviceName),
                   MakeStringChecker ())
    .AddAttribute ("SgwMacAddress", 
                   "MAC address used for the SGW ",
                   StringValue ("00:00:00:59:00:aa"),
                   MakeStringAccessor (&EmuEpcHelper::m_sgwMacAddress),
                   MakeStringChecker ())
    .AddAttribute ("EnbMacAddressBase", 
                   "First 5 bytes of the Enb MAC address base",
                   StringValue ("00:00:00:eb:00"),
                   MakeStringAccessor (&EmuEpcHelper::m_enbMacAddressBase),
                   MakeStringChecker ())
    ;
  return tid;
}

void
EmuEpcHelper::DoInitialize ()
{
  NS_LOG_LOGIC (this);   


  // we use a /8 net for all UEs
  m_ueAddressHelper.SetBase ("7.0.0.0", "255.0.0.0");

  
 
  // create SgwPgwNode
  m_sgwPgw = CreateObject<Node> ();
  InternetStackHelper internet;
  internet.SetIpv4StackInstall (true);
  internet.Install (m_sgwPgw);
  
  // create S1-U socket
  Ptr<Socket> sgwPgwS1uSocket = Socket::CreateSocket (m_sgwPgw, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  int retval = sgwPgwS1uSocket->Bind (InetSocketAddress (Ipv4Address::GetAny (), m_gtpuUdpPort));
  NS_ASSERT (retval == 0);

  // create TUN device implementing tunneling of user data over GTP-U/UDP/IP 
  m_tunDevice = CreateObject<VirtualNetDevice> ();
  // allow jumbo packets
  m_tunDevice->SetAttribute ("Mtu", UintegerValue (30000));

  // yes we need this
  m_tunDevice->SetAddress (Mac48Address::Allocate ()); 

  m_sgwPgw->AddDevice (m_tunDevice);
  NetDeviceContainer tunDeviceContainer;
  tunDeviceContainer.Add (m_tunDevice);
  
  // the TUN device is on the same subnet as the UEs, so when a packet
  // addressed to an UE arrives at the intenet to the WAN interface of
  // the PGW it will be forwarded to the TUN device. 
  Ipv4InterfaceContainer tunDeviceIpv4IfContainer = m_ueAddressHelper.Assign (tunDeviceContainer);  

  // create EpcSgwPgwApplication
  m_sgwPgwApp = CreateObject<EpcSgwPgwApplication> (m_tunDevice, sgwPgwS1uSocket);
  m_sgwPgw->AddApplication (m_sgwPgwApp);
  
  // connect SgwPgwApplication and virtual net device for tunneling
  m_tunDevice->SetSendCallback (MakeCallback (&EpcSgwPgwApplication::RecvFromTunDevice, m_sgwPgwApp));

  // Create MME and connect with SGW via S11 interface
  m_mme = CreateObject<EpcMme> ();
  m_mme->SetS11SapSgw (m_sgwPgwApp->GetS11SapSgw ());
  m_sgwPgwApp->SetS11SapMme (m_mme->GetS11SapMme ());

  // Create EmuFdNetDevice for SGW
  EmuFdNetDeviceHelper emu;
  NS_LOG_LOGIC ("SGW device: " << m_sgwDeviceName);
  emu.SetDeviceName (m_sgwDeviceName);
  NetDeviceContainer sgwDevices = emu.Install (m_sgwPgw);
  Ptr<NetDevice> sgwDevice = sgwDevices.Get (0);
  NS_LOG_LOGIC ("MAC address of SGW: " << m_sgwMacAddress);
  sgwDevice->SetAttribute ("Address", Mac48AddressValue (m_sgwMacAddress.c_str ()));

  // we use a /8 subnet so the SGW and the eNBs can talk directly to each other
  m_epcIpv4AddressHelper.SetBase ("10.0.0.0", "255.255.255.0", "0.0.0.1");  
  m_sgwIpIfaces = m_epcIpv4AddressHelper.Assign (sgwDevices);
  m_epcIpv4AddressHelper.SetBase ("10.0.0.0", "255.0.0.0", "0.0.0.101");  
  
  
  EpcHelper::DoInitialize ();
}

void
EmuEpcHelper::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_tunDevice->SetSendCallback (MakeNullCallback<bool, Ptr<Packet>, const Address&, const Address&, uint16_t> ());
  m_tunDevice = 0;
  m_sgwPgwApp = 0;  
  m_sgwPgw->Dispose ();
}


void
EmuEpcHelper::AddEnb (Ptr<Node> enb, Ptr<NetDevice> lteEnbNetDevice, uint16_t cellId)
{
  NS_LOG_FUNCTION (this << enb << lteEnbNetDevice << cellId);

  Initialize ();

  NS_ASSERT (enb == lteEnbNetDevice->GetNode ());  

  // add an IPv4 stack to the previously created eNB
  InternetStackHelper internet;
  internet.Install (enb);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB after node creation: " << enb->GetObject<Ipv4> ()->GetNInterfaces ());



  // Create an EmuFdNetDevice for the eNB to connect with the SGW and other eNBs
  EmuFdNetDeviceHelper emu;
  NS_LOG_LOGIC ("eNB device: " << m_enbDeviceName);
  emu.SetDeviceName (m_enbDeviceName);  
  NetDeviceContainer enbDevices = emu.Install (enb);

  NS_ABORT_IF ((cellId == 0) || (cellId > 255));
  std::ostringstream enbMacAddress;
  enbMacAddress << m_enbMacAddressBase << ":" << std::hex << std::setfill ('0') << std::setw (2) << cellId;
  NS_LOG_LOGIC ("MAC address of enB with cellId " << cellId << " : " << enbMacAddress.str ());
  Ptr<NetDevice> enbDev = enbDevices.Get (0);
  enbDev->SetAttribute ("Address", Mac48AddressValue (enbMacAddress.str ().c_str ()));

  //emu.EnablePcap ("enbDevice", enbDev);

  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB after installing emu dev: " << enb->GetObject<Ipv4> ()->GetNInterfaces ());  
  Ipv4InterfaceContainer enbIpIfaces = m_epcIpv4AddressHelper.Assign (enbDevices);
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB after assigning Ipv4 addr to S1 dev: " << enb->GetObject<Ipv4> ()->GetNInterfaces ());
  
  Ipv4Address enbAddress = enbIpIfaces.GetAddress (0);
  Ipv4Address sgwAddress = m_sgwIpIfaces.GetAddress (0);

  // create S1-U socket for the ENB
  Ptr<Socket> enbS1uSocket = Socket::CreateSocket (enb, TypeId::LookupByName ("ns3::UdpSocketFactory"));
  int retval = enbS1uSocket->Bind (InetSocketAddress (enbAddress, m_gtpuUdpPort));
  NS_ASSERT (retval == 0);
    
  // create LTE socket for the ENB 
  Ptr<Socket> enbLteSocket = Socket::CreateSocket (enb, TypeId::LookupByName ("ns3::PacketSocketFactory"));
  PacketSocketAddress enbLteSocketBindAddress;
  enbLteSocketBindAddress.SetSingleDevice (lteEnbNetDevice->GetIfIndex ());
  enbLteSocketBindAddress.SetProtocol (Ipv4L3Protocol::PROT_NUMBER);
  retval = enbLteSocket->Bind (enbLteSocketBindAddress);
  NS_ASSERT (retval == 0);  
  PacketSocketAddress enbLteSocketConnectAddress;
  enbLteSocketConnectAddress.SetPhysicalAddress (Mac48Address::GetBroadcast ());
  enbLteSocketConnectAddress.SetSingleDevice (lteEnbNetDevice->GetIfIndex ());
  enbLteSocketConnectAddress.SetProtocol (Ipv4L3Protocol::PROT_NUMBER);
  retval = enbLteSocket->Connect (enbLteSocketConnectAddress);
  NS_ASSERT (retval == 0);  
  

  NS_LOG_INFO ("create EpcEnbApplication");
  Ptr<EpcEnbApplication> enbApp = CreateObject<EpcEnbApplication> (enbLteSocket, enbS1uSocket, enbAddress, sgwAddress, cellId);
  enb->AddApplication (enbApp);
  NS_ASSERT (enb->GetNApplications () == 1);
  NS_ASSERT_MSG (enb->GetApplication (0)->GetObject<EpcEnbApplication> () != 0, "cannot retrieve EpcEnbApplication");
  NS_LOG_LOGIC ("enb: " << enb << ", enb->GetApplication (0): " << enb->GetApplication (0));

  
  NS_LOG_INFO ("Create EpcX2 entity");
  Ptr<EpcX2> x2 = CreateObject<EpcX2> ();
  enb->AggregateObject (x2);

  NS_LOG_INFO ("connect S1-AP interface");
  m_mme->AddEnb (cellId, enbAddress, enbApp->GetS1apSapEnb ());
  m_sgwPgwApp->AddEnb (cellId, enbAddress, sgwAddress);
  enbApp->SetS1apSapMme (m_mme->GetS1apSapMme ());
}


void
EmuEpcHelper::AddX2Interface (Ptr<Node> enb1, Ptr<Node> enb2)
{
  NS_LOG_FUNCTION (this << enb1 << enb2);

  NS_LOG_WARN ("X2 support still untested");


  // for X2, we reuse the same device and IP address of the S1-U interface
  Ptr<Ipv4> enb1Ipv4 = enb1->GetObject<Ipv4> ();
  Ptr<Ipv4> enb2Ipv4 = enb2->GetObject<Ipv4> ();
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #1: " << enb1Ipv4->GetNInterfaces ());
  NS_LOG_LOGIC ("number of Ipv4 ifaces of the eNB #2: " << enb2Ipv4->GetNInterfaces ());
  NS_LOG_LOGIC ("number of NetDevices of the eNB #1: " << enb1->GetNDevices ());
  NS_LOG_LOGIC ("number of NetDevices of the eNB #2: " << enb2->GetNDevices ());

  // 0 is the LTE device, 1 is localhost, 2 is the EPC NetDevice
  Ptr<NetDevice> enb1EpcDev = enb1->GetDevice (2);
  Ptr<NetDevice> enb2EpcDev = enb2->GetDevice (2);

  int32_t enb1Interface =  enb1Ipv4->GetInterfaceForDevice (enb1EpcDev);
  int32_t enb2Interface =  enb2Ipv4->GetInterfaceForDevice (enb2EpcDev);
  NS_ASSERT (enb1Interface >= 0);
  NS_ASSERT (enb2Interface >= 0);
  NS_ASSERT (enb1Ipv4->GetNAddresses (enb1Interface) == 1);
  NS_ASSERT (enb2Ipv4->GetNAddresses (enb2Interface) == 1);
  Ipv4Address enb1Addr = enb1Ipv4->GetAddress (enb1Interface, 0).GetLocal (); 
  Ipv4Address enb2Addr = enb2Ipv4->GetAddress (enb2Interface, 0).GetLocal (); 
  NS_LOG_LOGIC (" eNB 1 IP address: " << enb1Addr); 
  NS_LOG_LOGIC (" eNB 2 IP address: " << enb2Addr);
  
  // Add X2 interface to both eNBs' X2 entities
  Ptr<EpcX2> enb1X2 = enb1->GetObject<EpcX2> ();
  Ptr<LteEnbNetDevice> enb1LteDev = enb1->GetDevice (0)->GetObject<LteEnbNetDevice> ();
  uint16_t enb1CellId = enb1LteDev->GetCellId ();
  NS_LOG_LOGIC ("LteEnbNetDevice #1 = " << enb1LteDev << " - CellId = " << enb1CellId);

  Ptr<EpcX2> enb2X2 = enb2->GetObject<EpcX2> ();
  Ptr<LteEnbNetDevice> enb2LteDev = enb2->GetDevice (0)->GetObject<LteEnbNetDevice> ();
  uint16_t enb2CellId = enb2LteDev->GetCellId ();
  NS_LOG_LOGIC ("LteEnbNetDevice #2 = " << enb2LteDev << " - CellId = " << enb2CellId);

  enb1X2->AddX2Interface (enb1CellId, enb1Addr, enb2CellId, enb2Addr);
  enb2X2->AddX2Interface (enb2CellId, enb2Addr, enb1CellId, enb1Addr);

  enb1LteDev->GetRrc ()->AddX2Neighbour (enb2LteDev->GetCellId ());
  enb2LteDev->GetRrc ()->AddX2Neighbour (enb1LteDev->GetCellId ());
}


void 
EmuEpcHelper::AddUe (Ptr<NetDevice> ueDevice, uint64_t imsi)
{
  NS_LOG_FUNCTION (this << imsi << ueDevice );
  
  m_mme->AddUe (imsi);
  m_sgwPgwApp->AddUe (imsi);
  
}

void
EmuEpcHelper::ActivateEpsBearer (Ptr<NetDevice> ueDevice, uint64_t imsi, Ptr<EpcTft> tft, EpsBearer bearer)
{
  NS_LOG_FUNCTION (this << ueDevice << imsi);

  // we now retrieve the IPv4 address of the UE and notify it to the SGW;
  // we couldn't do it before since address assignment is triggered by
  // the user simulation program, rather than done by the EPC   
  Ptr<Node> ueNode = ueDevice->GetNode (); 
  Ptr<Ipv4> ueIpv4 = ueNode->GetObject<Ipv4> ();
  NS_ASSERT_MSG (ueIpv4 != 0, "UEs need to have IPv4 installed before EPS bearers can be activated");
  int32_t interface =  ueIpv4->GetInterfaceForDevice (ueDevice);
  NS_ASSERT (interface >= 0);
  NS_ASSERT (ueIpv4->GetNAddresses (interface) == 1);
  Ipv4Address ueAddr = ueIpv4->GetAddress (interface, 0).GetLocal ();
  NS_LOG_LOGIC (" UE IP address: " << ueAddr);  m_sgwPgwApp->SetUeAddress (imsi, ueAddr);
  
  m_mme->AddBearer (imsi, tft, bearer);
  Ptr<LteUeNetDevice> ueLteDevice = ueDevice->GetObject<LteUeNetDevice> ();
  if (ueLteDevice)
    {
      Simulator::ScheduleNow (&EpcUeNas::ActivateEpsBearer, ueLteDevice->GetNas (), bearer, tft);
    }
}


Ptr<Node>
EmuEpcHelper::GetPgwNode ()
{
  return m_sgwPgw;
}


Ipv4InterfaceContainer 
EmuEpcHelper::AssignUeIpv4Address (NetDeviceContainer ueDevices)
{
  return m_ueAddressHelper.Assign (ueDevices);
}



Ipv4Address
EmuEpcHelper::GetUeDefaultGatewayAddress ()
{
  // return the address of the tun device
  return m_sgwPgw->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ();
}


} // namespace ns3
