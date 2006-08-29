/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2006 INRIA
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

#ifndef PACKET_LOGGER_H
#define PACKET_LOGGER_H

#include "ns3/callback.h"
#include "packet.h"

namespace yans {

/**
 * \brief log packets
 */
class PacketLogger {
public:
	typedef Callback<void,Packet const> PacketLoggerCallback;
	PacketLogger ();
	/**
	 * \param packet to log
	 * If a non-null callback was set, the packet
	 * is forwarded to that callback.
	 */
	void log (Packet const packet);
	/**
	 * \param callback callback to store
	 */
	void set_callback (PacketLoggerCallback callback);
private:
	PacketLoggerCallback m_callback;
};

}; // namespace yans

#endif /* PACKET_LOGGER_H */
