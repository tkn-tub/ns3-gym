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


#ifndef MULTI_MODEL_SPECTRUM_CHANNEL_H
#define MULTI_MODEL_SPECTRUM_CHANNEL_H

#include <ns3/spectrum-value.h>
#include <ns3/spectrum-converter.h>
#include <ns3/spectrum-channel.h>
#include <ns3/spectrum-propagation-loss-model.h>
#include <ns3/propagation-delay-model.h>
#include <map>
#include <set>

namespace ns3 {


typedef std::map<SpectrumModelUid_t, SpectrumConverter> SpectrumConverterMap_t;


/**
 * \ingroup spectrum
 *
 */
class TxSpectrumModelInfo
{
public:
  TxSpectrumModelInfo (Ptr<const SpectrumModel> txSpectrumModel);

  Ptr<const SpectrumModel> m_txSpectrumModel;
  SpectrumConverterMap_t m_spectrumConverterMap;
};

typedef std::map<SpectrumModelUid_t, TxSpectrumModelInfo> TxSpectrumModelInfoMap_t;


/**
 * \ingroup spectrum
 *
 */
class RxSpectrumModelInfo
{
public:
  RxSpectrumModelInfo (Ptr<const SpectrumModel> rxSpectrumModel);

  Ptr<const SpectrumModel> m_rxSpectrumModel;
  std::set<Ptr<SpectrumPhy> > m_rxPhySet;
};

typedef std::map<SpectrumModelUid_t, RxSpectrumModelInfo> RxSpectrumModelInfoMap_t;




/**
 * \ingroup spectrum
 *
 * This SpectrumChannel implementation can handle the presence of
 * SpectrumPhy instances which can use
 * different spectrum models, i.e.,  different SpectrumModel. 
 *
 * \note It is allowed for a receiving SpectrumPhy to switch to a
 * different SpectrumModel during the simulation. The requirement
 * for this to work is that, after the SpectrumPhy switched its
 * SpectrumModel,  MultiModelSpectrumChannel::AddRx () is
 * called again passing the pointer to that SpectrumPhy.
 */
class MultiModelSpectrumChannel : public SpectrumChannel
{

public:
  MultiModelSpectrumChannel ();

  static TypeId GetTypeId (void);

  // inherited from SpectrumChannel
  virtual void AddPropagationLossModel (Ptr<PropagationLossModel> loss);
  virtual void AddSpectrumPropagationLossModel (Ptr<SpectrumPropagationLossModel> loss);
  virtual void SetPropagationDelayModel (Ptr<PropagationDelayModel> delay);
  virtual void AddRx (Ptr<SpectrumPhy> phy);
  virtual void StartTx (Ptr<SpectrumSignalParameters> params);


  // inherited from Channel
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

  virtual Ptr<SpectrumPropagationLossModel> GetSpectrumPropagationLossModel (void);


protected:
  void DoDispose ();



private:
  /**
   * this method checks if m_rxSpectrumModelInfoMap contains an entry
   * for the given TX SpectrumModel. If such entry exists, it returns
   * an interator pointing to it. If not, it creates a new entry in
   * m_txSpectrumMpodelInfoMap, and returns an iterator to it.
   *
   * @param txSpectrumModel the TX SpectrumModel  being considered
   *
   * @return an iterator pointing to the corresponding entry in m_txSpectrumModelInfoMap
   */
  TxSpectrumModelInfoMap_t::const_iterator FindAndEventuallyAddTxSpectrumModel (Ptr<const SpectrumModel> txSpectrumModel);

  /**
   * used internally to reschedule transmission after the propagation delay
   *
   * @param params
   * @param receiver
   */
  virtual void StartRx (Ptr<SpectrumSignalParameters> params, Ptr<SpectrumPhy> receiver);



  /**
   * propagation delay model to be used with this channel
   *
   */
  Ptr<PropagationDelayModel> m_propagationDelay;

  /**
    * single-frequency propagation loss model to be used with this channel
    *
    */
  Ptr<PropagationLossModel> m_propagationLoss;

  /**
   * frequency-dependent propagation loss model to be used with this channel
   *
   */
  Ptr<SpectrumPropagationLossModel> m_spectrumPropagationLoss;


  /**
   * data structure holding, for each TX SpectrumModel,  all the
   * converters to any RX SpectrumModel, and all the corresponding
   * SpectrumPhy instances.
   *
   */
  TxSpectrumModelInfoMap_t m_txSpectrumModelInfoMap;


  /**
   * data structure holding, for each RX spectrum model, all the
   * corresponding SpectrumPhy instances.
   *
   */
  RxSpectrumModelInfoMap_t m_rxSpectrumModelInfoMap;

  uint32_t m_numDevices;

  double m_maxLossDb;

  /**
   * \deprecated The non-const \c Ptr<SpectrumPhy> argument
   * is deprecated and will be changed to \c Ptr<const SpectrumPhy>
   * in a future release.
   */
  TracedCallback<Ptr<SpectrumPhy>, Ptr<SpectrumPhy>, double > m_pathLossTrace;
};



}



#endif /* MULTI_MODEL_SPECTRUM_CHANNEL_H */
