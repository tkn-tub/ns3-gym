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

#ifndef SPECTRUM_ANALYZER_H
#define SPECTRUM_ANALYZER_H


#include <ns3/spectrum-value.h>
#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/net-device.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-channel.h>
#include <string>
#include <fstream>

namespace ns3 {


/**
 * \ingroup spectrum
 *
 * Simple SpectrumPhy implemetation that averages the spectrum power
 * density of incoming transmissions to produce a spectrogram.
 *
 */
class SpectrumAnalyzer : public SpectrumPhy
{

public:
  SpectrumAnalyzer ();
  virtual ~SpectrumAnalyzer ();

  static TypeId GetTypeId (void);

// inherited from SpectrumPhy
  void SetChannel (Ptr<SpectrumChannel> c);
  void SetMobility (Ptr<MobilityModel> m);
  void SetDevice (Ptr<NetDevice> d);
  Ptr<MobilityModel> GetMobility ();
  Ptr<NetDevice> GetDevice ();
  Ptr<const SpectrumModel> GetRxSpectrumModel () const;
  void StartRx (Ptr<SpectrumSignalParameters> params);


  /**
   * Set the spectrum model used by the SpectrumAnalyzer to represent incoming signals
   *
   * @param m
   */
  void SetRxSpectrumModel (Ptr<SpectrumModel> m);


  /**
   * Start the spectrum analyzer
   *
   */
  virtual void Start ();

  /**
   * Stop the spectrum analyzer
   *
   */
  virtual void Stop ();


protected:
  void DoDispose ();

private:
  Ptr<MobilityModel> m_mobility;
  Ptr<NetDevice> m_netDevice;
  Ptr<SpectrumChannel> m_channel;

  virtual void GenerateReport ();

  void AddSignal (Ptr<const SpectrumValue> psd);
  void SubtractSignal  (Ptr<const SpectrumValue> psd);
  void UpdateEnergyReceivedSoFar ();

  Ptr<SpectrumModel> m_spectrumModel;
  Ptr<SpectrumValue> m_sumPowerSpectralDensity;
  Ptr<SpectrumValue> m_energySpectralDensity;
  double m_noisePowerSpectralDensity;
  Time m_resolution;
  Time m_lastChangeTime;
  bool m_active;

  TracedCallback<Ptr<const SpectrumValue> > m_averagePowerSpectralDensityReportTrace;

};






}





#endif /* SPECTRUM_ANALYZER_H */
