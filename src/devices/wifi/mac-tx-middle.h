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

#ifndef MAC_TX_MIDDLE_H
#define MAC_TX_MIDDLE_H

#include <stdint.h>

namespace ns3 {

class WifiMacHeader;

class MacTxMiddle {
public:
  MacTxMiddle ();

  uint16_t GetNextSequenceNumberfor (const WifiMacHeader *hdr);

private:
  uint16_t m_qosSequences[16];
  uint16_t m_sequence;
};

} // namespace ns3

#endif /* MAC_TX_MIDDLE_H */
