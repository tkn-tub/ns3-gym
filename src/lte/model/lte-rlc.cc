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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */


#include "lte-rlc.h"
#include "lte-mac-sap.h"
#include "ff-mac-sched-sap.h"

#include <ns3/log.h>
#include <ns3/simulator.h>

NS_LOG_COMPONENT_DEFINE ("LteRlc");

namespace ns3 {


///////////////////////////////////////

class LteRlcSpecificLteMacSapUser : public LteMacSapUser
{
public:
  LteRlcSpecificLteMacSapUser (LteRlc* rlc);

  // inherited from LteMacSapUser
  virtual void NotifyTxOpportunity (uint32_t bytes);
  virtual void NotifyHarqDeliveryFailure ();
  virtual void ReceivePdu (Ptr<Packet> p);

private:
  LteRlcSpecificLteMacSapUser ();
  LteRlc* m_rlc;
};

LteRlcSpecificLteMacSapUser::LteRlcSpecificLteMacSapUser (LteRlc* rlc)
  : m_rlc (rlc)
{
}

LteRlcSpecificLteMacSapUser::LteRlcSpecificLteMacSapUser ()
{
}

void
LteRlcSpecificLteMacSapUser::NotifyTxOpportunity (uint32_t bytes)
{
  m_rlc->DoNotifyTxOpportunity (bytes);
}

void
LteRlcSpecificLteMacSapUser::NotifyHarqDeliveryFailure ()
{
  m_rlc->DoNotifyHarqDeliveryFailure ();
}

void
LteRlcSpecificLteMacSapUser::ReceivePdu (Ptr<Packet> p)
{
  m_rlc->DoReceivePdu (p);
}


///////////////////////////////////////


LteRlc::LteRlc ()
  : m_macSapProvider (0),
    m_rnti (0),
    m_lcid (0)
{
  NS_LOG_FUNCTION (this);
  m_macSapUser = new LteRlcSpecificLteMacSapUser (this);
}

void
LteRlc::SetRnti (uint8_t rnti)
{
  NS_LOG_FUNCTION (this << (uint32_t)  rnti);
  m_rnti = rnti;
}

void
LteRlc::SetLcId (uint8_t lcId)
{
  NS_LOG_FUNCTION (this << (uint32_t) lcId);
  m_lcid = lcId;
}

LteRlc::~LteRlc ()
{
  NS_LOG_FUNCTION (this);
  delete (m_macSapUser);
}

void
LteRlc::SetLteMacSapProvider (LteMacSapProvider * s)
{
  NS_LOG_FUNCTION (this << s);
  m_macSapProvider = s;
}

LteMacSapUser*
LteRlc::GetLteMacSapUser ()
{
  NS_LOG_FUNCTION (this);
  return m_macSapUser;
}





// //////////////////////////////////////

LteRlcSm::LteRlcSm ()
{
  NS_LOG_FUNCTION (this);
  Simulator::ScheduleNow (&LteRlcSm::Start, this);
}

LteRlcSm::~LteRlcSm ()
{
  NS_LOG_FUNCTION (this);
}

void
LteRlcSm::DoReceivePdu (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << p);
}

void
LteRlcSm::DoNotifyTxOpportunity (uint32_t bytes)
{
  NS_LOG_FUNCTION (this << bytes);
  LteMacSapProvider::TransmitPduParameters params;
  params.pdu = Create<Packet> (bytes);
  params.rnti = m_rnti;
  params.lcid = m_lcid;
  m_macSapProvider->TransmitPdu (params);
}

void
LteRlcSm::DoNotifyHarqDeliveryFailure ()
{
  NS_LOG_FUNCTION (this);
}

void
LteRlcSm::Start ()
{
  NS_LOG_FUNCTION (this);
  LteMacSapProvider::ReportBufferStatusParameters p;
  p.rnti = m_rnti;
  p.lcid = m_lcid;
  p.txQueueSize = 1000000000;

  m_macSapProvider->ReportBufferStatus (p);
}




//////////////////////////////////////////

// LteRlcTm::~LteRlcTm ()
// {
// }

//////////////////////////////////////////

// LteRlcUm::~LteRlcUm ()
// {
// }

//////////////////////////////////////////

// LteRlcAm::~LteRlcAm ()
// {
// }


} // namespace ns3
