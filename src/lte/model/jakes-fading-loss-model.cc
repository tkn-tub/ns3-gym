/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 *        remove dependencies from Phy and Mobility models
 */

#include <cmath>
#include <ns3/log.h>
#include "jakes-fading-loss-model.h"
#include <ns3/simulator.h>
#include <stdint.h>
#include "stdlib.h"
#include <ns3/mobility-model.h>
#include <ns3/string.h>
#include <ns3/double.h>
#include <fstream>


NS_LOG_COMPONENT_DEFINE ("JakesFadingLossModel");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (JakesFadingLossModel);

JakesFadingLossModel::JakesFadingLossModel ()
  : m_traceLength (10.0),
    m_samplesNum (10000),
    m_windowSize (0.5),
    //m_startJakes (1, (m_traceLength-m_windowSize)*1000),
    m_rbNum (100)
{
  NS_LOG_FUNCTION (this);
}


TypeId
JakesFadingLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::JakesFadingLossModel")
    .SetParent<Object> ()
    .AddConstructor<JakesFadingLossModel> ()
    .AddAttribute ("TraceFilename",
                   "Name of file to load a trace from. By default, uses /JakesTraces/fading_trace_EPA_3kmph.fad",
                   StringValue ("/home/mmiozzo/ns3/lena/ns-3-lena-pem/src/lte/model/JakesTraces/fading_trace_EPA_3kmph.fad"),
                   MakeStringAccessor (&JakesFadingLossModel::m_traceFile),
                   MakeStringChecker ())
    .AddAttribute ("TraceLength",
                  "The total length of the fading trace (default value 10 s.)",
                  TimeValue (Seconds (10.0)),
                  MakeTimeAccessor (&JakesFadingLossModel::m_traceLength),
                  MakeTimeChecker ())
    .AddAttribute ("SamplesNum",
                  "The number of samples the trace is made of (default 10000)",
                  DoubleValue (10000),
                  MakeDoubleAccessor (&JakesFadingLossModel::m_samplesNum),
                  MakeDoubleChecker<double> ())
    .AddAttribute ("WindowSize",
                  "The size of the window for the fading trace (default value 0.5 s.)",
                  TimeValue (Seconds (0.5)),
                  MakeTimeAccessor (&JakesFadingLossModel::m_windowSize),
                  MakeTimeChecker ())
  ;
  return tid;
}


JakesFadingLossModel::~JakesFadingLossModel ()
{
}

void
JakesFadingLossModel::LoadTrace (std::string filename)
{
  NS_LOG_FUNCTION (this << "Loading Fading Trace " << filename);
  std::ifstream ifTraceFile;
  ifTraceFile.open (filename.c_str (), std::ifstream::in);
  m_fadingTrace.clear ();
  if (!ifTraceFile.good ())
    {
      NS_LOG_ERROR (this << " Fading trace file not found");
    }

  NS_LOG_INFO (this << " length " << m_traceLength);
  NS_LOG_INFO (this << " RB " << m_rbNum << " samples " << m_samplesNum);
  for (uint32_t i = 0; i < m_rbNum; i++)
    {
      FadingTraceSample rbTimeFadingTrace;
      for (uint32_t j = 0; j < m_samplesNum; j++)
        {
          double sample;
          ifTraceFile >> sample;
          rbTimeFadingTrace.push_back (sample);
        }
      m_fadingTrace.push_back (rbTimeFadingTrace);
    }
  ifTraceFile.close ();
  m_startJakes = new UniformVariable (1, (m_traceLength.GetSeconds () - m_windowSize.GetSeconds ()) * 1000.0);
  m_windowOffset = m_startJakes->GetValue ();
}



double
JakesFadingLossModel::GetValue (int subChannel, double speed)
{
  NS_LOG_FUNCTION (this << subChannel);
  NS_LOG_INFO (this << Simulator::Now ().GetSeconds () << " " << m_lastWindowUpdate.GetSeconds () << " " << m_windowSize.GetSeconds ());
  if (m_fadingTrace.empty ())
    {
      LoadTrace (m_traceFile);
      //SetLastUpdate ();
      m_lastWindowUpdate = Simulator::Now ();
    }
  //if (NeedForUpdate ())
  if (Simulator::Now ().GetSeconds () >= m_lastWindowUpdate.GetSeconds () + m_windowSize.GetSeconds ())
    {
      NS_LOG_INFO ("Fading Window Updated");
      //SetValue (speed);
      m_windowOffset = m_startJakes->GetValue ();
      //SetLastUpdate ();
      m_lastWindowUpdate = Simulator::Now ();
    }

  int now_ms = static_cast<int> (Simulator::Now ().GetSeconds () * 1000);
  int lastUpdate_ms = static_cast<int> (m_lastWindowUpdate.GetSeconds () * 1000);
  NS_LOG_INFO (this << " offset " << m_windowOffset << " now " << now_ms << " lastUpd " << lastUpdate_ms);
  int index = m_windowOffset + now_ms - lastUpdate_ms;
  NS_LOG_INFO (this << "subchannel " << subChannel << " index " << index);

  //NS_LOG_FUNCTION (this << subChannel << now_ms
  //                      << lastUpdate_ms << index << m_multipath.at (subChannel).at (index));
  NS_LOG_FUNCTION (this << subChannel << now_ms
                        << lastUpdate_ms << index << m_fadingTrace.at (subChannel).at (index));
                        
  return (m_fadingTrace.at (subChannel).at (index));
}



} // namespace ns3
