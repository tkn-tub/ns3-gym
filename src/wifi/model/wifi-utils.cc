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
#include <cmath>

namespace ns3 {

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

} //namespace ns3
