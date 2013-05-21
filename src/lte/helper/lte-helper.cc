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


#include "lte-helper.h"
#include <ns3/string.h>
#include <ns3/log.h>
#include <ns3/abort.h>
#include <ns3/pointer.h>
#include <ns3/lte-enb-rrc.h>
#include <ns3/epc-ue-nas.h>
#include <ns3/epc-enb-application.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-ue-mac.h>
#include <ns3/lte-enb-mac.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-enb-phy.h>
#include <ns3/lte-ue-phy.h>
#include <ns3/lte-spectrum-phy.h>
#include <ns3/lte-sinr-chunk-processor.h>
#include <ns3/multi-model-spectrum-channel.h>
#include <ns3/friis-spectrum-propagation-loss.h>
#include <ns3/trace-fading-loss-model.h>
#include <ns3/isotropic-antenna-model.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-net-device.h>
#include <ns3/ff-mac-scheduler.h>
#include <ns3/lte-rlc.h>
#include <ns3/lte-rlc-um.h>
#include <ns3/lte-rlc-am.h>
#include <ns3/epc-enb-s1-sap.h>
#include <ns3/lte-rrc-protocol-ideal.h>
#include <ns3/lte-rrc-protocol-real.h>

#include <ns3/epc-helper.h>
#include <iostream>
#include <ns3/buildings-propagation-loss-model.h>
#include <ns3/lte-spectrum-value-helper.h>
#include <ns3/epc-x2.h>

NS_LOG_COMPONENT_DEFINE ("LteHelper");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LteHelper);

LteHelper::LteHelper (void)
  :  m_fadingStreamsAssigned (false),
     m_imsiCounter (0),
     m_cellIdCounter (0)
{
  NS_LOG_FUNCTION (this);
  m_enbNetDeviceFactory.SetTypeId (LteEnbNetDevice::GetTypeId ());
  m_enbAntennaModelFactory.SetTypeId (IsotropicAntennaModel::GetTypeId ());
  m_ueAntennaModelFactory.SetTypeId (IsotropicAntennaModel::GetTypeId ());
  m_channelFactory.SetTypeId (MultiModelSpectrumChannel::GetTypeId ());
}

void 
LteHelper::DoInitialize (void)
{
  NS_LOG_FUNCTION (this);
  m_downlinkChannel = m_channelFactory.Create<SpectrumChannel> ();
  m_uplinkChannel = m_channelFactory.Create<SpectrumChannel> ();

  m_downlinkPathlossModel = m_dlPathlossModelFactory.Create ();
  Ptr<SpectrumPropagationLossModel> dlSplm = m_downlinkPathlossModel->GetObject<SpectrumPropagationLossModel> ();
  if (dlSplm != 0)
    {
      NS_LOG_LOGIC (this << " using a SpectrumPropagationLossModel in DL");
      m_downlinkChannel->AddSpectrumPropagationLossModel (dlSplm);
    }
  else
    {
      NS_LOG_LOGIC (this << " using a PropagationLossModel in DL");
      Ptr<PropagationLossModel> dlPlm = m_downlinkPathlossModel->GetObject<PropagationLossModel> ();
      NS_ASSERT_MSG (dlPlm != 0, " " << m_downlinkPathlossModel << " is neither PropagationLossModel nor SpectrumPropagationLossModel");
      m_downlinkChannel->AddPropagationLossModel (dlPlm);
    }

  m_uplinkPathlossModel = m_ulPathlossModelFactory.Create ();
  Ptr<SpectrumPropagationLossModel> ulSplm = m_uplinkPathlossModel->GetObject<SpectrumPropagationLossModel> ();
  if (ulSplm != 0)
    {
      NS_LOG_LOGIC (this << " using a SpectrumPropagationLossModel in UL");
      m_uplinkChannel->AddSpectrumPropagationLossModel (ulSplm);
    }
  else
    {
      NS_LOG_LOGIC (this << " using a PropagationLossModel in UL");
      Ptr<PropagationLossModel> ulPlm = m_uplinkPathlossModel->GetObject<PropagationLossModel> ();
      NS_ASSERT_MSG (ulPlm != 0, " " << m_uplinkPathlossModel << " is neither PropagationLossModel nor SpectrumPropagationLossModel");
      m_uplinkChannel->AddPropagationLossModel (ulPlm);
    }
  if (!m_fadingModelType.empty ())
    {
      m_fadingModule = m_fadingModelFactory.Create<SpectrumPropagationLossModel> ();
      m_fadingModule->Initialize ();
      m_downlinkChannel->AddSpectrumPropagationLossModel (m_fadingModule);
      m_uplinkChannel->AddSpectrumPropagationLossModel (m_fadingModule);
    }
  m_phyStats = CreateObject<PhyStatsCalculator> ();
  m_phyTxStats = CreateObject<PhyTxStatsCalculator> ();
  m_phyRxStats = CreateObject<PhyRxStatsCalculator> ();
  m_macStats = CreateObject<MacStatsCalculator> ();
  Object::DoInitialize ();

}

LteHelper::~LteHelper (void)
{
  NS_LOG_FUNCTION (this);
}

TypeId LteHelper::GetTypeId (void)
{
  static TypeId
    tid =
    TypeId ("ns3::LteHelper")
    .SetParent<Object> ()
    .AddConstructor<LteHelper> ()
    .AddAttribute ("Scheduler",
                   "The type of scheduler to be used for eNBs",
                   StringValue ("ns3::PfFfMacScheduler"),
                   MakeStringAccessor (&LteHelper::SetSchedulerType),
                   MakeStringChecker ())
    .AddAttribute ("PathlossModel",
                   "The type of pathloss model to be used",
                   StringValue ("ns3::FriisPropagationLossModel"),
                   MakeStringAccessor (&LteHelper::SetPathlossModelType),
                   MakeStringChecker ())
    .AddAttribute ("FadingModel",
                   "The type of fading model to be used. If the type is set "
                   "to an empty string, no fading model is used.",
                   StringValue (""),
                   MakeStringAccessor (&LteHelper::SetFadingModel),
                   MakeStringChecker ())
    .AddAttribute ("UseIdealRrc",
                   "If true, LteRrcProtocolIdeal will be used for RRC signaling. "
                   "If false, LteRrcProtocolReal will be used.",
                   BooleanValue (true), 
                   MakeBooleanAccessor (&LteHelper::m_useIdealRrc),
                   MakeBooleanChecker ())
  ;
  return tid;
}

void
LteHelper::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_downlinkChannel = 0;
  m_uplinkChannel = 0;
  Object::DoDispose ();
}


void 
LteHelper::SetEpcHelper (Ptr<EpcHelper> h)
{
  NS_LOG_FUNCTION (this << h);
  m_epcHelper = h;
}

void 
LteHelper::SetSchedulerType (std::string type) 
{
  NS_LOG_FUNCTION (this << type);
  m_schedulerFactory = ObjectFactory ();
  m_schedulerFactory.SetTypeId (type);
}

