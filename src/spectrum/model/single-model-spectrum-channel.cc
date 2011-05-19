/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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

#include <ns3/object.h>
#include <ns3/simulator.h>
#include <ns3/log.h>
#include <ns3/packet.h>
#include <ns3/packet-burst.h>
#include <ns3/net-device.h>
#include <ns3/node.h>
#include <ns3/mobility-model.h>
#include <ns3/spectrum-phy.h>
#include "single-model-spectrum-channel.h"


NS_LOG_COMPONENT_DEFINE ("SingleModelSpectrumChannel");


namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (SingleModelSpectrumChannel);

SingleModelSpectrumChannel::SingleModelSpectrumChannel ()
  : m_spectrumModel (0),
    m_PropagationDelay (0),
    m_PropagationLoss (0)
{
  NS_LOG_FUNCTION (this);
}

void
SingleModelSpectrumChannel::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_phyList.clear ();
  m_spectrumModel = 0;
  m_PropagationDelay = 0;
  m_PropagationLoss = 0;
  SpectrumChannel::DoDispose ();
}

TypeId
SingleModelSpectrumChannel::GetTypeId (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  static TypeId tid = TypeId ("ns3::SingleModelSpectrumChannel")
    .SetParent<SpectrumChannel> ()
    .AddConstructor<SingleModelSpectrumChannel> ()
  ;
  return tid;
}


void
SingleModelSpectrumChannel::AddRx (Ptr<SpectrumPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);
  m_phyList.push_back (phy);
}


void
SingleModelSpectrumChannel::StartTx (Ptr<PacketBurst> p, Ptr <SpectrumValue> txPsd, SpectrumType st, Time duration, Ptr<SpectrumPhy> txPhy)
{
  NS_LOG_FUNCTION (this << p << *txPsd << st << duration << txPhy);
  NS_ASSERT_MSG (p, "NULL PacketBurst");
  NS_ASSERT_MSG (txPsd, "NULL txPsd");
  NS_ASSERT_MSG (txPhy, "NULL txPhy");

  // just a sanity check routine. We might want to remove it to save some computational load -- one "if" statement  ;-)
  if (m_spectrumModel == 0)
    {
      // first pak, record SpectrumModel
      m_spectrumModel = txPsd->GetSpectrumModel ();
    }
  else
    {
      // all attached SpectrumPhy instances must use the same SpectrumModel
      NS_ASSERT (*(txPsd->GetSpectrumModel ()) == *m_spectrumModel);
    }


  PhyList::const_iterator rxPhyIterator = m_phyList.begin ();

  Ptr<MobilityModel> senderMobility = txPhy->GetMobility ()->GetObject<MobilityModel> ();

  NS_ASSERT (rxPhyIterator != m_phyList.end ());

  while (rxPhyIterator != m_phyList.end ())
    {
      if ((*rxPhyIterator) != txPhy)
        {
          Ptr <SpectrumValue> rxPsd;
          Time delay;
          Ptr<MobilityModel> receiverMobility = (*rxPhyIterator)->GetMobility ()->GetObject<MobilityModel> ();

          if (senderMobility && receiverMobility)
            {


              if (m_PropagationLoss)
                {
                  rxPsd = m_PropagationLoss->CalcRxPowerSpectralDensity (txPsd, senderMobility, receiverMobility);
                }
              else
                {
                  rxPsd = txPsd;
                }

              if (m_PropagationDelay)
                {
                  delay = m_PropagationDelay->GetDelay (senderMobility, receiverMobility);
                }
              else
                {
                  delay = MicroSeconds (0);
                }
            }
          else
            {
              rxPsd = txPsd;
              delay = MicroSeconds (0);
            }

          Ptr<PacketBurst> pktBurstCopy = p->Copy ();
          Ptr<Object> netDevObj = (*rxPhyIterator)->GetDevice ();
          if (netDevObj)
            {
              // the receiver has a NetDevice, so we expect that it is attached to a Node
              uint32_t dstNode =  netDevObj->GetObject<NetDevice> ()->GetNode ()->GetId ();
              Simulator::ScheduleWithContext (dstNode, delay, &SingleModelSpectrumChannel::StartRx, this,
                                              pktBurstCopy, rxPsd, st, duration, *rxPhyIterator);
            }
          else
            {
              // the receiver is not attached to a NetDevice, so we cannot assume that it is attached to a node
              Simulator::Schedule (delay, &SingleModelSpectrumChannel::StartRx, this,
                                   pktBurstCopy, rxPsd, st, duration, *rxPhyIterator);
            }
        }
      ++rxPhyIterator;
    }

}

void
SingleModelSpectrumChannel::StartRx (Ptr<PacketBurst> p, Ptr <SpectrumValue> rxPsd, SpectrumType st, Time duration, Ptr<SpectrumPhy> receiver)
{
  NS_LOG_FUNCTION (this << p << *rxPsd << st << duration << receiver);
  receiver->StartRx (p, rxPsd, st, duration);
}



uint32_t
SingleModelSpectrumChannel::GetNDevices (void) const
{
  NS_LOG_FUNCTION (this);
  return m_phyList.size ();
}


Ptr<NetDevice>
SingleModelSpectrumChannel::GetDevice (uint32_t i) const
{
  NS_LOG_FUNCTION (this << i);
  return m_phyList.at (i)->GetDevice ()->GetObject<NetDevice> ();
}



void
SingleModelSpectrumChannel::AddSpectrumPropagationLossModel (Ptr<SpectrumPropagationLossModel> loss)
{
  NS_LOG_FUNCTION (this << loss);
  NS_ASSERT (m_PropagationLoss == 0);
  m_PropagationLoss = loss;
}

void
SingleModelSpectrumChannel::SetPropagationDelayModel (Ptr<PropagationDelayModel> delay)
{
  NS_LOG_FUNCTION (this << delay);
  NS_ASSERT (m_PropagationDelay == 0);
  m_PropagationDelay = delay;
}


Ptr<SpectrumPropagationLossModel>
SingleModelSpectrumChannel::GetSpectrumPropagationLossModel (void)
{
  NS_LOG_FUNCTION (this);
  return m_PropagationLoss;
}


} // namespace ns3
