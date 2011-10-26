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
#include <cmath>
#include "path-loss-model.h"
#include <ns3/mobility-model.h>
#include <ns3/vector.h>


NS_LOG_COMPONENT_DEFINE ("PathLossModel");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (PathLossModel);

PathLossModel::PathLossModel ()
{
  NS_LOG_FUNCTION (this);
  SetLastUpdate ();
  SetSamplingPeriod (0.5); // defauld value
  m_pl = 0;
}


TypeId
PathLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::PathLossModel")
    .SetParent<DiscreteTimeLossModel> ()
    .AddConstructor<PathLossModel> ()
  ;
  return tid;
}


PathLossModel::~PathLossModel ()
{
}


void
PathLossModel::SetValue (double pl)
{
  NS_LOG_FUNCTION (this << pl);
  m_pl = pl;
}


double
PathLossModel::GetValue (Ptr<const MobilityModel> a, Ptr<const MobilityModel> b)
{
  NS_LOG_FUNCTION (this << a << b);
  /*
   * According to  ---  insert standard 3gpp ---
   * the Path Loss Model For Urban Environment is
   * L = I + 37.6log10(R)
   * R, in kilometers, is the distance between two nodes
   * I = 128.1 at 2GHz
   */
  double distance = CalculateDistance (a->GetPosition (), b->GetPosition ());
  SetValue (128.1 + (37.6 * log10 (distance * 0.001)));
  return m_pl;
}


} // namespace ns3
