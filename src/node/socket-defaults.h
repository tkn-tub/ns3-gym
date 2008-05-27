/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
#ifndef SOCKET_DEFAULTS_H
#define SOCKET_DEFAULTS_H

#include "ns3/object.h"

namespace ns3 {

/**
 * \brief Object to hold socket option defaults
 *
 * This class can be aggregated to a Node and can be used to store
 * socket defaults for a Node.  
 * 
 */
class SocketDefaults : public Object
{
public:
  static TypeId GetTypeId (void);

private:
  uint32_t m_defaultSndBufLimit;
  uint32_t m_defaultRcvBufLimit;
};

} // namespace ns3

#endif /* SOCKET_DEFAULTS_H */
