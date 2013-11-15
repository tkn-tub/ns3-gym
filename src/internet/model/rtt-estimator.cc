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
#include "ns3/uinteger.h"
#include "ns3/log.h"

NS_LOG_COMPONENT_DEFINE ("RttEstimator");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (RttEstimator)
  ;

TypeId 
RttEstimator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RttEstimator")
    .SetParent<Object> ()
    .AddAttribute ("MaxMultiplier", 
                   "Maximum RTO Multiplier",
                   UintegerValue (64),
                   MakeUintegerAccessor (&RttEstimator::m_maxMultiplier),
                   MakeUintegerChecker<uint16_t> ())
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
  NS_LOG_FUNCTION (this << minRto);
  m_minRto = minRto;
}
Time 
RttEstimator::GetMinRto (void) const
{
  return m_minRto;
}
void 
RttEstimator::SetCurrentEstimate (Time estimate)
{
  NS_LOG_FUNCTION (this << estimate);
  m_currentEstimatedRtt = estimate;
}
Time 
RttEstimator::GetCurrentEstimate (void) const
{
  return m_currentEstimatedRtt;
}


//RttHistory methods
RttHistory::RttHistory (SequenceNumber32 s, uint32_t c, Time t)
  : seq (s), count (c), time (t), retx (false)
{
  NS_LOG_FUNCTION (this);
}

RttHistory::RttHistory (const RttHistory& h)
  : seq (h.seq), count (h.count), time (h.time), retx (h.retx)
{
  NS_LOG_FUNCTION (this);
}

// Base class methods

RttEstimator::RttEstimator ()
  : m_next (1), m_history (),
    m_nSamples (0),
    m_multiplier (1)
{ 
  NS_LOG_FUNCTION (this);
  //note next=1 everywhere since first segment will have sequence 1
  
  // We need attributes initialized here, not later, so use the 
  // ConstructSelf() technique documented in the manual
  ObjectBase::ConstructSelf (AttributeConstructionList ());
  m_currentEstimatedRtt = m_initialEstimatedRtt;
  NS_LOG_DEBUG ("Initialize m_currentEstimatedRtt to " << m_currentEstimatedRtt.GetSeconds () << " sec.");
}

RttEstimator::RttEstimator (const RttEstimator& c)
  : Object (c), m_next (c.m_next), m_history (c.m_history), 
    m_maxMultiplier (c.m_maxMultiplier), 
    m_initialEstimatedRtt (c.m_initialEstimatedRtt),
    m_currentEstimatedRtt (c.m_currentEstimatedRtt), m_minRto (c.m_minRto),
    m_nSamples (c.m_nSamples), m_multiplier (c.m_multiplier)
{
  NS_LOG_FUNCTION (this);
}

RttEstimator::~RttEstimator ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
RttEstimator::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void RttEstimator::SentSeq (SequenceNumber32 seq, uint32_t size)
{ 
  NS_LOG_FUNCTION (this << seq << size);
  // Note that a particular sequence has been sent
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
{ 
  NS_LOG_FUNCTION (this << ackSeq);
  // An ack has been received, calculate rtt and log this measurement
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
{ 
  NS_LOG_FUNCTION (this);
  // Clear all history entries
  m_next = 1;
  m_history.clear ();
}

void RttEstimator::IncreaseMultiplier ()
{
  NS_LOG_FUNCTION (this);
  m_multiplier = (m_multiplier*2 < m_maxMultiplier) ? m_multiplier*2 : m_maxMultiplier;
  NS_LOG_DEBUG ("Multiplier increased to " << m_multiplier);
}

void RttEstimator::ResetMultiplier ()
{
  NS_LOG_FUNCTION (this);
  m_multiplier = 1;
}

void RttEstimator::Reset ()
{ 
  NS_LOG_FUNCTION (this);
  // Reset to initial state
  m_next = 1;
  m_currentEstimatedRtt = m_initialEstimatedRtt;
  m_history.clear ();         // Remove all info from the history
  m_nSamples = 0;
  ResetMultiplier ();
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Mean-Deviation Estimator

NS_OBJECT_ENSURE_REGISTERED (RttMeanDeviation)
  ;

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
  NS_LOG_FUNCTION (this);
}

RttMeanDeviation::RttMeanDeviation (const RttMeanDeviation& c)
  : RttEstimator (c), m_gain (c.m_gain), m_variance (c.m_variance)
{
  NS_LOG_FUNCTION (this);
}

TypeId
RttMeanDeviation::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void RttMeanDeviation::Measurement (Time m)
{
  NS_LOG_FUNCTION (this << m);
  if (m_nSamples)
    { // Not first
      Time err (m - m_currentEstimatedRtt);
      double gErr = err.ToDouble (Time::S) * m_gain;
      m_currentEstimatedRtt += Time::FromDouble (gErr, Time::S);
      Time difference = Abs (err) - m_variance;
      NS_LOG_DEBUG ("m_variance += " << Time::FromDouble (difference.ToDouble (Time::S) * m_gain, Time::S));
      m_variance += Time::FromDouble (difference.ToDouble (Time::S) * m_gain, Time::S);
    }
  else
    { // First sample
      m_currentEstimatedRtt = m;             // Set estimate to current
      //variance = sample / 2;               // And variance to current / 2
      m_variance = m; // try this
      NS_LOG_DEBUG ("(first sample) m_variance += " << m);
    }
  m_nSamples++;
}

Time RttMeanDeviation::RetransmitTimeout ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("RetransmitTimeout:  var " << m_variance.GetSeconds () << " est " << m_currentEstimatedRtt.GetSeconds () << " multiplier " << m_multiplier);
  // RTO = srtt + 4* rttvar
  int64_t temp = m_currentEstimatedRtt.ToInteger (Time::MS) + 4 * m_variance.ToInteger (Time::MS);
  if (temp < m_minRto.ToInteger (Time::MS))
    {
      temp = m_minRto.ToInteger (Time::MS);
    } 
  temp = temp * m_multiplier; // Apply backoff
  Time retval = Time::FromInteger (temp, Time::MS);
  NS_LOG_DEBUG ("RetransmitTimeout:  return " << retval.GetSeconds ());
  return (retval);  
}

Ptr<RttEstimator> RttMeanDeviation::Copy () const
{
  NS_LOG_FUNCTION (this);
  return CopyObject<RttMeanDeviation> (this);
}

void RttMeanDeviation::Reset ()
{ 
  NS_LOG_FUNCTION (this);
  // Reset to initial state
  m_variance = Seconds (0);
  RttEstimator::Reset ();
}
void RttMeanDeviation::Gain (double g)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT_MSG( (g > 0) && (g < 1), "RttMeanDeviation: Gain must be less than 1 and greater than 0" );
  m_gain = g;
}

} //namespace ns3
