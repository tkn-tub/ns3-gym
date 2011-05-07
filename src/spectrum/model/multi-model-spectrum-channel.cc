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
#include <ns3/spectrum-converter.h>
#include <iostream>
#include <utility>
#include "multi-model-spectrum-channel.h"


NS_LOG_COMPONENT_DEFINE ("MultiModelSpectrumChannel");


namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (MultiModelSpectrumChannel);


std::ostream& operator<< (std::ostream& lhs, TxSpectrumModelInfoMap_t& rhs)
{
  for (TxSpectrumModelInfoMap_t::iterator it = rhs.begin ();
       it != rhs.end ();
       ++it)
    {
      SpectrumConverterMap_t::iterator jt;
      for (jt = it->second.m_spectrumConverterMap.begin ();
           jt != it->second.m_spectrumConverterMap.end ();
           ++jt)
        {
          lhs << "(" << it->first << "," << jt->first << ") ";
        }
    }
  return lhs;
}

TxSpectrumModelInfo::TxSpectrumModelInfo (Ptr<const SpectrumModel> txSpectrumModel)
  : m_txSpectrumModel (txSpectrumModel)
{
}


RxSpectrumModelInfo::RxSpectrumModelInfo (Ptr<const SpectrumModel> rxSpectrumModel)
  : m_rxSpectrumModel (rxSpectrumModel)
{
}


MultiModelSpectrumChannel::MultiModelSpectrumChannel ()
  : m_PropagationDelay (0),
    m_PropagationLoss (0)
{
  NS_LOG_FUNCTION (this);
}

void
MultiModelSpectrumChannel::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_PropagationLoss = 0;
  m_PropagationDelay = 0;
  m_txSpectrumModelInfoMap.clear ();
  m_rxSpectrumModelInfoMap.clear ();
  m_phyVector.clear ();
  SpectrumChannel::DoDispose ();
}

TypeId
MultiModelSpectrumChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MultiModelSpectrumChannel")
    .SetParent<SpectrumChannel> ()
    .AddConstructor<MultiModelSpectrumChannel> ()
  ;
  return tid;
}



void
MultiModelSpectrumChannel::AddRx (Ptr<SpectrumPhy> phy)
{
  NS_LOG_FUNCTION (this << phy);

  Ptr<const SpectrumModel> rxSpectrumModel = phy->GetRxSpectrumModel ();

  NS_ASSERT_MSG ((0 != rxSpectrumModel), "phy->GetRxSpectrumModel () returned 0. Please check that the RxSpectrumModel is already set for the phy before calling MultiModelSpectrumChannel::AddRx (phy)");

  SpectrumModelUid_t rxSpectrumModelUid = rxSpectrumModel->GetUid ();

  std::vector<Ptr<SpectrumPhy> >::const_iterator it;

  // make sure this phy had not been already added
  for ( it = m_phyVector.begin (); it != m_phyVector.end (); ++it)
    {
      NS_ASSERT (*it != phy);
    }
  m_phyVector.push_back (phy);

  RxSpectrumModelInfoMap_t::iterator rxInfoIterator = m_rxSpectrumModelInfoMap.find (rxSpectrumModelUid);

  if (rxInfoIterator == m_rxSpectrumModelInfoMap.end ())
    {
      // spectrum model unknown, add it to the list of RxSpectrumModels
      std::pair<RxSpectrumModelInfoMap_t::iterator, bool> ret;
      ret = m_rxSpectrumModelInfoMap.insert (std::make_pair (rxSpectrumModelUid, RxSpectrumModelInfo (rxSpectrumModel)));
      NS_ASSERT (ret.second);
      // also add the phy to the newly created list of SpectrumPhy for this RxSpectrumModel
      ret.first->second.m_rxPhyList.push_back (phy);

      // and create the necessary converters for all the TX spectrum models that we know of
      for (TxSpectrumModelInfoMap_t::iterator txInfoIterator = m_txSpectrumModelInfoMap.begin ();
           txInfoIterator != m_txSpectrumModelInfoMap.end ();
           ++txInfoIterator)
        {
          Ptr<const SpectrumModel> txSpectrumModel = txInfoIterator->second.m_txSpectrumModel;
          NS_LOG_LOGIC ("Creating converters between SpectrumModelUids " << txSpectrumModel->GetUid () << " and " << rxSpectrumModelUid );
          SpectrumConverter converter (txSpectrumModel, rxSpectrumModel);
          std::pair<SpectrumConverterMap_t::iterator, bool> ret2;
          ret2 = txInfoIterator->second.m_spectrumConverterMap.insert (std::make_pair (rxSpectrumModelUid, converter));
          NS_ASSERT (ret2.second);
        }
    }
  else
    {
      // spectrum model is already known, just add the device to the corresponding list
      rxInfoIterator->second.m_rxPhyList.push_back (phy);
    }

}


