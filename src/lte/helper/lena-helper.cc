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
 * Author: Nicola Baldo <nbaldo@cttc.es> (re-wrote from scratch this helper)
 *         Giuseppe Piro <g.piro@poliba.it> (parts of the PHY & channel  creation & configuration copied from the GSoC 2011 code)
 */


#include "lena-helper.h"

#include <ns3/string.h>
#include <ns3/log.h>

#include <ns3/lte-enb-rrc.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-ue-mac.h>
#include <ns3/lte-enb-mac.h>
#include <ns3/lte-enb-net-device.h>

#include <ns3/lte-enb-phy.h>
#include <ns3/lte-ue-phy.h>
#include <ns3/lte-spectrum-phy.h>
#include <ns3/lte-sinr-chunk-processor.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/friis-spectrum-propagation-loss.h>

#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-net-device.h>

#include <ns3/ff-mac-scheduler.h>

#include <iostream>

#include <ns3/buildings-propagation-loss-model.h>

#include <ns3/lte-spectrum-value-helper.h>


NS_LOG_COMPONENT_DEFINE ("LenaHelper");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LenaHelper);

LenaHelper::LenaHelper (void)
{
  NS_LOG_FUNCTION (this);
}

void 
LenaHelper::DoStart (void)
{
  NS_LOG_FUNCTION (this);
  m_downlinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  m_uplinkChannel = CreateObject<SingleModelSpectrumChannel> ();
  if ((m_dlPropagationModelFactory.GetTypeId ().GetName ().compare ( "ns3::FriisSpectrumPropagationLossModel") == 0) || (m_dlPropagationModelFactory.GetTypeId ().GetName ().compare ( "ns3::ConstantSpectrumPropagationLossModel") == 0) )
    {
      Ptr<SpectrumPropagationLossModel> dlPropagationModel = m_dlPropagationModelFactory.Create<SpectrumPropagationLossModel> ();
      Ptr<SpectrumPropagationLossModel> ulPropagationModel = m_ulPropagationModelFactory.Create<SpectrumPropagationLossModel> ();
      m_downlinkChannel->AddSpectrumPropagationLossModel (dlPropagationModel);
      m_uplinkChannel->AddSpectrumPropagationLossModel (ulPropagationModel);
    }
  else if (m_dlPropagationModelFactory.GetTypeId ().GetName ().compare ( "ns3::BuildingsPropagationLossModel") == 0)
    {
      //m_downlinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
      m_downlinkPropagationLossModel = m_dlPropagationModelFactory.Create<PropagationLossModel> ();
    //   m_downlinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (2.1140e9)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
    //   m_downlinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /2.1140e9)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
      //m_uplinkPropagationLossModel = CreateObject<BuildingsPropagationLossModel> ();
      m_uplinkPropagationLossModel = m_ulPropagationModelFactory.Create<PropagationLossModel> ();
      
    //   m_uplinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (1.950e9)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
    //   m_uplinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /1.950e9)); // E_UTRA BAND #1 see table 5.5-1 of 36.101
      m_downlinkChannel->AddPropagationLossModel (m_downlinkPropagationLossModel);
      m_uplinkChannel->AddPropagationLossModel (m_uplinkPropagationLossModel);
    }
  else
    {
      NS_LOG_ERROR ("Unknown propagation model");
    }
  if (m_fadingModelFactory.GetTypeId ().GetName ().compare ( "ns3::TraceFadingLossModel") == 0)
    {
      m_fadingModule = CreateObject<TraceFadingLossModel> ();
      m_downlinkChannel->AddSpectrumPropagationLossModel (m_fadingModule);
      m_uplinkChannel->AddSpectrumPropagationLossModel (m_fadingModule);
    }
  m_macStats = CreateObject<MacStatsCalculator> ();
  m_rlcStats = CreateObject<RlcStatsCalculator> ();
  Object::DoStart ();
}

LenaHelper::~LenaHelper (void)
{
  NS_LOG_FUNCTION (this);
}


void
LenaHelper::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_downlinkChannel = 0;
  m_uplinkChannel = 0;
  Object::DoDispose ();
}