void 
LteHelper::SetSchedulerAttribute (std::string n, const AttributeValue &v)
{
  NS_LOG_FUNCTION (this << n);
  m_schedulerFactory.Set (n, v);
}


void 
LteHelper::SetPathlossModelType (std::string type) 
{
  NS_LOG_FUNCTION (this << type);
  m_dlPathlossModelFactory = ObjectFactory ();
  m_dlPathlossModelFactory.SetTypeId (type);
  m_ulPathlossModelFactory = ObjectFactory ();
  m_ulPathlossModelFactory.SetTypeId (type);
}

void 
LteHelper::SetPathlossModelAttribute (std::string n, const AttributeValue &v)
{
  NS_LOG_FUNCTION (this << n);
  m_dlPathlossModelFactory.Set (n, v);
  m_ulPathlossModelFactory.Set (n, v);
}

void
LteHelper::SetEnbDeviceAttribute (std::string n, const AttributeValue &v)
{
  NS_LOG_FUNCTION (this);
  m_enbNetDeviceFactory.Set (n, v);
}


void 
LteHelper::SetEnbAntennaModelType (std::string type)
{
  NS_LOG_FUNCTION (this);
  m_enbAntennaModelFactory.SetTypeId (type);
}

void 
LteHelper::SetEnbAntennaModelAttribute (std::string n, const AttributeValue &v)
{
  NS_LOG_FUNCTION (this);
  m_enbAntennaModelFactory.Set (n, v);
}

void 
LteHelper::SetUeAntennaModelType (std::string type)
{
  NS_LOG_FUNCTION (this);
  m_ueAntennaModelFactory.SetTypeId (type);
}

void 
LteHelper::SetUeAntennaModelAttribute (std::string n, const AttributeValue &v)
{
  NS_LOG_FUNCTION (this);
  m_ueAntennaModelFactory.Set (n, v);
}

void 
LteHelper::SetFadingModel (std::string type) 
{
  NS_LOG_FUNCTION (this << type);
  m_fadingModelType = type;
  if (!type.empty ())
    {
      m_fadingModelFactory = ObjectFactory ();
      m_fadingModelFactory.SetTypeId (type);
    }
}

void 
LteHelper::SetFadingModelAttribute (std::string n, const AttributeValue &v)
{
  m_fadingModelFactory.Set (n, v);
}

void 
LteHelper::SetSpectrumChannelType (std::string type) 
{
  NS_LOG_FUNCTION (this << type);
  m_channelFactory.SetTypeId (type);
}

void 
LteHelper::SetSpectrumChannelAttribute (std::string n, const AttributeValue &v)
{
  m_channelFactory.Set (n, v);
}


NetDeviceContainer
LteHelper::InstallEnbDevice (NodeContainer c)
{
  NS_LOG_FUNCTION (this);
  Initialize ();  // will run DoInitialize () if necessary
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
LteHelper::InstallUeDevice (NodeContainer c)
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


Ptr<NetDevice>
LteHelper::InstallSingleEnbDevice (Ptr<Node> n)
{

  NS_ABORT_MSG_IF (m_cellIdCounter == 65535, "max num eNBs exceeded");
  uint16_t cellId = ++m_cellIdCounter;

  Ptr<LteSpectrumPhy> dlPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteSpectrumPhy> ulPhy = CreateObject<LteSpectrumPhy> ();

  Ptr<LteEnbPhy> phy = CreateObject<LteEnbPhy> (dlPhy, ulPhy);

  Ptr<LteHarqPhy> harq = Create<LteHarqPhy> ();
  dlPhy->SetHarqPhyModule (harq);
  ulPhy->SetHarqPhyModule (harq);
  phy->SetHarqPhyModule (harq);

  Ptr<LteCtrlSinrChunkProcessor> pCtrl = Create<LteCtrlSinrChunkProcessor> (phy->GetObject<LtePhy> ());
  ulPhy->AddCtrlSinrChunkProcessor (pCtrl); // for evaluating SRS UL-CQI

  Ptr<LteDataSinrChunkProcessor> pData = Create<LteDataSinrChunkProcessor> (ulPhy, phy);
  ulPhy->AddDataSinrChunkProcessor (pData); // for evaluating PUSCH UL-CQI

  Ptr<LteInterferencePowerChunkProcessor> pInterf = Create<LteInterferencePowerChunkProcessor> (phy);
  ulPhy->AddInterferenceDataChunkProcessor (pInterf); // for interference power tracing

  dlPhy->SetChannel (m_downlinkChannel);
  ulPhy->SetChannel (m_uplinkChannel);

  Ptr<MobilityModel> mm = n->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mm, "MobilityModel needs to be set on node before calling LteHelper::InstallUeDevice ()");
  dlPhy->SetMobility (mm);
  ulPhy->SetMobility (mm);

  Ptr<AntennaModel> antenna = (m_enbAntennaModelFactory.Create ())->GetObject<AntennaModel> ();
  NS_ASSERT_MSG (antenna, "error in creating the AntennaModel object");
  dlPhy->SetAntenna (antenna);
  ulPhy->SetAntenna (antenna);

  Ptr<LteEnbMac> mac = CreateObject<LteEnbMac> ();
  Ptr<FfMacScheduler> sched = m_schedulerFactory.Create<FfMacScheduler> ();
  Ptr<LteEnbRrc> rrc = CreateObject<LteEnbRrc> ();

  if (m_useIdealRrc)
    {
      Ptr<LteEnbRrcProtocolIdeal> rrcProtocol = CreateObject<LteEnbRrcProtocolIdeal> ();
      rrcProtocol->SetLteEnbRrcSapProvider (rrc->GetLteEnbRrcSapProvider ());
      rrc->SetLteEnbRrcSapUser (rrcProtocol->GetLteEnbRrcSapUser ());
      rrc->AggregateObject (rrcProtocol);
      rrcProtocol->SetCellId (cellId);
    }
  else
    {
      Ptr<LteEnbRrcProtocolReal> rrcProtocol = CreateObject<LteEnbRrcProtocolReal> ();
      rrcProtocol->SetLteEnbRrcSapProvider (rrc->GetLteEnbRrcSapProvider ());
      rrc->SetLteEnbRrcSapUser (rrcProtocol->GetLteEnbRrcSapUser ());
      rrc->AggregateObject (rrcProtocol);
      rrcProtocol->SetCellId (cellId);
    }

  if (m_epcHelper != 0)
    {
      EnumValue epsBearerToRlcMapping;
      rrc->GetAttribute ("EpsBearerToRlcMapping", epsBearerToRlcMapping);
      // it does not make sense to use RLC/SM when also using the EPC
      if (epsBearerToRlcMapping.Get () == LteEnbRrc::RLC_SM_ALWAYS)
        {
          rrc->SetAttribute ("EpsBearerToRlcMapping", EnumValue (LteEnbRrc::RLC_UM_ALWAYS));
        }
    }

  rrc->SetLteEnbCmacSapProvider (mac->GetLteEnbCmacSapProvider ());
  mac->SetLteEnbCmacSapUser (rrc->GetLteEnbCmacSapUser ());
  rrc->SetLteMacSapProvider (mac->GetLteMacSapProvider ());

  mac->SetFfMacSchedSapProvider (sched->GetFfMacSchedSapProvider ());
  mac->SetFfMacCschedSapProvider (sched->GetFfMacCschedSapProvider ());

  sched->SetFfMacSchedSapUser (mac->GetFfMacSchedSapUser ());
  sched->SetFfMacCschedSapUser (mac->GetFfMacCschedSapUser ());

  phy->SetLteEnbPhySapUser (mac->GetLteEnbPhySapUser ());
  mac->SetLteEnbPhySapProvider (phy->GetLteEnbPhySapProvider ());


  phy->SetLteEnbCphySapUser (rrc->GetLteEnbCphySapUser ());
  rrc->SetLteEnbCphySapProvider (phy->GetLteEnbCphySapProvider ());

  Ptr<LteEnbNetDevice> dev = m_enbNetDeviceFactory.Create<LteEnbNetDevice> ();
  dev->SetNode (n);
  dev->SetAttribute ("CellId", UintegerValue (cellId)); 
  dev->SetAttribute ("LteEnbPhy", PointerValue (phy));
  dev->SetAttribute ("LteEnbMac", PointerValue (mac));
  dev->SetAttribute ("FfMacScheduler", PointerValue (sched));
  dev->SetAttribute ("LteEnbRrc", PointerValue (rrc)); 

  phy->SetDevice (dev);
  dlPhy->SetDevice (dev);
  ulPhy->SetDevice (dev);

  n->AddDevice (dev);
  ulPhy->SetLtePhyRxDataEndOkCallback (MakeCallback (&LteEnbPhy::PhyPduReceived, phy));
  ulPhy->SetLtePhyRxCtrlEndOkCallback (MakeCallback (&LteEnbPhy::ReceiveLteControlMessageList, phy));
  ulPhy->SetLtePhyUlHarqFeedbackCallback (MakeCallback (&LteEnbPhy::ReceiveLteUlHarqFeedback, phy));
  rrc->SetForwardUpCallback (MakeCallback (&LteEnbNetDevice::Receive, dev));

  NS_LOG_LOGIC ("set the propagation model frequencies");
  double dlFreq = LteSpectrumValueHelper::GetCarrierFrequency (dev->GetDlEarfcn ());
  NS_LOG_LOGIC ("DL freq: " << dlFreq);
  bool dlFreqOk = m_downlinkPathlossModel->SetAttributeFailSafe ("Frequency", DoubleValue (dlFreq));
  if (!dlFreqOk)
    {
      NS_LOG_WARN ("DL propagation model does not have a Frequency attribute");
    }
  double ulFreq = LteSpectrumValueHelper::GetCarrierFrequency (dev->GetUlEarfcn ());
  NS_LOG_LOGIC ("UL freq: " << ulFreq);
  bool ulFreqOk = m_uplinkPathlossModel->SetAttributeFailSafe ("Frequency", DoubleValue (ulFreq));
  if (!ulFreqOk)
    {
      NS_LOG_WARN ("UL propagation model does not have a Frequency attribute");
    }
  

  dev->Initialize ();

  m_uplinkChannel->AddRx (ulPhy);

  if (m_epcHelper != 0)
    {
      NS_LOG_INFO ("adding this eNB to the EPC");
      m_epcHelper->AddEnb (n, dev, dev->GetCellId ());
      Ptr<EpcEnbApplication> enbApp = n->GetApplication (0)->GetObject<EpcEnbApplication> ();
      NS_ASSERT_MSG (enbApp != 0, "cannot retrieve EpcEnbApplication");

      // S1 SAPs
      rrc->SetS1SapProvider (enbApp->GetS1SapProvider ());
      enbApp->SetS1SapUser (rrc->GetS1SapUser ());
      
      // X2 SAPs
      Ptr<EpcX2> x2 = n->GetObject<EpcX2> ();
      x2->SetEpcX2SapUser (rrc->GetEpcX2SapUser ());
      rrc->SetEpcX2SapProvider (x2->GetEpcX2SapProvider ());
    }

  return dev;
}

