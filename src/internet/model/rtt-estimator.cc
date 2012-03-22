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
#include "ns3/integer.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RttEstimator);

//RttEstimator iid
TypeId 
RttEstimator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RttEstimator")
    .SetParent<Object> ()
    .AddAttribute ("MaxMultiplier", 
                   "Maximum RTO Multiplier",
                   IntegerValue (64),
                   MakeDoubleAccessor (&RttEstimator::m_maxMultiplier),
                   MakeIntegerChecker<u_int16_t> ())
    .AddAttribute ("InitialEstimation", 
                   "Initial RTT estimation",
                   TimeValue (Seconds (1.0)),
                   MakeTimeAccessor (&RttEstimator::m_initialEstimatedRtt),
                   MakeTimeChecker ())
    .AddAttribute ("MinRTO", 
                   "Minimum retransmit timeout value",
                   TimeValue (Seconds (0.2)), // RFC2988 says min RTO=1 sec, but Linux uses 200ms. See http://www.postel.org/pipermail/end2end-interest/2004-November/004402.html
                   MakeTimeAccessor (&RttEstimator::SetMinRto,
                                     &RttEstimator::GetMinRto),
                   MakeTimeChecker ())
  ;
  return tid;
}

void 
RttEstimator::SetMinRto (Time minRto)
{
  m_minRto = minRto;
}
Time 
RttEstimator::GetMinRto (void) const
{
  return Time (m_minRto);
}
void 
RttEstimator::SetCurrentEstimate (Time estimate)
{
  m_currentEstimatedRtt = estimate;
}
Time 
RttEstimator::GetCurrentEstimate (void) const
{
  return Time (m_currentEstimatedRtt);
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

RttEstimator::RttEstimator ()
  : m_next (1), m_history (),
    m_currentEstimatedRtt(m_initialEstimatedRtt),
    m_nSamples (0),
    m_multiplier (1)
{ 
  //note next=1 everywhere since first segment will have sequence 1
}

RttEstimator::~RttEstimator ()
{
}

void RttEstimator::SentSeq (SequenceNumber32 seq, uint32_t size)
{ // Note that a particular sequence has been sent
  if (seq == m_next)
    { // This is the next expected one, just log at end
      m_history.push_back (RttHistory (seq, size, Simulator::Now () ));
      m_next = seq + SequenceNumber32 (size); // Update next expected
    }
  else
    { // This is a retransmit, find in list and mark as re-tx
      for (RttHistory_t::iterator i = m_history.begin (); i != m_history.end (); ++i)
        {
          if ((seq >= i->seq) && (seq < (i->seq + SequenceNumber32 (i->count))))
            { // Found it
              i->retx = true;
              // One final test..be sure this re-tx does not extend "next"
              if ((seq + SequenceNumber32 (size)) > m_next)
                {
                  m_next = seq + SequenceNumber32 (size);
                  i->count = ((seq + SequenceNumber32 (size)) - i->seq); // And update count in hist
                }
              break;
            }
        }
    }
}

Time RttEstimator::AckSeq (SequenceNumber32 ackSeq)
{ // An ack has been received, calculate rtt and log this measurement
  // Note we use a linear search (O(n)) for this since for the common
  // case the ack'ed packet will be at the head of the list
  Time m = Seconds (0.0);
  if (m_history.size () == 0) return (m);    // No pending history, just exit
  RttHistory& h = m_history.front ();
  if (!h.retx && ackSeq >= (h.seq + SequenceNumber32 (h.count)))
    { // Ok to use this sample
      m = Simulator::Now () - h.time; // Elapsed time
      Measurement (m);                // Log the measurement
      ResetMultiplier ();             // Reset multiplier on valid measurement
    }
  // Now delete all ack history with seq <= ack
  while(m_history.size () > 0)
    {
      RttHistory& h = m_history.front ();
      if ((h.seq + SequenceNumber32 (h.count)) > ackSeq) break;               // Done removing
      m_history.pop_front (); // Remove
    }
  return m;
}

void RttEstimator::ClearSent ()
{ // Clear all history entries
  m_next = 1;
  m_history.clear ();
}

void RttEstimator::IncreaseMultiplier ()
{
  m_multiplier = (m_multiplier*2 < m_maxMultiplier) ? m_multiplier*2 : m_maxMultiplier;
}

void RttEstimator::ResetMultiplier ()
{
  m_multiplier = 1;
}

void RttEstimator::Reset ()
{ // Reset to initial state
  m_next = 1;
  m_currentEstimatedRtt = m_initialEstimatedRtt;
  m_history.clear ();         // Remove all info from the history
  m_nSamples = 0;
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
                   "Gain used in estimating the RTT, must be 0 < Gain < 1",
                   DoubleValue (0.1),
                   MakeDoubleAccessor (&RttMeanDeviation::m_gain),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

RttMeanDeviation::RttMeanDeviation() :
  m_variance (0) 
{ 
}

RttMeanDeviation::RttMeanDeviation (const RttMeanDeviation& c)
  : RttEstimator (c), m_gain (c.m_gain), m_variance (c.m_variance)
{
}

void RttMeanDeviation::Measurement (Time m)
{
  if (m_nSamples)
    { // Not first
      Time err(m - m_currentEstimatedRtt);
      m_currentEstimatedRtt += m_gain * err;  // estimated rtt
      m_variance += m_gain * (Abs (err) - m_variance);   // variance of rtt
    }
  else
    { // First sample
      m_currentEstimatedRtt = m;             // Set estimate to current
      //variance = sample / 2;               // And variance to current / 2
      m_variance = m; // try this
    }
  m_nSamples++;
}

Time RttMeanDeviation::RetransmitTimeout ()
{
  // If not enough samples, just return 2 times estimate
  //if (nSamples < 2) return est * 2;
  int64x64_t retval;
  if (m_variance < m_currentEstimatedRtt / 4.0)
    {
      retval = m_currentEstimatedRtt * 2 * m_multiplier;            // At least twice current est
    }
  else
    {
      retval = (m_currentEstimatedRtt + 4 * m_variance) * m_multiplier; // As suggested by Jacobson
    }
  retval = Max (retval, m_minRto);
  return Time (retval);
}

Ptr<RttEstimator> RttMeanDeviation::Copy () const
{
  return CopyObject<RttMeanDeviation> (this);
}

void RttMeanDeviation::Reset ()
{ // Reset to initial state
  m_variance = 0;
  RttEstimator::Reset ();
}
void RttMeanDeviation::Gain (double g)
{
  NS_ASSERT_MSG( (g > 0) && (g < 1), "RttMeanDeviation: Gain must be less than 1 and greater than 0" );
  m_gain = g;
}

} //namepsace ns3
