/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "wifi-phy.h"
#include "wifi-mode.h"
#include "wifi-channel.h"
#include "wifi-preamble.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/trace-source-accessor.h"
#include <cmath>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("WifiPhy");

/****************************************************************
 *       This destructor is needed.
 ****************************************************************/

WifiPhyListener::~WifiPhyListener ()
{
}

/****************************************************************
 *       The actual WifiPhy class
 ****************************************************************/

NS_OBJECT_ENSURE_REGISTERED (WifiPhy);

TypeId
WifiPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiPhy")
    .SetParent<Object> ()
    .SetGroupName ("Wifi")
    .AddTraceSource ("PhyTxBegin",
                     "Trace source indicating a packet "
                     "has begun transmitting over the channel medium",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyTxBeginTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyTxEnd",
                     "Trace source indicating a packet "
                     "has been completely transmitted over the channel. "
                     "NOTE: the only official WifiPhy implementation "
                     "available to this date (YansWifiPhy) never fires "
                     "this trace source.",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyTxEndTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyTxDrop",
                     "Trace source indicating a packet "
                     "has been dropped by the device during transmission",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyTxDropTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyRxBegin",
                     "Trace source indicating a packet "
                     "has begun being received from the channel medium "
                     "by the device",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyRxBeginTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyRxEnd",
                     "Trace source indicating a packet "
                     "has been completely received from the channel medium "
                     "by the device",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyRxEndTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyRxDrop",
                     "Trace source indicating a packet "
                     "has been dropped by the device during reception",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyRxDropTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("MonitorSnifferRx",
                     "Trace source simulating a wifi device in monitor mode "
                     "sniffing all received frames",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyMonitorSniffRxTrace),
                     "ns3::WifiPhy::MonitorSnifferRxCallback")
    .AddTraceSource ("MonitorSnifferTx",
                     "Trace source simulating the capability of a wifi device "
                     "in monitor mode to sniff all frames being transmitted",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyMonitorSniffTxTrace),
                     "ns3::WifiPhy::MonitorSnifferTxCallback")
  ;
  return tid;
}

WifiPhy::WifiPhy ()
{
  NS_LOG_FUNCTION (this);
  m_totalAmpduSize = 0;
  m_totalAmpduNumSymbols = 0;
}

WifiPhy::~WifiPhy ()
{
  NS_LOG_FUNCTION (this);
}

WifiMode
WifiPhy::GetHTPlcpHeaderMode (WifiMode payloadMode, WifiPreamble preamble)
{
  switch (payloadMode.GetBandwidth ())
    {
    case 20000000:
    default:
      return WifiPhy::GetOfdmRate6_5MbpsBW20MHz ();
    case 40000000:
      return WifiPhy::GetOfdmRate13_5MbpsBW40MHz ();
    }
}

Time
WifiPhy::GetPlcpHtTrainingSymbolDuration (WifiPreamble preamble, WifiTxVector txvector)
{
  uint8_t Ndltf, Neltf;
  //We suppose here that STBC = 0.
  //If STBC > 0, we need a different mapping between Nss and Nltf (IEEE 802.11n-2012 standard, page 1682).
  if (txvector.GetNss () < 3)
    {
      Ndltf = txvector.GetNss ();
    }
  else
    {
      Ndltf = 4;
    }
  if (txvector.GetNess () < 3)
    {
      Neltf = txvector.GetNess ();
    }
  else
    {
      Neltf = 4;
    }

  switch (preamble)
    {
    case WIFI_PREAMBLE_HT_MF:
      return MicroSeconds (4 + (4 * Ndltf) + (4 * Neltf));
    case WIFI_PREAMBLE_HT_GF:
      return MicroSeconds ((4 * Ndltf) + (4 * Neltf));
    default:
      //no training for non HT
      return MicroSeconds (0);
    }
}

Time
WifiPhy::GetPlcpHtSigHeaderDuration (WifiPreamble preamble)
{
  switch (preamble)
    {
    case WIFI_PREAMBLE_HT_MF:
    case WIFI_PREAMBLE_HT_GF:
      //HT-SIG
      return MicroSeconds (8);
    default:
      //no HT-SIG for non HT
      return MicroSeconds (0);
    }
}

