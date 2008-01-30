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


// Ported from:
// Georgia Tech Network Simulator - Round Trip Time Estimation Class
// George F. Riley.  Georgia Tech, Spring 2002

// Implements several variations of round trip time estimators

#include <iostream>

#include "rtt-estimator.h"
#include "ns3/simulator.h"
#include "ns3/type-id-default-value.h"

namespace ns3{

NS_OBJECT_ENSURE_REGISTERED (RttEstimator);

//RttEstimator iid
TypeId 
RttEstimator::GetTypeId (void)
{
  static TypeId tid = TypeId ("RttEstimator")
    .SetParent<Object> ();
  return tid;
}

//Default values
TypeIdDefaultValue RttEstimator::defaultTid ("RttEstimator",
                                             "Tahoe round trip time estimation",
                                             RttEstimator::GetTypeId (), "RttMeanDeviation");
NumericDefaultValue<double> RttEstimator::defaultMaxMultiplier ("RttMaxMultiplier","",64.0);

// RttEstimator Static Member variables
Time        RttEstimator::initialEstimate = Seconds (1.0); // Default initial estimate

//RttHistory methods
RttHistory::RttHistory (SequenceNumber s, uint32_t c, Time t)
  : seq (s), count (c), time (t), retx (false)
  {
  }

RttHistory::RttHistory (const RttHistory& h)
  : seq (h.seq), count (h.count), time (h.time), retx (h.retx)
  {
  }

// Base class methods

RttEstimator::RttEstimator () : next (1), history (), est (initialEstimate),
    nSamples (0), multiplier (1.0) 
{ 
  //note next=1 everywhere since first segment will have sequence 1
}

RttEstimator::RttEstimator (Time e) : next (1), history (), est (e),
    nSamples (0), multiplier (1.0) 
{ }

RttEstimator::RttEstimator(const RttEstimator& c)
  : next(c.next), history(c.history), est(c.est), nSamples(c.nSamples),
    multiplier(c.multiplier)
{}

RttEstimator::~RttEstimator ()
{
}

void RttEstimator::SentSeq (SequenceNumber s, uint32_t c)
{ // Note that a particular sequence has been sent
  if (s == next)
    { // This is the next expected one, just log at end
      history.push_back (RttHistory (s, c, Simulator::Now () ));
      next = s + SequenceNumber (c); // Update next expected
    }
  else
    { // This is a retransmit, find in list and mark as re-tx
      for (RttHistory_t::iterator i = history.begin (); i != history.end (); ++i)
        {
          if ((s >= i->seq) && (s < (i->seq + SequenceNumber (i->count))))
            { // Found it
              i->retx = true;
              // One final test..be sure this re-tx does not extend "next"
              if ((s + SequenceNumber (c)) > next)
                {
                  next = s + SequenceNumber (c);
                  i->count = ((s + SequenceNumber (c)) - i->seq); // And update count in hist
                }
              break;
            }
        }
    }
}

Time RttEstimator::AckSeq (SequenceNumber a)
{ // An ack has been received, calculate rtt and log this measurement
  // Note we use a linear search (O(n)) for this since for the common
  // case the ack'ed packet will be at the head of the list
  Time m = Seconds (0.0);
  if (history.size () == 0) return (m);    // No pending history, just exit
  RttHistory& h = history.front ();
  if (!h.retx && a >= (h.seq + SequenceNumber (h.count)))
    { // Ok to use this sample
      m = Simulator::Now () - h.time; // Elapsed time
      Measurement(m);                // Log the measurement
      ResetMultiplier();             // Reset multiplier on valid measurement
    }
  // Now delete all ack history with seq <= ack
  while(history.size() > 0)
    {
      RttHistory& h = history.front ();
      if ((h.seq + SequenceNumber(h.count)) > a) break;                // Done removing
      history.pop_front (); // Remove
    }
  return m;
}

void RttEstimator::ClearSent ()
{ // Clear all history entries
  next = 1;
  history.clear ();
}

void RttEstimator::IncreaseMultiplier ()
{
  multiplier = std::min (multiplier * 2.0, defaultMaxMultiplier.GetValue ());
}

void RttEstimator::ResetMultiplier ()
{
  multiplier = 1.0;
}

void RttEstimator::Reset ()
{ // Reset to initial state
  next = 1;
  est = initialEstimate;
  history.clear ();         // Remove all info from the history
  nSamples = 0;
  ResetMultiplier ();
}

// Base class, static methods
void RttEstimator::InitialEstimate (Time e)
{ // Set a new default initial estimate
  initialEstimate = e;
}

Ptr<RttEstimator> RttEstimator::CreateDefault ()
{
  TypeId tid = defaultTid.GetValue ();
  Ptr<RttEstimator> rtte = tid.CreateObject (0.1, initialEstimate)->QueryInterface<RttEstimator> ();
  return rtte;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Mean-Deviation Estimator

NS_OBJECT_ENSURE_REGISTERED (RttMeanDeviation);

TypeId 
RttMeanDeviation::GetTypeId (void)
{
  static TypeId tid = TypeId ("RttMeanDeviation")
    .SetParent<RttEstimator> ()
    .AddConstructor<RttMeanDeviation, double> ()
    .AddConstructor<RttMeanDeviation, double, Time> ();
  return tid;
}

RttMeanDeviation::RttMeanDeviation(double g) :
  gain (g), variance (ns3::Seconds(0)) 
{ 
}

RttMeanDeviation::RttMeanDeviation (const RttMeanDeviation& c)
  : RttEstimator (c), gain (c.gain), variance (c.variance)
{
}

RttMeanDeviation::RttMeanDeviation (double g, Time e) : 
  RttEstimator (e), gain (g), variance (ns3::Seconds(0)) 
{ 
}

void RttMeanDeviation::Measurement (Time m)
{
  if (nSamples)
    { // Not first
      Time err = m - est;
      est = est + Scalar (gain) * err;         // estimated rtt
      err = Abs (err);        // absolute value of error
      variance = variance + Scalar (gain) * (err - variance); // variance of rtt
    }
  else
    { // First sample
      est = m;                        // Set estimate to current
      //variance = m / 2;               // And variance to current / 2
      variance = m; // try this
    }
  nSamples++;
}

Time RttMeanDeviation::RetransmitTimeout ()
{
  // If not enough samples, justjust return 2 times estimate   
  //if (nSamples < 2) return est * 2;
  if (variance < est / Scalar (4.0))
    return est * Scalar (2 * multiplier);            // At least twice current est
  return (est + Scalar (4) * variance) * Scalar (multiplier); // As suggested by Jacobson
}

RttEstimator* RttMeanDeviation::Copy () const
{
  return new RttMeanDeviation (*this);
}

void RttMeanDeviation::Reset ()
{ // Reset to initial state
  variance = Seconds (0.0);
  RttEstimator::Reset ();
}
}//namepsace ns3
