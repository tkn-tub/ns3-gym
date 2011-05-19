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

#ifndef SPECTRUM_PHY_H
#define SPECTRUM_PHY_H


#include <ns3/object.h>
#include <ns3/nstime.h>
#include <ns3/spectrum-type.h>

namespace ns3 {

class PacketBurst;
class SpectrumChannel;
class MobilityModel;
class SpectrumValue;
class SpectrumModel;
class NetDevice;

/**
 * \ingroup spectrum
 *
 * Abstract base class for Spectrum-aware PHY layers
 *
 */
class SpectrumPhy  : public Object
{

public:
  virtual ~SpectrumPhy ();

  static TypeId GetTypeId (void);

  /**
   * set the associated NetDevice instance
   *
   * @param d the NetDevice instance
   */
  virtual void SetDevice (Ptr<Object> d) = 0;

  /**
   * get the associated NetDevice instance
   *
   * @return a Ptr to the associated NetDevice instance
   */
  virtual Ptr<Object> GetDevice () = 0;

  /**
   * Set the mobility model associated with this device.
   *
   * @param m the mobility model
   */
  virtual void SetMobility (Ptr<Object> m) = 0;

  /**
   * get the associated MobilityModel instance
   *
   * @return a Ptr to the associated NetDevice instance
   */
  virtual Ptr<Object> GetMobility () = 0;


  /**
   * Set the channel attached to this device.
   *
   * @param c the channel
   */
  virtual void SetChannel (Ptr<SpectrumChannel> c) = 0;

  /**
   *
   * @return returns the SpectrumModel that this SpectrumPhy expects to be used
   * for all SpectrumValues that are passed to StartRx. If 0 is
   * returned, it means that any model will be accepted.
   */
  virtual Ptr<const SpectrumModel> GetRxSpectrumModel () const = 0;

  /**
   * Notify the SpectrumPhy instance of an incoming waveform
   *
   * @param p the PacketBurst associated with the incoming waveform
   * @param rxPsd the Power Spectral Density of the incoming
   * waveform. The units of the PSD are the same specified for SpectrumChannel::StartTx().
   * @param st spectrum type
   * @param duration the duration of the incoming waveform
   */
  virtual void StartRx (Ptr<PacketBurst> p, Ptr <const SpectrumValue> rxPsd, SpectrumType st, Time duration) = 0;


};









} // namespace ns3





#endif /* SPECTRUM_PHY_H */
