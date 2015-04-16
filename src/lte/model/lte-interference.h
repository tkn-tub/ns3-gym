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


#ifndef LTE_INTERFERENCE_H
#define LTE_INTERFERENCE_H

#include <ns3/object.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/spectrum-value.h>

#include <list>

namespace ns3 {




class LteChunkProcessor;



/**
 * This class implements a gaussian interference model, i.e., all
 * incoming signals are added to the total interference.
 *
 */
class LteInterference : public Object
{
public:
  LteInterference ();
  virtual ~LteInterference ();

  // inherited from Object
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  /**
   * Add a LteChunkProcessor that will use the time-vs-frequency SINR
   * calculated by this LteInterference instance. Note that all the
   * added LteChunkProcessors will work in parallel.
   *
   * @param p
   */
  void AddSinrChunkProcessor (Ptr<LteChunkProcessor> p);

  /**
   * Add a LteChunkProcessor that will use the time-vs-frequency
   *  interference calculated by this LteInterference instance. Note 
   *  that all the added LteChunkProcessors will work in parallel.
   *
   * @param p
   */
  void AddInterferenceChunkProcessor (Ptr<LteChunkProcessor> p);

  /**
   * Add a LteChunkProcessor that will use the time-vs-frequency
   *  power calculated by this LteInterference instance. Note
   *  that all the added LteChunkProcessors will work in parallel.
   *
   * @param p
   */
  void AddRsPowerChunkProcessor (Ptr<LteChunkProcessor> p);

  /**
   * notify that the PHY is starting a RX attempt
   *
   * @param rxPsd the power spectral density of the signal being RX
   */
  void StartRx (Ptr<const SpectrumValue> rxPsd);


  /**
   * notify that the RX attempt has ended. The receiving PHY must call
   * this method when RX ends or RX is aborted.
   *
   */
  void EndRx ();


  /**
   * notify that a new signal is being perceived in the medium. This
   * method is to be called for all incoming signal, regardless of
   * wether they're useful signals or interferers.
   *
   * @param spd the power spectral density of the new signal
   * @param duration the duration of the new signal
   */
  void AddSignal (Ptr<const SpectrumValue> spd, const Time duration);


  /**
   *
   * @param noisePsd the Noise Power Spectral Density in power units
   * (Watt, Pascal...) per Hz.
   */
  void SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd);

private:
  void ConditionallyEvaluateChunk ();
  void DoAddSignal  (Ptr<const SpectrumValue> spd);
  void DoSubtractSignal  (Ptr<const SpectrumValue> spd, uint32_t signalId);



  bool m_receiving;

  Ptr<SpectrumValue> m_rxSignal; /**< stores the power spectral density of
                                  * the signal whose RX is being
                                  * attempted
                                  */

  Ptr<SpectrumValue> m_allSignals; /**< stores the spectral
                                    * power density of the sum of incoming signals;
                                    * does not include noise, includes the SPD of the signal being RX
                                    */

  Ptr<const SpectrumValue> m_noise;

  Time m_lastChangeTime;     /**< the time of the last change in
                                m_TotalPower */

  uint32_t m_lastSignalId;
  uint32_t m_lastSignalIdBeforeReset;

  /** all the processor instances that need to be notified whenever
  a new interference chunk is calculated */
  std::list<Ptr<LteChunkProcessor> > m_rsPowerChunkProcessorList;

  /** all the processor instances that need to be notified whenever
      a new SINR chunk is calculated */
  std::list<Ptr<LteChunkProcessor> > m_sinrChunkProcessorList;

  /** all the processor instances that need to be notified whenever
      a new interference chunk is calculated */
  std::list<Ptr<LteChunkProcessor> > m_interfChunkProcessorList;


};



} // namespace ns3





#endif /* LTE_INTERFERENCE_H */