Ptr<NetDevice>
LteHelper::InstallSingleUeDevice (Ptr<Node> n)
{
  NS_LOG_FUNCTION (this);
  Ptr<LteSpectrumPhy> dlPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteSpectrumPhy> ulPhy = CreateObject<LteSpectrumPhy> ();

  Ptr<LteUePhy> phy = CreateObject<LteUePhy> (dlPhy, ulPhy);

  Ptr<LteHarqPhy> harq = Create<LteHarqPhy> ();
  dlPhy->SetHarqPhyModule (harq);
  ulPhy->SetHarqPhyModule (harq);
  phy->SetHarqPhyModule (harq);

  Ptr<LteRsReceivedPowerChunkProcessor> pRs = Create<LteRsReceivedPowerChunkProcessor> (phy->GetObject<LtePhy> ());
  dlPhy->AddRsPowerChunkProcessor (pRs);

  Ptr<LteInterferencePowerChunkProcessor> pInterf = Create<LteInterferencePowerChunkProcessor> (phy);
  dlPhy->AddInterferenceCtrlChunkProcessor (pInterf); // for RSRQ evaluation of UE Measurements
  
  Ptr<LteCtrlSinrChunkProcessor> pCtrl = Create<LteCtrlSinrChunkProcessor> (phy->GetObject<LtePhy> (), dlPhy);
  dlPhy->AddCtrlSinrChunkProcessor (pCtrl);

  Ptr<LteDataSinrChunkProcessor> pData = Create<LteDataSinrChunkProcessor> (dlPhy);
  dlPhy->AddDataSinrChunkProcessor (pData);

  dlPhy->SetChannel (m_downlinkChannel);
  ulPhy->SetChannel (m_uplinkChannel);

  Ptr<MobilityModel> mm = n->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mm, "MobilityModel needs to be set on node before calling LteHelper::InstallUeDevice ()");
  dlPhy->SetMobility (mm);
  ulPhy->SetMobility (mm);

  Ptr<AntennaModel> antenna = (m_ueAntennaModelFactory.Create ())->GetObject<AntennaModel> ();
  NS_ASSERT_MSG (antenna, "error in creating the AntennaModel object");
  dlPhy->SetAntenna (antenna);
  ulPhy->SetAntenna (antenna);

  Ptr<LteUeMac> mac = CreateObject<LteUeMac> ();
  Ptr<LteUeRrc> rrc = CreateObject<LteUeRrc> ();

  if (m_useIdealRrc)
    {      
      Ptr<LteUeRrcProtocolIdeal> rrcProtocol = CreateObject<LteUeRrcProtocolIdeal> ();      
      rrcProtocol->SetUeRrc (rrc);
      rrc->AggregateObject (rrcProtocol);
      rrcProtocol->SetLteUeRrcSapProvider (rrc->GetLteUeRrcSapProvider ());
      rrc->SetLteUeRrcSapUser (rrcProtocol->GetLteUeRrcSapUser ());      
    }
  else
    {
      Ptr<LteUeRrcProtocolReal> rrcProtocol = CreateObject<LteUeRrcProtocolReal> ();      
      rrcProtocol->SetUeRrc (rrc);
      rrc->AggregateObject (rrcProtocol);
      rrcProtocol->SetLteUeRrcSapProvider (rrc->GetLteUeRrcSapProvider ());
      rrc->SetLteUeRrcSapUser (rrcProtocol->GetLteUeRrcSapUser ());      
    }

  if (m_epcHelper != 0)
    {
      rrc->SetUseRlcSm (false);
    }
  Ptr<EpcUeNas> nas = CreateObject<EpcUeNas> ();
 
  nas->SetAsSapProvider (rrc->GetAsSapProvider ());
  rrc->SetAsSapUser (nas->GetAsSapUser ());

  rrc->SetLteUeCmacSapProvider (mac->GetLteUeCmacSapProvider ());
  mac->SetLteUeCmacSapUser (rrc->GetLteUeCmacSapUser ());
  rrc->SetLteMacSapProvider (mac->GetLteMacSapProvider ());

  phy->SetLteUePhySapUser (mac->GetLteUePhySapUser ());
  mac->SetLteUePhySapProvider (phy->GetLteUePhySapProvider ());

  phy->SetLteUeCphySapUser (rrc->GetLteUeCphySapUser ());
  rrc->SetLteUeCphySapProvider (phy->GetLteUeCphySapProvider ());

  NS_ABORT_MSG_IF (m_imsiCounter >= 0xFFFFFFFF, "max num UEs exceeded");  
  uint64_t imsi = ++m_imsiCounter;
  Ptr<LteUeNetDevice> dev = CreateObject<LteUeNetDevice> (n, phy, mac, rrc, nas, imsi);
  phy->SetDevice (dev);
  dlPhy->SetDevice (dev);
  ulPhy->SetDevice (dev);
  nas->SetDevice (dev);

  n->AddDevice (dev);
  dlPhy->SetLtePhyRxDataEndOkCallback (MakeCallback (&LteUePhy::PhyPduReceived, phy));
  dlPhy->SetLtePhyRxCtrlEndOkCallback (MakeCallback (&LteUePhy::ReceiveLteControlMessageList, phy));
  dlPhy->SetLtePhyRxPssCallback (MakeCallback (&LteUePhy::ReceivePss, phy));
  dlPhy->SetLtePhyDlHarqFeedbackCallback (MakeCallback (&LteUePhy::ReceiveLteDlHarqFeedback, phy));
  nas->SetForwardUpCallback (MakeCallback (&LteUeNetDevice::Receive, dev));

  if (m_epcHelper != 0)
    {
      m_epcHelper->AddUe (dev, dev->GetImsi ());
    }

  dev->Initialize ();

  return dev;
}


