/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro <peppe.piro@gmail.com>, <g.piro@poliba.it>
 */


#include "lte-helper.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include <string>
#include "ns3/config.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/lte-spectrum-phy.h"
#include "ns3/enb-lte-spectrum-phy.h"
#include "ns3/ue-lte-spectrum-phy.h"
#include "ns3/ue-net-device.h"
#include "ns3/enb-net-device.h"
#include "ns3/ue-manager.h"
#include "ns3/spectrum-propagation-loss-model.h"
#include "ns3/lte-propagation-loss-model.h"

NS_LOG_COMPONENT_DEFINE ("LteHelper");

namespace ns3 {

LteHelper::LteHelper (void)
  : m_downlinkChannel (CreateObject<SingleModelSpectrumChannel> ()),
    m_uplinkChannel (CreateObject<SingleModelSpectrumChannel> ())
{
  Ptr<LtePropagationLossModel> model = CreateObject<LtePropagationLossModel> ();
  m_downlinkChannel->AddSpectrumPropagationLossModel (model);
}

LteHelper::~LteHelper (void)
{
  m_downlinkChannel = 0;
  m_uplinkChannel = 0;
}


Ptr<LtePhy> 
LteHelper::CreatePhy (Ptr<SpectrumChannel> dlChannel, Ptr<SpectrumChannel> ulChannel, NetDeviceType t)
{
  Ptr<LtePhy> phy;
  Ptr<LteSpectrumPhy> dl;
  Ptr<LteSpectrumPhy> ul;

  if (t == LteHelper::DEVICE_TYPE_ENODEB)
    {
      phy =  CreateObject<EnbLtePhy> ();

      dl = CreateObject<EnbLteSpectrumPhy> ();
      ul = CreateObject<EnbLteSpectrumPhy> ();
    }

  if (t == LteHelper::DEVICE_TYPE_USER_EQUIPMENT)
    {
      phy =  CreateObject<UeLtePhy> ();

      dl = CreateObject<UeLteSpectrumPhy> ();
      ul = CreateObject<UeLteSpectrumPhy> ();
    }

  phy->SetDownlinkSpectrumPhy (dl);
  phy->SetUplinkSpectrumPhy (ul);

  if (t == LteHelper::DEVICE_TYPE_ENODEB)
    {
      dl->SetChannel (dlChannel);
      ul->SetChannel (ulChannel);

      m_downlinkChannel->AddRx (dl);
      m_uplinkChannel->AddRx (ul);
    }
  else if (t == LteHelper::DEVICE_TYPE_USER_EQUIPMENT)
    {
      dl->SetChannel (dlChannel);
      ul->SetChannel (ulChannel);

      m_downlinkChannel->AddRx (dl);
    }
  else
    {
      NS_FATAL_ERROR ("LteHelper: Invalid Device type");
    }

  return phy;

}



Ptr<LtePhy> 
LteHelper::CreatePhy (NetDeviceType t)
{
  Ptr<LtePhy> phy;
  Ptr<LteSpectrumPhy> dl;
  Ptr<LteSpectrumPhy> ul;

  if (t == LteHelper::DEVICE_TYPE_ENODEB)
    {
      phy =  CreateObject<EnbLtePhy> ();

      dl = CreateObject<EnbLteSpectrumPhy> ();
      ul = CreateObject<EnbLteSpectrumPhy> ();
    }

  if (t == LteHelper::DEVICE_TYPE_USER_EQUIPMENT)
    {
      phy =  CreateObject<UeLtePhy> ();

      dl = CreateObject<UeLteSpectrumPhy> ();
      ul = CreateObject<UeLteSpectrumPhy> ();
    }

  phy->SetDownlinkSpectrumPhy (dl);
  phy->SetUplinkSpectrumPhy (ul);

  if (t == LteHelper::DEVICE_TYPE_ENODEB)
    {
      dl->SetChannel (m_downlinkChannel);
      ul->SetChannel (m_uplinkChannel);

      m_downlinkChannel->AddRx (dl);
      m_uplinkChannel->AddRx (ul);
    }
  else if (t == LteHelper::DEVICE_TYPE_USER_EQUIPMENT)
    {
      dl->SetChannel (m_downlinkChannel);
      ul->SetChannel (m_uplinkChannel);

      m_downlinkChannel->AddRx (dl);
    }
  else
    {
      NS_FATAL_ERROR ("LteHelper: Invalid Device type");
    }

  return phy;
}


Ptr<UeLtePhy> 
LteHelper::CreateUePhy (void)
{
  Ptr<UeLtePhy> phy = CreateObject<UeLtePhy> ();

  Ptr<UeLteSpectrumPhy> dl = CreateObject<UeLteSpectrumPhy> ();;
  Ptr<UeLteSpectrumPhy> ul = CreateObject<UeLteSpectrumPhy> ();;

  phy->SetDownlinkSpectrumPhy (dl);
  phy->SetUplinkSpectrumPhy (ul);

  dl->SetChannel (m_downlinkChannel);
  ul->SetChannel (m_uplinkChannel);

  m_downlinkChannel->AddRx (dl);
 
  return phy;
}


Ptr<EnbLtePhy> 
LteHelper::CreateEnbPhy (void)
{
  Ptr<EnbLtePhy> phy = CreateObject<EnbLtePhy> ();

  Ptr<EnbLteSpectrumPhy> dl = CreateObject<EnbLteSpectrumPhy> ();;
  Ptr<EnbLteSpectrumPhy> ul = CreateObject<EnbLteSpectrumPhy> ();;

  phy->SetDownlinkSpectrumPhy (dl);
  phy->SetUplinkSpectrumPhy (ul);

  dl->SetChannel (m_downlinkChannel);
  ul->SetChannel (m_uplinkChannel);

  m_downlinkChannel->AddRx (dl);
  m_uplinkChannel->AddRx (ul);
 
  return phy;
}


void 
LteHelper::AddMobility (Ptr<LtePhy> phy, Ptr<MobilityModel> m)
{
  phy->GetDownlinkSpectrumPhy ()->SetMobility (m);
  phy->GetUplinkSpectrumPhy ()->SetMobility (m);
}


NetDeviceContainer 
LteHelper::Install (NodeContainer c, NetDeviceType type)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<LteNetDevice> device;
      Ptr<LtePhy> phy;

