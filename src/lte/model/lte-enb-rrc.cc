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
#include "ns3/pointer.h"

#include "lte-enb-rrc.h"
#include "lte-rlc.h"
#include "ns3/object-map.h"


NS_LOG_COMPONENT_DEFINE ("LteEnbRrc");

namespace ns3 {





// ///////////////////////////
// CMAC SAP forwarder
// ///////////////////////////

class EnbRrcMemberLteEnbCmacSapUser : public LteEnbCmacSapUser
{
public:
  EnbRrcMemberLteEnbCmacSapUser (LteEnbRrc* rrc);

  virtual void NotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success);

private:
  LteEnbRrc* m_rrc;
};

EnbRrcMemberLteEnbCmacSapUser::EnbRrcMemberLteEnbCmacSapUser (LteEnbRrc* rrc)
  : m_rrc (rrc)
{
}

void
EnbRrcMemberLteEnbCmacSapUser::NotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success)
{
  m_rrc->DoNotifyLcConfigResult (rnti, lcid, success);
}



// /////////////////////////////////////////
// per-UE radio bearer info management
// /////////////////////////////////////////

class EnbRadioBearerInfo : public Object
{

public:
  EnbRadioBearerInfo (void);
  virtual ~EnbRadioBearerInfo (void);
  static TypeId GetTypeId (void);

  void SetRlc (Ptr<LteRlc> rlc);

private:
  Ptr<LteRlc> m_rlc;

};

NS_OBJECT_ENSURE_REGISTERED (EnbRadioBearerInfo);

EnbRadioBearerInfo::EnbRadioBearerInfo (void)
{
  // Nothing to do here
}

EnbRadioBearerInfo::~EnbRadioBearerInfo (void)
{
  // Nothing to do here
}

TypeId EnbRadioBearerInfo::GetTypeId (void)
{
  static TypeId
  tid =
    TypeId ("ns3::EnbRadioBearerInfo")
    .SetParent<Object> ()
    .AddConstructor<EnbRadioBearerInfo> ()
    .AddAttribute ("LteRlc", "RLC instance of the radio bearer.",
                   PointerValue (),
                   MakePointerAccessor (&EnbRadioBearerInfo::m_rlc),
                   MakePointerChecker<LteRlc> ())
  ;
  return tid;
}

void EnbRadioBearerInfo::SetRlc (Ptr<LteRlc> rlc)
{
  m_rlc = rlc;
}


NS_OBJECT_ENSURE_REGISTERED (UeInfo);

UeInfo::UeInfo (void)
  : m_lastAllocatedId (0)
{
  m_imsi = 0;
}

UeInfo::UeInfo (uint64_t imsi)
  : m_lastAllocatedId (0)
{
  m_imsi = imsi;
}



UeInfo::~UeInfo (void)
{
  // Nothing to do here
}

TypeId UeInfo::GetTypeId (void)
{
  static TypeId  tid = TypeId ("ns3::UeInfo")
    .SetParent<Object> ()
    .AddConstructor<UeInfo> ()
    .AddAttribute ("RadioBearerMap", "List of UE RadioBearerInfo by LCID.",
                   ObjectMapValue (),
                   MakeObjectMapAccessor (&UeInfo::m_rbMap),
                   MakeObjectMapChecker<EnbRadioBearerInfo> ())
  ;
  return tid;
}

uint64_t
UeInfo::GetImsi (void)
{
  return m_imsi;
}

uint8_t
UeInfo::AddRadioBearer (Ptr<EnbRadioBearerInfo> rbi)
{
  NS_LOG_FUNCTION (this);
  for (uint8_t lcid = m_lastAllocatedId; lcid != m_lastAllocatedId - 1; ++lcid)
    {
      if (lcid != 0)
        {
          if (m_rbMap.find (lcid) == m_rbMap.end ())
            {
              m_rbMap.insert (std::pair<uint8_t, Ptr<EnbRadioBearerInfo> > (lcid, rbi));
              m_lastAllocatedId = lcid;
              return lcid;
            }
        }
    }
  NS_LOG_WARN ("no more logical channel ids available");
  return 0;
}