void
LteHelper::Attach (NetDeviceContainer ueDevices, Ptr<NetDevice> enbDevice)
{
  NS_LOG_FUNCTION (this);
  for (NetDeviceContainer::Iterator i = ueDevices.Begin (); i != ueDevices.End (); ++i)
    {
      Attach (*i, enbDevice);
    }
}

void
LteHelper::Attach (Ptr<NetDevice> ueDevice, Ptr<NetDevice> enbDevice)
{
  NS_LOG_FUNCTION (this);
  //enbRrc->SetCellId (enbDevice->GetObject<LteEnbNetDevice> ()->GetCellId ());

  Ptr<LteUeNetDevice> ueLteDevice = ueDevice->GetObject<LteUeNetDevice> ();
  Ptr<LteEnbNetDevice> enbLteDevice = enbDevice->GetObject<LteEnbNetDevice> ();

  Ptr<EpcUeNas> ueNas = ueLteDevice->GetNas ();
  ueNas->Connect (enbLteDevice->GetCellId (), enbLteDevice->GetDlEarfcn ());

  if (m_epcHelper != 0)
    {
      // activate default EPS bearer
      m_epcHelper->ActivateEpsBearer (ueDevice, ueLteDevice->GetImsi (), EpcTft::Default (), EpsBearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT));
    }
  
  // tricks needed for the simplified LTE-only simulations 
  if (m_epcHelper == 0)
    {
      ueDevice->GetObject<LteUeNetDevice> ()->SetTargetEnb (enbDevice->GetObject<LteEnbNetDevice> ());
    }
}

void
LteHelper::AttachToClosestEnb (NetDeviceContainer ueDevices, NetDeviceContainer enbDevices)
{
  NS_LOG_FUNCTION (this);
  for (NetDeviceContainer::Iterator i = ueDevices.Begin (); i != ueDevices.End (); ++i)
    {
      AttachToClosestEnb (*i, enbDevices);
    }
}

void
LteHelper::AttachToClosestEnb (Ptr<NetDevice> ueDevice, NetDeviceContainer enbDevices)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG (enbDevices.GetN () > 0, "empty enb device container");
  Vector uepos = ueDevice->GetNode ()->GetObject<MobilityModel> ()->GetPosition ();
  double minDistance = std::numeric_limits<double>::infinity ();
  Ptr<NetDevice> closestEnbDevice;
  for (NetDeviceContainer::Iterator i = enbDevices.Begin (); i != enbDevices.End (); ++i)
    {
      Vector enbpos = (*i)->GetNode ()->GetObject<MobilityModel> ()->GetPosition ();
      double distance = CalculateDistance (uepos, enbpos);
      if (distance < minDistance)
        {
          minDistance = distance;
          closestEnbDevice = *i;
        }      
    }
  NS_ASSERT (closestEnbDevice != 0);
  Attach (ueDevice, closestEnbDevice);
}

void
LteHelper::ActivateDedicatedEpsBearer (NetDeviceContainer ueDevices, EpsBearer bearer, Ptr<EpcTft> tft)
{
  NS_LOG_FUNCTION (this);
  for (NetDeviceContainer::Iterator i = ueDevices.Begin (); i != ueDevices.End (); ++i)
    {
      ActivateDedicatedEpsBearer (*i, bearer, tft);
    }
}


void
LteHelper::ActivateDedicatedEpsBearer (Ptr<NetDevice> ueDevice, EpsBearer bearer, Ptr<EpcTft> tft)
{
  NS_LOG_FUNCTION (this);

  NS_ASSERT_MSG (m_epcHelper != 0, "dedicated EPS bearers cannot be set up when EPC is not used");
  
  uint64_t imsi = ueDevice->GetObject<LteUeNetDevice> ()->GetImsi ();
  m_epcHelper->ActivateEpsBearer (ueDevice, imsi, tft, bearer);
}

class DrbActivator : public SimpleRefCount<DrbActivator>
{
public:
  DrbActivator (Ptr<NetDevice> ueDevice, EpsBearer bearer);
  static void ActivateCallback (Ptr<DrbActivator> a, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti);
  void ActivateDrb (uint64_t imsi, uint16_t cellId, uint16_t rnti);
private:
  bool m_active;
  Ptr<NetDevice> m_ueDevice;
  EpsBearer m_bearer;
  uint64_t m_imsi;
};

