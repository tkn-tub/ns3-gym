/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of Jyvaskyla
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
 * Author: Budiarto Herman <buherman@student.jyu.fi>
 *
 */

#include "lte-handover-algorithm.h"
#include <ns3/lte-handover-management-sap.h>
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("LteHandoverAlgorithm");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LteHandoverAlgorithm);


LteHandoverAlgorithm::LteHandoverAlgorithm ()
{
}


LteHandoverAlgorithm::~LteHandoverAlgorithm ()
{
}


void
LteHandoverAlgorithm::DoDispose ()
{
}


TypeId
LteHandoverAlgorithm::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteHandoverAlgorithm")
    .SetParent<Object> ()
  ;
  return tid;
}



///////////////////////////////////////////
// Bare Handover Algorithm
///////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (BareHandoverAlgorithm);


BareHandoverAlgorithm::BareHandoverAlgorithm ()
  : m_handoverManagementSapUser (0)
{
  m_handoverManagementSapProvider = new MemberLteHandoverManagementSapProvider<BareHandoverAlgorithm> (this);
}


BareHandoverAlgorithm::~BareHandoverAlgorithm ()
{
}


void
BareHandoverAlgorithm::DoDispose ()
{
  delete m_handoverManagementSapProvider;
}


TypeId
BareHandoverAlgorithm::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::BareHandoverAlgorithm")
    .SetParent<LteHandoverAlgorithm> ()
    .AddConstructor<BareHandoverAlgorithm> ()
  ;
  return tid;
}


void
BareHandoverAlgorithm::SetLteHandoverManagementSapUser (LteHandoverManagementSapUser* s)
{
  m_handoverManagementSapUser = s;
}


LteHandoverManagementSapProvider*
BareHandoverAlgorithm::GetLteHandoverManagementSapProvider ()
{
  return m_handoverManagementSapProvider;
}


void
BareHandoverAlgorithm::DoInitialize ()
{
  LteHandoverAlgorithm::DoInitialize ();
}


void
BareHandoverAlgorithm::DoReportUeMeas (uint16_t rnti,
                                       LteRrcSap::MeasResults measResults)
{
}



} // end of namespace ns3