WifiMode
WifiPhy::GetPlcpHeaderMode (WifiMode payloadMode, WifiPreamble preamble)
{
  switch (payloadMode.GetModulationClass ())
    {
    case WIFI_MOD_CLASS_OFDM:
    case WIFI_MOD_CLASS_HT:
      {
        switch (payloadMode.GetBandwidth ())
          {
          case 5000000:
            return WifiPhy::GetOfdmRate1_5MbpsBW5MHz ();
          case 10000000:
            return WifiPhy::GetOfdmRate3MbpsBW10MHz ();
          default:
            //(Section 18.3.2 "PLCP frame format"; IEEE Std 802.11-2012)
            //actually this is only the first part of the PlcpHeader,
            //because the last 16 bits of the PlcpHeader are using the
            //same mode of the payload
            return WifiPhy::GetOfdmRate6Mbps ();
          }
      }
    case WIFI_MOD_CLASS_ERP_OFDM:
      return WifiPhy::GetErpOfdmRate6Mbps ();
    case WIFI_MOD_CLASS_DSSS:
      if (preamble == WIFI_PREAMBLE_LONG)
        {
          //(Section 16.2.3 "PLCP field definitions" and Section 17.2.2.2 "Long PPDU format"; IEEE Std 802.11-2012)
          return WifiPhy::GetDsssRate1Mbps ();
        }
      else //WIFI_PREAMBLE_SHORT
        {
          //(Section 17.2.2.3 "Short PPDU format"; IEEE Std 802.11-2012)
          return WifiPhy::GetDsssRate2Mbps ();
        }
    default:
      NS_FATAL_ERROR ("unsupported modulation class");
      return WifiMode ();
    }
}

Time
WifiPhy::GetPlcpHeaderDuration (WifiMode payloadMode, WifiPreamble preamble)
{
  if (preamble == WIFI_PREAMBLE_NONE)
    {
      return MicroSeconds (0);
    }
  switch (payloadMode.GetModulationClass ())
    {
    case WIFI_MOD_CLASS_OFDM:
      {
        switch (payloadMode.GetBandwidth ())
          {
          case 20000000:
          default:
            //(Section 18.3.3 "PLCP preamble (SYNC))" and Figure 18-4 "OFDM training structure"; IEEE Std 802.11-2012)
            //also (Section 18.3.2.4 "Timing related parameters" Table 18-5 "Timing-related parameters"; IEEE Std 802.11-2012)
            //We return the duration of the SIGNAL field only, since the
            //SERVICE field (which strictly speaking belongs to the PLCP
            //header, see Section 18.3.2 and Figure 18-1) is sent using the
            //payload mode.
            return MicroSeconds (4);
          case 10000000:
            //(Section 18.3.2.4 "Timing related parameters" Table 18-5 "Timing-related parameters"; IEEE Std 802.11-2012)
            return MicroSeconds (8);
          case 5000000:
            //(Section 18.3.2.4 "Timing related parameters" Table 18-5 "Timing-related parameters"; IEEE Std 802.11-2012)
            return MicroSeconds (16);
          }
      }
    case WIFI_MOD_CLASS_HT:
      {
        //L-SIG
        //IEEE 802.11n Figure 20.1
        switch (preamble)
          {
          case WIFI_PREAMBLE_HT_MF:
          default:
            return MicroSeconds (4);
          case WIFI_PREAMBLE_HT_GF:
            return MicroSeconds (0);
          }
      }
    case WIFI_MOD_CLASS_ERP_OFDM:
      return MicroSeconds (4);
    case WIFI_MOD_CLASS_DSSS:
      if (preamble == WIFI_PREAMBLE_SHORT)
        {
          //(Section 17.2.2.3 "Short PPDU format" and Figure 17-2 "Short PPDU format"; IEEE Std 802.11-2012)
          return MicroSeconds (24);
        }
      else //WIFI_PREAMBLE_LONG
        {
          //(Section 17.2.2.2 "Long PPDU format" and Figure 17-1 "Short PPDU format"; IEEE Std 802.11-2012)
          return MicroSeconds (48);
        }
    default:
      NS_FATAL_ERROR ("unsupported modulation class");
      return MicroSeconds (0);
    }
}

