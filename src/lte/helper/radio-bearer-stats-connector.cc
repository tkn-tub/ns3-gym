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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */


#include <ns3/log.h>


#include "radio-bearer-stats-connector.h"
#include "radio-bearer-stats-calculator.h"
#include <ns3/lte-enb-rrc.h>
#include <ns3/lte-enb-net-device.h>
#include <ns3/lte-ue-rrc.h>
#include <ns3/lte-ue-net-device.h>

NS_LOG_COMPONENT_DEFINE ("RadioBearerStatsConnector");


namespace ns3 {



bool
operator < (const RadioBearerStatsConnector::CellIdRnti& a, const RadioBearerStatsConnector::CellIdRnti& b)
{
  return ( (a.cellId < b.cellId) || ( (a.cellId == b.cellId) && (a.rnti < b.rnti) ) );
}

struct BoundCallbackArgument : public SimpleRefCount<BoundCallbackArgument>
{
public:
  Ptr<RadioBearerStatsCalculator> stats;
  uint64_t imsi;
  uint16_t cellId;
};


void
DlTxPduCallback (Ptr<BoundCallbackArgument> arg, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_LOGIC (path << rnti << (uint16_t)lcid << packetSize);
  arg->stats->DlTxPdu (arg->cellId, arg->imsi, rnti, lcid, packetSize);
}

void
DlRxPduCallback (Ptr<BoundCallbackArgument> arg, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_LOGIC (path << rnti << (uint16_t)lcid << packetSize << delay);
  arg->stats->DlRxPdu (arg->cellId, arg->imsi, rnti, lcid, packetSize, delay);
}



void
UlTxPduCallback (Ptr<BoundCallbackArgument> arg, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize)
{
  NS_LOG_LOGIC (path << rnti << (uint16_t)lcid << packetSize);
 
  arg->stats->UlTxPdu (arg->cellId, arg->imsi, rnti, lcid, packetSize);
}

void
UlRxPduCallback (Ptr<BoundCallbackArgument> arg, std::string path,
                 uint16_t rnti, uint8_t lcid, uint32_t packetSize, uint64_t delay)
{
  NS_LOG_LOGIC (path << rnti << (uint16_t)lcid << packetSize << delay);
 
  arg->stats->UlRxPdu (arg->cellId, arg->imsi, rnti, lcid, packetSize, delay);
}



RadioBearerStatsConnector::RadioBearerStatsConnector ()
  : m_connected (false)
{
}

void 
RadioBearerStatsConnector::EnableRlcStats (Ptr<RadioBearerStatsCalculator> rlcStats)
{
  m_rlcStats = rlcStats;
  EnsureConnected ();
}

void 
RadioBearerStatsConnector::EnablePdcpStats (Ptr<RadioBearerStatsCalculator> pdcpStats)
{
  m_pdcpStats = pdcpStats;
  EnsureConnected ();
}

void 
RadioBearerStatsConnector::EnsureConnected ()
{
  NS_LOG_FUNCTION (this);
  if (!m_connected)
    {
      Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/NewUeContext",
		       MakeBoundCallback (&RadioBearerStatsConnector::NotifyNewUeContextEnb, this));
      Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/RandomAccessSuccessful",
		       MakeBoundCallback (&RadioBearerStatsConnector::NotifyRandomAccessSuccessfulUe, this));
      Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/ConnectionReconfiguration",
		       MakeBoundCallback (&RadioBearerStatsConnector::NotifyConnectionReconfigurationEnb, this));
      Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/ConnectionReconfiguration",
		       MakeBoundCallback (&RadioBearerStatsConnector::NotifyConnectionReconfigurationUe, this));
      Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverStart",
		       MakeBoundCallback (&RadioBearerStatsConnector::NotifyHandoverStartEnb, this));
      Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverStart",
		       MakeBoundCallback (&RadioBearerStatsConnector::NotifyHandoverStartUe, this));
      Config::Connect ("/NodeList/*/DeviceList/*/LteEnbRrc/HandoverEndOk",
		       MakeBoundCallback (&RadioBearerStatsConnector::NotifyHandoverEndOkEnb, this));
      Config::Connect ("/NodeList/*/DeviceList/*/LteUeRrc/HandoverEndOk",
		       MakeBoundCallback (&RadioBearerStatsConnector::NotifyHandoverEndOkUe, this));
      m_connected = true;
    }
}

