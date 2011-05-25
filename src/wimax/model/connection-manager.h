/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008,2009 INRIA, UDcast
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
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                               <amine.ismail@UDcast.com>
 */

#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <stdint.h>
#include "cid.h"
#include "wimax-connection.h"
#include "ns3/mac48-address.h"

namespace ns3 {

class CidFactory;
class SSRecord;
class RngRsp;
class WimaxNetDevice;
class SubscriberStationNetDevice;

/**
 * \ingroup wimax
 * The same connection manager class serves both for BS and SS though some functions are exclusive to only one of them.
 */

class ConnectionManager : public Object
{
public:
  ConnectionManager (void);
  ~ConnectionManager (void);
  void DoDispose (void);
  void SetCidFactory (CidFactory *cidFactory);
  /**
   * \brief allocates the management connection for an ss record. This method is only used by BS
   * \param ssRecord the ss record to wich the management connection will be allocated
   * \param rngrsp the ranging response message
   */
  void AllocateManagementConnections (SSRecord *ssRecord, RngRsp *rngrsp);
  /**
   * \brief create a connection of type type
   * \param type type of the connection to create
   */
  Ptr<WimaxConnection> CreateConnection (Cid::Type type);
  /**
   * \brief add a connection to the list of managed connections
   * \param connection the connection to add
   * \param type the type of connection to add
   */
  void AddConnection (Ptr<WimaxConnection> connection, Cid::Type type);
  /**
   * \return the connection which has as identifier cid
   */
  Ptr<WimaxConnection> GetConnection (Cid cid);
  /**
   * \return a listy of all connection which have as type type
   */
  std::vector<Ptr<WimaxConnection> > GetConnections (Cid::Type type) const;
  uint32_t GetNPackets (Cid::Type type, ServiceFlow::SchedulingType schedulingType) const;
  /**
   * \return true if one of the managed connection has at least one packet to send, false otherwise
   */
  bool HasPackets (void) const;
private:
  std::vector<Ptr<WimaxConnection> > m_basicConnections;
  std::vector<Ptr<WimaxConnection> > m_primaryConnections;
  std::vector<Ptr<WimaxConnection> > m_transportConnections;
  std::vector<Ptr<WimaxConnection> > m_multicastConnections;
  // only for BS
  CidFactory *m_cidFactory;
};

}
; // namespace ns3

#endif /* CONNECTION_MANAGER_H */