Time
WifiPhy::GetPlcpPreambleDuration (WifiMode payloadMode, WifiPreamble preamble)
{
  if (preamble == WIFI_PREAMBLE_NONE)
    {
      return MicroSeconds (0);
    }
  switch (payloadMode.GetModulationClass ())
    {
    case WIFI_MOD_CLASS_OFDM:
      {
        switch (payloadMode.GetBandwidth ())
          {
          case 20000000:
          default:
            //(Section 18.3.3 "PLCP preamble (SYNC))" Figure 18-4 "OFDM training structure"
            //also Section 18.3.2.3 "Modulation-dependent parameters" Table 18-4 "Modulation-dependent parameters"; IEEE Std 802.11-2012)
            return MicroSeconds (16);
          case 10000000:
            //(Section 18.3.3 "PLCP preamble (SYNC))" Figure 18-4 "OFDM training structure"
            //also Section 18.3.2.3 "Modulation-dependent parameters" Table 18-4 "Modulation-dependent parameters"; IEEE Std 802.11-2012)
            return MicroSeconds (32);
          case 5000000:
            //(Section 18.3.3 "PLCP preamble (SYNC))" Figure 18-4 "OFDM training structure"
            //also Section 18.3.2.3 "Modulation-dependent parameters" Table 18-4 "Modulation-dependent parameters"; IEEE Std 802.11-2012)
            return MicroSeconds (64);
          }
      }
    case WIFI_MOD_CLASS_HT:
      //IEEE 802.11n Figure 20.1 the training symbols before L_SIG or HT_SIG
      return MicroSeconds (16);
    case WIFI_MOD_CLASS_ERP_OFDM:
      return MicroSeconds (16);
    case WIFI_MOD_CLASS_DSSS:
      if (preamble == WIFI_PREAMBLE_SHORT)
        {
          //(Section 17.2.2.3 "Short PPDU format)" Figure 17-2 "Short PPDU format"; IEEE Std 802.11-2012)
          return MicroSeconds (72);
        }
      else   //WIFI_PREAMBLE_LONG
        {
          //(Section 17.2.2.2 "Long PPDU format)" Figure 17-1 "Long PPDU format"; IEEE Std 802.11-2012)
          return MicroSeconds (144);
        }
    default:
      NS_FATAL_ERROR ("unsupported modulation class");
      return MicroSeconds (0);
    }
}

