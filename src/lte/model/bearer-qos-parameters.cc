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

#include "bearer-qos-parameters.h"
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("BearerQosParameters");

namespace ns3 {

BearerQosParameters::BearerQosParameters (void)
  : m_qci (0),
    m_arpPreEmptionCapability (false),
    m_arpPreEmptionVulnerability (false),
    m_gbr (0),
    m_mbr (0),
    m_maxDelay (0)
{

}

BearerQosParameters::BearerQosParameters (int qci,
                                          double gbr,
                                          double mbr)
  : m_qci (qci),
    m_arpPreEmptionCapability (false),
    m_arpPreEmptionVulnerability (false),
    m_gbr (gbr),
    m_mbr (mbr),
    m_maxDelay (0)
{

}

BearerQosParameters::BearerQosParameters (int qci,
                                          bool apec,
                                          bool apev,
                                          double gbr,
                                          double mbr)
  : m_qci (qci),
    m_arpPreEmptionCapability (apec),
    m_arpPreEmptionVulnerability (apev),
    m_gbr (gbr),
    m_mbr (mbr),
    m_maxDelay (0)

{
}


BearerQosParameters::~BearerQosParameters ()
{
}


void
BearerQosParameters::SetQci (int qci)
{
  m_qci = qci;
}


int
BearerQosParameters::GetQci (void) const
{
  return m_qci;
}


void
BearerQosParameters::SetArpPreEmptionCapability (bool apec)
{
  m_arpPreEmptionCapability = apec;
}


bool
BearerQosParameters::GetArpPreEmptionCapability (void) const
{
  return m_arpPreEmptionCapability;
}


void
BearerQosParameters::SetArpPreEmptionVulnerability (bool apev)
{
  m_arpPreEmptionVulnerability = apev;
}


bool
BearerQosParameters::GetArpPreEmptionVulnerability (void) const
{
  return m_arpPreEmptionVulnerability;
}


void
BearerQosParameters::SetGbr (double gbr)
{
  m_gbr = gbr;
}


double
BearerQosParameters::GetGbr (void) const
{
  return m_gbr;
}


void
BearerQosParameters::SetMbr (double mbr)
{
  m_mbr = mbr;
}


double
BearerQosParameters::GetMbr (void) const
{
  return m_mbr;
}


void
BearerQosParameters::SetMaxDelay (double targetDelay)
{
  m_maxDelay = targetDelay;
}


double
BearerQosParameters::GetMaxDelay (void) const
{
  return m_maxDelay;
}


void
BearerQosParameters::SetBearerQosType (BearerQosType QosType)
{
  m_bearerQosType = QosType;
}


BearerQosParameters::BearerQosType
BearerQosParameters::GetBearerQosType (void) const
{
  return m_bearerQosType;
}

} // namespace ns3
