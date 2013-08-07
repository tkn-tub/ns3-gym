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

#include "handover-algorithm.h"
#include <ns3/handover-management-sap.h>
#include <ns3/log.h>

NS_LOG_COMPONENT_DEFINE ("HandoverAlgorithm");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (HandoverAlgorithm);


HandoverAlgorithm::HandoverAlgorithm ()
{
}


HandoverAlgorithm::~HandoverAlgorithm ()
{
}


void
HandoverAlgorithm::DoDispose ()
{
}


TypeId
HandoverAlgorithm::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HandoverAlgorithm")
    .SetParent<Object> ()
  ;
  return tid;
}



///////////////////////////////////////////
// Handover Management SAP forwarder
///////////////////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (BareHandoverAlgorithm);


/**
 * \brief Class for forwarding Handover Management SAP Provider functions, used
 *        by ns3::BareHandoverAlgorithm.
 */
class BareMemberHandoverManagementSapProvider : public HandoverManagementSapProvider
{
public:
  BareMemberHandoverManagementSapProvider (BareHandoverAlgorithm* handoverAlgorithm);

  // methods inherited from HandoverManagementSapProvider go here
  virtual void ReportUeMeas (uint16_t rnti, LteRrcSap::MeasResults measResults);

private:
  BareHandoverAlgorithm* m_handoverAlgorithm;
};

BareMemberHandoverManagementSapProvider::BareMemberHandoverManagementSapProvider (BareHandoverAlgorithm* handoverAlgorithm)
  : m_handoverAlgorithm (handoverAlgorithm)
{
}

void
BareMemberHandoverManagementSapProvider::ReportUeMeas (uint16_t rnti,
                                                       LteRrcSap::MeasResults measResults)
{
  m_handoverAlgorithm->DoReportUeMeas (rnti, measResults);
}



///////////////////////////////////////////
// Bare Handover Algorithm
///////////////////////////////////////////


BareHandoverAlgorithm::BareHandoverAlgorithm ()
  : m_handoverManagementSapUser (0)
{
  m_handoverManagementSapProvider = new BareMemberHandoverManagementSapProvider (this);
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
    .SetParent<HandoverAlgorithm> ()
    .AddConstructor<BareHandoverAlgorithm> ()
  ;
  return tid;
}


void
BareHandoverAlgorithm::SetHandoverManagementSapUser (HandoverManagementSapUser* s)
{
  m_handoverManagementSapUser = s;
}


HandoverManagementSapProvider*
BareHandoverAlgorithm::GetHandoverManagementSapProvider ()
{
  return m_handoverManagementSapProvider;
}


void
BareHandoverAlgorithm::DoInitialize ()
{
  HandoverAlgorithm::DoInitialize ();
}


void
BareHandoverAlgorithm::DoReportUeMeas (uint16_t rnti,
                                       LteRrcSap::MeasResults measResults)
{
}



} // end of namespace ns3