TxSpectrumModelInfoMap_t::const_iterator
MultiModelSpectrumChannel::FindAndEventuallyAddTxSpectrumModel (Ptr<const SpectrumModel> txSpectrumModel)
{
  NS_LOG_FUNCTION (this << txSpectrumModel);
  SpectrumModelUid_t txSpectrumModelUid = txSpectrumModel->GetUid ();
  TxSpectrumModelInfoMap_t::iterator txInfoIterator = m_txSpectrumModelInfoMap.find (txSpectrumModelUid);

  if (txInfoIterator == m_txSpectrumModelInfoMap.end ())
    {
      // first time we see this TX SpectrumModel
      // we add it to the list
      std::pair<TxSpectrumModelInfoMap_t::iterator, bool> ret;
      ret = m_txSpectrumModelInfoMap.insert (std::make_pair (txSpectrumModelUid, TxSpectrumModelInfo (txSpectrumModel)));
      NS_ASSERT (ret.second);
      txInfoIterator = ret.first;

      // and we create the converters for all the RX SpectrumModels that we know of
      for (RxSpectrumModelInfoMap_t::const_iterator rxInfoIterator = m_rxSpectrumModelInfoMap.begin ();
           rxInfoIterator != m_rxSpectrumModelInfoMap.end ();
           ++rxInfoIterator)
        {
          Ptr<const SpectrumModel> rxSpectrumModel = rxInfoIterator->second.m_rxSpectrumModel;
          SpectrumModelUid_t rxSpectrumModelUid = rxSpectrumModel->GetUid ();

          if (rxSpectrumModelUid != txSpectrumModelUid)
            {
              NS_LOG_LOGIC ("Creating converters between SpectrumModelUids " << txSpectrumModelUid << " and " << rxSpectrumModelUid );

              SpectrumConverter converter (txSpectrumModel, rxSpectrumModel);
              std::pair<SpectrumConverterMap_t::iterator, bool> ret2;
              ret2 = txInfoIterator->second.m_spectrumConverterMap.insert (std::make_pair (rxSpectrumModelUid, converter));
              NS_ASSERT (ret2.second);
            }
        }
    }
  else
    {
      NS_LOG_LOGIC ("SpectrumModelUid " << txSpectrumModelUid << " already present");
    }
  return txInfoIterator;
}


