/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es> (Based on lte-helper.cc)
 */

#include "ns3/lte-simple-helper.h"
#include "ns3/log.h"
#include "ns3/callback.h"
#include "ns3/config.h"
#include "ns3/simple-channel.h"
#include "ns3/lte-simple-net-device.h"
#include "ns3/error-model.h"


NS_LOG_COMPONENT_DEFINE ("LteSimpleHelper");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (LteSimpleHelper);

LteSimpleHelper::LteSimpleHelper (void)
{
  NS_LOG_FUNCTION (this);
}

void
LteSimpleHelper::DoStart (void)
{
  NS_LOG_FUNCTION (this);

  m_downlinkChannel = CreateObject<SimpleChannel> ();
  m_uplinkChannel = CreateObject<SimpleChannel> ();

  m_rlcStats = CreateObject<RadioBearerStatsCalculator> ();
  m_rlcStats->SetDlOutputFilename("DlRlcStats.csv");
  m_rlcStats->SetUlOutputFilename("UlRlcStats.csv");
  m_pdcpStats = CreateObject<RadioBearerStatsCalculator> ();
  m_pdcpStats->SetDlOutputFilename("DlPdcpStats.csv");
  m_pdcpStats->SetUlOutputFilename("UlPdcpStats.csv");

  Object::DoStart ();
}

LteSimpleHelper::~LteSimpleHelper (void)
{
  NS_LOG_FUNCTION (this);
}

TypeId LteSimpleHelper::GetTypeId (void)
{
  static TypeId
  tid =
    TypeId ("ns3::LteSimpleHelper")
    .SetParent<Object> ()
    .AddConstructor<LteSimpleHelper> ()
    .AddAttribute ("RlcEntity",
                   "Specify which type of RLC will be used. ",
                   EnumValue (RLC_UM),
                   MakeEnumAccessor (&LteSimpleHelper::m_lteRlcEntityType),
                   MakeEnumChecker (RLC_UM, "RlcUm",
                                    RLC_AM, "RlcAm"))
  ;
  return tid;
}

void
LteSimpleHelper::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_downlinkChannel = 0;
  m_uplinkChannel = 0;
  Object::DoDispose ();
}


NetDeviceContainer
LteSimpleHelper::InstallEnbDevice (NodeContainer c)
{
  NS_LOG_FUNCTION (this);
  Start ();  // will run DoStart () if necessary
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<NetDevice> device = InstallSingleEnbDevice (node);
      devices.Add (device);
    }
  return devices;
}

NetDeviceContainer
LteSimpleHelper::InstallUeDevice (NodeContainer c)
{
  NS_LOG_FUNCTION (this);
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<NetDevice> device = InstallSingleUeDevice (node);
      devices.Add (device);
    }
  return devices;
}


Ptr<NetDevice>
LteSimpleHelper::InstallSingleEnbDevice (Ptr<Node> n)
{
  NS_LOG_FUNCTION (this);

  m_enbRrc = CreateObject<LteTestRrc> ();
  m_enbPdcp = CreateObject<LtePdcp> ();

  if (m_lteRlcEntityType == RLC_UM)
    {
      m_enbRlc = CreateObject<LteRlcUm> ();
    }
  else // m_lteRlcEntityType == RLC_AM
    {
      m_enbRlc = CreateObject<LteRlcAm> ();
    }

  m_enbRlc->SetRnti (11);
  m_enbRlc->SetLcId (12);

  Ptr<LteSimpleNetDevice> dlPhy = CreateObject<LteSimpleNetDevice> (n, m_enbRlc) ;
  Ptr<LteSimpleNetDevice> ulPhy = CreateObject<LteSimpleNetDevice> (n, m_enbRlc);

  dlPhy->SetNode (n);
  dlPhy->SetAddress (Mac48Address::Allocate ());

  n->AddDevice (dlPhy);
  n->AddDevice (ulPhy);

  dlPhy->SetChannel (m_downlinkChannel);
  ulPhy->SetChannel (m_uplinkChannel);

  m_enbMac = CreateObject<LteTestMac> ();
  m_enbMac->SetPdcpHeaderPresent (true);
  m_enbMac->SetDevice (dlPhy);

  dlPhy->SetReceiveCallback (MakeCallback (&LteTestMac::Receive, m_enbMac));

  // Connect SAPs: RRC <-> PDCP <-> RLC <-> MAC

  m_enbRrc->SetLtePdcpSapProvider (m_enbPdcp->GetLtePdcpSapProvider ());
  m_enbPdcp->SetLtePdcpSapUser (m_enbRrc->GetLtePdcpSapUser ());

  m_enbPdcp->SetLteRlcSapProvider (m_enbRlc->GetLteRlcSapProvider ());
  m_enbRlc->SetLteRlcSapUser (m_enbPdcp->GetLteRlcSapUser ());

  m_enbRlc->SetLteMacSapProvider (m_enbMac->GetLteMacSapProvider ());
  m_enbMac->SetLteMacSapUser (m_enbRlc->GetLteMacSapUser ());

  return dlPhy;
}