Time
WifiPhy::GetPayloadDuration (uint32_t size, WifiTxVector txvector, WifiPreamble preamble, double frequency, uint8_t packetType, uint8_t incFlag)
{
  WifiMode payloadMode = txvector.GetMode ();
  NS_LOG_FUNCTION (size << payloadMode);

  switch (payloadMode.GetModulationClass ())
    {
    case WIFI_MOD_CLASS_OFDM:
    case WIFI_MOD_CLASS_ERP_OFDM:
      {
        //(Section 18.3.2.4 "Timing related parameters" Table 18-5 "Timing-related parameters"; IEEE Std 802.11-2012
        //corresponds to T_{SYM} in the table)
        Time symbolDuration;
        switch (payloadMode.GetBandwidth ())
          {
          case 20000000:
          default:
            symbolDuration = MicroSeconds (4);
            break;
          case 10000000:
            symbolDuration = MicroSeconds (8);
            break;
          case 5000000:
            symbolDuration = MicroSeconds (16);
            break;
          }

        //(Section 18.3.2.3 "Modulation-dependent parameters" Table 18-4 "Modulation-dependent parameters"; IEEE Std 802.11-2012)
        //corresponds to N_{DBPS} in the table
        double numDataBitsPerSymbol = payloadMode.GetDataRate () * symbolDuration.GetNanoSeconds () / 1e9;

        //(Section 18.3.5.4 "Pad bits (PAD)" Equation 18-11; IEEE Std 802.11-2012)
        uint32_t numSymbols;

        if (packetType == 1 && preamble != WIFI_PREAMBLE_NONE)
          {
            //First packet in an A-MPDU
            numSymbols = ceil ((16 + size * 8.0 + 6) / numDataBitsPerSymbol);
            if (incFlag == 1)
              {
                m_totalAmpduSize += size;
                m_totalAmpduNumSymbols += numSymbols;
              }
          }
        else if (packetType == 1 && preamble == WIFI_PREAMBLE_NONE)
          {
            //consecutive packets in an A-MPDU
            numSymbols = ((size * 8.0) / numDataBitsPerSymbol);
            if (incFlag == 1)
              {
                m_totalAmpduSize += size;
                m_totalAmpduNumSymbols += numSymbols;
              }
          }
        else if (packetType == 2 && preamble == WIFI_PREAMBLE_NONE)
          {
            //last packet in an A-MPDU
            uint32_t totalAmpduSize = m_totalAmpduSize + size;
            numSymbols = lrint (ceil ((16 + totalAmpduSize * 8.0 + 6) / numDataBitsPerSymbol));
            numSymbols -= m_totalAmpduNumSymbols;
            if (incFlag == 1)
              {
                m_totalAmpduSize = 0;
                m_totalAmpduNumSymbols = 0;
              }
          }
        else if (packetType == 0 && preamble != WIFI_PREAMBLE_NONE)
          {
            //Not an A-MPDU
            numSymbols = lrint (ceil ((16 + size * 8.0 + 6.0) / numDataBitsPerSymbol));
          }
        else
          {
            NS_FATAL_ERROR ("Wrong combination of preamble and packet type");
          }

        //Add signal extension for ERP PHY
        if (payloadMode.GetModulationClass () == WIFI_MOD_CLASS_ERP_OFDM)
          {
            return Time (numSymbols * symbolDuration) + MicroSeconds (6);
          }
        else
          {
            return Time (numSymbols * symbolDuration);
          }
      }
    case WIFI_MOD_CLASS_HT:
      {
        Time symbolDuration;
        double m_Stbc;
        //if short GI data rate is used then symbol duration is 3.6us else symbol duration is 4us
        //In the future has to create a stationmanager that only uses these data rates if sender and reciever support GI
        if (payloadMode.GetUniqueName () == "OfdmRate135MbpsBW40MHzShGi" || payloadMode.GetUniqueName () == "OfdmRate65MbpsBW20MHzShGi" )
          {
            symbolDuration = NanoSeconds (3600);
          }
        else
          {
            switch (payloadMode.GetDataRate () / (txvector.GetNss ()))
              {
              //shortGi
              case 7200000:
              case 14400000:
              case 21700000:
              case 28900000:
              case 43300000:
              case 57800000:
              case 72200000:
              case 15000000:
              case 30000000:
              case 45000000:
              case 60000000:
              case 90000000:
              case 120000000:
              case 150000000:
                symbolDuration = NanoSeconds (3600);
                break;
              default:
                symbolDuration = MicroSeconds (4);
              }
          }

        if (txvector.IsStbc ())
          {
            m_Stbc = 2;
          }
        else
          {
            m_Stbc = 1;
          }

        //check tables 20-35 and 20-36 in the standard to get cases when nes =2
        double Nes = 1;

        //IEEE Std 802.11n, section 20.3.11, equation (20-32)
        uint32_t numSymbols;
        double numDataBitsPerSymbol = payloadMode.GetDataRate () * txvector.GetNss () * symbolDuration.GetNanoSeconds () / 1e9;

        if (packetType == 1 && preamble != WIFI_PREAMBLE_NONE)
          {
            //First packet in an A-MPDU
            numSymbols = ceil (m_Stbc * (16 + size * 8.0 + 6 * Nes) / (m_Stbc * numDataBitsPerSymbol));
            if (incFlag == 1)
              {
                m_totalAmpduSize += size;
                m_totalAmpduNumSymbols += numSymbols;
              }
          }
        else if (packetType == 1 && preamble == WIFI_PREAMBLE_NONE)
          {
            //consecutive packets in an A-MPDU
            numSymbols = m_Stbc * ((size * 8.0 ) / (m_Stbc * numDataBitsPerSymbol));
            if (incFlag == 1)
              {
                m_totalAmpduSize += size;
                m_totalAmpduNumSymbols += numSymbols;
              }
          }
        else if (packetType == 2 && preamble == WIFI_PREAMBLE_NONE)
          {
            //last packet in an A-MPDU
            uint32_t totalAmpduSize = m_totalAmpduSize + size;
            numSymbols = lrint (m_Stbc * ceil ((16 + totalAmpduSize * 8.0 + 6 * Nes) / (m_Stbc * numDataBitsPerSymbol)));
            NS_ASSERT (m_totalAmpduNumSymbols <= numSymbols);
            numSymbols -= m_totalAmpduNumSymbols;
            if (incFlag == 1)
              {
                m_totalAmpduSize = 0;
                m_totalAmpduNumSymbols = 0;
              }
          }
        else if (packetType == 0 && preamble != WIFI_PREAMBLE_NONE)
          {
            //Not an A-MPDU
            numSymbols = lrint (m_Stbc * ceil ((16 + size * 8.0 + 6.0 * Nes) / (m_Stbc * numDataBitsPerSymbol)));
          }
        else
          {
            NS_FATAL_ERROR ("Wrong combination of preamble and packet type");
          }

        if (frequency >= 2400 && frequency <= 2500 && ((packetType == 0 && preamble != WIFI_PREAMBLE_NONE) || (packetType == 2 && preamble == WIFI_PREAMBLE_NONE))) //at 2.4 GHz
          {
            return Time (numSymbols * symbolDuration) + MicroSeconds (6);
          }
        else //at 5 GHz
          {
            return Time (numSymbols * symbolDuration);
          }
      }
    case WIFI_MOD_CLASS_DSSS:
      //(Section 17.2.3.6 "Long PLCP LENGTH field"; IEEE Std 802.11-2012)
      NS_LOG_LOGIC (" size=" << size
                             << " mode=" << payloadMode
                             << " rate=" << payloadMode.GetDataRate ());
      return MicroSeconds (lrint (ceil ((size * 8.0) / (payloadMode.GetDataRate () / 1.0e6))));
    default:
      NS_FATAL_ERROR ("unsupported modulation class");
      return MicroSeconds (0);
    }
}

