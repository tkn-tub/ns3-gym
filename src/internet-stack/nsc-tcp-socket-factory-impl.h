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