void 
RadioBearerStatsConnector::NotifyRandomAccessSuccessfulUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  c->ConnectSrb0Traces (context, imsi, cellId, rnti);
}

void 
RadioBearerStatsConnector::NotifyConnectionSetupUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  c->ConnectSrb1TracesUe (context, imsi, cellId, rnti);
}

void 
RadioBearerStatsConnector::NotifyConnectionReconfigurationUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  c->ConnectTracesUeIfFirstTime (context, imsi, cellId, rnti);
}

void 
RadioBearerStatsConnector::NotifyHandoverStartUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti, uint16_t targetCellId)
{
  c->DisconnectTracesUe (context, imsi, cellId, rnti);
}

void 
RadioBearerStatsConnector::NotifyHandoverEndOkUe (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  c->ConnectTracesUe (context, imsi, cellId, rnti);
}

void 
RadioBearerStatsConnector::NotifyNewUeContextEnb (RadioBearerStatsConnector* c, std::string context, uint16_t cellId, uint16_t rnti)
{
  c->StoreUeManagerPath (context, cellId, rnti);
}

void 
RadioBearerStatsConnector::NotifyConnectionReconfigurationEnb (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  c->ConnectTracesEnbIfFirstTime (context, imsi, cellId, rnti);
}

void 
RadioBearerStatsConnector::NotifyHandoverStartEnb (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti, uint16_t targetCellId)
{
  c->DisconnectTracesEnb (context, imsi, cellId, rnti);
}

void 
RadioBearerStatsConnector::NotifyHandoverEndOkEnb (RadioBearerStatsConnector* c, std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  c->ConnectTracesEnb (context, imsi, cellId, rnti);
}

void 
RadioBearerStatsConnector::StoreUeManagerPath (std::string context, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << context << cellId << rnti);
  std::ostringstream ueManagerPath;
  ueManagerPath <<  context.substr (0, context.rfind ("/")) << "/UeMap/" << (uint32_t) rnti;
  CellIdRnti key;
  key.cellId = cellId;
  key.rnti = rnti;
  m_ueManagerPathByCellIdRnti[key] = ueManagerPath.str ();
}

void 
RadioBearerStatsConnector::ConnectSrb0Traces (std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << imsi << cellId << rnti);
  std::string ueRrcPath =  context.substr (0, context.rfind ("/"));
  CellIdRnti key;
  key.cellId = cellId;
  key.rnti = rnti;
  std::map<CellIdRnti, std::string>::iterator it = m_ueManagerPathByCellIdRnti.find (key);
  NS_ASSERT (it != m_ueManagerPathByCellIdRnti.end ());
  std::string ueManagerPath = it->second;  
  NS_LOG_LOGIC (this << " ueManagerPath: " << ueManagerPath);
  m_ueManagerPathByCellIdRnti.erase (it);

  if (m_rlcStats)
    {
      Ptr<BoundCallbackArgument> arg = Create<BoundCallbackArgument> ();
      arg->imsi = imsi;
      arg->cellId = cellId; 
      arg->stats = m_rlcStats;

      // diconnect eventually previously connected SRB0 both at UE and eNB
      Config::Disconnect (ueRrcPath + "/Srb0/LteRlc/TxPDU",
                          MakeBoundCallback (&UlTxPduCallback, arg));
      Config::Disconnect (ueRrcPath + "/Srb0/LteRlc/RxPDU",
                          MakeBoundCallback (&DlRxPduCallback, arg));
      Config::Disconnect (ueManagerPath + "/Srb0/LteRlc/TxPDU",
                          MakeBoundCallback (&DlTxPduCallback, arg));
      Config::Disconnect (ueManagerPath + "/Srb0/LteRlc/RxPDU",
                          MakeBoundCallback (&UlRxPduCallback, arg));

      // connect SRB0 both at UE and eNB
      Config::Connect (ueRrcPath + "/Srb0/LteRlc/TxPDU",
                       MakeBoundCallback (&UlTxPduCallback, arg));
      Config::Connect (ueRrcPath + "/Srb0/LteRlc/RxPDU",
                       MakeBoundCallback (&DlRxPduCallback, arg));
      Config::Connect (ueManagerPath + "/Srb0/LteRlc/TxPDU",
                       MakeBoundCallback (&DlTxPduCallback, arg));
      Config::Connect (ueManagerPath + "/Srb0/LteRlc/RxPDU",
                       MakeBoundCallback (&UlRxPduCallback, arg));

      // connect SRB1 at eNB only (at UE SRB1 will be setup later)
      Config::Connect (ueManagerPath + "/Srb1/LteRlc/TxPDU",
                       MakeBoundCallback (&DlTxPduCallback, arg));
      Config::Connect (ueManagerPath + "/Srb1/LteRlc/RxPDU",
                       MakeBoundCallback (&UlRxPduCallback, arg));
    }
  if (m_pdcpStats)
    {
      Ptr<BoundCallbackArgument> arg = Create<BoundCallbackArgument> ();
      arg->imsi = imsi;
      arg->cellId = cellId; 
      arg->stats = m_pdcpStats;

      // connect SRB1 at eNB only (at UE SRB1 will be setup later)
      Config::Connect (ueManagerPath + "/Srb1/LtePdcp/RxPDU",
		       MakeBoundCallback (&UlRxPduCallback, arg));
      Config::Connect (ueManagerPath + "/Srb1/LtePdcp/TxPDU",
		       MakeBoundCallback (&DlTxPduCallback, arg));
    }
}

