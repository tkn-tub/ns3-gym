/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef INTERFERENCE_HELPER_H
#define INTERFERENCE_HELPER_H

#include "ns3/nstime.h"
#include "ns3/packet.h"
#include "wifi-tx-vector.h"
#include "error-rate-model.h"

namespace ns3 {

/**
 * \ingroup wifi
 * \brief handles interference calculations
 */
class InterferenceHelper
{
public:
  /**
   * Signal event for a packet.
   */
  class Event : public SimpleRefCount<InterferenceHelper::Event>
  {
public:
    /**
     * Create an Event with the given parameters.
     *
     * \param packet the packet
     * \param txVector TXVECTOR of the packet
     * \param duration duration of the signal
     * \param rxPower the receive power (w)
     */
    Event (Ptr<const Packet> packet, WifiTxVector txVector, Time duration, double rxPower);
    ~Event ();

    /** Return the packet.
     *
     * \return the packet
     */
    Ptr<const Packet> GetPacket (void) const;
    /**
     * Return the start time of the signal.
     *
     * \return the start time of the signal
     */
    Time GetStartTime (void) const;
    /**
     * Return the end time of the signal.
     *
     * \return the end time of the signal
     */
    Time GetEndTime (void) const;
    /**
     * Return the receive power (w).
     *
     * \return the receive power (w)
     */
    double GetRxPowerW (void) const;
    /**
     * Return the TXVECTOR of the packet.
     *
     * \return the TXVECTOR of the packet
     */
    WifiTxVector GetTxVector (void) const;
    /**
     * Return the Wi-Fi mode used for the payload.
     *
     * \return the Wi-Fi mode used for the payload
     */
    WifiMode GetPayloadMode (void) const;


private:
    Ptr<const Packet> m_packet; ///< packet
    WifiTxVector m_txVector; ///< TXVECTOR
    Time m_startTime; ///< start time
    Time m_endTime; ///< end time
    double m_rxPowerW; ///< receive power in watts
  };

  /**
   * A struct for both SNR and PER
   */
  struct SnrPer
  {
    double snr; ///< SNR
    double per; ///< PER
  };

  InterferenceHelper ();
  ~InterferenceHelper ();

  /**
   * Set the noise figure.
   *
   * \param value noise figure
   */
  void SetNoiseFigure (double value);
  /**
   * Set the error rate model for this interference helper.
   *
   * \param rate Error rate model
   */
  void SetErrorRateModel (const Ptr<ErrorRateModel> rate);

  /**
   * Return the noise figure.
   *
   * \return the noise figure
   */
  double GetNoiseFigure (void) const;
  /**
   * Return the error rate model.
   *
   * \return Error rate model
   */
  Ptr<ErrorRateModel> GetErrorRateModel (void) const;
  /**
   * Set the number of RX antennas in the receiver corresponding to this
   * interference helper.
   *
   * \param rx the number of RX antennas
   */
  void SetNumberOfReceiveAntennas (uint8_t rx);

  /**
   * \param energyW the minimum energy (W) requested
   *
   * \returns the expected amount of time the observed
   *          energy on the medium will be higher than
   *          the requested threshold.
   */
  Time GetEnergyDuration (double energyW) const;

  /**
   * Add the packet-related signal to interference helper.
   *
   * \param packet the packet
   * \param txVector TXVECTOR of the packet
   * \param duration the duration of the signal
   * \param rxPower receive power (W)
   *
   * \return InterferenceHelper::Event
   */
  Ptr<InterferenceHelper::Event> Add (Ptr<const Packet> packet, WifiTxVector txVector, Time duration, double rxPower);

  /**
   * Add a non-Wifi signal to interference helper.
   * \param duration the duration of the signal
   * \param rxPower receive power (W)
   */
  void AddForeignSignal (Time duration, double rxPower);
  /**
   * Calculate the SNIR at the start of the plcp payload and accumulate
   * all SNIR changes in the snir vector.
   *
   * \param event the event corresponding to the first time the corresponding packet arrives
   *
   * \return struct of SNR and PER
   */
  struct InterferenceHelper::SnrPer CalculatePlcpPayloadSnrPer (Ptr<InterferenceHelper::Event> event);
  /**
   * Calculate the SNIR at the start of the plcp header and accumulate
   * all SNIR changes in the snir vector.
   *
   * \param event the event corresponding to the first time the corresponding packet arrives
   *
   * \return struct of SNR and PER
   */
  struct InterferenceHelper::SnrPer CalculatePlcpHeaderSnrPer (Ptr<InterferenceHelper::Event> event);

