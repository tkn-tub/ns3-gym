/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include <ns3/double.h>
#include <ns3/mobility-model.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-converter.h>
#include <ns3/spectrum-propagation-loss-model.h>
#include <ns3/propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <ns3/antenna-model.h>
#include <ns3/angles.h>
#include <iostream>
#include <utility>
#include "multi-model-spectrum-channel.h"


NS_LOG_COMPONENT_DEFINE ("MultiModelSpectrumChannel");


namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (MultiModelSpectrumChannel)
  ;


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
{
  NS_LOG_FUNCTION (this);
}

void
MultiModelSpectrumChannel::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_propagationDelay = 0;
  m_propagationLoss = 0;
  m_spectrumPropagationLoss = 0;
  m_txSpectrumModelInfoMap.clear ();
  m_rxSpectrumModelInfoMap.clear ();
  SpectrumChannel::DoDispose ();
}

TypeId
MultiModelSpectrumChannel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MultiModelSpectrumChannel")
    .SetParent<SpectrumChannel> ()
    .AddConstructor<MultiModelSpectrumChannel> ()
    .AddAttribute ("MaxLossDb",
                   "If a single-frequency PropagationLossModel is used, this value "
                   "represents the maximum loss in dB for which transmissions will be "
                   "passed to the receiving PHY. Signals for which the PropagationLossModel "
                   "returns a loss bigger than this value will not be propagated to the receiver. "
                   "This parameter is to be used to reduce "
                   "the computational load by not propagating signals that are far beyond "
                   "the interference range. Note that the default value corresponds to "
                   "considering all signals for reception. Tune this value with care. ",
                   DoubleValue (1.0e9),
                   MakeDoubleAccessor (&MultiModelSpectrumChannel::m_maxLossDb),
                   MakeDoubleChecker<double> ())
    .AddTraceSource ("PathLoss",
                     "This trace is fired "
                     "whenever a new path loss value is calculated. The first and second parameters "
                     "to the trace are pointers respectively to the TX and RX SpectrumPhy instances, "
                     "whereas the third parameters is the loss value in dB. Note that the loss value "
                     "reported by this trace is the single-frequency loss value obtained by evaluating "
                     "only the TX and RX AntennaModels and the PropagationLossModel. In particular, note that "
                     "SpectrumPropagationLossModel (even if present) is never used to evaluate the loss value "
                     "reported in this trace. ",
                     MakeTraceSourceAccessor (&MultiModelSpectrumChannel::m_pathLossTrace))
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

  // remove a previous entry of this phy if it exists
  // we need to scan for all rxSpectrumModel values since we don't
  // know which spectrum model the phy had when it was previously added
  // (it's probably different than the current one)
  for (RxSpectrumModelInfoMap_t::iterator rxInfoIterator = m_rxSpectrumModelInfoMap.begin ();
       rxInfoIterator !=  m_rxSpectrumModelInfoMap.end ();
       ++rxInfoIterator)
    {
      std::set<Ptr<SpectrumPhy> >::iterator phyIt = rxInfoIterator->second.m_rxPhySet.find (phy);
      if (phyIt !=  rxInfoIterator->second.m_rxPhySet.end ())
        {
          rxInfoIterator->second.m_rxPhySet.erase (phyIt);
          --m_numDevices;
          break; // there should be at most one entry
        }       
    }

  ++m_numDevices;

  RxSpectrumModelInfoMap_t::iterator rxInfoIterator = m_rxSpectrumModelInfoMap.find (rxSpectrumModelUid);

  if (rxInfoIterator == m_rxSpectrumModelInfoMap.end ())
    {
      // spectrum model unknown, add it to the list of RxSpectrumModels
      std::pair<RxSpectrumModelInfoMap_t::iterator, bool> ret;
      ret = m_rxSpectrumModelInfoMap.insert (std::make_pair (rxSpectrumModelUid, RxSpectrumModelInfo (rxSpectrumModel)));
      NS_ASSERT (ret.second);
      // also add the phy to the newly created set of SpectrumPhy for this RxSpectrumModel
      std::pair<std::set<Ptr<SpectrumPhy> >::iterator, bool> ret2 = ret.first->second.m_rxPhySet.insert (phy);
      NS_ASSERT (ret2.second);

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
      std::pair<std::set<Ptr<SpectrumPhy> >::iterator, bool> ret2 = rxInfoIterator->second.m_rxPhySet.insert (phy);
      NS_ASSERT (ret2.second);
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
MultiModelSpectrumChannel::StartTx (Ptr<SpectrumSignalParameters> txParams)
{
  NS_LOG_FUNCTION (this << txParams);

  NS_ASSERT (txParams->txPhy);
  NS_ASSERT (txParams->psd);


  Ptr<MobilityModel> txMobility = txParams->txPhy->GetMobility ();
  SpectrumModelUid_t txSpectrumModelUid = txParams->psd->GetSpectrumModelUid ();
  NS_LOG_LOGIC (" txSpectrumModelUid " << txSpectrumModelUid);

  //
  TxSpectrumModelInfoMap_t::const_iterator txInfoIteratorerator = FindAndEventuallyAddTxSpectrumModel (txParams->psd->GetSpectrumModel ());
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
          NS_LOG_LOGIC ("no spectrum conversion needed");
          convertedTxPowerSpectrum = txParams->psd;
        }
      else
        {
          NS_LOG_LOGIC (" converting txPowerSpectrum SpectrumModelUids" << txSpectrumModelUid << " --> " << rxSpectrumModelUid);
          SpectrumConverterMap_t::const_iterator rxConverterIterator = txInfoIteratorerator->second.m_spectrumConverterMap.find (rxSpectrumModelUid);
          NS_ASSERT (rxConverterIterator != txInfoIteratorerator->second.m_spectrumConverterMap.end ());
          convertedTxPowerSpectrum = rxConverterIterator->second.Convert (txParams->psd);
        }


      for (std::set<Ptr<SpectrumPhy> >::const_iterator rxPhyIterator = rxInfoIterator->second.m_rxPhySet.begin ();
           rxPhyIterator != rxInfoIterator->second.m_rxPhySet.end ();
           ++rxPhyIterator)
        {
          NS_ASSERT_MSG ((*rxPhyIterator)->GetRxSpectrumModel ()->GetUid () == rxSpectrumModelUid,
                         "SpectrumModel change was not notified to MultiModelSpectrumChannel (i.e., AddRx should be called again after model is changed)");

          if ((*rxPhyIterator) != txParams->txPhy)
            {
              NS_LOG_LOGIC (" copying signal parameters " << txParams);
              Ptr<SpectrumSignalParameters> rxParams = txParams->Copy ();
              rxParams->psd = Copy<SpectrumValue> (convertedTxPowerSpectrum);
              Time delay = MicroSeconds (0);

              Ptr<MobilityModel> receiverMobility = (*rxPhyIterator)->GetMobility ();

              if (txMobility && receiverMobility)
                {
                  double pathLossDb = 0;
                  if (rxParams->txAntenna != 0)
                    {
                      Angles txAngles (receiverMobility->GetPosition (), txMobility->GetPosition ());
                      double txAntennaGain = rxParams->txAntenna->GetGainDb (txAngles);
                      NS_LOG_LOGIC ("txAntennaGain = " << txAntennaGain << " dB");
                      pathLossDb -= txAntennaGain;
                    }
                  Ptr<AntennaModel> rxAntenna = (*rxPhyIterator)->GetRxAntenna ();
                  if (rxAntenna != 0)
                    {
                      Angles rxAngles (txMobility->GetPosition (), receiverMobility->GetPosition ());
                      double rxAntennaGain = rxAntenna->GetGainDb (rxAngles);
                      NS_LOG_LOGIC ("rxAntennaGain = " << rxAntennaGain << " dB");
                      pathLossDb -= rxAntennaGain;
                    }
                  if (m_propagationLoss)
                    {
                      double propagationGainDb = m_propagationLoss->CalcRxPower (0, txMobility, receiverMobility);
                      NS_LOG_LOGIC ("propagationGainDb = " << propagationGainDb << " dB");
                      pathLossDb -= propagationGainDb;
                    }                    
                  NS_LOG_LOGIC ("total pathLoss = " << pathLossDb << " dB");    
                  m_pathLossTrace (txParams->txPhy, *rxPhyIterator, pathLossDb);
                  if ( pathLossDb > m_maxLossDb)
                    {
                      // beyond range
                      continue;
                    }
                  double pathGainLinear = std::pow (10.0, (-pathLossDb) / 10.0);
                  *(rxParams->psd) *= pathGainLinear;              

                  if (m_spectrumPropagationLoss)
                    {
                      rxParams->psd = m_spectrumPropagationLoss->CalcRxPowerSpectralDensity (rxParams->psd, txMobility, receiverMobility);
                    }

                  if (m_propagationDelay)
                    {
                      delay = m_propagationDelay->GetDelay (txMobility, receiverMobility);
                    }
                }

              Ptr<NetDevice> netDev = (*rxPhyIterator)->GetDevice ();
              if (netDev)
                {
                  // the receiver has a NetDevice, so we expect that it is attached to a Node
                  uint32_t dstNode =  netDev->GetNode ()->GetId ();
                  Simulator::ScheduleWithContext (dstNode, delay, &MultiModelSpectrumChannel::StartRx, this,
                                                  rxParams, *rxPhyIterator);
                }
              else
                {
                  // the receiver is not attached to a NetDevice, so we cannot assume that it is attached to a node
                  Simulator::Schedule (delay, &MultiModelSpectrumChannel::StartRx, this,
                                       rxParams, *rxPhyIterator);
                }
            }
        }

    }

}