DrbActivator::DrbActivator (Ptr<NetDevice> ueDevice, EpsBearer bearer)
  : m_active (false),
    m_ueDevice (ueDevice),
    m_bearer (bearer),
    m_imsi (m_ueDevice->GetObject<LteUeNetDevice> ()->GetImsi ())
{
}

void
DrbActivator::ActivateCallback (Ptr<DrbActivator> a, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (a << context << imsi << cellId << rnti);
  a->ActivateDrb (imsi, cellId, rnti);
}

void
DrbActivator::ActivateDrb (uint64_t imsi, uint16_t cellId, uint16_t rnti)
{ 
  NS_LOG_FUNCTION (this << imsi << cellId << rnti << m_active);
  if ((!m_active) && (imsi == m_imsi))
    {
      Ptr<LteUeRrc> ueRrc = m_ueDevice->GetObject<LteUeNetDevice> ()->GetRrc ();
      NS_ASSERT (ueRrc->GetState () == LteUeRrc::CONNECTED_NORMALLY);      
      uint16_t rnti = ueRrc->GetRnti();
      Ptr<LteEnbNetDevice> enbLteDevice = m_ueDevice->GetObject<LteUeNetDevice> ()->GetTargetEnb ();
      Ptr<LteEnbRrc> enbRrc = enbLteDevice->GetObject<LteEnbNetDevice> ()->GetRrc ();
      NS_ASSERT (ueRrc->GetCellId () == enbLteDevice->GetCellId ());
      Ptr<UeManager> ueManager = enbRrc->GetUeManager (rnti);
      NS_ASSERT (ueManager->GetState () == UeManager::CONNECTED_NORMALLY ||
                 ueManager->GetState () == UeManager::CONNECTION_RECONFIGURATION);
      EpcEnbS1SapUser::DataRadioBearerSetupRequestParameters params;
      params.rnti = rnti;
      params.bearer = m_bearer;
      params.bearerId = 0;
      params.gtpTeid = 0; // don't care
      enbRrc->GetS1SapUser ()->DataRadioBearerSetupRequest (params);
      m_active = true;
    }
}
  

void 
LteHelper::ActivateDataRadioBearer (Ptr<NetDevice> ueDevice, EpsBearer bearer)
{
  NS_LOG_FUNCTION (this << ueDevice);
  NS_ASSERT_MSG (m_epcHelper == 0, "this method must not be used when EPC is being used");  
  
  // Normally it is the EPC that takes care of activating DRBs
  // when the UE gets connected. When the EPC is not used, we achieve
  // the same behavior by hooking a dedicated DRB activation function
  // to the Enb RRC Connection Established trace source


  Ptr<LteEnbNetDevice> enbLteDevice = ueDevice->GetObject<LteUeNetDevice> ()->GetTargetEnb ();

  std::ostringstream path;
  path << "/NodeList/" << enbLteDevice->GetNode ()->GetId () 
       << "/DeviceList/" << enbLteDevice->GetIfIndex ()
       << "/LteEnbRrc/ConnectionEstablished";  
  Ptr<DrbActivator> arg = Create<DrbActivator> (ueDevice, bearer);
  Config::Connect (path.str (), MakeBoundCallback (&DrbActivator::ActivateCallback, arg));
}

void
LteHelper::AddX2Interface (NodeContainer enbNodes)
{
  NS_LOG_FUNCTION (this);

  for (NodeContainer::Iterator i = enbNodes.Begin (); i != enbNodes.End (); ++i)
    {
      for (NodeContainer::Iterator j = i + 1; j != enbNodes.End (); ++j)
        {
          AddX2Interface (*i, *j);
        }
    }
}

void
LteHelper::AddX2Interface (Ptr<Node> enbNode1, Ptr<Node> enbNode2)
{
  NS_LOG_FUNCTION (this);
  NS_LOG_INFO ("setting up the X2 interface");

  m_epcHelper->AddX2Interface (enbNode1, enbNode2);
}

void
LteHelper::HandoverRequest (Time hoTime, Ptr<NetDevice> ueDev, Ptr<NetDevice> sourceEnbDev, Ptr<NetDevice> targetEnbDev)
{
  NS_LOG_FUNCTION (this << ueDev << sourceEnbDev << targetEnbDev);
  NS_ASSERT_MSG (m_epcHelper, "Handover requires the use of the EPC - did you forget to call LteHelper::SetEpcHelper () ?");
  Simulator::Schedule (hoTime, &LteHelper::DoHandoverRequest, this, ueDev, sourceEnbDev, targetEnbDev);
}

void
LteHelper::DoHandoverRequest (Ptr<NetDevice> ueDev, Ptr<NetDevice> sourceEnbDev, Ptr<NetDevice> targetEnbDev)
{
  NS_LOG_FUNCTION (this << ueDev << sourceEnbDev << targetEnbDev);

  uint16_t targetCellId = targetEnbDev->GetObject<LteEnbNetDevice> ()->GetCellId ();
  Ptr<LteEnbRrc> sourceRrc = sourceEnbDev->GetObject<LteEnbNetDevice> ()->GetRrc ();
  uint16_t rnti = ueDev->GetObject<LteUeNetDevice> ()->GetRrc ()->GetRnti ();
  sourceRrc->SendHandoverRequest (rnti, targetCellId);  
}





void 
LteHelper::ActivateDataRadioBearer (NetDeviceContainer ueDevices, EpsBearer bearer)
{
  NS_LOG_FUNCTION (this);
   for (NetDeviceContainer::Iterator i = ueDevices.Begin (); i != ueDevices.End (); ++i)
    {
      ActivateDataRadioBearer (*i, bearer);
    }
}

