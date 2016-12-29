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

#include <cmath>

namespace ns3 {

/**
 * Convert from dBm to Watts.
 *
 * \param dbm the power in dBm
 *
 * \return the equivalent Watts for the given dBm
 */
constexpr double DbmToW (double dbm)
{
  return std::pow (10.0, dbm / 10.0) / 1000.0;
}

/**
 * Convert from dB to ratio.
 *
 * \param db
 *
 * \return ratio
 */
constexpr double DbToRatio (double db)
{
  return std::pow (10.0, db / 10.0);
}

/**
 * Convert from Watts to dBm.
 *
 * \param w the power in Watts
 *
 * \return the equivalent dBm for the given Watts
 */
constexpr double WToDbm (double w)
{
  return 10.0 * std::log10 (w * 1000.0);
}

/**
 * Convert from ratio to dB.
 *
 * \param ratio
 *
 * \return dB
 */
constexpr double RatioToDb (double ratio)
{
  return 10.0 * std::log10 (ratio);
}

} // namespace ns3

#endif /* WIFI_UTILS_H */
