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
#include "ns3/double.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RttEstimator);

//RttEstimator iid
TypeId 
RttEstimator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RttEstimator")
    .SetParent<Object> ()
    .AddAttribute ("MaxMultiplier", 
                   "XXX",
                   DoubleValue (64.0),
                   MakeDoubleAccessor (&RttEstimator::m_maxMultiplier),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("InitialEstimation", 
                   "XXX",
                   TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&RttEstimator::SetEstimate,
                                     &RttEstimator::GetEstimate),
                   MakeTimeChecker ())
    .AddAttribute ("MinRTO", 
                   "Minimum retransmit timeout value",
                   TimeValue (Seconds (0.2)),
                   MakeTimeAccessor (&RttEstimator::SetMinRto,
                                     &RttEstimator::GetMinRto),
                   MakeTimeChecker ())
  ;
  return tid;
}

void 
RttEstimator::SetMinRto (Time minRto)
{
  minrto = minRto;
}
Time 
RttEstimator::GetMinRto (void) const
{
  return Time (minrto);
}
void 
RttEstimator::SetEstimate (Time estimate)
{
  est = estimate;
}
Time 
RttEstimator::GetEstimate (void) const
{
  return Time (est);
}


//RttHistory methods
RttHistory::RttHistory (SequenceNumber32 s, uint32_t c, Time t)
  : seq (s), count (c), time (t), retx (false)
{
}

RttHistory::RttHistory (const RttHistory& h)
  : seq (h.seq), count (h.count), time (h.time), retx (h.retx)
{
}

// Base class methods

RttEstimator::RttEstimator () : next (1), history (),
                                nSamples (0), multiplier (1.0)
{ 
  //note next=1 everywhere since first segment will have sequence 1
}

RttEstimator::RttEstimator(const RttEstimator& c)
  : Object (c), next (c.next), history (c.history),
    m_maxMultiplier (c.m_maxMultiplier), est (c.est),
    minrto (c.minrto), nSamples (c.nSamples),
    multiplier (c.multiplier)
{
}

RttEstimator::~RttEstimator ()
{
}

void RttEstimator::SentSeq (SequenceNumber32 s, uint32_t c)
{ // Note that a particular sequence has been sent
  if (s == next)
    { // This is the next expected one, just log at end
      history.push_back (RttHistory (s, c, Simulator::Now () ));
      next = s + SequenceNumber32 (c); // Update next expected
    }
  else
    { // This is a retransmit, find in list and mark as re-tx
      for (RttHistory_t::iterator i = history.begin (); i != history.end (); ++i)
        {
          if ((s >= i->seq) && (s < (i->seq + SequenceNumber32 (i->count))))
            { // Found it
              i->retx = true;
              // One final test..be sure this re-tx does not extend "next"
              if ((s + SequenceNumber32 (c)) > next)
                {
                  next = s + SequenceNumber32 (c);
                  i->count = ((s + SequenceNumber32 (c)) - i->seq); // And update count in hist
                }
              break;
            }
        }
    }
}

Time RttEstimator::AckSeq (SequenceNumber32 a)
{ // An ack has been received, calculate rtt and log this measurement
  // Note we use a linear search (O(n)) for this since for the common
  // case the ack'ed packet will be at the head of the list
  Time m = Seconds (0.0);
  if (history.size () == 0) return (m);    // No pending history, just exit
  RttHistory& h = history.front ();
  if (!h.retx && a >= (h.seq + SequenceNumber32 (h.count)))
    { // Ok to use this sample
      m = Simulator::Now () - h.time; // Elapsed time
      Measurement (m);                // Log the measurement
      ResetMultiplier ();             // Reset multiplier on valid measurement
    }
  // Now delete all ack history with seq <= ack
  while(history.size () > 0)
    {
      RttHistory& h = history.front ();
      if ((h.seq + SequenceNumber32 (h.count)) > a) break;               // Done removing
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
  multiplier = std::min (multiplier * 2.0, m_maxMultiplier);
}

void RttEstimator::ResetMultiplier ()
{
  multiplier = 1.0;
}

void RttEstimator::Reset ()
{ // Reset to initial state
  next = 1;
  est = 1; // XXX: we should go back to the 'initial value' here. Need to add support in Object for this.
  history.clear ();         // Remove all info from the history
  nSamples = 0;
  ResetMultiplier ();
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Mean-Deviation Estimator

NS_OBJECT_ENSURE_REGISTERED (RttMeanDeviation);

TypeId 
RttMeanDeviation::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RttMeanDeviation")
    .SetParent<RttEstimator> ()
    .AddConstructor<RttMeanDeviation> ()
    .AddAttribute ("Gain",
                   "XXX",
                   DoubleValue (0.1),
                   MakeDoubleAccessor (&RttMeanDeviation::gain),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

RttMeanDeviation::RttMeanDeviation() :
  variance (0) 
{ 
}

RttMeanDeviation::RttMeanDeviation (const RttMeanDeviation& c)
  : RttEstimator (c), gain (c.gain), variance (c.variance)
{
}

void RttMeanDeviation::Measurement (Time m)
{
  if (nSamples)
    { // Not first
      int64x64_t err = m - est;
      est = est + gain * err;         // estimated rtt
      variance = variance + gain * (Abs (err) - variance); // variance of rtt
    }
  else
    { // First sample
      est = m;                        // Set estimate to current
      //variance = sample / 2;               // And variance to current / 2
      variance = m; // try this
    }
  nSamples++;
}

Time RttMeanDeviation::RetransmitTimeout ()
{
  // If not enough samples, justjust return 2 times estimate
  //if (nSamples < 2) return est * 2;
  int64x64_t retval;
  if (variance < est / 4.0)
    {
      retval = est * 2 * multiplier;            // At least twice current est
    }
  else
    {
      retval = (est + 4 * variance) * multiplier; // As suggested by Jacobson
    }
  retval = Max (retval, minrto);
  return Time (retval);
}

Ptr<RttEstimator> RttMeanDeviation::Copy () const
{
  return CopyObject<RttMeanDeviation> (this);
}

void RttMeanDeviation::Reset ()
{ // Reset to initial state
  variance = 0;
  RttEstimator::Reset ();
}
} //namepsace ns3