      if (type == LteHelper::DEVICE_TYPE_ENODEB)
        {
          Ptr<EnbLtePhy> p = CreateEnbPhy ();
          Ptr<EnbNetDevice> dev = CreateObject<EnbNetDevice> (node, p);

          p->GetUplinkSpectrumPhy ()->SetGenericPhyRxEndOkCallback (MakeCallback (&LteNetDevice::Receive, dev));

          device = dev;
          phy = p;
        }
      else if (type == LteHelper::DEVICE_TYPE_USER_EQUIPMENT)
        {
          Ptr<UeLtePhy> p = CreateUePhy ();
          Ptr<UeNetDevice> dev = CreateObject<UeNetDevice> (node, p);

          p->GetDownlinkSpectrumPhy ()->SetGenericPhyRxEndOkCallback (MakeCallback (&LteNetDevice::Receive, dev));

          device = dev;
          phy = p;
        }
      else
        {
          NS_FATAL_ERROR ("LteHelper: Invalid Device type");
        }



      device->SetAddress (Mac48Address::Allocate ());
      phy->SetDevice (device);

      phy->GetDownlinkSpectrumPhy ()->SetDevice (device);
      phy->GetUplinkSpectrumPhy ()->SetDevice (device);

      device->Start ();
      node->AddDevice (device);
      devices.Add (device);
    }
  return devices;
}


void 
LteHelper::RegisterUeToTheEnb (Ptr<UeNetDevice> ue, Ptr<EnbNetDevice> enb)
{
  ue->SetTargetEnb (enb);
  enb->GetUeManager ()->CreateUeRecord (ue, enb);

}


void 
LteHelper::AddDownlinkChannelRealization (Ptr<MobilityModel> enbMobility, Ptr<MobilityModel> ueMobility, Ptr<LtePhy> phy)
{
  Ptr<LtePropagationLossModel> model = m_downlinkChannel->GetSpectrumPropagationLossModel ()->GetObject<LtePropagationLossModel> ();

  model->CreateChannelRealization (enbMobility, ueMobility);
 
  //initialize multipath model
  Ptr<JakesFadingLossModel> m = model->GetChannelRealization (enbMobility, ueMobility)->GetJakesFadingLossModel ();
  m->SetPhy (phy);
}


void 
LteHelper::EnableLogComponents (void)
{
  LogComponentEnable ("LtePhy", LOG_LEVEL_ALL);
  LogComponentEnable ("EnbLtePhy", LOG_LEVEL_ALL);
  LogComponentEnable ("UeLtePhy", LOG_LEVEL_ALL);

  LogComponentEnable ("LteSpectrumPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("EnbLteSpectrumPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("UeLteSpectrumPhy", LOG_LEVEL_ALL);

  LogComponentEnable ("LtePropagationLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("LossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("ShadowingLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("PenetrationLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("MultipathLossModel", LOG_LEVEL_ALL);
  LogComponentEnable ("PathLossModel", LOG_LEVEL_ALL);


  LogComponentEnable ("RrcEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("MacEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("EnbMacEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("UeMacEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("RlcEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("RadioBearerInstance", LOG_LEVEL_ALL);
  LogComponentEnable ("LteMacQueue", LOG_LEVEL_ALL);

  LogComponentEnable ("LteNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("UeNetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("EnbNetDevice", LOG_LEVEL_ALL);

  LogComponentEnable ("UeManager", LOG_LEVEL_ALL);
  LogComponentEnable ("UeRecord", LOG_LEVEL_ALL);

  LogComponentEnable ("PacketScheduler", LOG_LEVEL_ALL);
  LogComponentEnable ("SimplePacketScheduler", LOG_LEVEL_ALL);


}


} // namespace ns3
