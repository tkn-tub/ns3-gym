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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 */

#include "lte-common.h"
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("LteFfConverter");

namespace ns3 {


LteFlowId_t::LteFlowId_t ()
{
}

LteFlowId_t::LteFlowId_t (const uint16_t a, const uint8_t b)
  : m_rnti (a),
    m_lcId (b)
{
}

bool
operator == (const LteFlowId_t &a, const LteFlowId_t &b)
{
  return ( (a.m_rnti == b.m_rnti) && (a.m_lcId == b.m_lcId) );
}

bool
operator < (const LteFlowId_t& a, const LteFlowId_t& b)
{
  return ( (a.m_rnti < b.m_rnti) || ( (a.m_rnti == b.m_rnti) && (a.m_lcId < b.m_lcId) ) );
}

ImsiLcidPair_t::ImsiLcidPair_t ()
{
}

ImsiLcidPair_t::ImsiLcidPair_t (const uint64_t a, const uint8_t b)
  : m_imsi (a),
    m_lcId (b)
{
}

bool
operator == (const ImsiLcidPair_t &a, const ImsiLcidPair_t &b)
{
  return ((a.m_imsi == b.m_imsi) && (a.m_lcId == b.m_lcId));
}

bool
operator < (const ImsiLcidPair_t& a, const ImsiLcidPair_t& b)
{
  return ((a.m_imsi < b.m_imsi) || ((a.m_imsi == b.m_imsi) && (a.m_lcId
                                                               < b.m_lcId)));
}


uint16_t
LteFfConverter::double2fpS11dot3 (double val)
{
  // convert from double to fixed point notation Sxxxxxxxxxxx.xxx
  int16_t valFp = (int16_t)(val * pow (2, 3));
  return (valFp);
}

double
LteFfConverter::fpS11dot3toDouble (uint16_t val)
{
  // convert from fixed point notation Sxxxxxxxxxxx.xxx to double
  double valD = ((int16_t)val) / pow (2, 3);
  return (valD);
}

double 
LteFfConverter::getMinFpS11dot3Value ()
{
  return (-4096);        // -4096 = 0x8000 = 1000 0000 0000 0000 b
}

//static double g_lowestFpS11dot3Value = -4096; // 0x8001 (1000 0000 0000 0000)



}; // namespace ns3

