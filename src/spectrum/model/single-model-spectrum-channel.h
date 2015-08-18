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

#ifndef SINGLE_MODEL_SPECTRUM_CHANNEL_H
#define SINGLE_MODEL_SPECTRUM_CHANNEL_H


#include <ns3/spectrum-channel.h>
#include <ns3/spectrum-model.h>
#include <ns3/traced-callback.h>

namespace ns3 {



/**
 * \ingroup spectrum
 *
 * @brief SpectrumChannel implementation which handles a single spectrum model
 *
 * All SpectrumPhy layers attached to this SpectrumChannel
 */
class SingleModelSpectrumChannel : public SpectrumChannel
{

public:
  SingleModelSpectrumChannel ();

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


  typedef std::vector<Ptr<SpectrumPhy> > PhyList;

  virtual Ptr<SpectrumPropagationLossModel> GetSpectrumPropagationLossModel (void);

private:
  virtual void DoDispose ();

  /**
   * used internally to reschedule transmission after the propagation delay
   *
   * @param params
   * @param receiver
   */
  void StartRx (Ptr<SpectrumSignalParameters> params, Ptr<SpectrumPhy> receiver);

  /**
   * list of SpectrumPhy instances attached to
   * the channel
   */
  PhyList m_phyList;

  /**
   * SpectrumModel that this channel instance
   * is supporting
   */
  Ptr<const SpectrumModel> m_spectrumModel;


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


  double m_maxLossDb;

  /**
   * \deprecated The non-const \c Ptr<SpectrumPhy> argument
   * is deprecated and will be changed to \c Ptr<const SpectrumPhy>
   * in a future release.
   */
  TracedCallback<Ptr<SpectrumPhy>, Ptr<SpectrumPhy>, double > m_pathLossTrace;
};



}



#endif /* SINGLE_MODEL_SPECTRUM_CHANNEL_H */
