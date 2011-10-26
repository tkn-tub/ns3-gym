/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#ifndef UAN_PROP_MODEL_H
#define UAN_PROP_MODEL_H

#include "ns3/object.h"
#include "ns3/mobility-model.h"
#include "ns3/nstime.h"


#include <vector>
#include <complex>
#include <utility>

namespace ns3 {

class UanTxMode;

/**
 * \class Tap
 * \brief Holds PDP Tap information (amplitude and delay)
 */
class Tap
{
public:
  /**
   * Default constructor.  Creates Tap with delay=0, amp=0
   */
  Tap ();
  /**
   * \param delay Time delay (usually from first arrival) of signal
   * \param amp Complex amplitude of arrival
   */
  Tap (Time delay, std::complex<double> amp);
  /**
   * \returns Complex amplitude of arrival
   */
  std::complex<double> GetAmp (void) const;
  /**
   * \returns Time delay (usually from first arrival) of signal
   */
  Time GetDelay (void) const;

private:
  std::complex<double> m_amplitude;
  Time m_delay;
};

/**
 * \class UanPdp
 *
 * Container class to describe power delay profile returned
 * from UAN propagation models using tapped delay line model.
 * This should model a channel impulse response as a set of
 * equally spaced signal arrivals.
 *
 * Generally, the profile should be normalized, such that
 * the sum of all taps should equal 1.  The received signal
 * power on any interval (t1, t2) can then be found from
 * summing the taps on the interval and multiplying by
 * the total received power at the receiver.
 *
 *
 */
class UanPdp
{
public:
  /**
   * \brief Iterator is a standard template library constant interator
   */
  typedef std::vector<Tap>::const_iterator Iterator;
  /**
   * \brief Create empty PDP object
   */
  UanPdp ();
  /**
   * \brief Create PDP object from a vector of Tap objects
   * \param taps Taps to include in this PDP
   * \param resolution Resolution of PDP object
   */
  UanPdp (std::vector<Tap> taps, Time resolution);
  /**
   * \brief Create PDP object from vector of arrival amplitudes
   * \param arrivals  Vector of complex amplitude arrivals
   * \param resolution Time duration between arrivals in vector
   */
  UanPdp (std::vector<std::complex<double > > arrivals, Time resolution);
  /**
   * \brief Create PDP object from real valued arrival amplitudes
   * \param arrivals vector of real valued arrivals
   * \param resolution Time duration between arrivals in vector
   */
  UanPdp (std::vector<double> arrivals, Time resolution);
  ~UanPdp ();

  /**
   * \param arrival complex arrival value
   * \param index index of arrival
   */
  void SetTap (std::complex<double> arrival, uint32_t index);
  /**
   * \param nTaps number of taps in this PDP
   */
  void SetNTaps (uint32_t nTaps);
  /**
   * \param resolution Resolution of PDP (time between arrivals)
   */
  void SetResolution (Time resolution);
  /**
   * \returns Iterator positioned at first arrival
   */
  Iterator GetBegin (void) const;
  /**
   * \returns Iterator positioned after last arrival
   */
  Iterator GetEnd (void) const;
  /**
   * \returns Number of taps in PDP
   */
  uint32_t GetNTaps (void) const;
  /**
   * \returns Tap object at index i
   * \param i Index number of tap to return (0 based)
   */
  const Tap &GetTap (uint32_t i) const;
  /**
   * \returns Resolution of PDP (time duration between arrivals)
   */
  Time GetResolution (void) const;
  /**
   * \param begin Time value to begin summing arrivals
   * \param end Time value to end summing arrivals
   * \returns Returns Non-coherent sum of arrivals between two time values
   *
   * Assuming that Tap at index 0 arrives at time 0,
   * this function sums non-coherently (sums amplitude of arrivals
   * ignoring phase difference) all arrivals between a start
   * and end time.
   */
  double SumTapsNc (Time begin, Time end) const;
  /**
   * \param begin Time value to begin summing arrivals
   * \param end Time value to end summing arrivals
   * \returns Returns coherent sum of arrivals between two time values
   *
   * Assuming that Tap at index 0 arrives at time 0,
   * this function sums coherently (sums amplitude of arrivals
   * considering phase difference) all arrivals between a start
   * and end time.
   */
  std::complex<double> SumTapsC (Time begin, Time end) const;
  /**
   * \param delay Time duratation after max to begin summing arrivals
   * \param duration Time duration to sum arrivals for
   * \returns non-coherent sum of arrivals after max in given window
   *
   * this function sums non-coherently (sums amplitude of arrivals
   * ignoring phase difference) all arrivals in a given duration
   * starting the given time after the maximum amplitude arrival received
   */
  double SumTapsFromMaxNc (Time delay, Time duration) const;
  /**
   * \param delay Time duratation after max to begin summing arrivals
   * \param duration Time duration to sum arrivals for
   * \returns coherent sum of arrivals after max in given window
   *
   * this function sums coherently (sums amplitude of arrivals
   * considering phase difference) all arrivals in a given duration
   * starting the given time after the maximum amplitude arrival received
   */
  std::complex<double> SumTapsFromMaxC (Time delay, Time duration) const;

  /**
   * \returns A PDP with a singlue unit impulse arrival at time 0
   */ 
  static UanPdp CreateImpulsePdp (void);
private:
  friend std::ostream &operator<< (std::ostream &os, UanPdp &pdp);
  friend std::istream &operator>> (std::istream &is, UanPdp &pdp);
  std::vector<Tap> m_taps;
  Time m_resolution;

};
/**
 * \brief Writes PDP to stream as list of arrivals
 */
std::ostream &operator<< (std::ostream &os, UanPdp &pdp);
/**
 * \brief Reads in list of arrivals from stream is
 */
std::istream &operator>> (std::ostream &is, UanPdp &pdp);

/**
 * \class UanPropModel
 *
 * Base class for implemented underwater propagation models
 */
class UanPropModel : public Object
{
public:
  static TypeId GetTypeId (void);

  /**
   * Computes pathloss between nodes a and b.
   * \returns Pathloss in dB re 1 uPa
   * \param a Ptr to mobility model of node a
   * \param b Ptr to mobility model of node b
   * \param txMode TX mode of transmission between a and b
   */
  virtual double GetPathLossDb (Ptr<MobilityModel> a, Ptr<MobilityModel> b, UanTxMode txMode) = 0;

  /**
   * \returns PDP for link between nodes a and b
   * \param a Ptr to mobility model of node a
   * \param b Ptr to mobility model of node b
   * \param mode TX mode of transmission from a to b
   */
  virtual UanPdp GetPdp (Ptr<MobilityModel> a, Ptr<MobilityModel> b, UanTxMode mode) = 0;
  /**
   * Finds propagation delay between nodes a and b
   * \param a Ptr to mobility model of node a
   * \param b Ptr to mobility model of node b
   * \param mode TX mode of transmission
   * \returns Propagation delay
   */
  virtual Time GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b, UanTxMode mode) = 0;

  /**
   * Clears all pointer references
   */
  virtual void Clear (void);

  virtual void DoDispose (void);
};

}

#endif /* UAN_PROP_MODEL_H */