Time
WifiPhy::CalculatePlcpPreambleAndHeaderDuration (WifiTxVector txvector, WifiPreamble preamble)
{
  WifiMode payloadMode = txvector.GetMode ();
  Time duration = GetPlcpPreambleDuration (payloadMode, preamble)
    + GetPlcpHeaderDuration (payloadMode, preamble)
    + GetPlcpHtSigHeaderDuration (preamble)
    + GetPlcpHtTrainingSymbolDuration (preamble, txvector);
  return duration;
}

Time
WifiPhy::CalculateTxDuration (uint32_t size, WifiTxVector txvector, WifiPreamble preamble, double frequency, uint8_t packetType, uint8_t incFlag)
{
  Time duration = CalculatePlcpPreambleAndHeaderDuration (txvector, preamble)
    + GetPayloadDuration (size, txvector, preamble, frequency, packetType, incFlag);
  return duration;
}

void
WifiPhy::NotifyTxBegin (Ptr<const Packet> packet)
{
  m_phyTxBeginTrace (packet);
}

void
WifiPhy::NotifyTxEnd (Ptr<const Packet> packet)
{
  m_phyTxEndTrace (packet);
}

void
WifiPhy::NotifyTxDrop (Ptr<const Packet> packet)
{
  m_phyTxDropTrace (packet);
}

void
WifiPhy::NotifyRxBegin (Ptr<const Packet> packet)
{
  m_phyRxBeginTrace (packet);
}

void
WifiPhy::NotifyRxEnd (Ptr<const Packet> packet)
{
  m_phyRxEndTrace (packet);
}

void
WifiPhy::NotifyRxDrop (Ptr<const Packet> packet)
{
  m_phyRxDropTrace (packet);
}

void
WifiPhy::NotifyMonitorSniffRx (Ptr<const Packet> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, WifiPreamble preamble, WifiTxVector txvector, struct mpduInfo aMpdu, struct snrDb snr)
{
  m_phyMonitorSniffRxTrace (packet, channelFreqMhz, channelNumber, rate, preamble, txvector, aMpdu, snr);
}

void
WifiPhy::NotifyMonitorSniffTx (Ptr<const Packet> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, WifiPreamble preamble, WifiTxVector txvector, struct mpduInfo aMpdu)
{
  m_phyMonitorSniffTxTrace (packet, channelFreqMhz, channelNumber, rate, preamble, txvector, aMpdu);
}


// Clause 15 rates (DSSS)

WifiMode
WifiPhy::GetDsssRate1Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("DsssRate1Mbps",
                                     WIFI_MOD_CLASS_DSSS,
                                     true,
                                     22000000, 1000000,
                                     WIFI_CODE_RATE_UNDEFINED,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetDsssRate2Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("DsssRate2Mbps",
                                     WIFI_MOD_CLASS_DSSS,
                                     true,
                                     22000000, 2000000,
                                     WIFI_CODE_RATE_UNDEFINED,
                                     4);
  return mode;
}


// Clause 18 rates (HR/DSSS)

WifiMode
WifiPhy::GetDsssRate5_5Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("DsssRate5_5Mbps",
                                     WIFI_MOD_CLASS_DSSS,
                                     true,
                                     22000000, 5500000,
                                     WIFI_CODE_RATE_UNDEFINED,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetDsssRate11Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("DsssRate11Mbps",
                                     WIFI_MOD_CLASS_DSSS,
                                     true,
                                     22000000, 11000000,
                                     WIFI_CODE_RATE_UNDEFINED,
                                     4);
  return mode;
}


// Clause 19.5 rates (ERP-OFDM)

