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


#ifndef SPECTRUM_CHANNEL_H
#define SPECTRUM_CHANNEL_H

#include <ns3/object.h>
#include <ns3/nstime.h>
#include <ns3/channel.h>
#include <ns3/spectrum-signal-parameters.h>

namespace ns3 {


class PacketBurst;
class SpectrumValue;
class SpectrumPhy;
class SpectrumPropagationLossModel;
class PropagationLossModel;
class PropagationDelayModel;

/**
 * \ingroup spectrum
 *
 * Defines the interface for spectrum-aware channel implementations
 *
 */
class SpectrumChannel : public Channel
{
public:
  virtual ~SpectrumChannel ();
  static TypeId GetTypeId (void);


  /**
   * set the single-frequency propagation loss model to be used
   * \warning only models that do not depend on the TX power should be used.
   *
   * \param loss a pointer to the propagation loss model to be used.
   */
  virtual void AddPropagationLossModel (Ptr<PropagationLossModel> loss) = 0;

  /**
   * set the frequency-dependent propagation loss model to be used
   * \param loss a pointer to the propagation loss model to be used.
   */
  virtual void AddSpectrumPropagationLossModel (Ptr<SpectrumPropagationLossModel> loss) = 0;

  /**
   * set the  propagation delay model to be used
   * \param delay Ptr to the propagation delay model to be used.
   */
  virtual void SetPropagationDelayModel (Ptr<PropagationDelayModel> delay) = 0;


  /**
   * Used by attached PHY instances to transmit signals on the channel
   *
   * @param params the parameters of the signals being transmitted
   */
  virtual void StartTx (Ptr<SpectrumSignalParameters> params) = 0;

  /**
   * @brief add a SpectrumPhy to a channel, so it can receive packets
   *
   * This method is used to attach a SpectrumPhy instance to a
   * SpectrumChannel instance, so that the SpectrumPhy can receive
   * packets sent on that channel. Note that a SpectrumPhy that only
   * transmits (without receiveing ever) does not need to be added to
   * the channel.
   *
   * This method is to be implemented by all classes inheriting from
   * SpectrumChannel.
   *
   * @param phy the SpectrumPhy instance to be added to the channel as
   * a receiver.
   */
  virtual void AddRx (Ptr<SpectrumPhy> phy) = 0;

};



}



#endif /* SPECTRUM_CHANNEL_H */
