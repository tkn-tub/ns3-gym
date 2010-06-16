/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/random-variable.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"
#include "ns3/enum.h"
#include "ns3/trace-source-accessor.h"
#include <math.h>

NS_LOG_COMPONENT_DEFINE ("WifiPhy");

namespace ns3 {

/****************************************************************
 *       This destructor is needed.
 ****************************************************************/

WifiPhyListener::~WifiPhyListener ()
{}

/****************************************************************
 *       The actual WifiPhy class
 ****************************************************************/

NS_OBJECT_ENSURE_REGISTERED (WifiPhy);

TypeId 
WifiPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::WifiPhy")
    .SetParent<Object> ()
    .AddTraceSource ("PhyTxBegin", 
                     "Trace source indicating a packet has begun transmitting over the channel medium",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyTxBeginTrace))
    .AddTraceSource ("PhyTxEnd", 
                     "Trace source indicating a packet has been completely transmitted over the channel. NOTE: the only official WifiPhy implementation available to this date (YansWifiPhy) never fires this trace source.",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyTxEndTrace))
    .AddTraceSource ("PhyTxDrop", 
                     "Trace source indicating a packet has been dropped by the device during transmission",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyTxDropTrace))
    .AddTraceSource ("PhyRxBegin", 
                     "Trace source indicating a packet has begun being received from the channel medium by the device",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyRxBeginTrace))
    .AddTraceSource ("PhyRxEnd", 
                     "Trace source indicating a packet has been completely received from the channel medium by the device",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyRxEndTrace))
    .AddTraceSource ("PhyRxDrop", 
                     "Trace source indicating a packet has been dropped by the device during reception",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyRxDropTrace))
    .AddTraceSource ("PromiscSnifferRx", 
                     "Trace source simulating a wifi device in monitor mode sniffing all received frames",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyPromiscSniffRxTrace))
    .AddTraceSource ("PromiscSnifferTx", 
                     "Trace source simulating the capability of a wifi device in monitor mode to sniff all frames being transmitted",
                     MakeTraceSourceAccessor (&WifiPhy::m_phyPromiscSniffTxTrace))
    ;
  return tid;
}

WifiPhy::WifiPhy ()
{
  NS_LOG_FUNCTION (this);
}

WifiPhy::~WifiPhy ()
{
  NS_LOG_FUNCTION (this);
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
WifiPhy::NotifyPromiscSniffRx (Ptr<const Packet> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, bool isShortPreamble, double signalDbm, double noiseDbm)
{
  m_phyPromiscSniffRxTrace (packet, channelFreqMhz, channelNumber, rate, isShortPreamble, signalDbm, noiseDbm);
}

void 
WifiPhy::NotifyPromiscSniffTx (Ptr<const Packet> packet, uint16_t channelFreqMhz, uint16_t channelNumber, uint32_t rate, bool isShortPreamble)
{
  m_phyPromiscSniffTxTrace (packet, channelFreqMhz, channelNumber, rate, isShortPreamble);
}


/**
 * Clause 15 rates (DSSS)
 */

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


/**
 * Clause 18 rates (HR/DSSS)
 */
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


/**
 * Clause 17 rates (OFDM)
 */
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

/* 10 MHz channel rates */
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

/* 5 MHz channel rates */
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

std::ostream& operator<< (std::ostream& os, enum WifiPhy::State state)
{
  switch (state) {
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
  default:
    NS_FATAL_ERROR ("Invalid WifiPhy state");
    return (os << "INVALID");
  }
}

} // namespace ns3

namespace {

static class Constructor
{
public:
  Constructor () {
    ns3::WifiPhy::GetDsssRate1Mbps ();
    ns3::WifiPhy::GetDsssRate2Mbps ();
    ns3::WifiPhy::GetDsssRate5_5Mbps ();
    ns3::WifiPhy::GetDsssRate11Mbps ();
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
  }
} g_constructor;
}
