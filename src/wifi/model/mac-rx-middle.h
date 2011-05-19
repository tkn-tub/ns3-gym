/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef MAC_RX_MIDDLE_H
#define MAC_RX_MIDDLE_H

#include <map>
#include <utility>
#include "ns3/callback.h"
#include "ns3/mac48-address.h"
#include "ns3/packet.h"

namespace ns3 {

class WifiMacHeader;
class OriginatorRxStatus;

/**
 * \ingroup wifi
 *
 * This class handles duplicate detection and recomposition of fragments.
 */
class MacRxMiddle
{
public:
  typedef Callback<void, Ptr<Packet>, const WifiMacHeader*> ForwardUpCallback;

  MacRxMiddle ();
  ~MacRxMiddle ();

  void SetForwardCallback (ForwardUpCallback callback);

  void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr);
private:
  friend class MacRxMiddleTest;
  OriginatorRxStatus* Lookup (const WifiMacHeader* hdr);
  bool IsDuplicate (const WifiMacHeader* hdr, OriginatorRxStatus *originator) const;
  Ptr<Packet> HandleFragments (Ptr<Packet> packet, const WifiMacHeader* hdr,
                               OriginatorRxStatus *originator);

  typedef std::map <Mac48Address, OriginatorRxStatus *, std::less<Mac48Address> > Originators;
  typedef std::map <std::pair<Mac48Address, uint8_t>, OriginatorRxStatus *, std::less<std::pair<Mac48Address,uint8_t> > > QosOriginators;
  typedef std::map <Mac48Address, OriginatorRxStatus *, std::less<Mac48Address> >::iterator OriginatorsI;
  typedef std::map <std::pair<Mac48Address, uint8_t>, OriginatorRxStatus *, std::less<std::pair<Mac48Address,uint8_t> > >::iterator QosOriginatorsI;
  Originators m_originatorStatus;
  QosOriginators m_qosOriginatorStatus;
  ForwardUpCallback m_callback;
};

} // namespace ns3


#endif /* MAC_RX_MIDDLE_H */