Ptr<NetDevice>
LteSimpleHelper::InstallSingleUeDevice (Ptr<Node> n)
{
  NS_LOG_FUNCTION (this);

  m_ueRrc = CreateObject<LteTestRrc> ();
  m_uePdcp = CreateObject<LtePdcp> ();

  if (m_lteRlcEntityType == RLC_UM)
    {
      m_ueRlc = CreateObject<LteRlcUm> ();
    }
  else // m_lteRlcEntityType == RLC_AM
    {
      m_ueRlc = CreateObject<LteRlcAm> ();
    }

  m_ueRlc->SetRnti (21);
  m_ueRlc->SetLcId (22);

  Ptr<LteSimpleNetDevice> dlPhy = CreateObject<LteSimpleNetDevice> (n, m_ueRlc);
  Ptr<LteSimpleNetDevice> ulPhy = CreateObject<LteSimpleNetDevice> (n, m_ueRlc);

  dlPhy->SetNode (n);
  dlPhy->SetAddress (Mac48Address::Allocate ());

  n->AddDevice (dlPhy);
  n->AddDevice (ulPhy);

  dlPhy->SetChannel (m_downlinkChannel);
  ulPhy->SetChannel (m_uplinkChannel);

  m_ueMac = CreateObject<LteTestMac> ();
  m_ueMac->SetPdcpHeaderPresent (true);
  m_ueMac->SetDevice (ulPhy);

  dlPhy->SetReceiveCallback (MakeCallback (&LteTestMac::Receive, m_ueMac));

  // Connect SAPs: RRC <-> PDCP <-> RLC <-> MAC

  m_ueRrc->SetLtePdcpSapProvider (m_uePdcp->GetLtePdcpSapProvider ());
  m_uePdcp->SetLtePdcpSapUser (m_ueRrc->GetLtePdcpSapUser ());

  m_uePdcp->SetLteRlcSapProvider (m_ueRlc->GetLteRlcSapProvider ());
  m_ueRlc->SetLteRlcSapUser (m_uePdcp->GetLteRlcSapUser ());

  m_ueRlc->SetLteMacSapProvider (m_ueMac->GetLteMacSapProvider ());
  m_ueMac->SetLteMacSapUser (m_ueRlc->GetLteMacSapUser ());

  return dlPhy;
}


void
LteSimpleHelper::EnableLogComponents (void)
{
  LogLevel level = (LogLevel) (LOG_LEVEL_ALL | LOG_PREFIX_TIME | LOG_PREFIX_NODE | LOG_PREFIX_FUNC);

  LogComponentEnable ("Config", level);
  LogComponentEnable ("LteSimpleHelper", level);
  LogComponentEnable ("LteTestEntities", level);
  LogComponentEnable ("LtePdcp", level);
  LogComponentEnable ("LteRlc", level);
  LogComponentEnable ("LteRlcUm", level);
  LogComponentEnable ("LteRlcAm", level);
  LogComponentEnable ("LteSimpleNetDevice", level);
  LogComponentEnable ("SimpleNetDevice", level);
  LogComponentEnable ("SimpleChannel", level);
  LogComponentEnable ("RadioBearerStatsCalculator", level);
  LogComponentEnable ("MacStatsCalculator", level);
}

