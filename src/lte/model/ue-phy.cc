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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#include <ns3/waveform-generator.h>
#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <math.h>
#include <ns3/simulator.h>
#include "ns3/spectrum-error-model.h"
#include "ue-phy.h"
#include "lte-net-device.h"
#include "ue-net-device.h"
#include "enb-net-device.h"
#include "lte-spectrum-value-helper.h"
#include "amc-module.h"
#include "ue-mac-entity.h"

NS_LOG_COMPONENT_DEFINE ("UeLtePhy");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (UeLtePhy);


UeLtePhy::UeLtePhy ()
{
  SetDevice (0);
  SetDownlinkSpectrumPhy (0);
  SetUplinkSpectrumPhy (0);
  SetTxPower (43); // XXX
  SetTti (0.001);
  SetNrFrames (0);
  SetNrSubFrames (0);
}


UeLtePhy::UeLtePhy (Ptr<LteNetDevice> d)
{
  SetDevice (0);
  SetDownlinkSpectrumPhy (0);
  SetUplinkSpectrumPhy (0);
  SetTxPower (43); // XXX
  SetTti (0.001);
  SetNrFrames (0);
  SetNrSubFrames (0);
}


TypeId
UeLtePhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UeLtePhy")
    .SetParent<LtePhy> ()
    .AddConstructor<UeLtePhy> ()
  ;
  return tid;
}


UeLtePhy::~UeLtePhy ()
{
}


bool
UeLtePhy::SendPacket (Ptr<PacketBurst> pb)
{
  NS_LOG_FUNCTION (this);
  return GetUplinkSpectrumPhy ()->StartTx (pb);
}


void
UeLtePhy::DoSetUplinkSubChannels ()
{
  NS_LOG_FUNCTION (this);

  /*
   *  XXX: the uplink scheduler is not implemented yet!
   *  Now, all uplink sub channels can be used for uplink transmission
   */
  SetSubChannelsForTransmission (GetUplinkSubChannels ());
}


void
UeLtePhy::SetSubChannelsForTransmission (std::vector <int> mask)
{
  NS_LOG_FUNCTION (this);

  m_subChannelsForTransmission = mask;

  Ptr<SpectrumValue> txPsd = CreateTxPowerSpectralDensity ();
  GetUplinkSpectrumPhy ()->SetTxPowerSpectralDensity (txPsd);
}


void
UeLtePhy::SetSubChannelsForReception (std::vector <int> mask)
{
  NS_LOG_FUNCTION (this);
  m_subChannelsForReception = mask;
}


std::vector <int>
UeLtePhy::GetSubChannelsForTransmission ()
{
  NS_LOG_FUNCTION (this);
  return m_subChannelsForTransmission;
}


std::vector <int>
UeLtePhy::GetSubChannelsForReception ()
{
  NS_LOG_FUNCTION (this);
  return m_subChannelsForReception;
}


Ptr<SpectrumValue>
UeLtePhy::CreateTxPowerSpectralDensity ()
{
  NS_LOG_FUNCTION (this);
  LteSpectrumValueHelper psdHelper;
  Ptr<SpectrumValue> psd = psdHelper.CreateUplinkTxPowerSpectralDensity (GetTxPower (), GetSubChannelsForTransmission ());

  return psd;
}

void
UeLtePhy::CreateCqiFeedbacks (std::vector<double> sinr)
{
  NS_LOG_FUNCTION (this);
  Ptr<UeNetDevice> thisDevice = GetDevice ()->GetObject<UeNetDevice> ();
  Ptr<UeMacEntity> mac = thisDevice->GetMacEntity ()->GetObject<UeMacEntity> ();
  Ptr<CqiIdealControlMessage> msg = mac->CreateCqiFeedbacks (sinr);
  SendIdealControlMessage (msg);
}

void
UeLtePhy::SendIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
  Ptr<LtePhy> phy = msg->GetDestinationDevice ()->GetPhy ();
  phy->ReceiveIdealControlMessage (msg);
}


void
UeLtePhy::ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);

  if (msg->GetMessageType () == IdealControlMessage::ALLOCATION_MAP)
    {
      Ptr<PdcchMapIdealControlMessage> msg2 = DynamicCast<PdcchMapIdealControlMessage> (msg);

      std::vector <int> ulSubChennels;
      std::vector <int> dlSubChennels;

      // store information about UL and DL sub channel assigned to this device
      PdcchMapIdealControlMessage::IdealPdcchMessage* message = msg2->GetMessage ();
      for (PdcchMapIdealControlMessage::IdealPdcchMessage::iterator it = message->begin ();
           it != message->end (); it++)
        {
          PdcchMapIdealControlMessage::IdealPdcchRecord record = *it;

          if (record.m_ue == GetDevice ())
            {
              if (record.m_direction == PdcchMapIdealControlMessage::DOWNLINK)
                {
                  dlSubChennels.push_back (record.m_idSubChannel);
                }
              else
                {
                  ulSubChennels.push_back (record.m_idSubChannel);
                }
            }
          else
            {
              // not for me!
            }
        }

      // UPDATE ASSIGNED UL and DL sub channels
      /*
       * XXX: do not update the UL sub channels. Since no uplink packet scheduler
       * is implemented yet, ulSubChennels is empty.
       */
      // SetSubChannelsForTransmission (ulSubChennels);

      SetSubChannelsForReception (dlSubChennels);
    }

  else
    {
      // XXX at this time, the UE must receive only Allocation Map messages!
    }


}


} // namespace ns3