TypeId LenaHelper::GetTypeId (void)
{
  static TypeId
  tid =
    TypeId ("ns3::LenaHelper")
    .SetParent<Object> ()
    .AddConstructor<LenaHelper> ()
    .AddAttribute ("Scheduler",
                   "The type of scheduler to be used for eNBs",
                   StringValue ("ns3::PfFfMacScheduler"),
                   MakeStringAccessor (&LenaHelper::SetSchedulerType),
                   MakeStringChecker ())
    .AddAttribute ("PropagationModel",
                   "The type of propagation model to be used",
                   StringValue ("ns3::BuildingsPropagationLossModel"),
                   MakeStringAccessor (&LenaHelper::SetPropagationModelType),
                   MakeStringChecker ())
     .AddAttribute ("FadingModel",
                   "The type of fading model to be used",
                   StringValue ("ns3::FriisSpectrumPropagationLossModel"), // fake module -> no fading 
                   MakeStringAccessor (&LenaHelper::SetFadingModel),
                   MakeStringChecker ())
  ;
  return tid;
}

void 
LenaHelper::SetSchedulerType (std::string type) 
{
  NS_LOG_FUNCTION (this << type);
  m_schedulerFactory = ObjectFactory ();
  m_schedulerFactory.SetTypeId (type);
}

void 
LenaHelper::SetSchedulerAttribute (std::string n, const AttributeValue &v)
{
  NS_LOG_FUNCTION (this << n);
  m_schedulerFactory.Set (n, v);
}


void 
LenaHelper::SetPropagationModelType (std::string type) 
{
  NS_LOG_FUNCTION (this << type);
  m_dlPropagationModelFactory = ObjectFactory ();
  m_dlPropagationModelFactory.SetTypeId (type);
  m_ulPropagationModelFactory = ObjectFactory ();
  m_ulPropagationModelFactory.SetTypeId (type);
}

void 
LenaHelper::SetPropagationModelAttribute (std::string n, const AttributeValue &v)
{
  NS_LOG_FUNCTION (this << n);
  m_dlPropagationModelFactory.Set (n, v);
  m_ulPropagationModelFactory.Set (n, v);
}


void 
LenaHelper::SetFadingModel (std::string type) 
{
  NS_LOG_FUNCTION (this << type);
  m_fadingModelFactory = ObjectFactory ();
  m_fadingModelFactory.SetTypeId (type);
}

void 
LenaHelper::SetFadingModelAttribute (std::string n, const AttributeValue &v)
{
  NS_LOG_FUNCTION (this << n);
  m_fadingModelFactory.Set (n, v);
}


NetDeviceContainer
LenaHelper::InstallEnbDevice (NodeContainer c)
{
  NS_LOG_FUNCTION (this);
  Start ();  // will run DoStart () if necessary
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<NetDevice> device = InstallSingleEnbDevice (node);
      devices.Add (device);
    }
  return devices;
}

NetDeviceContainer
LenaHelper::InstallUeDevice (NodeContainer c)
{
  NS_LOG_FUNCTION (this);
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<NetDevice> device = InstallSingleUeDevice (node);
      devices.Add (device);
    }
  return devices;
}


void
LenaHelper::SetEnbDeviceAttribute (std::string name, const AttributeValue &value)
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("not implemented yet");
}

void
LenaHelper::SetUeDeviceAttribute (std::string name, const AttributeValue &value)
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("not implemented yet");
}


