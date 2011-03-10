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

#include <ns3/lte-enb-rrc.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-ue-mac.h>
#include <ns3/lte-enb-mac.h>

#include <ns3/enb-phy.h>
#include <ns3/ue-phy.h>
#include <ns3/lte-spectrum-phy.h>
#include <ns3/lte-sinr-chunk-processor.h>
#include <ns3/single-model-spectrum-channel.h>
#include <ns3/friis-spectrum-propagation-loss.h>

#include <ns3/enb-net-device.h>
#include <ns3/ue-net-device.h>

#include <ns3/lte-spectrum-value-helper.h>


namespace ns3 {


LenaHelper::LenaHelper (void)
  : m_downlinkChannel (CreateObject<SingleModelSpectrumChannel> ()),
    m_uplinkChannel (CreateObject<SingleModelSpectrumChannel> ())
{
  Ptr<SpectrumPropagationLossModel> model = CreateObject<FriisSpectrumPropagationLossModel> ();
  m_downlinkChannel->AddSpectrumPropagationLossModel (model);
}

LenaHelper::~LenaHelper (void)
{
  m_downlinkChannel = 0;
  m_uplinkChannel = 0;
}



NetDeviceContainer
LenaHelper::InstallEnbDevice (NodeContainer c)
{
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

}

void
LenaHelper::SetUeDeviceAttribute (std::string name, const AttributeValue &value)
{

}


Ptr<NetDevice>
LenaHelper::InstallSingleEnbDevice (Ptr<Node> n)
{
  Ptr<EnbLtePhy> phy = CreateObject<EnbLtePhy> ();

  Ptr<LteSpectrumPhy> dlPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteSpectrumPhy> ulPhy = CreateObject<LteSpectrumPhy> ();

  phy->SetDownlinkSpectrumPhy (dlPhy);
  phy->SetUplinkSpectrumPhy (ulPhy);

  Ptr<SpectrumValue> noisePsd = LteSpectrumValueHelper::CreateUplinkNoisePowerSpectralDensity ();
  ulPhy->SetNoisePowerSpectralDensity (noisePsd);

  dlPhy->SetChannel (m_downlinkChannel);
  ulPhy->SetChannel (m_uplinkChannel);

  Ptr<MobilityModel> mm = n->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mm, "MobilityModel needs to be set on node before calling LenaHelper::InstallUeDevice ()");
  dlPhy->SetMobility (mm);
  ulPhy->SetMobility (mm);

  m_uplinkChannel->AddRx (ulPhy);

  Ptr<EnbNetDevice> dev = CreateObject<EnbNetDevice> (n, phy);
  phy->SetDevice (dev);
  dlPhy->SetDevice (dev);
  ulPhy->SetDevice (dev);

  n->AddDevice (dev);
  Ptr<LteEnbMac> mac = dev->GetMac ();
  ulPhy->SetPhyMacRxEndOkCallback (MakeCallback (&EnbLtePhy::PhyPduReceived, phy));

  dev->Start ();
  return dev;
}

Ptr<NetDevice>
LenaHelper::InstallSingleUeDevice (Ptr<Node> n)
{
  Ptr<UeLtePhy> phy = CreateObject<UeLtePhy> ();

  Ptr<LteSpectrumPhy> dlPhy = CreateObject<LteSpectrumPhy> ();
  Ptr<LteSpectrumPhy> ulPhy = CreateObject<LteSpectrumPhy> ();

  phy->SetDownlinkSpectrumPhy (dlPhy);
  phy->SetUplinkSpectrumPhy (ulPhy);

  Ptr<SpectrumValue> noisePsd = LteSpectrumValueHelper::CreateDownlinkNoisePowerSpectralDensity ();
  dlPhy->SetNoisePowerSpectralDensity (noisePsd);

  Ptr<LteCqiSinrChunkProcessor> p = Create<LteCqiSinrChunkProcessor> (phy->GetObject<LtePhy> ());
  dlPhy->AddSinrChunkProcessor (p);

  dlPhy->SetChannel (m_downlinkChannel);
  ulPhy->SetChannel (m_uplinkChannel);

  Ptr<MobilityModel> mm = n->GetObject<MobilityModel> ();
  NS_ASSERT_MSG (mm, "MobilityModel needs to be set on node before calling LenaHelper::InstallUeDevice ()");
  dlPhy->SetMobility (mm);
  ulPhy->SetMobility (mm);

  m_downlinkChannel->AddRx (dlPhy);

  Ptr<UeNetDevice> dev = CreateObject<UeNetDevice> (n, phy);
  phy->SetDevice (dev);
  dlPhy->SetDevice (dev);
  ulPhy->SetDevice (dev);

  n->AddDevice (dev);
  Ptr<LteUeMac> mac = dev->GetMac ();
  dlPhy->SetPhyMacRxEndOkCallback (MakeCallback (&UeLtePhy::PhyPduReceived, phy));

  dev->Start ();
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
  Ptr<LteEnbRrc> enbRrc = enbDevice->GetObject<EnbNetDevice> ()->GetRrc ();
  uint16_t rnti = enbRrc->AddUe ();
  Ptr<LteUeRrc> ueRrc = ueDevice->GetObject<UeNetDevice> ()->GetRrc ();
  ueRrc->ConfigureUe (rnti);

  // attach UE PHY to eNB
  ueDevice->GetObject<UeNetDevice> ()->SetTargetEnb (enbDevice->GetObject<EnbNetDevice> ());

  // WILD HACK - should be done through PHY SAP, probably passing by RRC
  Ptr<UeLtePhy> uePhy = ueDevice->GetObject<UeNetDevice> ()->GetPhy ();
  uePhy->SetRnti (rnti);

  Ptr<EnbLtePhy> enbPhy = enbDevice->GetObject<EnbNetDevice> ()->GetPhy ();
  enbPhy->AddUePhy (rnti, uePhy);
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
  Ptr<EnbNetDevice> enbDevice = ueDevice->GetObject<UeNetDevice> ()->GetTargetEnb ();
  Ptr<LteEnbRrc> enbRrc = enbDevice->GetObject<EnbNetDevice> ()->GetRrc ();
  Ptr<LteUeRrc> ueRrc = ueDevice->GetObject<UeNetDevice> ()->GetRrc ();
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
  LogComponentEnable ("RrPacketScheduler", LOG_LEVEL_ALL);

  LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
  LogComponentEnable ("EnbLtePhy", LOG_LEVEL_ALL);
  LogComponentEnable ("UeLtePhy", LOG_LEVEL_ALL);

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
  LogComponentEnable ("UeNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("EnbNetDevice", LOG_LEVEL_ALL);
}



} // namespace ns3
