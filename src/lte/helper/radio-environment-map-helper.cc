/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 CTTC
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


#include "radio-environment-map-helper.h"

#include <ns3/abort.h>
#include <ns3/log.h>
#include <ns3/double.h>
#include <ns3/uinteger.h>
#include <ns3/string.h>
#include <ns3/spectrum-channel.h>
#include <ns3/config.h>
#include <ns3/rem-spectrum-phy.h>
#include <ns3/buildings-mobility-model.h>
#include <ns3/simulator.h>
#include <ns3/node.h>

#include <fstream>


NS_LOG_COMPONENT_DEFINE ("RadioEnvironmentMapHelper");

namespace ns3 {



NS_OBJECT_ENSURE_REGISTERED (RadioEnvironmentMapHelper);

RadioEnvironmentMapHelper::RadioEnvironmentMapHelper ()
{
}


RadioEnvironmentMapHelper::~RadioEnvironmentMapHelper ()
{
}



void
RadioEnvironmentMapHelper::DoDispose ()
{
  NS_LOG_FUNCTION (this);
}

TypeId
RadioEnvironmentMapHelper::GetTypeId (void)
{
  NS_LOG_FUNCTION ("RadioEnvironmentMapHelper::GetTypeId");
  static TypeId tid = TypeId ("ns3::RadioEnvironmentMapHelper")
    .SetParent<Object> ()
    .AddConstructor<RadioEnvironmentMapHelper> ()
    .AddAttribute ("ChannelPath", "The path to the channel for which the Radio Environment Map is to be generated",
                   StringValue ("/ChannelList/0"),
                   MakeStringAccessor (&RadioEnvironmentMapHelper::m_channelPath),
                   MakeStringChecker ())
    .AddAttribute ("OutputFile", "the filename to which the Radio Environment Map is saved",
                   StringValue ("rem.out"),
                   MakeStringAccessor (&RadioEnvironmentMapHelper::m_outputFile),
                   MakeStringChecker ())
    .AddAttribute ("XMin", "The min x coordinate of the map.",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&RadioEnvironmentMapHelper::m_xMin),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("YMin", "The min y coordinate of the map.",
                   DoubleValue (0.0),
                   MakeDoubleAccessor (&RadioEnvironmentMapHelper::m_yMin),
                   MakeDoubleChecker<double> ())
   .AddAttribute ("XMax", "The max x coordinate of the map.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&RadioEnvironmentMapHelper::m_xMax),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("YMax", "The max y coordinate of the map.",
                   DoubleValue (1.0),
                   MakeDoubleAccessor (&RadioEnvironmentMapHelper::m_yMax),
                   MakeDoubleChecker<double> ())
   .AddAttribute ("XRes", "The resolution (number of points) of the map along the x axis.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&RadioEnvironmentMapHelper::m_xRes),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("YRes", "The resolution (number of points) of the map along the y axis.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&RadioEnvironmentMapHelper::m_yRes),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Z", "The value of the z coordinate for which the map is to be generated",
		   DoubleValue (0.0),
                   MakeDoubleAccessor (&RadioEnvironmentMapHelper::m_z),
                   MakeDoubleChecker<double> ())

  ;
  return tid;
}




void 
RadioEnvironmentMapHelper::Install ()
{
  NS_LOG_FUNCTION (this);
  if (!m_rem.empty ())
    {
      NS_FATAL_ERROR ("only one REM supported per instance of RadioEnvironmentMapHelper");
    }
  Config::MatchContainer match = Config::LookupMatches (m_channelPath);
  if (match.GetN () != 1)
    {
      NS_FATAL_ERROR ("Lookup " << m_channelPath << " should have exactly one match");
    }
  m_channel = match.Get (0)->GetObject<SpectrumChannel> ();
  NS_ABORT_MSG_IF (m_channel == 0, "object at " << m_channelPath << "is not of type SpectrumChannel");

  double xStep = (m_xMax - m_xMin)/m_xRes;
  double yStep = (m_yMax - m_yMin)/m_yRes;
  
  for (double x = m_xMin; x <= m_xMax ; x += xStep)
    {
      m_rem.push_back (std::list<RemPoint> ());
      for (double y = m_yMin; y <= m_yMax ; y += yStep)
        {
          RemPoint p;
          p.phy = CreateObject<RemSpectrumPhy> ();
          p.bmm = CreateObject<BuildingsMobilityModel> ();
          p.node = CreateObject<Node> ();
          p.node->AggregateObject (p.bmm);
          p.phy->SetMobility (p.bmm);
          p.bmm->SetPosition (Vector (x, y, m_z));
          m_rem.back ().push_back (p);
        }      
    }
  Simulator::Schedule (Seconds (0.0055), &RadioEnvironmentMapHelper::Connect, this);
  Simulator::Schedule (Seconds (0.0065), &RadioEnvironmentMapHelper::PrintAndDisconnect, this);  

}

void 
RadioEnvironmentMapHelper::Connect ()
{
  NS_LOG_FUNCTION (this);
  for (std::list<std::list<RemPoint> >::iterator it1 = m_rem.begin ();
       it1 != m_rem.end ();
       ++it1)
    {
      for (std::list<RemPoint>::iterator it2 = it1->begin ();
           it2 != it1->end ();
           ++it2)
        {
          NS_LOG_LOGIC ("adding phy " << it2->phy);
          m_channel->AddRx (it2->phy);
        }
    }
}

void 
RadioEnvironmentMapHelper::PrintAndDisconnect ()
{
  NS_LOG_FUNCTION (this);
  std::ofstream outFile;
  outFile.open (m_outputFile.c_str ());
  if (!outFile.is_open ())
    {
      NS_FATAL_ERROR ("Can't open file " << (m_outputFile));
      return;
    }
  
  for (std::list<std::list<RemPoint> >::iterator it1 = m_rem.begin ();
       it1 != m_rem.end ();
       ++it1)
    {
      for (std::list<RemPoint>::iterator it2 = it1->begin ();
           it2 != it1->end ();
           ++it2)
        {
          Vector pos = it2->bmm->GetPosition ();
          outFile << pos.x << "\t" 
                  << pos.y << "\t" 
                  << pos.z << "\t" 
                  << it2->phy->GetSinr ()
                  << std::endl;
          m_channel->RemoveRx (it2->phy);
        }
    }
  outFile.close ();
}


} // namespace ns3
