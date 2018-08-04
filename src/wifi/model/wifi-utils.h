/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include "block-ack-type.h"

namespace ns3 {

class WifiMacHeader;
class WifiMode;
class Packet;
class Time;

/**
 * Convert from dBm to Watts.
 *
 * \param dbm the power in dBm
 *
 * \return the equivalent Watts for the given dBm
 */
double DbmToW (double dbm);
/**
 * Convert from dB to ratio.
 *
 * \param db
 *
 * \return ratio
 */
double DbToRatio (double db);
/**
 * Convert from Watts to dBm.
 *
 * \param w the power in Watts
 *
 * \return the equivalent dBm for the given Watts
 */
double WToDbm (double w);
/**
 * Convert from ratio to dB.
 *
 * \param ratio
 *
 * \return dB
 */
double RatioToDb (double ratio);
/**
 * \param frequency the frequency to check
 * \return whether frequency is in the 2.4 GHz band
 */
bool Is2_4Ghz (double frequency);
/**
 * \param frequency the frequency to check
 * \return whether frequency is in the 5 GHz band
 */
bool Is5Ghz (double frequency);
/**
 * Convert the guard interval to nanoseconds based on the wifimode.
 *
 * \param mode the wifimode
 * \param htShortGuardInterval whether HT/VHT short guard interval is enabled
 * \param heGuardInterval the HE guard interval duration
 *
 * \return the guard interval duration in nanoseconds
 */
uint16_t ConvertGuardIntervalToNanoSeconds (WifiMode mode, bool htShortGuardInterval, Time heGuardInterval);
/**
 * Return the total ACK size (including FCS trailer).
 *
 * \return the total ACK size
 */
uint32_t GetAckSize (void);
/**
 * Return the total Block ACK size (including FCS trailer).
 *
 * \param type the Block ACK type
 * \return the total Block ACK size
 */
uint32_t GetBlockAckSize (BlockAckType type);
/**
 * Return the total RTS size (including FCS trailer).
 *
 * \return the total RTS size
 */
uint32_t GetRtsSize (void);
/**
 * Return the total CTS size (including FCS trailer).
 *
 * \return the total CTS size
 */
uint32_t GetCtsSize (void);
/**
 * \param seq MPDU sequence number
 * \param winstart sequence number window start
 * \param winsize the size of the sequence number window (currently default is 64)
 * \returns true if in the window
 *
 * This method checks if the MPDU's sequence number is inside the scoreboard boundaries or not
 */
bool IsInWindow (uint16_t seq, uint16_t winstart, uint16_t winsize);
/**
 * Add FCS trailer to a packet.
 *
 * \param packet
 */
void AddWifiMacTrailer (Ptr<Packet> packet);
/**
 * Return the total size of the packet after WifiMacHeader and FCS trailer
 * have been added.
 *
 * \param packet the packet to be encapsulated with WifiMacHeader and FCS trailer
 * \param hdr the WifiMacHeader
 * \param isAmpdu whether packet is part of an A-MPDU
 * \return the total packet size
 */
uint32_t GetSize (Ptr<const Packet> packet, const WifiMacHeader *hdr, bool isAmpdu);

} // namespace ns3

#endif /* WIFI_UTILS_H */