  /**
   * Notify that RX has started.
   */
  void NotifyRxStart ();
  /**
   * Notify that RX has ended.
   */
  void NotifyRxEnd ();
  /**
   * Erase all events.
   */
  void EraseEvents (void);


private:
  /**
   * Noise and Interference (thus Ni) event.
   */
  class NiChange
  {
public:
    /**
     * Create a NiChange at the given time and the amount of NI change.
     *
     * \param time time of the event
     * \param delta the power
     * \param event causes this NI change
     */
    NiChange (Time time, double delta, Ptr<InterferenceHelper::Event> event);
    /**
     * Return the event time.
     *
     * \return the event time.
     */
    Time GetTime (void) const;
    /**
     * Return the power
     *
     * \return the power
     */
    double GetDelta (void) const;
    /**
     * Return the event causes the corresponding NI change
     *
     * \return the event
     */
    Ptr<InterferenceHelper::Event> GetEvent (void) const;
    /**
     * Compare the event time of two NiChange objects (a < o).
     *
     * \param o
     * \return true if a < o.time, false otherwise
     */
    bool operator < (const NiChange& o) const;


private:
    Time m_time; ///< time
    double m_delta; ///< delta
    Ptr<InterferenceHelper::Event> m_event; ///< event
  };
  /**
   * typedef for a vector of NiChanges
   */
  typedef std::vector <NiChange> NiChanges;

  /**
   * Append the given Event.
   *
   * \param event
   */
  void AppendEvent (Ptr<Event> event);
  /**
   * Calculate noise and interference power in W.
   *
   * \param event
   * \param ni
   *
   * \return noise and interference power
   */
  double CalculateNoiseInterferenceW (Ptr<Event> event, NiChanges *ni) const;
  /**
   * Calculate SNR (linear ratio) from the given signal power and noise+interference power.
   * (Mode is not currently used)
   *
   * \param signal
   * \param noiseInterference
   * \param channelWidth
   *
   * \return SNR in liear ratio
   */
  double CalculateSnr (double signal, double noiseInterference, uint8_t channelWidth) const;
  /**
   * Calculate the success rate of the chunk given the SINR, duration, and Wi-Fi mode.
   * The duration and mode are used to calculate how many bits are present in the chunk.
   *
   * \param snir SINR
   * \param duration
   * \param mode
   * \param txVector
   *
   * \return the success rate
   */
  double CalculateChunkSuccessRate (double snir, Time duration, WifiMode mode, WifiTxVector txVector) const;
  /**
   * Calculate the error rate of the given plcp payload. The plcp payload can be divided into
   * multiple chunks (e.g. due to interference from other transmissions).
   *
   * \param event
   * \param ni
   *
   * \return the error rate of the packet
   */
  double CalculatePlcpPayloadPer (Ptr<const Event> event, NiChanges *ni) const;
  /**
   * Calculate the error rate of the plcp header. The plcp header can be divided into
   * multiple chunks (e.g. due to interference from other transmissions).
   *
   * \param event
   * \param ni
   *
   * \return the error rate of the packet
   */
  double CalculatePlcpHeaderPer (Ptr<const Event> event, NiChanges *ni) const;

  double m_noiseFigure; /**< noise figure (linear) */
  Ptr<ErrorRateModel> m_errorRateModel; ///< error rate model
  uint8_t m_numRxAntennas; /**< the number of RX antennas in the corresponding receiver */
  /// Experimental: needed for energy duration calculation
  NiChanges m_niChanges;
  double m_firstPower; ///< first power
  bool m_rxing; ///< flag whether it is in receiving state

  /**
   * Returns a const iterator to the first nichange, which is later than moment
   *
   * \param moment time to check from
   * \returns an iterator to the list of NiChanges
   */
  NiChanges::const_iterator GetPosition (Time moment);
  /**
   * Add NiChange to the list at the appropriate position.
   *
   * \param change
   */
  void AddNiChangeEvent (NiChange change);
};

} //namespace ns3

#endif /* INTERFERENCE_HELPER_H */
