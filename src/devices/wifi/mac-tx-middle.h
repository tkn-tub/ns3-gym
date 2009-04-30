/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005, 2009 INRIA
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

#ifndef MAC_TX_MIDDLE_H
#define MAC_TX_MIDDLE_H

#include <stdint.h>
#include <map>
#include "ns3/mac48-address.h"

namespace ns3 {

class WifiMacHeader;

class MacTxMiddle {
public:
  MacTxMiddle ();
  ~MacTxMiddle ();

  uint16_t GetNextSequenceNumberfor (const WifiMacHeader *hdr);

private:
  std::map <Mac48Address,uint16_t*> m_qosSequences;
  uint16_t m_sequence;
};

} // namespace ns3

#endif /* MAC_TX_MIDDLE_H */
