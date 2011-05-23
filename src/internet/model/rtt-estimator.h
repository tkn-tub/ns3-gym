/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: Rajib Bhattacharjea<raj.b@gatech.edu>
//

// Georgia Tech Network Simulator - Round Trip Time Estimation Class
// George F. Riley.  Georgia Tech, Spring 2002


#ifndef __rtt_estimator_h__
#define __rtt_estimator_h__

#include <deque>
#include "ns3/sequence-number.h"
#include "ns3/nstime.h"
#include "ns3/object.h"

namespace ns3 {

/**
 * \ingroup tcp
 *
 * \brief Implements several variations of round trip time estimators
 */
class RttHistory {
public:
  RttHistory (SequenceNumber32 s, uint32_t c, Time t);
  RttHistory (const RttHistory& h); // Copy constructor
public:
  SequenceNumber32  seq;    // First sequence number in packet sent
  uint32_t        count;  // Number of bytes sent
  Time            time;   // Time this one was sent
  bool            retx;   // True if this has been retransmitted
};

typedef std::deque<RttHistory> RttHistory_t;

class RttEstimator : public Object {  //  Base class for all RTT Estimators
public:
  static TypeId GetTypeId (void);

  RttEstimator();
  RttEstimator(const RttEstimator&); // Copy constructor
  virtual ~RttEstimator();

  virtual void SentSeq (SequenceNumber32, uint32_t);
  virtual Time AckSeq (SequenceNumber32);
  virtual void ClearSent ();
  virtual void   Measurement (Time t) = 0;
  virtual Time RetransmitTimeout () = 0;
  void Init (SequenceNumber32 s) { next = s; }
  virtual Ptr<RttEstimator> Copy () const = 0;
  virtual void IncreaseMultiplier ();
  virtual void ResetMultiplier ();
  virtual void Reset ();

  void SetMinRto (Time minRto);
  Time GetMinRto (void) const;
  void SetEstimate (Time estimate);
  Time GetEstimate (void) const;

private:
  SequenceNumber32        next;    // Next expected sequence to be sent
  RttHistory_t history; // List of sent packet
  double m_maxMultiplier;
public:
  int64x64_t       est;     // Current estimate
  int64x64_t       minrto; // minimum value of the timeout
  uint32_t      nSamples; // Number of samples
  double       multiplier;   // RTO Multiplier
};

// The "Mean-Deviation" estimator, as discussed by Van Jacobson
// "Congestion Avoidance and Control", SIGCOMM 88, Appendix A

//Doc:Class Class {\tt RttMeanDeviation} implements the "Mean--Deviation" estimator
//Doc:Class as described by Van Jacobson
//Doc:Class "Congestion Avoidance and Control", SIGCOMM 88, Appendix A
class RttMeanDeviation : public RttEstimator {
public:
  static TypeId GetTypeId (void);

  RttMeanDeviation ();


  //Doc:Method
  RttMeanDeviation (const RttMeanDeviation&); // Copy constructor
  //Doc:Desc Copy constructor.
  //Doc:Arg1 {\tt RttMeanDeviation} object to copy.

  void Measurement (Time);
  Time RetransmitTimeout ();
  Ptr<RttEstimator> Copy () const;
  void Reset ();
  void Gain (double g) { gain = g; }

public:
  double       gain;       // Filter gain
  int64x64_t   variance;   // Current variance
};
} //namespace ns3
#endif