void
LteSimpleHelper::EnableTraces (void)
{
//   EnableMacTraces ();
  EnableRlcTraces ();
  EnablePdcpTraces ();
}

void
LteSimpleHelper::EnableRlcTraces (void)
{
  EnableDlRlcTraces ();
  EnableUlRlcTraces ();
}


void
LteSimpleHelperDlTxPduCallback (Ptr<RadioBearerStatsCalculator> rlcStats, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_FUNCTION (rlcStats << path << rnti << (uint16_t)lcid << packetSize);
  uint64_t imsi = 111;
  uint16_t cellId = 222;
  rlcStats->DlTxPdu (cellId, imsi, rnti, lcid, packetSize);
}

void
LteSimpleHelperDlRxPduCallback (Ptr<RadioBearerStatsCalculator> rlcStats, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (rlcStats << path << rnti << (uint16_t)lcid << packetSize << delay);
  uint64_t imsi = 333;
  rlcStats->DlRxPdu (imsi, rnti, lcid, packetSize, delay);
}

void
LteSimpleHelper::EnableDlRlcTraces (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Config::Connect ("/NodeList/*/DeviceList/*/LteRlc/TxPDU",
                   MakeBoundCallback (&LteSimpleHelperDlTxPduCallback, m_rlcStats));
  Config::Connect ("/NodeList/*/DeviceList/*/LteRlc/RxPDU",
                   MakeBoundCallback (&LteSimpleHelperDlRxPduCallback, m_rlcStats));
}

void
LteSimpleHelperUlTxPduCallback (Ptr<RadioBearerStatsCalculator> rlcStats, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_FUNCTION (rlcStats << path << rnti << (uint16_t)lcid << packetSize);
  uint64_t imsi = 1111;
  rlcStats->UlTxPdu (imsi, rnti, lcid, packetSize);
}

void
LteSimpleHelperUlRxPduCallback (Ptr<RadioBearerStatsCalculator> rlcStats, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_FUNCTION (rlcStats << path << rnti << (uint16_t)lcid << packetSize << delay);
  uint64_t imsi = 444;
  uint16_t cellId = 555;
  rlcStats->UlRxPdu (cellId, imsi, rnti, lcid, packetSize, delay);
}


void
LteSimpleHelper::EnableUlRlcTraces (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Config::Connect ("/NodeList/*/DeviceList/*/LteRlc/TxPDU",
                   MakeBoundCallback (&LteSimpleHelperUlTxPduCallback, m_rlcStats));
  Config::Connect ("/NodeList/*/DeviceList/*/LteRlc/RxPDU",
                   MakeBoundCallback (&LteSimpleHelperUlRxPduCallback, m_rlcStats));
}


void
LteSimpleHelper::EnablePdcpTraces (void)
{
  EnableDlPdcpTraces ();
  EnableUlPdcpTraces ();
}

void
LteSimpleHelper::EnableDlPdcpTraces (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Config::Connect ("/NodeList/*/DeviceList/*/LtePdcp/TxPDU",
                   MakeBoundCallback (&LteSimpleHelperDlTxPduCallback, m_pdcpStats));
  Config::Connect ("/NodeList/*/DeviceList/*/LtePdcp/RxPDU",
                   MakeBoundCallback (&LteSimpleHelperDlRxPduCallback, m_pdcpStats));
}

void
LteSimpleHelper::EnableUlPdcpTraces (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Config::Connect ("/NodeList/*/DeviceList/*/LtePdcp/TxPDU",
                   MakeBoundCallback (&LteSimpleHelperUlTxPduCallback, m_pdcpStats));
  Config::Connect ("/NodeList/*/DeviceList/*/LtePdcp/RxPDU",
                   MakeBoundCallback (&LteSimpleHelperUlRxPduCallback, m_pdcpStats));
}


} // namespace ns3
