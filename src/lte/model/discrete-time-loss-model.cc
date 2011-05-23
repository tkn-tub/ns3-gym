/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 */


#include <ns3/log.h>
#include <ns3/simulator.h>
#include "discrete-time-loss-model.h"

NS_LOG_COMPONENT_DEFINE ("DiscreteTimeLossModel");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (DiscreteTimeLossModel);

DiscreteTimeLossModel::DiscreteTimeLossModel ()
{
}


TypeId
DiscreteTimeLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DiscreteTimeLossModel")
    .SetParent<Object> ()
    .AddConstructor<DiscreteTimeLossModel> ()
  ;
  return tid;
}


DiscreteTimeLossModel::~DiscreteTimeLossModel ()
{
}


void
DiscreteTimeLossModel::SetLastUpdate (void)
{
  NS_LOG_FUNCTION (this);
  m_lastUpdate = Simulator::Now ();
}


Time
DiscreteTimeLossModel::GetLastUpdate (void)
{
  NS_LOG_FUNCTION (this);
  return m_lastUpdate;
}


void
DiscreteTimeLossModel::SetSamplingPeriod (double sp)
{
  NS_LOG_FUNCTION (this << sp);
  m_samplingPeriod = sp;
}


double
DiscreteTimeLossModel::GetSamplingPeriod (void)
{
  NS_LOG_FUNCTION (this);
  return m_samplingPeriod;
}


bool
DiscreteTimeLossModel::NeedForUpdate (void)
{
  NS_LOG_FUNCTION (this);
  if (Simulator::Now ().GetSeconds () >= (GetLastUpdate ().GetSeconds () + GetSamplingPeriod ()))
    {
      return true;
    }
  else
    {
      return false;
    }
}

} // namespace ns3