void
LteHelper::EnableLogComponents (void)
{
  LogComponentEnable ("LteHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbRrc", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeRrc", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeMac", LOG_LEVEL_ALL);
  LogComponentEnable ("LteRlc", LOG_LEVEL_ALL);
  LogComponentEnable ("LteRlcUm", LOG_LEVEL_ALL);
  LogComponentEnable ("LteRlcAm", LOG_LEVEL_ALL);
  LogComponentEnable ("RrFfMacScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("PfFfMacScheduler", LOG_LEVEL_ALL);

  LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUePhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteSpectrumValueHelper", LOG_LEVEL_ALL);
  LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("LteInterference", LOG_LEVEL_ALL);
  LogComponentEnable ("LteSinrChunkProcessor", LOG_LEVEL_ALL);

  std::string propModelStr = m_dlPathlossModelFactory.GetTypeId ().GetName ().erase (0,5).c_str ();
  LogComponentEnable ("LteNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LteUeNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("LteEnbNetDevice", LOG_LEVEL_ALL);

  LogComponentEnable ("RadioBearerStatsCalculator", LOG_LEVEL_ALL);
  LogComponentEnable ("LteStatsCalculator", LOG_LEVEL_ALL);
  LogComponentEnable ("MacStatsCalculator", LOG_LEVEL_ALL);
  LogComponentEnable ("PhyTxStatsCalculator", LOG_LEVEL_ALL);
  LogComponentEnable ("PhyRxStatsCalculator", LOG_LEVEL_ALL);
  LogComponentEnable ("PhyStatsCalculator", LOG_LEVEL_ALL);


}

void
LteHelper::EnableTraces (void)
{
  EnablePhyTraces ();
  EnableMacTraces ();
  EnableRlcTraces ();
  EnablePdcpTraces ();
}

void
LteHelper::EnableRlcTraces (void)
{
  NS_ASSERT_MSG (m_rlcStats == 0, "please make sure that LteHelper::EnableRlcTraces is called at most once");
  m_rlcStats = CreateObject<RadioBearerStatsCalculator> ("RLC");
  m_radioBearerStatsConnector.EnableRlcStats (m_rlcStats);
}

int64_t
LteHelper::AssignStreams (NetDeviceContainer c, int64_t stream)
{
  int64_t currentStream = stream;
  if ((m_fadingModule != 0) && (m_fadingStreamsAssigned == false))
    {
      Ptr<TraceFadingLossModel> tflm = m_fadingModule->GetObject<TraceFadingLossModel> ();
      if (tflm != 0)
        {
          currentStream += tflm->AssignStreams (currentStream);
          m_fadingStreamsAssigned = true;
        }
    }
  Ptr<NetDevice> netDevice;
  for (NetDeviceContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      netDevice = (*i);
      Ptr<LteEnbNetDevice> lteEnb = DynamicCast<LteEnbNetDevice> (netDevice);
      if (lteEnb)
        {
          Ptr<LteSpectrumPhy> dlPhy = lteEnb->GetPhy ()->GetDownlinkSpectrumPhy ();
          Ptr<LteSpectrumPhy> ulPhy = lteEnb->GetPhy ()->GetUplinkSpectrumPhy ();
          currentStream += dlPhy->AssignStreams (currentStream);
          currentStream += ulPhy->AssignStreams (currentStream);
        }
      Ptr<LteUeNetDevice> lteUe = DynamicCast<LteUeNetDevice> (netDevice);
      if (lteUe)
        {
          Ptr<LteSpectrumPhy> dlPhy = lteUe->GetPhy ()->GetDownlinkSpectrumPhy ();
          Ptr<LteSpectrumPhy> ulPhy = lteUe->GetPhy ()->GetUplinkSpectrumPhy ();
          Ptr<LteUeMac> ueMac = lteUe->GetMac ();
          currentStream += dlPhy->AssignStreams (currentStream);
          currentStream += ulPhy->AssignStreams (currentStream);
          currentStream += ueMac->AssignStreams (currentStream);
        }
    }
  return (currentStream - stream);
}

uint64_t
FindImsiFromEnbRlcPath (std::string path)
{
  NS_LOG_FUNCTION (path);
  // Sample path input:
  // /NodeList/#NodeId/DeviceList/#DeviceId/LteEnbRrc/UeMap/#C-RNTI/DataRadioBearerMap/#LCID/LteRlc/RxPDU

  // We retrieve the UeManager associated to the C-RNTI and perform the IMSI lookup
  std::string ueMapPath = path.substr (0, path.find ("/DataRadioBearerMap"));
  Config::MatchContainer match = Config::LookupMatches (ueMapPath);

  if (match.GetN () != 0)
    {
      Ptr<Object> ueInfo = match.Get (0);
      NS_LOG_LOGIC ("FindImsiFromEnbRlcPath: " << path << ", " << ueInfo->GetObject<UeManager> ()->GetImsi ());
      return ueInfo->GetObject<UeManager> ()->GetImsi ();
    }
  else
    {
      NS_FATAL_ERROR ("Lookup " << ueMapPath << " got no matches");
    }
}

uint64_t
FindImsiFromUePhy (std::string path)
{
  NS_LOG_FUNCTION (path);
  // Sample path input:
  // /NodeList/#NodeId/DeviceList/#DeviceId/LteUePhy

  // We retrieve the UeInfo associated to the C-RNTI and perform the IMSI lookup
  std::string ueRlcPath = path.substr (0, path.find ("/LteUePhy"));
  ueRlcPath += "/LteUeRrc";
  Config::MatchContainer match = Config::LookupMatches (ueRlcPath);

  if (match.GetN () != 0)
    {
      Ptr<Object> ueRrc = match.Get (0);
      return ueRrc->GetObject<LteUeRrc> ()->GetImsi ();
    }
  else
    {
      NS_FATAL_ERROR ("Lookup " << ueRlcPath << " got no matches");
    }
  return 0;
}


uint64_t
FindImsiFromLteNetDevice (std::string path)
{
  NS_LOG_FUNCTION (path);
  // Sample path input:
  // /NodeList/#NodeId/DeviceList/#DeviceId/

  // We retrieve the Imsi associated to the LteUeNetDevice
  Config::MatchContainer match = Config::LookupMatches (path);

  if (match.GetN () != 0)
    {
      Ptr<Object> ueNetDevice = match.Get (0);
      NS_LOG_LOGIC ("FindImsiFromLteNetDevice: " << path << ", " << ueNetDevice->GetObject<LteUeNetDevice> ()->GetImsi ());
      return ueNetDevice->GetObject<LteUeNetDevice> ()->GetImsi ();
    }
  else
    {
      NS_FATAL_ERROR ("Lookup " << path << " got no matches");
    }
}

uint16_t
FindCellIdFromEnbRlcPath (std::string path)
{
  NS_LOG_FUNCTION (path);
  // Sample path input:
  // /NodeList/#NodeId/DeviceList/#DeviceId/LteEnbRrc/UeMap/#C-RNTI/DataRadioBearerMap/#LCID/LteRlc/RxPDU

  // We retrieve the CellId associated to the Enb
  std::string enbNetDevicePath = path.substr (0, path.find ("/LteEnbRrc"));
  Config::MatchContainer match = Config::LookupMatches (enbNetDevicePath);
  if (match.GetN () != 0)
    {
      Ptr<Object> enbNetDevice = match.Get (0);
      NS_LOG_LOGIC ("FindCellIdFromEnbRlcPath: " << path << ", " << enbNetDevice->GetObject<LteEnbNetDevice> ()->GetCellId ());
      return enbNetDevice->GetObject<LteEnbNetDevice> ()->GetCellId ();
    }
  else
    {
      NS_FATAL_ERROR ("Lookup " << enbNetDevicePath << " got no matches");
    }
}

uint64_t
FindImsiFromEnbMac (std::string path, uint16_t rnti)
{
  NS_LOG_FUNCTION (path << rnti);

  // /NodeList/#NodeId/DeviceList/#DeviceId/LteEnbMac/DlScheduling
  std::ostringstream oss;
  std::string p = path.substr (0, path.find ("/LteEnbMac"));
  oss << rnti;
  p += "/LteEnbRrc/UeMap/" + oss.str ();
  uint64_t imsi = FindImsiFromEnbRlcPath (p);
  NS_LOG_LOGIC ("FindImsiFromEnbMac: " << path << ", " << rnti << ", " << imsi);
  return imsi;
}

uint16_t
FindCellIdFromEnbMac (std::string path, uint16_t rnti)
{
  NS_LOG_FUNCTION (path << rnti);
  // /NodeList/#NodeId/DeviceList/#DeviceId/LteEnbMac/DlScheduling
  std::ostringstream oss;
  std::string p = path.substr (0, path.find ("/LteEnbMac"));
  oss << rnti;
  p += "/LteEnbRrc/UeMap/" + oss.str ();
  uint16_t cellId = FindCellIdFromEnbRlcPath (p);
  NS_LOG_LOGIC ("FindCellIdFromEnbMac: " << path << ", "<< rnti << ", " << cellId);
  return cellId;
}


uint64_t
FindImsiForEnb (std::string path, uint16_t rnti)
{
  NS_LOG_FUNCTION (path << rnti);
  uint64_t imsi = 0;
  if (path.find ("/DlPhyTransmission"))
    {
      // /NodeList/0/DeviceList/0/LteEnbPhy/DlPhyTransmission/LteEnbRrc/UeMap/1
      std::ostringstream oss;
      std::string p = path.substr (0, path.find ("/LteEnbPhy"));
      oss << rnti;
      p += "/LteEnbRrc/UeMap/" + oss.str ();
      imsi = FindImsiFromEnbRlcPath (p);
      NS_LOG_LOGIC ("FindImsiForEnb[Tx]: " << path << ", " << rnti << ", " << imsi);
    }
  else if (path.find ("/UlPhyReception"))
    {
      std::string p = path.substr (0, path.find ("/LteUePhy"));
      imsi = FindImsiFromLteNetDevice (p);
      NS_LOG_LOGIC ("FindImsiForEnb[Rx]: " << path << ", " << rnti << ", " << imsi);
    }
  return imsi;
}


uint64_t
FindImsiForUe (std::string path, uint16_t rnti)
{
  NS_LOG_FUNCTION (path << rnti);
  uint64_t imsi = 0;
  if (path.find ("/UlPhyTransmission"))
    {
      std::string p = path.substr (0, path.find ("/LteUePhy"));
      imsi = FindImsiFromLteNetDevice (p);
      NS_LOG_LOGIC ("FindImsiForUe[Tx]: " << path << ", " << rnti << ", " << imsi);
    }
  else if (path.find ("/DlPhyReception"))
    {
      // /NodeList/0/DeviceList/0/LteEnbPhy/LteSpectrumPhy
      std::ostringstream oss;
      std::string p = path.substr (0, path.find ("/LteEnbPhy"));
      oss << rnti;
      p += "/LteEnbRrc/UeMap/" + oss.str ();
      imsi = FindImsiFromEnbRlcPath (p);
      NS_LOG_LOGIC ("FindImsiForUe[Rx]: " << path << ", " << rnti << ", " << imsi);
    }
  return imsi;
}

void
DlPhyTransmissionCallback (Ptr<PhyTxStatsCalculator> phyTxStats,
                      std::string path, PhyTransmissionStatParameters params)
{
  NS_LOG_FUNCTION (phyTxStats << path);
  uint64_t imsi = 0;
  std::ostringstream pathAndRnti;
  pathAndRnti << path << "/" << params.m_rnti;
  if (phyTxStats->ExistsImsiPath (pathAndRnti.str ()) == true)
    {
      imsi = phyTxStats->GetImsiPath (pathAndRnti.str ());
    }
  else
    {
      imsi = FindImsiForEnb (path, params.m_rnti);
      phyTxStats->SetImsiPath (pathAndRnti.str (), imsi);
    }

  params.m_imsi = imsi;
  phyTxStats->DlPhyTransmission (params);
}

void
UlPhyTransmissionCallback (Ptr<PhyTxStatsCalculator> phyTxStats,
                      std::string path, PhyTransmissionStatParameters params)
{
  NS_LOG_FUNCTION (phyTxStats << path);
  uint64_t imsi = 0;
  std::ostringstream pathAndRnti;
  pathAndRnti << path << "/" << params.m_rnti;
  if (phyTxStats->ExistsImsiPath (pathAndRnti.str ()) == true)
    {
      imsi = phyTxStats->GetImsiPath (pathAndRnti.str ());
    }
  else
    {
      imsi = FindImsiForUe (path, params.m_rnti);
      phyTxStats->SetImsiPath (pathAndRnti.str (), imsi);
    }

  params.m_imsi = imsi;
  phyTxStats->UlPhyTransmission (params);
}


void
DlPhyReceptionCallback (Ptr<PhyRxStatsCalculator> phyRxStats,
                      std::string path, PhyReceptionStatParameters params)
{
  NS_LOG_FUNCTION (phyRxStats << path);
  uint64_t imsi = 0;
  std::ostringstream pathAndRnti;
  pathAndRnti << path << "/" << params.m_rnti;
  if (phyRxStats->ExistsImsiPath (pathAndRnti.str ()) == true)
    {
      imsi = phyRxStats->GetImsiPath (pathAndRnti.str ());
    }
  else
    {
      imsi = FindImsiForUe (path, params.m_rnti);
      phyRxStats->SetImsiPath (pathAndRnti.str (), imsi);
    }

  params.m_imsi = imsi;
  phyRxStats->DlPhyReception (params);
}

void
UlPhyReceptionCallback (Ptr<PhyRxStatsCalculator> phyRxStats,
                      std::string path, PhyReceptionStatParameters params)
{
  NS_LOG_FUNCTION (phyRxStats << path);
  uint64_t imsi = 0;
  std::ostringstream pathAndRnti;
  pathAndRnti << path << "/" << params.m_rnti;
  if (phyRxStats->ExistsImsiPath (pathAndRnti.str ()) == true)
    {
      imsi = phyRxStats->GetImsiPath (pathAndRnti.str ());
    }
  else
    {
      imsi = FindImsiForEnb (path, params.m_rnti);
      phyRxStats->SetImsiPath (pathAndRnti.str (), imsi);
    }

  params.m_imsi = imsi;
  phyRxStats->UlPhyReception (params);
}

void
LteHelper::EnablePhyTraces (void)
{
  EnableDlPhyTraces ();
  EnableUlPhyTraces ();
  EnableDlTxPhyTraces ();
  EnableUlTxPhyTraces ();
  EnableDlRxPhyTraces ();
  EnableUlRxPhyTraces ();
}

void
LteHelper::EnableDlTxPhyTraces (void)
{
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbPhy/DlPhyTransmission",
                   MakeBoundCallback (&DlPhyTransmissionCallback, m_phyTxStats));
}

void
LteHelper::EnableUlTxPhyTraces (void)
{
  Config::Connect ("/NodeList/*/DeviceList/*/LteUePhy/UlPhyTransmission",
                   MakeBoundCallback (&UlPhyTransmissionCallback, m_phyTxStats));
}

void
LteHelper::EnableDlRxPhyTraces (void)
{
  Config::Connect ("/NodeList/*/DeviceList/*/LteUePhy/DlSpectrumPhy/DlPhyReception",
                   MakeBoundCallback (&DlPhyReceptionCallback, m_phyRxStats));
}

void
LteHelper::EnableUlRxPhyTraces (void)
{
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbPhy/UlSpectrumPhy/UlPhyReception",
                   MakeBoundCallback (&UlPhyReceptionCallback, m_phyRxStats));
}



