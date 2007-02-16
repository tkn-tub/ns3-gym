/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */

#include "serial-channel.h"
#include "net-device-serial.h"
#include "ns3/packet.h"

namespace ns3 {

void
SerialChannel::Attach(SerialNetDevice* nd)
{
    nd->Attach(this);
    m_devices.push_back (nd);
}

bool
SerialChannel::Send(Packet& p, SerialNetDevice* caller)
{
    for (NetDevicesCI i = m_devices.begin (); i != m_devices.end (); i++) {
        if (caller != (*i)) {
            (*i)->Receive (p);
            return true;
        }
    }
    return false;
}

} // namespace ns3
