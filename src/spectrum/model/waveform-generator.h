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

#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H


#include <ns3/spectrum-value.h>
#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/net-device.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-channel.h>
#include <ns3/trace-source-accessor.h>
#include <ns3/event-id.h>

namespace ns3 {


class AntennaModel;

/**
 * \ingroup spectrum
 *
 * Simple SpectrumPhy implementation that sends customizable waveform.
 * The generated waveforms have a given Spectrum Power Density and
 * duration (set with the SetResolution()) . The generator activates
 * and deactivates periodically with a given period and with a duty
 * cycle of 1/2.
 *
 * This PHY model supports a single antenna model instance which is
 * used for both transmission and reception (though received signals
 * are discarded by this PHY).
 */
class WaveformGenerator : public SpectrumPhy
{

public:
  WaveformGenerator ();
  virtual ~WaveformGenerator ();

  static TypeId GetTypeId (void);

  // inherited from SpectrumPhy
  void SetChannel (Ptr<SpectrumChannel> c);
  void SetMobility (Ptr<MobilityModel> m);
  void SetDevice (Ptr<NetDevice> d);
  Ptr<MobilityModel> GetMobility ();
  Ptr<NetDevice> GetDevice ();
  Ptr<const SpectrumModel> GetRxSpectrumModel () const;
  Ptr<AntennaModel> GetRxAntenna ();
  void StartRx (Ptr<SpectrumSignalParameters> params);


  /**
   * Set the Power Spectral Density used for outgoing waveforms
   *
   * @param txs the Power Spectral Density
   */
  void SetTxPowerSpectralDensity (Ptr<SpectrumValue> txs);

  /**
   * Set the period according to which the WaveformGenerator switches
   * on and off
   *
   * @param period
   */
  void SetPeriod (Time period);


  /**
   *
   * @return the value of the period according to which the WaveformGenerator switches
   * on and off
   */
  Time GetPeriod () const;


  /**
   *
   * @param value the value of the duty cycle
   */
  void SetDutyCycle (double value);

  /**
   *
   * @return the value of the duty cycle
   */
  double GetDutyCycle () const;

  /**
   * set the AntennaModel to be used
   *
   * \param a the Antenna Model
   */
  void SetAntenna (Ptr<AntennaModel> a);

  /**
   * Start the waveform generator
   *
   */
  virtual void Start ();

  /**
   * Stop the waveform generator
   *
   */
  virtual void Stop ();


private:
  virtual void DoDispose (void);

  Ptr<MobilityModel> m_mobility;
  Ptr<AntennaModel> m_antenna;
  Ptr<NetDevice> m_netDevice;
  Ptr<SpectrumChannel> m_channel;

  virtual void GenerateWaveform ();

  Ptr<SpectrumValue> m_txPowerSpectralDensity;
  Time   m_period;
  double m_dutyCycle;
  Time m_startTime;
  EventId m_nextWave;

  TracedCallback<Ptr<const Packet> > m_phyTxStartTrace;
  TracedCallback<Ptr<const Packet> > m_phyTxEndTrace;
};






} // namespace ns3





#endif /* WAVEFORM_GENERATOR_H */