void
DlSchedulingCallback (Ptr<MacStatsCalculator> macStats,
                      std::string path, uint32_t frameNo, uint32_t subframeNo,
                      uint16_t rnti, uint8_t mcsTb1, uint16_t sizeTb1,
                      uint8_t mcsTb2, uint16_t sizeTb2)
{
  NS_LOG_FUNCTION (macStats << path);
  uint64_t imsi = 0;
  std::ostringstream pathAndRnti;
  pathAndRnti << path << "/" << rnti;
  if (macStats->ExistsImsiPath (pathAndRnti.str ()) == true)
    {
      imsi = macStats->GetImsiPath (pathAndRnti.str ());
    }
  else
    {
      imsi = FindImsiFromEnbMac (path, rnti);
      macStats->SetImsiPath (pathAndRnti.str (), imsi);
    }

  uint16_t cellId = 0;
  if (macStats->ExistsCellIdPath (pathAndRnti.str ()) == true)
    {
      cellId = macStats->GetCellIdPath (pathAndRnti.str ());
    }
  else
    {
      cellId = FindCellIdFromEnbMac (path, rnti);
      macStats->SetCellIdPath (pathAndRnti.str (), cellId);
    }

  macStats->DlScheduling (cellId, imsi, frameNo, subframeNo, rnti, mcsTb1, sizeTb1, mcsTb2, sizeTb2);
}



