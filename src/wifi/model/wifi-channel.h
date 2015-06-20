/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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
 * Author: Mathieu Lacage, <mathieu.lacage@sophia.inria.fr>
 */

#ifndef WIFI_CHANNEL_H
#define WIFI_CHANNEL_H

#include "ns3/channel.h"

namespace ns3 {

class WifiNetDevice;
class WifiPhy;

/**
 * \brief Wifi Channel interface specification
 * \ingroup wifi
 *
 * This class works in tandem with the ns3::WifiPhy class. If you want to
 * provide a new Wifi PHY layer, you have to subclass both ns3::WifiChannel
 * and ns3::WifiPhy.
 *
 * Typically, MyWifiChannel will define a Send method whose job is to distribute
 * packets from a MyWifiPhy source to a set of MyWifiPhy destinations. MyWifiPhy
 * also typically defines a Receive method which is invoked by MyWifiPhy.
 */
class WifiChannel : public Channel
{
public:
  static TypeId GetTypeId (void);
};

} //namespace ns3


#endif /* WIFI_CHANNEL_H */
