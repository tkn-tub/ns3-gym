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

#include <ns3/epc-helper.h>

#include "lte-enb-net-device.h"
#include "epc-ue-nas.h"
#include "lte-as-sap.h"

NS_LOG_COMPONENT_DEFINE ("EpcUeNas");

namespace ns3 {



NS_OBJECT_ENSURE_REGISTERED (EpcUeNas);

EpcUeNas::EpcUeNas ()
  : m_asSapProvider (0),
    m_bidCounter (0)
{
  NS_LOG_FUNCTION (this);
  m_asSapUser = new MemberLteAsSapUser<EpcUeNas> (this);
}


EpcUeNas::~EpcUeNas ()
{
  NS_LOG_FUNCTION (this);
}

void
EpcUeNas::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_asSapUser;
  m_epcHelper = 0;
}

TypeId
EpcUeNas::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EpcUeNas")
    .SetParent<Object> ()
    .AddConstructor<EpcUeNas> ()
  ;
  return tid;
}


void 
EpcUeNas::SetEpcHelper (Ptr<EpcHelper> epcHelper)
{
  m_epcHelper = epcHelper;
}

void 
EpcUeNas::SetDevice (Ptr<NetDevice> dev)
{
  m_device = dev;
}

void 
EpcUeNas::SetImsi (uint64_t imsi)
{
  m_imsi = imsi;
}

void
EpcUeNas::SetAsSapProvider (LteAsSapProvider* s)
{
  m_asSapProvider = s;
}

LteAsSapUser* 
EpcUeNas::GetAsSapUser ()
{
  return m_asSapUser;
}

void 
EpcUeNas::SetForwardUpCallback (Callback <void, Ptr<Packet> > cb)
{
  m_forwardUpCallback = cb;
}

void 
EpcUeNas::Connect (Ptr<NetDevice> enbDevice)
{

  // since RRC Idle Mode cell selection is not supported yet, we
  // force the UE RRC to be camped on a specific eNB
  Ptr<LteEnbNetDevice> enbLteDevice = enbDevice->GetObject<LteEnbNetDevice> ();
  m_asSapProvider->ForceCampedOnEnb (enbLteDevice, enbLteDevice->GetCellId ());

  // tell RRC to go into connected mode
  m_asSapProvider->Connect ();

  if (m_epcHelper)
    {
      m_epcHelper->AttachUe (m_device, m_imsi, enbDevice);
      // also activate default EPS bearer
      ActivateEpsBearer (EpsBearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT), EpcTft::Default ());
    }
}

void 
EpcUeNas::ActivateEpsBearer (EpsBearer bearer, Ptr<EpcTft> tft)
{
  NS_ASSERT_MSG (m_bidCounter < 11, "cannot have more than 11 EPS bearers");
  uint8_t bid = ++m_bidCounter;
  m_epcHelper->ActivateEpsBearer (m_device, m_imsi, tft, bearer);
  m_tftClassifier.Add (tft, bid);
}

bool
EpcUeNas::Send (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);
  
  uint32_t id = m_tftClassifier.Classify (packet, EpcTft::UPLINK);
  NS_ASSERT ((id & 0xFFFFFF00) == 0);
  uint8_t bid = (uint8_t) (id & 0x000000FF);
  if (bid == 0)
    {
      return false;
    }
  else
    {
      m_asSapProvider->SendData (packet, bid); 
      return true;
    }
}

void 
EpcUeNas::DoNotifyConnectionSuccessful ()
{
}

void 
EpcUeNas::DoNotifyConnectionFailed ()
{
  NS_FATAL_ERROR ("connection failed, it should not happen with the current model");
}

void
EpcUeNas::DoRecvData (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this);
  m_forwardUpCallback (packet);
}




} // namespace ns3

