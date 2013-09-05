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

#include <stdint.h>
#include <vector>
#include <list>
#include "wifi-mode.h"
#include "wifi-preamble.h"
#include "wifi-phy-standard.h"
#include "ns3/nstime.h"
#include "ns3/simple-ref-count.h"
#include "ns3/wifi-tx-vector.h"

namespace ns3 {

class ErrorRateModel;

/**
 * \ingroup wifi
 * \brief handles interference calculations
 */
class InterferenceHelper
{
public:
  class Event : public SimpleRefCount<InterferenceHelper::Event>
  {
public:
    Event (uint32_t size, WifiMode payloadMode,
           enum WifiPreamble preamble,
           Time duration, double rxPower, WifiTxVector txvector);
    ~Event ();

    Time GetDuration (void) const;
    Time GetStartTime (void) const;
    Time GetEndTime (void) const;
    double GetRxPowerW (void) const;
    uint32_t GetSize (void) const;
    WifiMode GetPayloadMode (void) const;
    enum WifiPreamble GetPreambleType (void) const;
    WifiTxVector GetTxVector (void) const;
private:
    uint32_t m_size;
    WifiMode m_payloadMode;
    enum WifiPreamble m_preamble;
    Time m_startTime;
    Time m_endTime;
    double m_rxPowerW;
    WifiTxVector m_txVector;
  };
  struct SnrPer
  {
    double snr;
    double per;
  };

  InterferenceHelper ();
  ~InterferenceHelper ();

  void SetNoiseFigure (double value);
  void SetErrorRateModel (Ptr<ErrorRateModel> rate);

  double GetNoiseFigure (void) const;
  Ptr<ErrorRateModel> GetErrorRateModel (void) const;


  /**
   * \param energyW the minimum energy (W) requested
   * \returns the expected amount of time the observed
   *          energy on the medium will be higher than
   *          the requested threshold.
   */
  Time GetEnergyDuration (double energyW);


  Ptr<InterferenceHelper::Event> Add (uint32_t size, WifiMode payloadMode,
                                      enum WifiPreamble preamble,
                                      Time duration, double rxPower, WifiTxVector txvector);

  struct InterferenceHelper::SnrPer CalculateSnrPer (Ptr<InterferenceHelper::Event> event);
  void NotifyRxStart ();
  void NotifyRxEnd ();
  void EraseEvents (void);
private:
  class NiChange
  {
public:
    NiChange (Time time, double delta);
    Time GetTime (void) const;
    double GetDelta (void) const;
    bool operator < (const NiChange& o) const;
private:
    Time m_time;
    double m_delta;
  };
  typedef std::vector <NiChange> NiChanges;
  typedef std::list<Ptr<Event> > Events;

  InterferenceHelper (const InterferenceHelper &o);
  InterferenceHelper &operator = (const InterferenceHelper &o);
  void AppendEvent (Ptr<Event> event);
  double CalculateNoiseInterferenceW (Ptr<Event> event, NiChanges *ni) const;
  double CalculateSnr (double signal, double noiseInterference, WifiMode mode) const;
  double CalculateChunkSuccessRate (double snir, Time delay, WifiMode mode) const;
  double CalculatePer (Ptr<const Event> event, NiChanges *ni) const;

  double m_noiseFigure; /**< noise figure (linear) */
  Ptr<ErrorRateModel> m_errorRateModel;
  /// Experimental: needed for energy duration calculation
  NiChanges m_niChanges;
  double m_firstPower;
  bool m_rxing;
  /// Returns an iterator to the first nichange, which is later than moment
  NiChanges::iterator GetPosition (Time moment);
  void AddNiChangeEvent (NiChange change);
};

} // namespace ns3

#endif /* INTERFERENCE_HELPER_H */
