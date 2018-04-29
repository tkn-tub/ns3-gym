/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 CTTC
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
 * Authors: Nicola Baldo <nbaldo@cttc.es>
 *          Ghada Badawy <gbadawy@gmail.com>
 */

#include "wifi-tx-vector.h"

namespace ns3 {

WifiTxVector::WifiTxVector ()
  : m_preamble (WIFI_PREAMBLE_NONE),
    m_channelWidth (20),
    m_guardInterval (800),
    m_nTx (1),
    m_nss (1),
    m_ness (0),
    m_aggregation (false),
    m_stbc (false),
    m_modeInitialized (false),
    m_txPowerLevelInitialized (false)
{
}

WifiTxVector::WifiTxVector (WifiMode mode,
                            uint8_t powerLevel,
                            WifiPreamble preamble,
                            uint16_t guardInterval,
                            uint8_t nTx,
                            uint8_t nss,
                            uint8_t ness,
                            uint16_t channelWidth,
                            bool aggregation,
                            bool stbc)
  : m_mode (mode),
    m_txPowerLevel (powerLevel),
    m_preamble (preamble),
    m_channelWidth (channelWidth),
    m_guardInterval (guardInterval),
    m_nTx (nTx),
    m_nss (nss),
    m_ness (ness),
    m_aggregation (aggregation),
    m_stbc (stbc),
    m_modeInitialized (true),
    m_txPowerLevelInitialized (true)
{
}

WifiMode
WifiTxVector::GetMode (void) const
{
  if (!m_modeInitialized)
    {
      NS_FATAL_ERROR ("WifiTxVector mode must be set before using");
    }
  return m_mode;
}

uint8_t
WifiTxVector::GetTxPowerLevel (void) const
{
  if (!m_txPowerLevelInitialized)
    {
      NS_FATAL_ERROR ("WifiTxVector txPowerLevel must be set before using");
    }
  return m_txPowerLevel;
}

WifiPreamble
WifiTxVector::GetPreambleType (void) const
{
  return m_preamble;
}

uint16_t
WifiTxVector::GetChannelWidth (void) const
{
  return m_channelWidth;
}

uint16_t
WifiTxVector::GetGuardInterval (void) const
{
  return m_guardInterval;
}

uint8_t
WifiTxVector::GetNTx (void) const
{
  return m_nTx;
}

uint8_t
WifiTxVector::GetNss (void) const
{
  return m_nss;
}

uint8_t
WifiTxVector::GetNess (void) const
{
  return m_ness;
}

bool
WifiTxVector::IsAggregation (void) const
{
  return m_aggregation;
}

bool
WifiTxVector::IsStbc (void) const
{
  return m_stbc;
}

void
WifiTxVector::SetMode (WifiMode mode)
{
  m_mode = mode;
  m_modeInitialized = true;
}

void
WifiTxVector::SetTxPowerLevel (uint8_t powerlevel)
{
  m_txPowerLevel = powerlevel;
  m_txPowerLevelInitialized = true;
}

void
WifiTxVector::SetPreambleType (WifiPreamble preamble)
{
  m_preamble = preamble;
}

void
WifiTxVector::SetChannelWidth (uint16_t channelWidth)
{
  m_channelWidth = channelWidth;
}

void
WifiTxVector::SetGuardInterval (uint16_t guardInterval)
{
  m_guardInterval = guardInterval;
}

void
WifiTxVector::SetNTx (uint8_t nTx)
{
  m_nTx = nTx;
}

void
WifiTxVector::SetNss (uint8_t nss)
{
  m_nss = nss;
}

void
WifiTxVector::SetNess (uint8_t ness)
{
  m_ness = ness;
}

void
WifiTxVector::SetAggregation (bool aggregation)
{
  m_aggregation = aggregation;
}

void
WifiTxVector::SetStbc (bool stbc)
{
  m_stbc = stbc;
}

bool
WifiTxVector::IsValid (void) const
{
  std::string modeName = m_mode.GetUniqueName ();
  if (m_channelWidth == 20)
    {
      if (m_nss != 3 && m_nss != 6)
        {
          return (modeName != "VhtMcs9");
        }
    }
  else if (m_channelWidth == 80)
    {
      if (m_nss == 3 || m_nss == 7)
        {
          return (modeName != "VhtMcs6");
        }
      else if (m_nss == 6)
        {
          return (modeName != "VhtMcs9");
        }
    }
  else if (m_channelWidth == 160)
    {
      if (m_nss == 3)
        {
          return (modeName != "VhtMcs9");
        }
    }
  return true;
}

std::ostream & operator << ( std::ostream &os, const WifiTxVector &v)
{
  os << "mode: " << v.GetMode () <<
    " txpwrlvl: " << +v.GetTxPowerLevel () <<
    " preamble: " << v.GetPreambleType () <<
    " channel width: " << v.GetChannelWidth () <<
    " GI: " << v.GetGuardInterval () <<
    " NTx: " << +v.GetNTx () <<
    " Nss: " << +v.GetNss () <<
    " Ness: " << +v.GetNess () <<
    " MPDU aggregation: " << v.IsAggregation () <<
    " STBC: " << v.IsStbc ();
  return os;
}

} //namespace ns3
