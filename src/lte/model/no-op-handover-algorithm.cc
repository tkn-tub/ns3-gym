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

#include "no-op-handover-algorithm.h"
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("NoOpHandoverAlgorithm");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (NoOpHandoverAlgorithm);


NoOpHandoverAlgorithm::NoOpHandoverAlgorithm ()
  : m_handoverManagementSapUser (0)
{
  m_handoverManagementSapProvider = new MemberLteHandoverManagementSapProvider<NoOpHandoverAlgorithm> (this);
}


NoOpHandoverAlgorithm::~NoOpHandoverAlgorithm ()
{
}


void
NoOpHandoverAlgorithm::DoDispose ()
{
  delete m_handoverManagementSapProvider;
}


TypeId
NoOpHandoverAlgorithm::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NoOpHandoverAlgorithm")
    .SetParent<LteHandoverAlgorithm> ()
    .AddConstructor<NoOpHandoverAlgorithm> ()
  ;
  return tid;
}


void
NoOpHandoverAlgorithm::SetLteHandoverManagementSapUser (LteHandoverManagementSapUser* s)
{
  m_handoverManagementSapUser = s;
}


LteHandoverManagementSapProvider*
NoOpHandoverAlgorithm::GetLteHandoverManagementSapProvider ()
{
  return m_handoverManagementSapProvider;
}


void
NoOpHandoverAlgorithm::DoInitialize ()
{
  LteHandoverAlgorithm::DoInitialize ();
}


void
NoOpHandoverAlgorithm::DoReportUeMeas (uint16_t rnti,
                                       LteRrcSap::MeasResults measResults)
{
}


} // end of namespace ns3