Ptr<EnbRadioBearerInfo>
UeInfo::GetRadioBearer (uint8_t lcid)
{
  NS_LOG_FUNCTION (this << (uint32_t) lcid);
  NS_ASSERT (0 != lcid);
  std::map <uint8_t, Ptr<EnbRadioBearerInfo> >::iterator it = m_rbMap.find (lcid);
  if (it != m_rbMap.end ())
    {
      return m_rbMap.find (lcid)->second;
    }
  else
    {
      return 0;
    }
}


void
UeInfo::RemoveRadioBearer (uint8_t lcid)
{
  NS_LOG_FUNCTION (this << (uint32_t) lcid);
  std::map <uint8_t, Ptr<EnbRadioBearerInfo> >::iterator it = m_rbMap.find (lcid);
  NS_ASSERT_MSG (it != m_rbMap.end (), "request to remove radio bearer with unknown lcid " << lcid);
  m_rbMap.erase (it);
}





// ///////////////////////////
// eNB RRC methods
// ///////////////////////////

NS_OBJECT_ENSURE_REGISTERED (LteEnbRrc);

LteEnbRrc::LteEnbRrc ()
  : m_cmacSapProvider (0),
    m_ffMacSchedSapProvider (0),
    m_macSapProvider (0),
    m_configured (false),
    m_lastAllocatedRnti (0)
{
  NS_LOG_FUNCTION (this);
  m_cmacSapUser = new EnbRrcMemberLteEnbCmacSapUser (this);
}


LteEnbRrc::~LteEnbRrc ()
{
  NS_LOG_FUNCTION (this);
}


void
LteEnbRrc::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_cmacSapUser;
}

TypeId
LteEnbRrc::GetTypeId (void)
{
  NS_LOG_FUNCTION ("LteEnbRrc::GetTypeId");
  static TypeId tid = TypeId ("ns3::LteEnbRrc")
    .SetParent<Object> ()
    .AddConstructor<LteEnbRrc> ()
    .AddAttribute ("UeMap", "List of UE Info by C-RNTI.",
                   ObjectMapValue (),
                   MakeObjectMapAccessor (&LteEnbRrc::m_ueMap),
                   MakeObjectMapChecker<UeInfo> ())
  ;
  return tid;
}

uint16_t
LteEnbRrc::GetLastAllocatedRnti () const
{
  NS_LOG_FUNCTION (this);
  return m_lastAllocatedRnti;
}
std::map<uint16_t,Ptr<UeInfo> > LteEnbRrc::GetUeMap (void) const
{
  return m_ueMap;
}

void LteEnbRrc::SetUeMap (std::map<uint16_t,Ptr<UeInfo> > ueMap)
{
  this->m_ueMap = ueMap;
}


void
LteEnbRrc::SetLastAllocatedRnti (uint16_t lastAllocatedRnti)
{
  NS_LOG_FUNCTION (this << lastAllocatedRnti);
  m_lastAllocatedRnti = lastAllocatedRnti;
}



void
LteEnbRrc::SetLteEnbCmacSapProvider (LteEnbCmacSapProvider * s)
{
  NS_LOG_FUNCTION (this << s);
  m_cmacSapProvider = s;
}

LteEnbCmacSapUser*
LteEnbRrc::GetLteEnbCmacSapUser ()
{
  NS_LOG_FUNCTION (this);
  return m_cmacSapUser;
}

void
LteEnbRrc::SetFfMacSchedSapProvider (FfMacSchedSapProvider * s)
{
  NS_LOG_FUNCTION (this);
  m_ffMacSchedSapProvider = s;
}


void
LteEnbRrc::SetLteMacSapProvider (LteMacSapProvider * s)
{
  NS_LOG_FUNCTION (this);
  m_macSapProvider = s;
}


void
LteEnbRrc::ConfigureCell (uint8_t ulBandwidth, uint8_t dlBandwidth)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (!m_configured);
  m_cmacSapProvider->ConfigureMac (ulBandwidth, dlBandwidth);
  m_configured = true;
}