WifiMode
WifiPhy::GetErpOfdmRate6Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("ErpOfdmRate6Mbps",
                                     WIFI_MOD_CLASS_ERP_OFDM,
                                     true,
                                     20000000, 6000000,
                                     WIFI_CODE_RATE_1_2,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetErpOfdmRate9Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("ErpOfdmRate9Mbps",
                                     WIFI_MOD_CLASS_ERP_OFDM,
                                     false,
                                     20000000, 9000000,
                                     WIFI_CODE_RATE_3_4,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetErpOfdmRate12Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("ErpOfdmRate12Mbps",
                                     WIFI_MOD_CLASS_ERP_OFDM,
                                     true,
                                     20000000, 12000000,
                                     WIFI_CODE_RATE_1_2,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetErpOfdmRate18Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("ErpOfdmRate18Mbps",
                                     WIFI_MOD_CLASS_ERP_OFDM,
                                     false,
                                     20000000, 18000000,
                                     WIFI_CODE_RATE_3_4,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetErpOfdmRate24Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("ErpOfdmRate24Mbps",
                                     WIFI_MOD_CLASS_ERP_OFDM,
                                     true,
                                     20000000, 24000000,
                                     WIFI_CODE_RATE_1_2,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetErpOfdmRate36Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("ErpOfdmRate36Mbps",
                                     WIFI_MOD_CLASS_ERP_OFDM,
                                     false,
                                     20000000, 36000000,
                                     WIFI_CODE_RATE_3_4,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetErpOfdmRate48Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("ErpOfdmRate48Mbps",
                                     WIFI_MOD_CLASS_ERP_OFDM,
                                     false,
                                     20000000, 48000000,
                                     WIFI_CODE_RATE_2_3,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetErpOfdmRate54Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("ErpOfdmRate54Mbps",
                                     WIFI_MOD_CLASS_ERP_OFDM,
                                     false,
                                     20000000, 54000000,
                                     WIFI_CODE_RATE_3_4,
                                     64);
  return mode;
}


// Clause 17 rates (OFDM)

WifiMode
WifiPhy::GetOfdmRate6Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate6Mbps",
                                     WIFI_MOD_CLASS_OFDM,
                                     true,
                                     20000000, 6000000,
                                     WIFI_CODE_RATE_1_2,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate9Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate9Mbps",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     20000000, 9000000,
                                     WIFI_CODE_RATE_3_4,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate12Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate12Mbps",
                                     WIFI_MOD_CLASS_OFDM,
                                     true,
                                     20000000, 12000000,
                                     WIFI_CODE_RATE_1_2,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate18Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate18Mbps",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     20000000, 18000000,
                                     WIFI_CODE_RATE_3_4,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate24Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate24Mbps",
                                     WIFI_MOD_CLASS_OFDM,
                                     true,
                                     20000000, 24000000,
                                     WIFI_CODE_RATE_1_2,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate36Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate36Mbps",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     20000000, 36000000,
                                     WIFI_CODE_RATE_3_4,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate48Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate48Mbps",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     20000000, 48000000,
                                     WIFI_CODE_RATE_2_3,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate54Mbps ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate54Mbps",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     20000000, 54000000,
                                     WIFI_CODE_RATE_3_4,
                                     64);
  return mode;
}


// 10 MHz channel rates

WifiMode
WifiPhy::GetOfdmRate3MbpsBW10MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate3MbpsBW10MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     true,
                                     10000000, 3000000,
                                     WIFI_CODE_RATE_1_2,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate4_5MbpsBW10MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate4_5MbpsBW10MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     10000000, 4500000,
                                     WIFI_CODE_RATE_3_4,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate6MbpsBW10MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate6MbpsBW10MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     true,
                                     10000000, 6000000,
                                     WIFI_CODE_RATE_1_2,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate9MbpsBW10MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate9MbpsBW10MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     10000000, 9000000,
                                     WIFI_CODE_RATE_3_4,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate12MbpsBW10MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate12MbpsBW10MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     true,
                                     10000000, 12000000,
                                     WIFI_CODE_RATE_1_2,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate18MbpsBW10MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate18MbpsBW10MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     10000000, 18000000,
                                     WIFI_CODE_RATE_3_4,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate24MbpsBW10MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate24MbpsBW10MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     10000000, 24000000,
                                     WIFI_CODE_RATE_2_3,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate27MbpsBW10MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate27MbpsBW10MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     10000000, 27000000,
                                     WIFI_CODE_RATE_3_4,
                                     64);
  return mode;
}


// 5 MHz channel rates

WifiMode
WifiPhy::GetOfdmRate1_5MbpsBW5MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate1_5MbpsBW5MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     true,
                                     5000000, 1500000,
                                     WIFI_CODE_RATE_1_2,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate2_25MbpsBW5MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate2_25MbpsBW5MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     5000000, 2250000,
                                     WIFI_CODE_RATE_3_4,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate3MbpsBW5MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate3MbpsBW5MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     true,
                                     5000000, 3000000,
                                     WIFI_CODE_RATE_1_2,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate4_5MbpsBW5MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate4_5MbpsBW5MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     5000000, 4500000,
                                     WIFI_CODE_RATE_3_4,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate6MbpsBW5MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate6MbpsBW5MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     true,
                                     5000000, 6000000,
                                     WIFI_CODE_RATE_1_2,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate9MbpsBW5MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate9MbpsBW5MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     5000000, 9000000,
                                     WIFI_CODE_RATE_3_4,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate12MbpsBW5MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate12MbpsBW5MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     5000000, 12000000,
                                     WIFI_CODE_RATE_2_3,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate13_5MbpsBW5MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate13_5MbpsBW5MHz",
                                     WIFI_MOD_CLASS_OFDM,
                                     false,
                                     5000000, 13500000,
                                     WIFI_CODE_RATE_3_4,
                                     64);
  return mode;
}


