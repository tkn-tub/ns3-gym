/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#include "wifi-utils.h"
#include "wifi-mac-header.h"
#include <cmath>

namespace ns3 {

double
Log2 (double val)
{
  return std::log (val) / std::log (2.0);
}


double
DbToRatio (double dB)
{
  double ratio = std::pow (10.0, dB / 10.0);
  return ratio;
}

double
DbmToW (double dBm)
{
  double mW = std::pow (10.0, dBm / 10.0);
  return mW / 1000.0;
}

double
WToDbm (double w)
{
  return 10.0 * std::log10 (w * 1000.0);
}

double
RatioToDb (double ratio)
{
  return 10.0 * std::log10 (ratio);
}

bool
Is2_4Ghz (double frequency)
{
  if (frequency >= 2400 && frequency <= 2500)
    {
      return true;
    }
  return false;
}

bool
Is5Ghz (double frequency)
{
  if (frequency >= 5000 && frequency <= 6000)
    {
      return true;
    }
  return false;
}

uint16_t
ConvertGuardIntervalToNanoSeconds (WifiMode mode, bool htShortGuardInterval, Time heGuardInterval)
{
  uint16_t gi;
  if (mode.GetModulationClass () == WIFI_MOD_CLASS_HE)
    {
      gi = heGuardInterval.GetNanoSeconds ();
    }
  else if (mode.GetModulationClass () == WIFI_MOD_CLASS_HT || mode.GetModulationClass () == WIFI_MOD_CLASS_VHT)
    {
      gi = htShortGuardInterval ? 400 : 800;
    }
  else
    {
      gi = 800;
    }
  return gi;
}

uint32_t
GetAckSize (void)
{
  WifiMacHeader ack;
  ack.SetType (WIFI_MAC_CTL_ACK);
  return ack.GetSize () + 4;
}

uint32_t
GetBlockAckSize (BlockAckType type)
{
  WifiMacHeader hdr;
  hdr.SetType (WIFI_MAC_CTL_BACKRESP);
  CtrlBAckResponseHeader blockAck;
  if (type == BASIC_BLOCK_ACK)
    {
      blockAck.SetType (BASIC_BLOCK_ACK);
    }
  else if (type == COMPRESSED_BLOCK_ACK)
    {
      blockAck.SetType (COMPRESSED_BLOCK_ACK);
    }
  else if (type == MULTI_TID_BLOCK_ACK)
    {
      //Not implemented
      NS_ASSERT (false);
    }
  return hdr.GetSize () + blockAck.GetSerializedSize () + 4;
}

uint32_t
GetRtsSize (void)
{
  WifiMacHeader rts;
  rts.SetType (WIFI_MAC_CTL_RTS);
  return rts.GetSize () + 4;
}

uint32_t
GetCtsSize (void)
{
  WifiMacHeader cts;
  cts.SetType (WIFI_MAC_CTL_CTS);
  return cts.GetSize () + 4;
}

bool
IsInWindow (uint16_t seq, uint16_t winstart, uint16_t winsize)
{
  return ((seq - winstart + 4096) % 4096) < winsize;
}

void
AddWifiMacTrailer (Ptr<Packet> packet)
{
  WifiMacTrailer fcs;
  packet->AddTrailer (fcs);
}

uint32_t
GetSize (Ptr<const Packet> packet, const WifiMacHeader *hdr, bool isAmpdu)
{
  uint32_t size;
  WifiMacTrailer fcs;
  if (isAmpdu)
    {
      size = packet->GetSize ();
    }
  else
    {
      size = packet->GetSize () + hdr->GetSize () + fcs.GetSerializedSize ();
    }
  return size;
}

} //namespace ns3