void 
RadioBearerStatsConnector::ConnectSrb1TracesUe (std::string ueRrcPath, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << imsi << cellId << rnti);
   if (m_rlcStats)
    {
      Ptr<BoundCallbackArgument> arg = Create<BoundCallbackArgument> ();
      arg->imsi = imsi;
      arg->cellId = cellId; 
      arg->stats = m_rlcStats;
      Config::Connect (ueRrcPath + "/Srb1/LteRlc/TxPDU",
                       MakeBoundCallback (&UlTxPduCallback, arg));
      Config::Connect (ueRrcPath + "/Srb1/LteRlc/RxPDU",
                       MakeBoundCallback (&DlRxPduCallback, arg));
    }
  if (m_pdcpStats)
    {
      Ptr<BoundCallbackArgument> arg = Create<BoundCallbackArgument> ();
      arg->imsi = imsi;
      arg->cellId = cellId; 
      arg->stats = m_pdcpStats;
      Config::Connect (ueRrcPath + "/Srb1/LtePdcp/RxPDU",
		       MakeBoundCallback (&DlRxPduCallback, arg));
      Config::Connect (ueRrcPath + "/Srb1/LtePdcp/TxPDU",
		       MakeBoundCallback (&UlTxPduCallback, arg));
    }
}
  
void 
RadioBearerStatsConnector::ConnectTracesUeIfFirstTime (std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << context);
  if (m_imsiSeenUe.find (imsi) == m_imsiSeenUe.end ())
    {
      m_imsiSeenUe.insert (imsi);
      ConnectTracesUe (context, imsi, cellId, rnti);
    }
}
 
void 
RadioBearerStatsConnector::ConnectTracesEnbIfFirstTime (std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << context);
   if (m_imsiSeenEnb.find (imsi) == m_imsiSeenEnb.end ())
    {
      m_imsiSeenEnb.insert (imsi);
      ConnectTracesEnb (context, imsi, cellId, rnti);
    }
}

