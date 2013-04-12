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
 * Modified by : Marco Miozzo <mmiozzo@cttc.es>
 *        (move from CQI to Ctrl and Data SINR Chunk processors
 */


#include <ns3/log.h>

#include "lte-sinr-chunk-processor.h"

NS_LOG_COMPONENT_DEFINE ("LteSinrChunkProcessor");

namespace ns3 {

LteSinrChunkProcessor::~LteSinrChunkProcessor ()
{
  NS_LOG_FUNCTION (this);
}


// ------------- LteCtrlSinrChunkProcessor ------------------------------

LteCtrlSinrChunkProcessor::LteCtrlSinrChunkProcessor (Ptr<LtePhy> p)
  : m_phy (p),
  m_spectrumPhy (0)
{
  NS_LOG_FUNCTION (this << p);
  NS_ASSERT (m_phy);
}

LteCtrlSinrChunkProcessor::LteCtrlSinrChunkProcessor (Ptr<LtePhy> p, Ptr<LteSpectrumPhy> s)
: m_phy (p),
  m_spectrumPhy (s)
{
  NS_LOG_FUNCTION (this << p);
  NS_ASSERT (m_phy);
  NS_ASSERT (m_spectrumPhy);
}


LteCtrlSinrChunkProcessor::~LteCtrlSinrChunkProcessor ()
{
  NS_LOG_FUNCTION (this);
}


void 
LteCtrlSinrChunkProcessor::Start ()
{
  NS_LOG_FUNCTION (this);
  m_sumSinr = 0;
  m_totDuration = MicroSeconds (0);
}


void 
LteCtrlSinrChunkProcessor::EvaluateSinrChunk (const SpectrumValue& sinr, Time duration)
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
LteCtrlSinrChunkProcessor::End ()
{
  NS_LOG_FUNCTION (this);
  if (m_totDuration.GetSeconds () > 0)
    {
      m_phy->GenerateCtrlCqiReport ((*m_sumSinr) / m_totDuration.GetSeconds ());
      if (m_spectrumPhy)
        {
          m_spectrumPhy->UpdateSinrPerceived ((*m_sumSinr) / m_totDuration.GetSeconds ());
        }
    }
  else
    {
      NS_LOG_WARN ("m_numSinr == 0");
    }
}


// ------------- LteDataSinrChunkProcessor ------------------------------

LteDataSinrChunkProcessor::LteDataSinrChunkProcessor (Ptr<LteSpectrumPhy> s, Ptr<LtePhy> p)
: m_spectrumPhy (s),
  m_phy (p)
{
  NS_LOG_FUNCTION (this << p);
  NS_ASSERT (m_spectrumPhy);
  NS_ASSERT (m_phy);
}

LteDataSinrChunkProcessor::LteDataSinrChunkProcessor (Ptr<LteSpectrumPhy> p)
: m_spectrumPhy (p),
  m_phy (0)
{
  NS_LOG_FUNCTION (this << p);
  NS_ASSERT (m_spectrumPhy);
  
}



LteDataSinrChunkProcessor::~LteDataSinrChunkProcessor ()
{
  NS_LOG_FUNCTION (this);
}


void 
LteDataSinrChunkProcessor::Start ()
{
  NS_LOG_FUNCTION (this);
  m_sumSinr = 0;
  m_totDuration = MicroSeconds (0);
}


void 
LteDataSinrChunkProcessor::EvaluateSinrChunk (const SpectrumValue& sinr, Time duration)
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
LteDataSinrChunkProcessor::End ()
{
  NS_LOG_FUNCTION (this);
  if (m_totDuration.GetSeconds () > 0)
  {
    m_spectrumPhy->UpdateSinrPerceived ((*m_sumSinr) / m_totDuration.GetSeconds ());
    if (m_phy)
      {
        m_phy->GenerateDataCqiReport ((*m_sumSinr) / m_totDuration.GetSeconds ());
      }
  }
  else
  {
    NS_LOG_WARN ("m_numSinr == 0");
  }
}




// ------------- LteRsReceivedPowerChunkProcessor ------------------------------

LteRsReceivedPowerChunkProcessor::LteRsReceivedPowerChunkProcessor (Ptr<LtePhy> p)
: m_phy (p)
{
  NS_LOG_FUNCTION (this << p);
  NS_ASSERT (m_phy);
}

LteRsReceivedPowerChunkProcessor::~LteRsReceivedPowerChunkProcessor ()
{
  NS_LOG_FUNCTION (this);
}


void
LteRsReceivedPowerChunkProcessor::Start ()
{
  NS_LOG_FUNCTION (this);
  m_sumSinr = 0;
  m_totDuration = MicroSeconds (0);
}


void
LteRsReceivedPowerChunkProcessor::EvaluateSinrChunk (const SpectrumValue& sinr, Time duration)
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
LteRsReceivedPowerChunkProcessor::End ()
{
  NS_LOG_FUNCTION (this);
  if (m_totDuration.GetSeconds () > 0)
    {
      m_phy->ReportRsReceivedPower ((*m_sumSinr) / m_totDuration.GetSeconds ());
    }
  else
    {
      NS_LOG_WARN ("m_numSinr == 0");
    }
}





// ------------- LteInterferencePowerChunkProcessor ------------------------------

LteInterferencePowerChunkProcessor::LteInterferencePowerChunkProcessor (Ptr<LtePhy> p)
: m_phy (p)
{
  NS_LOG_FUNCTION (this << p);
  NS_ASSERT (m_phy);
}

LteInterferencePowerChunkProcessor::~LteInterferencePowerChunkProcessor ()
{
  NS_LOG_FUNCTION (this);
}


void
LteInterferencePowerChunkProcessor::Start ()
{
  NS_LOG_FUNCTION (this);
  m_sumSinr = 0;
  m_totDuration = MicroSeconds (0);
}


void
LteInterferencePowerChunkProcessor::EvaluateSinrChunk (const SpectrumValue& sinr, Time duration)
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
LteInterferencePowerChunkProcessor::End ()
{
  NS_LOG_FUNCTION (this);
  if (m_totDuration.GetSeconds () > 0)
    {
      m_phy->ReportInterference ((*m_sumSinr) / m_totDuration.GetSeconds ());
    }
  else
    {
      NS_LOG_WARN ("m_numSinr == 0");
    }
}


} // namespace ns3
