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


#ifndef SPECTRUM_CHANNEL_H
#define SPECTRUM_CHANNEL_H

#include <ns3/object.h>
#include <ns3/nstime.h>
#include <ns3/channel.h>
#include <ns3/spectrum-type.h>

namespace ns3 {


class PacketBurst;
class SpectrumValue;
class SpectrumPhy;
class SpectrumPropagationLossModel;
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
   * set the propagation loss model to be used
   * \param loss Ptr to the propagation loss model to be used.
   */
  virtual void AddSpectrumPropagationLossModel (Ptr<SpectrumPropagationLossModel> loss) = 0;

  /**
   * set the  propagation delay model to be used
   * \param delay Ptr to the propagation delay model to be used.
   */
  virtual void SetPropagationDelayModel (Ptr<PropagationDelayModel> delay) = 0;


  /**
   * Used by attached PHY instances to transmit waveforms on the channel
   *
   * @param p the PacketBurst associated with the waveform being transmitted
   * @param txPsd the Power Spectral Density of the
   * waveform, in linear units. The exact unit will depend on the
   * type of transmission medium involved: W for radio communications, Pa for
   * underwater acoustic communications. Other transmission media to be defined.
   * @param st spectrum type
   * @param duration duration of the packet transmission. It is
   * assumed that the Power Spectral Density remains constant for the
   * whole duration of the transmission. In other words, all waveform
   * have a rect shape with respect to time.
   * @param sender the SpectrumPhy instance making this function call
   */
  virtual void StartTx (Ptr<PacketBurst> p, Ptr <SpectrumValue> txPsd, SpectrumType st, Time duration, Ptr<SpectrumPhy> sender) = 0;

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
