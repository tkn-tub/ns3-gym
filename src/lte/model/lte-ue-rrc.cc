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

#include <ns3/fatal-error.h>
#include <ns3/log.h>
#include "ns3/object-map.h"

#include "lte-ue-rrc.h"
#include "lte-rlc.h"


NS_LOG_COMPONENT_DEFINE ("LteUeRrc");

namespace ns3 {



/////////////////////////////
// CMAC SAP forwarder
/////////////////////////////

class UeMemberLteUeCmacSapUser : public LteUeCmacSapUser
{
public:
  UeMemberLteUeCmacSapUser (LteUeRrc* rrc);

  virtual void LcConfigCompleted ();

private:
  LteUeRrc* m_rrc;
};

UeMemberLteUeCmacSapUser::UeMemberLteUeCmacSapUser (LteUeRrc* rrc)
  : m_rrc (rrc)
{
}

void
UeMemberLteUeCmacSapUser::LcConfigCompleted ()
{
  m_rrc->DoLcConfigCompleted ();
}







/////////////////////////////
// ue RRC methods
/////////////////////////////

NS_OBJECT_ENSURE_REGISTERED (LteUeRrc);

LteUeRrc::LteUeRrc ()
  : m_cmacSapProvider (0),
    m_macSapProvider (0),
    m_cellId (0)
{
  NS_LOG_FUNCTION (this);
  m_cmacSapUser = new UeMemberLteUeCmacSapUser (this);
}


LteUeRrc::~LteUeRrc ()
{
  NS_LOG_FUNCTION (this);
}

void
LteUeRrc::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_cmacSapUser;
  m_rlcMap.clear ();
}

TypeId
LteUeRrc::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteUeRrc")
    .SetParent<Object> ()
    .AddConstructor<LteUeRrc> ()
    .AddAttribute ("RlcMap", "List of UE RadioBearerInfo by LCID.",
                   ObjectMapValue (),
                   MakeObjectMapAccessor (&LteUeRrc::m_rlcMap),
                   MakeObjectMapChecker<LteRlc> ())
    .AddAttribute ("CellId",
                   "Serving cell identifier",
                   UintegerValue (1),
                   MakeUintegerAccessor (&LteUeRrc::m_cellId),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("C-RNTI",
                   "Cell Radio Network Temporary Identifier",
                   UintegerValue (1),
                   MakeUintegerAccessor (&LteUeRrc::m_rnti),
                   MakeUintegerChecker<uint16_t> ())
  ;
  return tid;
}

void
LteUeRrc::SetLteUeCmacSapProvider (LteUeCmacSapProvider * s)
{
  NS_LOG_FUNCTION (this << s);
  m_cmacSapProvider = s;
}

LteUeCmacSapUser*
LteUeRrc::GetLteUeCmacSapUser ()
{
  NS_LOG_FUNCTION (this);
  return m_cmacSapUser;
}

void
LteUeRrc::SetLteMacSapProvider (LteMacSapProvider * s)
{
  NS_LOG_FUNCTION (this << s);
  m_macSapProvider = s;
}



void
LteUeRrc::ConfigureUe (uint16_t rnti, uint16_t cellId)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  m_rnti = rnti;
  m_cellId = cellId;
  m_cmacSapProvider->ConfigureUe (rnti);
}

void
LteUeRrc::SetupRadioBearer (uint16_t rnti, EpsBearer bearer, uint8_t lcid)
{
  NS_LOG_FUNCTION (this << (uint32_t)  rnti << (uint32_t) lcid);
  // create RLC instance
  // for now we support RLC SM only

  Ptr<LteRlc> rlc = CreateObject<LteRlcSm> ();
  rlc->SetLteMacSapProvider (m_macSapProvider);
  rlc->SetRnti (rnti);
  rlc->SetLcId (lcid);

  std::map<uint8_t, Ptr<LteRlc> >::iterator it =   m_rlcMap.find (lcid);
  NS_ASSERT_MSG (it == m_rlcMap.end (), "bearer with same lcid already existing");
  m_rlcMap.insert (std::pair<uint8_t, Ptr<LteRlc> > (lcid, rlc));

  m_cmacSapProvider->AddLc (lcid, rlc->GetLteMacSapUser ());
}

void
LteUeRrc::ReleaseRadioBearer (uint16_t rnti, uint8_t lcid)
{
  NS_LOG_FUNCTION (this << (uint32_t)  rnti << (uint32_t) lcid);
  std::map<uint8_t, Ptr<LteRlc> >::iterator it =   m_rlcMap.find (lcid);
  NS_ASSERT_MSG (it != m_rlcMap.end (), "could not find bearer with given lcid");
  m_rlcMap.erase (it);
}

void
LteUeRrc::DoLcConfigCompleted ()
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("not implemented");
}

uint16_t
LteUeRrc::GetRnti ()
{
  NS_LOG_FUNCTION (this);
  return m_rnti;
}

uint16_t
LteUeRrc::GetCellId ()
{
  NS_LOG_FUNCTION (this);
  return m_cellId;
}

std::vector<uint8_t>
LteUeRrc::GetLcIdVector ()
{
  std::vector<uint8_t> v;
  for (std::map<uint8_t, Ptr<LteRlc> >::iterator it = m_rlcMap.begin (); it != m_rlcMap.end (); ++it)
    {
      v.push_back (it->first);
    }
  return v;
}


} // namespace ns3

