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
#include "shadowing-loss-model.h"

NS_LOG_COMPONENT_DEFINE ("ShadowingLossModel");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (ShadowingLossModel);

ShadowingLossModel::ShadowingLossModel ()
  : m_randVariable (.0, 8.)
{
  SetValue (m_randVariable.GetValue ());
  SetLastUpdate ();
  SetSamplingPeriod (0.5); // defauld value
}


ShadowingLossModel::ShadowingLossModel (double mu, double sigma, double samplingPeriod)
  : m_randVariable (mu, sigma)
{
  SetValue (m_randVariable.GetValue ());
  SetLastUpdate ();
  SetSamplingPeriod (samplingPeriod); // defauld value
}


TypeId
ShadowingLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ShadowingLossModel")
    .SetParent<DiscreteTimeLossModel> ()
    .AddConstructor<ShadowingLossModel> ()
  ;
  return tid;
}


ShadowingLossModel::~ShadowingLossModel ()
{
}


void
ShadowingLossModel::SetValue (double sh)
{
  NS_LOG_FUNCTION (this << sh);
  m_shadowingValue = sh;
}


double
ShadowingLossModel::GetValue (void)
{
  NS_LOG_FUNCTION (this);
  if (NeedForUpdate ())
    {
      double sh = m_randVariable.GetValue ();
      SetValue (sh);
      SetLastUpdate ();
    }

  return 0; // m_shadowingValue; XXX: LogNormalVariable doeas not work correctly
}


} // namespace ns3