Ptr<NetDevice>
LenaHelper::InstallSingleEnbDevice (Ptr<Node> n)
{
  Ptr<LteSpectrumPhy> dlPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteSpectrumPhy> ulPhy = CreateObject<LteSpectrumPhy> ();

  Ptr<LteEnbPhy> phy = CreateObject<LteEnbPhy> (dlPhy, ulPhy);

  Ptr<LteCqiSinrChunkProcessor> p = Create<LteCqiSinrChunkProcessor> (phy->GetObject<LtePhy> ());
  ulPhy->AddSinrChunkProcessor (p);

  dlPhy->SetChannel (m_downlinkChannel);
  ulPhy->SetChannel (m_uplinkChannel);

  Ptr<MobilityModel> mm = n->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mm, "MobilityModel needs to be set on node before calling LenaHelper::InstallUeDevice ()");
  dlPhy->SetMobility (mm);
  ulPhy->SetMobility (mm);
  m_uplinkChannel->AddRx (ulPhy);

  Ptr<LteEnbMac> mac = CreateObject<LteEnbMac> ();
  Ptr<FfMacScheduler> sched = m_schedulerFactory.Create<FfMacScheduler> ();
  Ptr<LteEnbRrc> rrc = CreateObject<LteEnbRrc> ();


  // connect SAPs
  rrc->SetLteEnbCmacSapProvider (mac->GetLteEnbCmacSapProvider ());
  mac->SetLteEnbCmacSapUser (rrc->GetLteEnbCmacSapUser ());
  rrc->SetLteMacSapProvider (mac->GetLteMacSapProvider ());

  mac->SetFfMacSchedSapProvider (sched->GetFfMacSchedSapProvider ());
  mac->SetFfMacCschedSapProvider (sched->GetFfMacCschedSapProvider ());

  sched->SetFfMacSchedSapUser (mac->GetFfMacSchedSapUser ());
  sched->SetFfMacCschedSapUser (mac->GetFfMacCschedSapUser ());

  phy->SetLteEnbPhySapUser (mac->GetLteEnbPhySapUser ());
  mac->SetLteEnbPhySapProvider (phy->GetLteEnbPhySapProvider ());

  Ptr<LteEnbNetDevice> dev = CreateObject<LteEnbNetDevice> (n, phy, mac, sched, rrc);
  phy->SetDevice (dev);
  dlPhy->SetDevice (dev);
  ulPhy->SetDevice (dev);

  n->AddDevice (dev);
  ulPhy->SetGenericPhyRxEndOkCallback (MakeCallback (&LteEnbPhy::PhyPduReceived, phy));
  // set the propagation model frequencies
  if (m_dlPropagationModelFactory.GetTypeId ().GetName ().compare ( "ns3::BuildingsPropagationLossModel") == 0)
    {
      double dlFreq = LteSpectrumValueHelper::GetCarrierFrequency (dev->GetDlEarfcn ());
      m_downlinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (dlFreq));
      m_downlinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /dlFreq));
    }
  if (m_ulPropagationModelFactory.GetTypeId ().GetName ().compare ( "ns3::BuildingsPropagationLossModel") == 0)
    {
      double ulFreq = LteSpectrumValueHelper::GetCarrierFrequency (dev->GetUlEarfcn ());
      m_uplinkPropagationLossModel->SetAttribute ("Frequency", DoubleValue (ulFreq));
      m_uplinkPropagationLossModel->SetAttribute ("Lambda", DoubleValue (300000000.0 /ulFreq));
    }
  
  dev->Start ();
  return dev;
}

Ptr<NetDevice>
LenaHelper::InstallSingleUeDevice (Ptr<Node> n)
{
  Ptr<LteSpectrumPhy> dlPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteSpectrumPhy> ulPhy = CreateObject<LteSpectrumPhy> ();

  Ptr<LteUePhy> phy = CreateObject<LteUePhy> (dlPhy, ulPhy);

  Ptr<LteCqiSinrChunkProcessor> p = Create<LteCqiSinrChunkProcessor> (phy->GetObject<LtePhy> ());
  dlPhy->AddSinrChunkProcessor (p);

  dlPhy->SetChannel (m_downlinkChannel);
  ulPhy->SetChannel (m_uplinkChannel);

  Ptr<MobilityModel> mm = n->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mm, "MobilityModel needs to be set on node before calling LenaHelper::InstallUeDevice ()");
  dlPhy->SetMobility (mm);
  ulPhy->SetMobility (mm);

  m_downlinkChannel->AddRx (dlPhy);

  Ptr<LteUeMac> mac = CreateObject<LteUeMac> ();
  Ptr<LteUeRrc> rrc = CreateObject<LteUeRrc> ();

  // connect SAPs
  rrc->SetLteUeCmacSapProvider (mac->GetLteUeCmacSapProvider ());
  mac->SetLteUeCmacSapUser (rrc->GetLteUeCmacSapUser ());
  rrc->SetLteMacSapProvider (mac->GetLteMacSapProvider ());
  phy->SetLteUePhySapUser (mac->GetLteUePhySapUser ());
  mac->SetLteUePhySapProvider (phy->GetLteUePhySapProvider ());

  Ptr<LteUeNetDevice> dev = CreateObject<LteUeNetDevice> (n, phy, mac, rrc);
  phy->SetDevice (dev);
  dlPhy->SetDevice (dev);
  ulPhy->SetDevice (dev);

  n->AddDevice (dev);
  dlPhy->SetGenericPhyRxEndOkCallback (MakeCallback (&LteUePhy::PhyPduReceived, phy));

  return dev;
}


