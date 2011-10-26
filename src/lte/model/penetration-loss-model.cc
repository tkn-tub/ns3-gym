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
 */


#include <ns3/log.h>
#include "penetration-loss-model.h"
#include <ns3/simulator.h>

NS_LOG_COMPONENT_DEFINE ("PenetrationLossModel");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (PenetrationLossModel);

PenetrationLossModel::PenetrationLossModel ()
{
  NS_LOG_FUNCTION (this);
  SetLastUpdate ();
  SetSamplingPeriod (0.5); // defauld value
  m_pnl = 10;
}


TypeId
PenetrationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PenetrationLossModel")
    .SetParent<DiscreteTimeLossModel> ()
    .AddConstructor<PenetrationLossModel> ()
  ;
  return tid;
}


PenetrationLossModel::~PenetrationLossModel ()
{
}


void
PenetrationLossModel::SetValue (double pnl)
{
  NS_LOG_FUNCTION (this << pnl);
  m_pnl = pnl;
}


double
PenetrationLossModel::GetValue (void)
{
  NS_LOG_FUNCTION (this);
  return m_pnl;
}


} // namespace ns3