void
MultiModelSpectrumChannel::StartTx (Ptr<PacketBurst> p, Ptr <SpectrumValue> originalTxPowerSpectrum, SpectrumType st, Time duration, Ptr<SpectrumPhy> txPhy)
{
  NS_LOG_FUNCTION (this << p << *originalTxPowerSpectrum << duration << txPhy);

  NS_ASSERT (txPhy);
  NS_ASSERT (originalTxPowerSpectrum);


  Ptr<MobilityModel> txMobility = txPhy->GetMobility ()->GetObject<MobilityModel> ();
  SpectrumModelUid_t txSpectrumModelUid = originalTxPowerSpectrum->GetSpectrumModelUid ();
  NS_LOG_LOGIC (" txSpectrumModelUid " << txSpectrumModelUid);

  //
  TxSpectrumModelInfoMap_t::const_iterator txInfoIteratorerator = FindAndEventuallyAddTxSpectrumModel (originalTxPowerSpectrum->GetSpectrumModel ());
  NS_ASSERT (txInfoIteratorerator != m_txSpectrumModelInfoMap.end ());

  NS_LOG_LOGIC ("converter map for TX SpectrumModel with Uid " << txInfoIteratorerator->first);
  NS_LOG_LOGIC ("converter map size: " << txInfoIteratorerator->second.m_spectrumConverterMap.size ());
  NS_LOG_LOGIC ("converter map first element: " << txInfoIteratorerator->second.m_spectrumConverterMap.begin ()->first);

  for (RxSpectrumModelInfoMap_t::const_iterator rxInfoIterator = m_rxSpectrumModelInfoMap.begin ();
       rxInfoIterator != m_rxSpectrumModelInfoMap.end ();
       ++rxInfoIterator)
    {
      SpectrumModelUid_t rxSpectrumModelUid = rxInfoIterator->second.m_rxSpectrumModel->GetUid ();
      NS_LOG_LOGIC (" rxSpectrumModelUids " << rxSpectrumModelUid);

      Ptr <SpectrumValue> convertedTxPowerSpectrum;

      if (txSpectrumModelUid == rxSpectrumModelUid)
        {
          NS_LOG_LOGIC ("no conversion needed");
          convertedTxPowerSpectrum = originalTxPowerSpectrum;
        }
      else
        {
          NS_LOG_LOGIC (" converting txPowerSpectrum SpectrumModelUids" << txSpectrumModelUid << " --> " << rxSpectrumModelUid);
          SpectrumConverterMap_t::const_iterator rxConverterIterator = txInfoIteratorerator->second.m_spectrumConverterMap.find (rxSpectrumModelUid);
          NS_ASSERT (rxConverterIterator != txInfoIteratorerator->second.m_spectrumConverterMap.end ());
          convertedTxPowerSpectrum = rxConverterIterator->second.Convert (originalTxPowerSpectrum);
        }

      std::list<Ptr<SpectrumPhy> >::const_iterator rxPhyIterator = rxInfoIterator->second.m_rxPhyList.begin ();
      while (rxPhyIterator != rxInfoIterator->second.m_rxPhyList.end ())
        {
          NS_ASSERT_MSG ((*rxPhyIterator)->GetRxSpectrumModel ()->GetUid () == rxSpectrumModelUid,
                         "MultiModelSpectrumChannel only supports devices that use a single RxSpectrumModel that does not change for the whole simulation");

          if ((*rxPhyIterator) != txPhy)
            {
              Ptr <SpectrumValue> rxPowerSpectrum;
              Time delay;
              Ptr<MobilityModel> receiverMobility = (*rxPhyIterator)->GetMobility ()->GetObject<MobilityModel> ();

              if (txMobility && receiverMobility)
                {
                  if (m_PropagationLoss)
                    {
                      rxPowerSpectrum = m_PropagationLoss->CalcRxPowerSpectralDensity (convertedTxPowerSpectrum, txMobility, receiverMobility);
                    }
                  else
                    {
                      // rxPowerSpectrum = Copy<SpectrumValue> (convertedTxPowerSpectrum);
                      rxPowerSpectrum = convertedTxPowerSpectrum->Copy ();
                    }

                  if (m_PropagationDelay)
                    {
                      delay = m_PropagationDelay->GetDelay (txMobility, receiverMobility);
                    }
                  else
                    {
                      delay = MicroSeconds (0);
                    }
                }
              else
                {
                  // rxPowerSpectrum = Copy<SpectrumValue> (convertedTxPowerSpectrum);
                  rxPowerSpectrum = convertedTxPowerSpectrum->Copy ();
                  delay = MicroSeconds (0);
                }

              Ptr<PacketBurst> pktBurstCopy = p->Copy ();
              Ptr<Object> netDevObj = (*rxPhyIterator)->GetDevice ();
              if (netDevObj)
                {
                  // the receiver has a NetDevice, so we expect that it is attached to a Node
                  uint32_t dstNode =  netDevObj->GetObject<NetDevice> ()->GetNode ()->GetId ();
                  Simulator::ScheduleWithContext (dstNode, delay, &MultiModelSpectrumChannel::StartRx, this,
                                                  pktBurstCopy, rxPowerSpectrum, st, duration, *rxPhyIterator);
                }
              else
                {
                  // the receiver is not attached to a NetDevice, so we cannot assume that it is attached to a node
                  Simulator::Schedule (delay, &MultiModelSpectrumChannel::StartRx, this,
                                       pktBurstCopy, rxPowerSpectrum, st, duration, *rxPhyIterator);
                }
            }
          ++rxPhyIterator;
        }

    }

}

void
MultiModelSpectrumChannel::StartRx (Ptr<PacketBurst> pb, Ptr <SpectrumValue> rxPsd, SpectrumType st, Time duration, Ptr<SpectrumPhy> receiver)
{
  NS_LOG_FUNCTION (this);
  receiver->StartRx (pb, rxPsd, st, duration);
}



uint32_t
MultiModelSpectrumChannel::GetNDevices (void) const
{
  return m_phyVector.size ();

}


Ptr<NetDevice>
MultiModelSpectrumChannel::GetDevice (uint32_t i) const
{
  return m_phyVector.at (i)->GetDevice ()->GetObject<NetDevice> ();
}



void
MultiModelSpectrumChannel::AddSpectrumPropagationLossModel (Ptr<SpectrumPropagationLossModel> loss)
{
  NS_ASSERT (m_PropagationLoss == 0);
  m_PropagationLoss = loss;
}

void
MultiModelSpectrumChannel::SetPropagationDelayModel (Ptr<PropagationDelayModel> delay)
{
  NS_ASSERT (m_PropagationDelay == 0);
  m_PropagationDelay = delay;
}

Ptr<SpectrumPropagationLossModel>
MultiModelSpectrumChannel::GetSpectrumPropagationLossModel (void)
{
  NS_LOG_FUNCTION (this);
  return m_PropagationLoss;
}


} // namespace ns3
