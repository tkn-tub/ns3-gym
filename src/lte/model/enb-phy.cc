/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "enb-phy.h"
#include "lte-net-device.h"
#include "lte-spectrum-value-helper.h"
#include "ideal-control-messages.h"
#include "enb-net-device.h"
#include "enb-mac-entity.h"
#include "packet-scheduler.h"

NS_LOG_COMPONENT_DEFINE ("EnbLtePhy");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (EnbLtePhy);


EnbLtePhy::EnbLtePhy ()
{
  SetDevice (0);
  SetDownlinkSpectrumPhy (0);
  SetUplinkSpectrumPhy (0);
  SetTxPower (43); // dBm
  SetTti (0.001);
  SetNrFrames (0);
  SetNrSubFrames (0);
}


EnbLtePhy::EnbLtePhy (Ptr<LteNetDevice> d)
{
  SetDevice (d);
  SetDownlinkSpectrumPhy (0);
  SetUplinkSpectrumPhy (0);
  SetTxPower (43); // dBm
  SetTti (0.001);
  SetNrFrames (0);
  SetNrSubFrames (0);
}


TypeId
EnbLtePhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EnbLtePhy")
    .SetParent<LtePhy> ()
    .AddConstructor<EnbLtePhy> ()
  ;
  return tid;
}


EnbLtePhy::~EnbLtePhy ()
{
}


bool
EnbLtePhy::SendPacket (Ptr<PacketBurst> pb)
{
  NS_LOG_FUNCTION (this << pb->GetNPackets () << pb->GetSize ());
  return GetDownlinkSpectrumPhy ()->StartTx (pb);
}

void
EnbLtePhy::DoSetDownlinkSubChannels ()
{
  NS_LOG_FUNCTION (this);
  Ptr<SpectrumValue> txPsd = CreateTxPowerSpectralDensity ();
  GetDownlinkSpectrumPhy ()->SetTxPowerSpectralDensity (txPsd);
}


Ptr<SpectrumValue>
EnbLtePhy::CreateTxPowerSpectralDensity ()
{
  NS_LOG_FUNCTION (this);

  LteSpectrumValueHelper psdHelper;
  Ptr<SpectrumValue> psd = psdHelper.CreateDownlinkTxPowerSpectralDensity (GetTxPower (), GetDownlinkSubChannels ());

  return psd;
}


void
EnbLtePhy::CalcChannelQualityForUe (std::vector <double> sinr, Ptr<LteSpectrumPhy> ue)
{
  NS_LOG_FUNCTION (this);
}

void
EnbLtePhy::SendIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
}


void
EnbLtePhy::ReceiveIdealControlMessage (Ptr<IdealControlMessage> msg)
{
  NS_LOG_FUNCTION (this << msg);
  if (msg->GetMessageType () == IdealControlMessage::CQI_FEEDBACKS)
    {
      Ptr<CqiIdealControlMessage> msg2 = DynamicCast<CqiIdealControlMessage> (msg);
      Ptr<EnbMacEntity> macEntity = GetDevice ()->GetObject<EnbNetDevice> ()->GetMacEntity ();
      macEntity->ReceiveCqiIdealControlMessage (msg2);
    }

  else
    {
      // XXX at this time, the eNB must receive only CQI feedbacks!
    }

}



void
EnbLtePhy::StartFrame (void)
{
  NS_LOG_FUNCTION (this << Simulator::Now ().GetSeconds ());
  NS_LOG_INFO ("-----frame " << GetNrFrames () + 1 << "-----");

  SetNrFrames (GetNrFrames () + 1);
  SetNrSubFrames (0);

  StartSubFrame ();
}


void
EnbLtePhy::StartSubFrame (void)
{
  NS_LOG_FUNCTION (this << Simulator::Now ().GetSeconds ());
  NS_LOG_INFO ("-----sub frame " << GetNrSubFrames () + 1 << "-----");

  SetNrSubFrames (GetNrSubFrames () + 1);


  /*
   XXX: the packet scheduler is not implemented yet!
   The enb take the fist packet from the default bearer
   and send it.
   */

  Ptr<EnbMacEntity> macEntity = GetDevice ()->GetObject<EnbNetDevice> ()->GetMacEntity ();

  // macEntity->GetUplinkPacketScheduler ()->RunPacketScheduler ();
  macEntity->GetDownlinkPacketScheduler ()->RunPacketScheduler ();

  Simulator::Schedule (Seconds (GetTti ()),
                       &EnbLtePhy::EndSubFrame,
                       this);

}


void
EnbLtePhy::EndSubFrame (void)
{
  NS_LOG_FUNCTION (this << Simulator::Now ().GetSeconds ());
  if (GetNrSubFrames () == 10)
    {
      Simulator::ScheduleNow (&EnbLtePhy::EndFrame, this);
    }
  else
    {
      Simulator::ScheduleNow (&EnbLtePhy::StartSubFrame, this);
    }
}


void
EnbLtePhy::EndFrame (void)
{
  NS_LOG_FUNCTION (this << Simulator::Now ().GetSeconds ());
  Simulator::ScheduleNow (&EnbLtePhy::StartFrame, this);
}



};
