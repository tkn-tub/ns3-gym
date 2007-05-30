/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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

#include "packet-printer.h"

namespace ns3 {

PacketPrinter::PacketPrinter ()
  : m_forward (true)
{}

void 
PacketPrinter::PrintForward (void)
{}
void 
PacketPrinter::PrintBackward (void)
{}
void 
PacketPrinter::AddPayloadPrinter (Callback<void,
				  std::ostream &, 
				  uint32_t, 
				  uint32_t, 
				  struct PacketPrinter::FragmentInformation> printer)
{
  m_payloadPrinter = printer;
}
  /**
   * \param printer Arguments: output stream, packet uid, size, header/trailer name, fragment information
   */
  void AddDefaultPrinter (Callback<void,
			           std::ostream &, 
			           uint32_t, 
			           uint32_t, 
			           std::string &,
			           struct PacketPrinter::FragmentInformation> printer);

} // namespace ns3