void
LteHelper::EnableMacTraces (void)
{
  EnableDlMacTraces ();
  EnableUlMacTraces ();
}


void
LteHelper::EnableDlMacTraces (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbMac/DlScheduling",
                   MakeBoundCallback (&DlSchedulingCallback, m_macStats));
}

void
UlSchedulingCallback (Ptr<MacStatsCalculator> macStats, std::string path,
                      uint32_t frameNo, uint32_t subframeNo, uint16_t rnti,
                      uint8_t mcs, uint16_t size)
{
  NS_LOG_FUNCTION (macStats << path);

  uint64_t imsi = 0;
  std::ostringstream pathAndRnti;
  pathAndRnti << path << "/" << rnti;
  if (macStats->ExistsImsiPath (pathAndRnti.str ()) == true)
    {
      imsi = macStats->GetImsiPath (pathAndRnti.str ());
    }
  else
    {
      imsi = FindImsiFromEnbMac (path, rnti);
      macStats->SetImsiPath (pathAndRnti.str (), imsi);
    }
  uint16_t cellId = 0;
  if (macStats->ExistsCellIdPath (pathAndRnti.str ()) == true)
    {
      cellId = macStats->GetCellIdPath (pathAndRnti.str ());
    }
  else
    {
      cellId = FindCellIdFromEnbMac (path, rnti);
      macStats->SetCellIdPath (pathAndRnti.str (), cellId);
    }

  macStats->UlScheduling (cellId, imsi, frameNo, subframeNo, rnti, mcs, size);
}

void
LteHelper::EnableUlMacTraces (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbMac/UlScheduling",
                   MakeBoundCallback (&UlSchedulingCallback, m_macStats));
}

void
ReportCurrentCellRsrpSinrCallback (Ptr<PhyStatsCalculator> phyStats,
                      std::string path, uint16_t cellId, uint16_t rnti,
                      double rsrp, double sinr)
{
  NS_LOG_FUNCTION (phyStats << path);
  uint64_t imsi = 0;
  std::string pathUePhy  = path.substr (0, path.find ("/ReportCurrentCellRsrpSinr"));
  if (phyStats->ExistsImsiPath (pathUePhy) == true)
    {
      imsi = phyStats->GetImsiPath (pathUePhy);
    }
  else
    {
      imsi = FindImsiFromUePhy (pathUePhy);
      phyStats->SetImsiPath (pathUePhy, imsi);
    }

  phyStats->ReportCurrentCellRsrpSinr (cellId, imsi, rnti, rsrp,sinr);
}

void
LteHelper::EnableDlPhyTraces (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Config::Connect ("/NodeList/*/DeviceList/*/LteUePhy/ReportCurrentCellRsrpSinr",
                   MakeBoundCallback (&ReportCurrentCellRsrpSinrCallback, m_phyStats));
}

void
ReportUeSinr (Ptr<PhyStatsCalculator> phyStats, std::string path,
              uint16_t cellId, uint16_t rnti, double sinrLinear)
{
  NS_LOG_FUNCTION (phyStats << path);

  uint64_t imsi = 0;
  std::ostringstream pathAndRnti;
  pathAndRnti << path << "/" << rnti;
  std::string pathEnbMac  = path.substr (0, path.find ("LteEnbPhy/ReportUeSinr"));
  pathEnbMac += "LteEnbMac/DlScheduling";
  if (phyStats->ExistsImsiPath (pathAndRnti.str ()) == true)
    {
      imsi = phyStats->GetImsiPath (pathAndRnti.str ());
    }
  else
    {
      imsi = FindImsiFromEnbMac (pathEnbMac, rnti);
      phyStats->SetImsiPath (pathAndRnti.str (), imsi);
    }

  phyStats->ReportUeSinr (cellId, imsi, rnti, sinrLinear);
}

void
ReportInterference (Ptr<PhyStatsCalculator> phyStats, std::string path,
                    uint16_t cellId, Ptr<SpectrumValue> interference)
{
  NS_LOG_FUNCTION (phyStats << path);
  phyStats->ReportInterference (cellId, interference);
}

void
LteHelper::EnableUlPhyTraces (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbPhy/ReportUeSinr",
                   MakeBoundCallback (&ReportUeSinr, m_phyStats));
  Config::Connect ("/NodeList/*/DeviceList/*/LteEnbPhy/ReportInterference",
                   MakeBoundCallback (&ReportInterference, m_phyStats));

}

Ptr<RadioBearerStatsCalculator>
LteHelper::GetRlcStats (void)
{
  return m_rlcStats;
}

void
LteHelper::EnablePdcpTraces (void)
{
  NS_ASSERT_MSG (m_pdcpStats == 0, "please make sure that LteHelper::EnablePdcpTraces is called at most once");
  m_pdcpStats = CreateObject<RadioBearerStatsCalculator> ("PDCP");
  m_radioBearerStatsConnector.EnablePdcpStats (m_pdcpStats);
}

Ptr<RadioBearerStatsCalculator>
LteHelper::GetPdcpStats (void)
{
  return m_pdcpStats;
}

} // namespace ns3