void
LenaHelper::Attach (NetDeviceContainer ueDevices, Ptr<NetDevice> enbDevice)
{
  for (NetDeviceContainer::Iterator i = ueDevices.Begin (); i != ueDevices.End (); ++i)
    {
      Attach (*i, enbDevice);
    }
}

void
LenaHelper::Attach (Ptr<NetDevice> ueDevice, Ptr<NetDevice> enbDevice)
{
  // setup RRC connection
  Ptr<LteEnbRrc> enbRrc = enbDevice->GetObject<LteEnbNetDevice> ()->GetRrc ();
  uint16_t rnti = enbRrc->AddUe (ueDevice->GetObject<LteUeNetDevice> ()->GetImsi ());
  Ptr<LteUeRrc> ueRrc = ueDevice->GetObject<LteUeNetDevice> ()->GetRrc ();
  ueRrc->ConfigureUe (rnti, enbDevice->GetObject<LteEnbNetDevice> ()->GetCellId () );

  // attach UE to eNB
  ueDevice->GetObject<LteUeNetDevice> ()->SetTargetEnb (enbDevice->GetObject<LteEnbNetDevice> ());


  // connect at the PHY layer
  Ptr<LteEnbPhy> enbPhy = enbDevice->GetObject<LteEnbNetDevice> ()->GetPhy ();
  Ptr<LteUePhy> uePhy = ueDevice->GetObject<LteUeNetDevice> ()->GetPhy ();
  enbPhy->AddUePhy (rnti, uePhy);
  
  if (m_fadingModelFactory.GetTypeId ().GetName ().compare ( "ns3::TraceFadingLossModel") == 0)
    {
       Ptr<MobilityModel> mm_enb = enbDevice->GetObject<MobilityModel> ();
       Ptr<MobilityModel> mm_ue = ueDevice->GetObject<MobilityModel> ();
       m_fadingModule->CreateFadingChannelRealization (mm_enb, mm_ue); //downlink
       m_fadingModule->CreateFadingChannelRealization (mm_ue, mm_enb); //uplink
       
    }
 
  // WILD HACK - should be done through PHY SAP, probably passing by RRC
  uePhy->SetRnti (rnti);
  uePhy->DoSetBandwidth (enbDevice->GetObject<LteEnbNetDevice> ()->GetUlBandwidth (),
                         enbDevice->GetObject<LteEnbNetDevice> ()->GetDlBandwidth ());
  uePhy->DoSetEarfcn (enbDevice->GetObject<LteEnbNetDevice> ()->GetDlEarfcn (),
                      enbDevice->GetObject<LteEnbNetDevice> ()->GetUlEarfcn ());

  ueDevice->Start ();
}


void
LenaHelper::ActivateEpsBearer (NetDeviceContainer ueDevices, EpsBearer bearer)
{
  for (NetDeviceContainer::Iterator i = ueDevices.Begin (); i != ueDevices.End (); ++i)
    {
      ActivateEpsBearer (*i, bearer);
    }
}


void
LenaHelper::ActivateEpsBearer (Ptr<NetDevice> ueDevice, EpsBearer bearer)
{
  Ptr<LteEnbNetDevice> enbDevice = ueDevice->GetObject<LteUeNetDevice> ()->GetTargetEnb ();
  Ptr<LteEnbRrc> enbRrc = enbDevice->GetObject<LteEnbNetDevice> ()->GetRrc ();
  Ptr<LteUeRrc> ueRrc = ueDevice->GetObject<LteUeNetDevice> ()->GetRrc ();
  uint16_t rnti = ueRrc->GetRnti ();
  uint8_t lcid = enbRrc->SetupRadioBearer (rnti, bearer);
  ueRrc->SetupRadioBearer (rnti, bearer, lcid);
}