// Clause 20

WifiMode
WifiPhy::GetOfdmRate6_5MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate6_5MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     true,
                                     20000000, 6500000,
                                     WIFI_CODE_RATE_1_2,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate7_2MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate7_2MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     20000000, 7200000,
                                     WIFI_CODE_RATE_1_2,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate13MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate13MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     true,
                                     20000000, 13000000,
                                     WIFI_CODE_RATE_1_2,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate14_4MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate14_4MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     20000000, 14400000,
                                     WIFI_CODE_RATE_1_2,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate19_5MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate19_5MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     true,
                                     20000000, 19500000,
                                     WIFI_CODE_RATE_3_4,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate21_7MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate21_7MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     20000000, 21700000,
                                     WIFI_CODE_RATE_3_4,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate26MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate26MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     true,
                                     20000000, 26000000,
                                     WIFI_CODE_RATE_1_2,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate28_9MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate28_9MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     20000000, 28900000,
                                     WIFI_CODE_RATE_1_2,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate39MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate39MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     true,
                                     20000000, 39000000,
                                     WIFI_CODE_RATE_3_4,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate43_3MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate43_3MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     20000000, 43300000,
                                     WIFI_CODE_RATE_3_4,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate52MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate52MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     true,
                                     20000000, 52000000,
                                     WIFI_CODE_RATE_2_3,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate57_8MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate57_8MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     20000000, 57800000,
                                     WIFI_CODE_RATE_2_3,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate58_5MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate58_5MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     true,
                                     20000000, 58500000,
                                     WIFI_CODE_RATE_3_4,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate65MbpsBW20MHzShGi ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate65MbpsBW20MHzShGi",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     20000000, 65000000,
                                     WIFI_CODE_RATE_3_4,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate65MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate65MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     true,
                                     20000000, 65000000,
                                     WIFI_CODE_RATE_5_6,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate72_2MbpsBW20MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate72_2MbpsBW20MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     20000000, 72200000,
                                     WIFI_CODE_RATE_5_6,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate13_5MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate13_5MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 13500000,
                                     WIFI_CODE_RATE_1_2,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate15MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate15MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 15000000,
                                     WIFI_CODE_RATE_1_2,
                                     2);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate27MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate27MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 27000000,
                                     WIFI_CODE_RATE_1_2,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate30MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate30MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 30000000,
                                     WIFI_CODE_RATE_1_2,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate40_5MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate40_5MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 40500000,
                                     WIFI_CODE_RATE_3_4,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate45MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate45MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 45000000,
                                     WIFI_CODE_RATE_3_4,
                                     4);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate54MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate54MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 54000000,
                                     WIFI_CODE_RATE_1_2,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate60MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate60MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 60000000,
                                     WIFI_CODE_RATE_1_2,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate81MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate81MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 81000000,
                                     WIFI_CODE_RATE_3_4,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate90MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate90MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 90000000,
                                     WIFI_CODE_RATE_3_4,
                                     16);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate108MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate108MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 108000000,
                                     WIFI_CODE_RATE_2_3,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate120MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate120MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 120000000,
                                     WIFI_CODE_RATE_2_3,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate121_5MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate121_5MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 121500000,
                                     WIFI_CODE_RATE_3_4,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate135MbpsBW40MHzShGi ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate135MbpsBW40MHzShGi",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 135000000,
                                     WIFI_CODE_RATE_3_4,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate135MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate135MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 135000000,
                                     WIFI_CODE_RATE_5_6,
                                     64);
  return mode;
}

WifiMode
WifiPhy::GetOfdmRate150MbpsBW40MHz ()
{
  static WifiMode mode =
    WifiModeFactory::CreateWifiMode ("OfdmRate150MbpsBW40MHz",
                                     WIFI_MOD_CLASS_HT,
                                     false,
                                     40000000, 150000000,
                                     WIFI_CODE_RATE_5_6,
                                     64);
  return mode;
}