void
MultiModelSpectrumChannel::StartRx (Ptr<SpectrumSignalParameters> params, Ptr<SpectrumPhy> receiver)
{
  NS_LOG_FUNCTION (this);
  receiver->StartRx (params);
}



uint32_t
MultiModelSpectrumChannel::GetNDevices (void) const
{
  return m_numDevices;

}


Ptr<NetDevice>
MultiModelSpectrumChannel::GetDevice (uint32_t i) const
{
  NS_ASSERT (i < m_numDevices);
  // this method implementation is computationally intensive. This
  // method would be faster if we actually used a std::vector for
  // storing devices, which we don't due to the need to have fast 
  // SpectrumModel conversions and to allow PHY devices to changea
  // SpectrumModel at run time. Note that having this method slow is
  // acceptable as it is not used much at run time (often not at all).
  // On the other hand, having slow SpectrumModel conversion would be
  // less acceptable. 
  uint32_t j = 0;
  for (RxSpectrumModelInfoMap_t::const_iterator rxInfoIterator = m_rxSpectrumModelInfoMap.begin ();
       rxInfoIterator !=  m_rxSpectrumModelInfoMap.end ();
       ++rxInfoIterator)
    {
      for (std::set<Ptr<SpectrumPhy> >::const_iterator phyIt = rxInfoIterator->second.m_rxPhySet.begin ();
           phyIt != rxInfoIterator->second.m_rxPhySet.end ();
           ++phyIt)        
        {
          if (j == i)
            {
              return (*phyIt)->GetDevice ();
            }
        }
    }
  NS_FATAL_ERROR ("m_numDevice > actual number of devices");
  return 0;
}



void
MultiModelSpectrumChannel::AddPropagationLossModel (Ptr<PropagationLossModel> loss)
{
  NS_LOG_FUNCTION (this << loss);
  NS_ASSERT (m_propagationLoss == 0);
  m_propagationLoss = loss;
}

void
MultiModelSpectrumChannel::AddSpectrumPropagationLossModel (Ptr<SpectrumPropagationLossModel> loss)
{
  NS_ASSERT (m_spectrumPropagationLoss == 0);
  m_spectrumPropagationLoss = loss;
}

void
MultiModelSpectrumChannel::SetPropagationDelayModel (Ptr<PropagationDelayModel> delay)
{
  NS_ASSERT (m_propagationDelay == 0);
  m_propagationDelay = delay;
}

Ptr<SpectrumPropagationLossModel>
MultiModelSpectrumChannel::GetSpectrumPropagationLossModel (void)
{
  NS_LOG_FUNCTION (this);
  return m_spectrumPropagationLoss;
}


} // namespace ns3