void
LenaHelper::EnableLogComponents (void)
{
  LogComponentEnable ("LteEnbRrc", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeRrc", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LteRlc", LOG_LEVEL_ALL);
  LogComponentEnable ("RrFfMacScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("PfFfMacScheduler", LOG_LEVEL_ALL);

  LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteSpectrumValueHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteInterference", LOG_LEVEL_ALL);
  LogComponentEnable ("LteSinrChunkProcessor", LOG_LEVEL_ALL);

  LogComponentEnable ("LtePropagationLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("LossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("ShadowingLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("PenetrationLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("MultipathLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("PathLossModel", LOG_LEVEL_ALL);

  LogComponentEnable ("LteNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbNetDevice", LOG_LEVEL_ALL);

  LogComponentEnable ("RlcStatsCalculator", LOG_LEVEL_ALL);
}



void
LenaHelper::EnableRlcTraces (void)
{
  EnableDlRlcTraces ();
  EnableUlRlcTraces ();

}




uint64_t
FindImsiFromEnbRlcPath (std::string path)
{
  // Sample path input:
  // /NodeList/#NodeId/DeviceList/#DeviceId/LteEnbRrc/UeMap/#C-RNTI/RadioBearerMap/#LCID/LteRlc/RxPDU

  // We retrieve the UeInfo associated to the C-RNTI and perform the IMSI lookup
  std::string ueMapPath = path.substr (0, path.find ("/RadioBearerMap"));
  Config::MatchContainer match = Config::LookupMatches (ueMapPath);

  if (match.GetN () != 0)
    {
      Ptr<Object> ueInfo = match.Get (0);
      return ueInfo->GetObject<UeInfo> ()->GetImsi ();
    }
  else
    {
      NS_FATAL_ERROR ("Lookup " << ueMapPath << " got no matches");
    }
}

uint16_t
FindCellIdFromEnbRlcPath (std::string path)
{
  // Sample path input:
  // /NodeList/#NodeId/DeviceList/#DeviceId/LteEnbRrc/UeMap/#C-RNTI/RadioBearerMap/#LCID/LteRlc/RxPDU

  // We retrieve the CellId associated to the Enb
  std::string enbNetDevicePath = path.substr (0, path.find ("/LteEnbRrc"));
  Config::MatchContainer match = Config::LookupMatches (enbNetDevicePath);

  if (match.GetN () != 0)
    {
      Ptr<Object> enbNetDevice = match.Get (0);
      return enbNetDevice->GetObject<LteEnbNetDevice> ()->GetCellId ();
    }
  else
    {
      NS_FATAL_ERROR ("Lookup " << enbNetDevicePath << " got no matches");
    }
}

uint64_t
FindImsiFromUeRlcPath (std::string path)
{
  // Sample path input:
  // /NodeList/#NodeId/DeviceList/#DeviceId/LteUeRrc/RlcMap/#LCID/RxPDU

  // We retrieve the LteUeNetDevice path
  std::string lteUeNetDevicePath = path.substr (0, path.find ("/LteUeRrc"));
  Config::MatchContainer match = Config::LookupMatches (lteUeNetDevicePath);

  if (match.GetN () != 0)
    {
      Ptr<Object> ueNetDevice = match.Get (0);
      return ueNetDevice->GetObject<LteUeNetDevice> ()->GetImsi ();
    }
  else
    {
      NS_FATAL_ERROR ("Lookup " << lteUeNetDevicePath << " got no matches");
    }

}

uint64_t
FindImsiFromEnbMac (std::string path, uint16_t rnti)
{
  // /NodeList/#NodeId/DeviceList/#DeviceId/LteEnbMac/DlScheduling
  std::ostringstream oss;
  std::string p = path.substr (0, path.find ("/LteEnbMac"));
  oss << rnti;
  p += "\\/LteEnbRrc/UeMap/" + oss.str ();
  return FindImsiFromEnbRlcPath (p);
}

uint16_t
FindCellIdFromEnbMac (std::string path, uint16_t rnti)
{
  // /NodeList/#NodeId/DeviceList/#DeviceId/LteEnbMac/DlScheduling
  std::ostringstream oss;
  std::string p = path.substr (0, path.find ("/LteEnbMac"));
  oss << rnti;
  p += "\\/LteEnbRrc/UeMap/" + oss.str ();
  return FindCellIdFromEnbRlcPath (p);
}


void
DlTxPduCallback (Ptr<RlcStatsCalculator> m_rlcStats, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  uint64_t imsi = FindImsiFromEnbRlcPath (path);
  uint16_t cellId = FindCellIdFromEnbRlcPath (path);
  m_rlcStats->DlTxPdu (cellId, imsi, rnti, lcid, packetSize);
}

void
DlRxPduCallback (Ptr<RlcStatsCalculator> m_rlcStats, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  uint64_t imsi = FindImsiFromUeRlcPath (path);
  m_rlcStats->DlRxPdu (imsi, rnti, lcid, packetSize, delay);
}

void
LenaHelper::EnableDlRlcTraces (void)
{
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/UeMap/*/RadioBearerMap/*/LteRlc/TxPDU",
                   MakeBoundCallback (&DlTxPduCallback, m_rlcStats));
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/RlcMap/*/RxPDU",
                   MakeBoundCallback (&DlRxPduCallback, m_rlcStats));
}

void
UlTxPduCallback (Ptr<RlcStatsCalculator> m_rlcStats, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  uint64_t imsi = FindImsiFromUeRlcPath (path);
  m_rlcStats->UlTxPdu (imsi, rnti, lcid, packetSize);
}

void
UlRxPduCallback (Ptr<RlcStatsCalculator> m_rlcStats, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  uint64_t imsi = FindImsiFromEnbRlcPath (path);
  uint16_t cellId = FindCellIdFromEnbRlcPath (path);
  m_rlcStats->UlRxPdu (cellId, imsi, rnti, lcid, packetSize, delay);
}

void
DlSchedulingCallback (Ptr<MacStatsCalculator> mac, std::string path,
                      uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                      uint8_t mcsTb1, uint16_t sizeTb1, uint8_t mcsTb2, uint16_t sizeTb2)
{
  uint64_t imsi = FindImsiFromEnbMac (path, rnti);
  uint16_t cellId = FindCellIdFromEnbMac (path, rnti);
  mac->DlScheduling (cellId, imsi, frameNo, subframeNo, rnti, mcsTb1, sizeTb1, mcsTb2, sizeTb2);
}

void
LenaHelper::EnableUlRlcTraces (void)
{
  Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/RlcMap/*/TxPDU",
                   MakeBoundCallback (&UlTxPduCallback, m_rlcStats));
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/UeMap/*/RadioBearerMap/*/LteRlc/RxPDU",
                   MakeBoundCallback (&UlRxPduCallback, m_rlcStats));
}

void
LenaHelper::EnableMacTraces (void)
{
  EnableDlMacTraces ();
  EnableUlMacTraces ();
}


void
LenaHelper::EnableDlMacTraces (void)
{
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbMac/DlScheduling",
                   MakeBoundCallback (&DlSchedulingCallback, m_macStats));
}

void
UlSchedulingCallback (Ptr<MacStatsCalculator> mac, std::string path,
                      uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                      uint8_t mcs, uint16_t size)
{
  uint64_t imsi = FindImsiFromEnbMac (path, rnti);
  uint16_t cellId = FindCellIdFromEnbMac (path, rnti);
  mac->UlScheduling (cellId, imsi, frameNo, subframeNo, rnti, mcs, size);
}

void
LenaHelper::EnableUlMacTraces (void)
{
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbMac/UlScheduling",
                   MakeBoundCallback (&UlSchedulingCallback, m_macStats));
}

Ptr<RlcStatsCalculator>
LenaHelper::GetRlcStats (void)
{
  return m_rlcStats;
}

} // namespace ns3
