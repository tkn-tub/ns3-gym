/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2009 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef ADHOC_WIFI_MAC_H
#define ADHOC_WIFI_MAC_H

#include "regular-wifi-mac.h"

#include "amsdu-subframe-header.h"

namespace ns3 {

/**
 * \ingroup wifi
 *
 *
 */
class AdhocWifiMac : public RegularWifiMac
{
public:
  static TypeId GetTypeId (void);

  AdhocWifiMac ();
  virtual ~AdhocWifiMac ();

  /**
   * \param address the current address of this MAC layer.
   */
  virtual void SetAddress (Mac48Address address);

  /**
   * \param linkUp the callback to invoke when the link becomes up.
   */
  virtual void SetLinkUpCallback (Callback<void> linkUp);

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to);

private:
  virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr);
};

} // namespace ns3

#endif /* ADHOC_WIFI_MAC_H */
