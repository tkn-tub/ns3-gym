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


#include "lte-interference.h"
#include "lte-sinr-chunk-processor.h"

#include <ns3/simulator.h>
#include <ns3/log.h>


NS_LOG_COMPONENT_DEFINE ("LteInterference");

namespace ns3 {


LteInterference::LteInterference ()
  : m_receiving (false)
{
  NS_LOG_FUNCTION (this);
}

LteInterference::~LteInterference ()
{
  NS_LOG_FUNCTION (this);
}

void 
LteInterference::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_sinrChunkProcessorList.clear ();
  m_rxSignal = 0;
  m_allSignals = 0;
  m_noise = 0;
  Object::DoDispose ();
} 


TypeId
LteInterference::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteInterference")
    .SetParent<Object> ()
  ;
  return tid;
}


void
LteInterference::StartRx (Ptr<const SpectrumValue> rxPsd)
{ 
  NS_LOG_FUNCTION (this << *rxPsd);
  if (m_receiving == false)
    {
      NS_LOG_LOGIC ("first signal");
      m_rxSignal = rxPsd->Copy ();
      m_lastChangeTime = Now ();
      m_receiving = true;
      for (std::list<Ptr<LteSinrChunkProcessor> >::const_iterator it = m_sinrChunkProcessorList.begin (); it != m_sinrChunkProcessorList.end (); ++it)
        {
          (*it)->Start (); 
        }
    }
  else
    {
      NS_LOG_LOGIC ("additional signal");
      // receiving multiple simultaneous signals, make sure they are synchronized
      NS_ASSERT (m_lastChangeTime == Now ());
      // make sure they use orthogonal resource blocks
      NS_ASSERT (Sum ((*rxPsd) * (*m_rxSignal)) == 0.0);
      (*m_rxSignal) += (*rxPsd);
    }
}


void
LteInterference::EndRx ()
{
  NS_LOG_FUNCTION (this);
  ConditionallyEvaluateChunk ();
  m_receiving = false;
  for (std::list<Ptr<LteSinrChunkProcessor> >::const_iterator it = m_sinrChunkProcessorList.begin (); it != m_sinrChunkProcessorList.end (); ++it)
    {
      (*it)->End (); 
    }
}


void
LteInterference::AddSignal (Ptr<const SpectrumValue> spd, const Time duration)
{
  NS_LOG_FUNCTION (this << *spd << duration);
  // if this is the first signal that we see, we need to initialize the interference calculation
  if (m_allSignals == 0) 
    {
      m_allSignals = Create<SpectrumValue> (spd->GetSpectrumModel ());
    }
  DoAddSignal (spd);
  Simulator::Schedule (duration, &LteInterference::DoSubtractSignal, this, spd);
}


void
LteInterference::DoAddSignal  (Ptr<const SpectrumValue> spd)
{ 
  NS_LOG_FUNCTION (this << *spd);
  ConditionallyEvaluateChunk ();
  (*m_allSignals) += (*spd);
  m_lastChangeTime = Now ();
}

void
LteInterference::DoSubtractSignal  (Ptr<const SpectrumValue> spd)
{ 
  NS_LOG_FUNCTION (this << *spd);
  ConditionallyEvaluateChunk ();
  (*m_allSignals) -= (*spd);
  m_lastChangeTime = Now ();
}


void
LteInterference::ConditionallyEvaluateChunk ()
{
  NS_LOG_FUNCTION (this);
  if (m_receiving)
    {
      NS_LOG_DEBUG (this << " Receiving");
    }
  NS_LOG_DEBUG (this << " now "  << Now () << " last " << m_lastChangeTime);
  if (m_receiving && (Now () > m_lastChangeTime))
    {
      NS_LOG_LOGIC (this << " signal = " << *m_rxSignal << " allSignals = " << *m_allSignals << " noise = " << *m_noise);

      SpectrumValue sinr = (*m_rxSignal) / ((*m_allSignals) - (*m_rxSignal) + (*m_noise));
      Time duration = Now () - m_lastChangeTime;
      for (std::list<Ptr<LteSinrChunkProcessor> >::const_iterator it = m_sinrChunkProcessorList.begin (); it != m_sinrChunkProcessorList.end (); ++it)
        {
          (*it)->EvaluateSinrChunk (sinr, duration);
        }
    }
  else
    {
      NS_LOG_DEBUG (this << " NO EV");
    }
}

void
LteInterference::SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd)
{
  NS_LOG_FUNCTION (this << *noisePsd);
  m_noise = noisePsd;
  // we can initialize m_allSignal only now, because earlier we
  // didn't know what spectrum model was going to be used.
  // we'll now create a zeroed SpectrumValue using the same
  // SpectrumModel which is being specified for the noise.
  m_allSignals = Create<SpectrumValue> (noisePsd->GetSpectrumModel ());
}

void
LteInterference::AddSinrChunkProcessor (Ptr<LteSinrChunkProcessor> p)
{
  NS_LOG_FUNCTION (this << p);
  m_sinrChunkProcessorList.push_back (p);
}




} // namespace ns3


