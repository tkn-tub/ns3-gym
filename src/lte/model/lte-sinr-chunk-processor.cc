/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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


#include <ns3/log.h>

#include "lte-sinr-chunk-processor.h"

NS_LOG_COMPONENT_DEFINE ("LteSinrChunkProcessor");

namespace ns3 {

LteSinrChunkProcessor::~LteSinrChunkProcessor ()
{
  NS_LOG_FUNCTION (this);
}


LteCqiSinrChunkProcessor::LteCqiSinrChunkProcessor (Ptr<LtePhy> p)
  : m_phy (p)
{
  NS_LOG_FUNCTION (this << p);
  NS_ASSERT (m_phy);
}


LteCqiSinrChunkProcessor::~LteCqiSinrChunkProcessor ()
{
  NS_LOG_FUNCTION (this);
}


void 
LteCqiSinrChunkProcessor::Start ()
{
  NS_LOG_FUNCTION (this);
  m_sumSinr = 0;
  m_totDuration = MicroSeconds (0);
}


void 
LteCqiSinrChunkProcessor::EvaluateSinrChunk (const SpectrumValue& sinr, Time duration)
{
  NS_LOG_FUNCTION (this << sinr << duration);
  if (m_sumSinr == 0)
    {
      m_sumSinr = Create<SpectrumValue> (sinr.GetSpectrumModel ());
    }
  (*m_sumSinr) += sinr * duration.GetSeconds ();
  m_totDuration += duration;
}
 
void 
LteCqiSinrChunkProcessor::End ()
{
  NS_LOG_FUNCTION (this);
  if (m_totDuration.GetSeconds () > 0)
    {
      m_phy->GenerateCqiReport ((*m_sumSinr) / m_totDuration.GetSeconds ());
    }
  else
    {
      NS_LOG_WARN ("m_numSinr == 0");
    }
}




LtePemSinrChunkProcessor::LtePemSinrChunkProcessor (Ptr<LteSpectrumPhy> p)
: m_phy (p)
{
  NS_LOG_FUNCTION (this << p);
  NS_ASSERT (m_phy);
}


LtePemSinrChunkProcessor::~LtePemSinrChunkProcessor ()
{
  NS_LOG_FUNCTION (this);
}


void 
LtePemSinrChunkProcessor::Start ()
{
  NS_LOG_FUNCTION (this);
  m_sumSinr = 0;
  m_totDuration = MicroSeconds (0);
}


void 
LtePemSinrChunkProcessor::EvaluateSinrChunk (const SpectrumValue& sinr, Time duration)
{
  NS_LOG_FUNCTION (this << sinr << duration);
  if (m_sumSinr == 0)
  {
    m_sumSinr = Create<SpectrumValue> (sinr.GetSpectrumModel ());
  }
  (*m_sumSinr) += sinr * duration.GetSeconds ();
  m_totDuration += duration;
}

void 
LtePemSinrChunkProcessor::End ()
{
  NS_LOG_FUNCTION (this);
  if (m_totDuration.GetSeconds () > 0)
  {
    m_phy->UpdateSinrPerceived ((*m_sumSinr) / m_totDuration.GetSeconds ());
  }
  else
  {
    NS_LOG_WARN ("m_numSinr == 0");
  }
}


} // namespace ns3
