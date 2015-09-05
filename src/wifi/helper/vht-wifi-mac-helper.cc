/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 SEBASTIEN DERONNE
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

#include "vht-wifi-mac-helper.h"
#include "ns3/wifi-mac.h"
#include "ns3/edca-txop-n.h"
#include "ns3/pointer.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"

namespace ns3 {

VhtWifiMacHelper::VhtWifiMacHelper ()
{
}

VhtWifiMacHelper::~VhtWifiMacHelper ()
{
}

VhtWifiMacHelper
VhtWifiMacHelper::Default (void)
{
  VhtWifiMacHelper helper;

  helper.SetType ("ns3::StaWifiMac",
                  "QosSupported", BooleanValue (true),
                  "HtSupported", BooleanValue (true), //by default, it also supports HT features
                  "VhtSupported", BooleanValue (true));

  //MPDU aggregation is always supported
  helper.SetMpduAggregatorForAc (AC_VO, "ns3::MpduStandardAggregator");
  helper.SetMpduAggregatorForAc (AC_VI, "ns3::MpduStandardAggregator");
  helper.SetMpduAggregatorForAc (AC_BE, "ns3::MpduStandardAggregator");
  helper.SetMpduAggregatorForAc (AC_BK, "ns3::MpduStandardAggregator");

  return helper;
}

StringValue
VhtWifiMacHelper::DataRateForMcs (int mcs)
{
  std::stringstream sstmp;
  std::string strtmp, dataRate;

  sstmp << mcs;
  sstmp >> strtmp;
  dataRate = "VhtMcs" + strtmp;
  
  return StringValue (dataRate);
}

} //namespace ns3
