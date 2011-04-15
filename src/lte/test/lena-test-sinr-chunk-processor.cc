/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es> : Based on lte-sinr-chunk-processor code
 *         Nicola Baldo <nbaldo>@cttc.es>
 */

#include <ns3/log.h>

#include "lena-test-sinr-chunk-processor.h"

NS_LOG_COMPONENT_DEFINE ("LenaTestSinrChunkProcessor");

namespace ns3 {


LenaTestSinrChunkProcessor::LenaTestSinrChunkProcessor (Ptr<LtePhy> p)
  : m_phy (p)
{
  NS_LOG_FUNCTION (this << p);
  NS_ASSERT (m_phy);
}

LenaTestSinrChunkProcessor::~LenaTestSinrChunkProcessor ()
{
  NS_LOG_FUNCTION (this);
}

void
LenaTestSinrChunkProcessor::Start ()
{
  NS_LOG_FUNCTION (this);
  m_sumSinr = 0;
  m_totDuration = MicroSeconds (0);
}

void
LenaTestSinrChunkProcessor::EvaluateSinrChunk (const SpectrumValue& sinr, Time duration)
{
  NS_LOG_FUNCTION (this << sinr << duration);
  if (m_sumSinr == 0)
    {
      m_sumSinr = Create<SpectrumValue> (sinr.GetSpectrumModel ());
    }
  (*m_sumSinr) += sinr * duration.GetSeconds ();
  m_totDuration += duration;
  NS_LOG_LOGIC (this << " m_sumSinr = " << *m_sumSinr);
  NS_LOG_LOGIC (this << " m_totDuration = " << m_totDuration);
}

void
LenaTestSinrChunkProcessor::End ()
{
  NS_LOG_FUNCTION (this);
  if (m_totDuration.GetSeconds () > 0)
    {
      NS_LOG_LOGIC (this << " m_sumSinr = " << *m_sumSinr);
      NS_LOG_LOGIC (this << " m_totDuration = " << m_totDuration);
      NS_LOG_LOGIC (this << " m_sumSinr / m_totDuration = " << (*m_sumSinr) / m_totDuration.GetSeconds ());
    }
  else
    {
      NS_LOG_WARN ("m_numSinr == 0");
    }
}

SpectrumValue
LenaTestSinrChunkProcessor::GetSinr ()
{
  NS_LOG_FUNCTION (this);

  return (*m_sumSinr) / m_totDuration.GetSeconds ();
}

} // namespace ns3