std::ostream& operator<< (std::ostream& os, enum WifiPhy::State state)
{
  switch (state)
    {
    case WifiPhy::IDLE:
      return (os << "IDLE");
    case WifiPhy::CCA_BUSY:
      return (os << "CCA_BUSY");
    case WifiPhy::TX:
      return (os << "TX");
    case WifiPhy::RX:
      return (os << "RX");
    case WifiPhy::SWITCHING:
      return (os << "SWITCHING");
    case WifiPhy::SLEEP:
      return (os << "SLEEP");
    default:
      NS_FATAL_ERROR ("Invalid WifiPhy state");
      return (os << "INVALID");
    }
}

} //namespace ns3

namespace {

static class Constructor
{
public:
  Constructor ()
  {
    ns3::WifiPhy::GetDsssRate1Mbps ();
    ns3::WifiPhy::GetDsssRate2Mbps ();
    ns3::WifiPhy::GetDsssRate5_5Mbps ();
    ns3::WifiPhy::GetDsssRate11Mbps ();
    ns3::WifiPhy::GetErpOfdmRate6Mbps ();
    ns3::WifiPhy::GetErpOfdmRate9Mbps ();
    ns3::WifiPhy::GetErpOfdmRate12Mbps ();
    ns3::WifiPhy::GetErpOfdmRate18Mbps ();
    ns3::WifiPhy::GetErpOfdmRate24Mbps ();
    ns3::WifiPhy::GetErpOfdmRate36Mbps ();
    ns3::WifiPhy::GetErpOfdmRate48Mbps ();
    ns3::WifiPhy::GetErpOfdmRate54Mbps ();
    ns3::WifiPhy::GetOfdmRate6Mbps ();
    ns3::WifiPhy::GetOfdmRate9Mbps ();
    ns3::WifiPhy::GetOfdmRate12Mbps ();
    ns3::WifiPhy::GetOfdmRate18Mbps ();
    ns3::WifiPhy::GetOfdmRate24Mbps ();
    ns3::WifiPhy::GetOfdmRate36Mbps ();
    ns3::WifiPhy::GetOfdmRate48Mbps ();
    ns3::WifiPhy::GetOfdmRate54Mbps ();
    ns3::WifiPhy::GetOfdmRate3MbpsBW10MHz ();
    ns3::WifiPhy::GetOfdmRate4_5MbpsBW10MHz ();
    ns3::WifiPhy::GetOfdmRate6MbpsBW10MHz ();
    ns3::WifiPhy::GetOfdmRate9MbpsBW10MHz ();
    ns3::WifiPhy::GetOfdmRate12MbpsBW10MHz ();
    ns3::WifiPhy::GetOfdmRate18MbpsBW10MHz ();
    ns3::WifiPhy::GetOfdmRate24MbpsBW10MHz ();
    ns3::WifiPhy::GetOfdmRate27MbpsBW10MHz ();
    ns3::WifiPhy::GetOfdmRate1_5MbpsBW5MHz ();
    ns3::WifiPhy::GetOfdmRate2_25MbpsBW5MHz ();
    ns3::WifiPhy::GetOfdmRate3MbpsBW5MHz ();
    ns3::WifiPhy::GetOfdmRate4_5MbpsBW5MHz ();
    ns3::WifiPhy::GetOfdmRate6MbpsBW5MHz ();
    ns3::WifiPhy::GetOfdmRate9MbpsBW5MHz ();
    ns3::WifiPhy::GetOfdmRate12MbpsBW5MHz ();
    ns3::WifiPhy::GetOfdmRate13_5MbpsBW5MHz ();
    ns3::WifiPhy::GetOfdmRate6_5MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate13MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate19_5MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate26MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate39MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate52MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate58_5MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate65MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate13_5MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate27MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate40_5MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate54MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate81MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate108MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate121_5MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate135MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate7_2MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate14_4MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate21_7MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate28_9MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate43_3MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate57_8MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate65MbpsBW20MHzShGi ();
    ns3::WifiPhy::GetOfdmRate72_2MbpsBW20MHz ();
    ns3::WifiPhy::GetOfdmRate15MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate30MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate45MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate60MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate90MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate120MbpsBW40MHz ();
    ns3::WifiPhy::GetOfdmRate135MbpsBW40MHzShGi ();
    ns3::WifiPhy::GetOfdmRate150MbpsBW40MHz ();
  }
} g_constructor;

}
