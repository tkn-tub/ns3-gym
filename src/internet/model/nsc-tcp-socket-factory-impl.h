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
#ifndef NSC_TCP_SOCKET_FACTORY_IMPL_H
#define NSC_TCP_SOCKET_FACTORY_IMPL_H

#include "ns3/tcp-socket-factory.h"
#include "ns3/ptr.h"

namespace ns3 {

class NscTcpL4Protocol;

/**
 * \ingroup internet
 * \defgroup nsctcp NscTcp
 *
 * An alternate implementation of TCP for ns-3 is provided by the
 * Network Simulation Cradle (NSC) project. NSC is a separately linked
 * library that provides ported TCP stacks from popular operating systems
 * such as Linux and FreeBSD.  Glue code such as the ns-3 NSC code
 * allows users to delegate Internet stack processing to the logic
 * from these operating systems.  This allows a user to reproduce
 * with high fidelity the behavior of a real TCP stack.
 */

/**
 * \ingroup nsctcp
 *
 * \brief socket factory implementation for creating instances of NSC TCP
 */
class NscTcpSocketFactoryImpl : public TcpSocketFactory
{
public:
  NscTcpSocketFactoryImpl ();
  virtual ~NscTcpSocketFactoryImpl ();

  void SetTcp (Ptr<NscTcpL4Protocol> tcp);

  virtual Ptr<Socket> CreateSocket (void);

protected:
  virtual void DoDispose (void);
private:
  Ptr<NscTcpL4Protocol> m_tcp;
};

} // namespace ns3

#endif /* NSC_TCP_SOCKET_FACTORY_IMPL_H */