void 
RadioBearerStatsConnector::ConnectTracesUe (std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << context);
  NS_LOG_LOGIC (this << "expected context should match /NodeList/*/DeviceList/*/LteUeRrc/");
  std::string basePath = context.substr (0, context.rfind ("/"));
  if (m_rlcStats)
    {
      Ptr<BoundCallbackArgument> arg = Create<BoundCallbackArgument> ();
      arg->imsi = imsi;
      arg->cellId = cellId; 
      arg->stats = m_rlcStats;
      Config::Connect (basePath + "/DataRadioBearerMap/*/LteRlc/TxPDU",
		       MakeBoundCallback (&UlTxPduCallback, arg));
      Config::Connect (basePath + "/DataRadioBearerMap/*/LteRlc/RxPDU",
		       MakeBoundCallback (&DlRxPduCallback, arg));
      Config::Connect (basePath + "/Srb1/LteRlc/TxPDU",
		       MakeBoundCallback (&UlTxPduCallback, arg));
      Config::Connect (basePath + "/Srb1/LteRlc/RxPDU",
		       MakeBoundCallback (&DlRxPduCallback, arg));

    }
  if (m_pdcpStats)
    {
      Ptr<BoundCallbackArgument> arg = Create<BoundCallbackArgument> ();
      arg->imsi = imsi;
      arg->cellId = cellId; 
      arg->stats = m_pdcpStats;
      Config::Connect (basePath + "/DataRadioBearerMap/*/LtePdcp/RxPDU",
		       MakeBoundCallback (&DlRxPduCallback, arg));
      Config::Connect (basePath + "/DataRadioBearerMap/*/LtePdcp/TxPDU",
		       MakeBoundCallback (&UlTxPduCallback, arg));
      Config::Connect (basePath + "/Srb1/LtePdcp/RxPDU",
		       MakeBoundCallback (&DlRxPduCallback, arg));
      Config::Connect (basePath + "/Srb1/LtePdcp/TxPDU",
		       MakeBoundCallback (&UlTxPduCallback, arg));
    }
}

void 
RadioBearerStatsConnector::ConnectTracesEnb (std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this << context);
  NS_LOG_LOGIC (this << "expected context  should match /NodeList/*/DeviceList/*/LteEnbRrc/");
  std::ostringstream basePath;
  basePath <<  context.substr (0, context.rfind ("/")) << "/UeMap/" << (uint32_t) rnti;
  if (m_rlcStats)
    {
      Ptr<BoundCallbackArgument> arg = Create<BoundCallbackArgument> ();
      arg->imsi = imsi;
      arg->cellId = cellId; 
      arg->stats = m_rlcStats;
      Config::Connect (basePath.str () + "/DataRadioBearerMap/*/LteRlc/RxPDU",
		       MakeBoundCallback (&UlRxPduCallback, arg));
      Config::Connect (basePath.str () + "/DataRadioBearerMap/*/LteRlc/TxPDU",
		       MakeBoundCallback (&DlTxPduCallback, arg));
      Config::Connect (basePath.str () + "/Srb0/LteRlc/RxPDU",
		       MakeBoundCallback (&UlRxPduCallback, arg));
      Config::Connect (basePath.str () + "/Srb0/LteRlc/TxPDU",
		       MakeBoundCallback (&DlTxPduCallback, arg));
      Config::Connect (basePath.str () + "/Srb1/LteRlc/RxPDU",
		       MakeBoundCallback (&UlRxPduCallback, arg));
      Config::Connect (basePath.str () + "/Srb1/LteRlc/TxPDU",
		       MakeBoundCallback (&DlTxPduCallback, arg));
    }
  if (m_pdcpStats)
    {
      Ptr<BoundCallbackArgument> arg = Create<BoundCallbackArgument> ();
      arg->imsi = imsi;
      arg->cellId = cellId; 
      arg->stats = m_pdcpStats;
      Config::Connect (basePath.str () + "/DataRadioBearerMap/*/LtePdcp/TxPDU",
		       MakeBoundCallback (&DlTxPduCallback, arg));
      Config::Connect (basePath.str () + "/DataRadioBearerMap/*/LtePdcp/RxPDU",
		       MakeBoundCallback (&UlRxPduCallback, arg));
      Config::Connect (basePath.str () + "/Srb1/LtePdcp/TxPDU",
		       MakeBoundCallback (&DlTxPduCallback, arg));
      Config::Connect (basePath.str () + "/Srb1/LtePdcp/RxPDU",
		       MakeBoundCallback (&UlRxPduCallback, arg));
    }
}

void 
RadioBearerStatsConnector::DisconnectTracesUe (std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this);
}


void 
RadioBearerStatsConnector::DisconnectTracesEnb (std::string context, uint64_t imsi, uint16_t cellId, uint16_t rnti)
{
  NS_LOG_FUNCTION (this);
}



} // namespace ns3
