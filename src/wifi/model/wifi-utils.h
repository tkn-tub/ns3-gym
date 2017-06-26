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

#include "wifi-mode.h"
#include "ns3/nstime.h"
#include "ns3/uinteger.h"

namespace ns3 {

 /**
  * Return the logarithm of the given value to base 2.
  *
  * \param val
  *
  * \return the logarithm of val to base 2.
  */
  double Log2 (double val);
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
 * Convert the guard interval to nanoseconds based on the wifimode.
 *
 * \param mode the wifimode
 * \param htShortGuardInterval whether HT/VHT short guard interval is enabled
 * \param heGuardInterval the HE guard interval duration
 *
 * \return the guard interval duration in nanoseconds
 */
uint16_t ConvertGuardIntervalToNanoSeconds (WifiMode mode, bool htShortGuardInterval, Time heGuardInterval);

} // namespace ns3

#endif /* WIFI_UTILS_H */