uint16_t
LteEnbRrc::AddUe (uint64_t imsi)
{
  NS_LOG_FUNCTION (this << imsi);
  // no Call Admission Control for now
  uint16_t rnti = CreateUeInfo (imsi); // side effect: create UeInfo for this UE
  NS_ASSERT_MSG (rnti != 0, "CreateUeInfo returned RNTI==0");
  m_cmacSapProvider->AddUe (rnti);
  return rnti;
}

void
LteEnbRrc::RemoveUe (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  RemoveUeInfo (rnti);
  NS_FATAL_ERROR ("missing RemoveUe method in CMAC SAP");
}

uint8_t
LteEnbRrc::SetupRadioBearer (uint16_t rnti, EpsBearer bearer)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  Ptr<UeInfo> ueInfo = GetUeInfo (rnti);

  // create RLC instance
  // for now we support RLC SM only

  Ptr<LteRlc> rlc = CreateObject<LteRlcSm> ();
  rlc->SetLteMacSapProvider (m_macSapProvider);
  rlc->SetRnti (rnti);

  Ptr<EnbRadioBearerInfo> rbInfo = CreateObject<EnbRadioBearerInfo> ();
  rbInfo->SetRlc (rlc);
  uint8_t lcid = ueInfo->AddRadioBearer (rbInfo);
  rlc->SetLcId (lcid);

  LteEnbCmacSapProvider::LcInfo lcinfo;
  lcinfo.rnti = rnti;
  lcinfo.lcId = lcid;
  lcinfo.lcGroup = 0; // TBD
  lcinfo.qci = bearer.qci;
  lcinfo.isGbr = bearer.IsGbr ();
  lcinfo.mbrUl = bearer.gbrQosInfo.mbrUl;
  lcinfo.mbrDl = bearer.gbrQosInfo.mbrDl;
  lcinfo.gbrUl = bearer.gbrQosInfo.gbrUl;
  lcinfo.gbrDl = bearer.gbrQosInfo.gbrDl;
  m_cmacSapProvider->AddLc (lcinfo, rlc->GetLteMacSapUser ());

  return lcid;
}

void
LteEnbRrc::ReleaseRadioBearer (uint16_t rnti, uint8_t lcId)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  Ptr<UeInfo> ueInfo = GetUeInfo (rnti);
  ueInfo->RemoveRadioBearer (lcId);
}

void
LteEnbRrc::DoNotifyLcConfigResult (uint16_t rnti, uint8_t lcid, bool success)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  NS_FATAL_ERROR ("not implemented");
}



// /////////////////////////////////////////
// management of multiple UE info instances
// /////////////////////////////////////////


uint16_t
LteEnbRrc::CreateUeInfo (uint64_t imsi)
{
  NS_LOG_FUNCTION (this << imsi);
  for (uint16_t rnti = m_lastAllocatedRnti; rnti != m_lastAllocatedRnti - 1; ++rnti)
    {
      if (rnti != 0)
        {
          if (m_ueMap.find (rnti) == m_ueMap.end ())
            {
              m_lastAllocatedRnti = rnti;
              m_ueMap.insert (std::pair<uint16_t, Ptr<UeInfo> > (rnti, CreateObject<UeInfo> (imsi)));
              return rnti;
            }
        }
    }
  return 0;
}

Ptr<UeInfo>
LteEnbRrc::GetUeInfo (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  NS_ASSERT (0 != rnti);
  return m_ueMap.find (rnti)->second;
}

void
LteEnbRrc::RemoveUeInfo (uint16_t rnti)
{
  NS_LOG_FUNCTION (this << (uint32_t) rnti);
  std::map <uint16_t, Ptr<UeInfo> >::iterator it = m_ueMap.find (rnti);
  NS_ASSERT_MSG (it != m_ueMap.end (), "request to remove UE info with unknown rnti " << rnti);
  m_ueMap.erase (it);
}





} // namespace ns3

