#ifndef IPV6_RAW_SOCKET_FACTORY_IMPL_H
#define IPV6_RAW_SOCKET_FACTORY_IMPL_H

#include "ns3/ipv6-raw-socket-factory.h"

namespace ns3
{

/**
 * \class Ipv6RawSocketFactoryImpl
 * \brief Implementation of IPv6 raw socket factory.
 */
class Ipv6RawSocketFactoryImpl : public Ipv6RawSocketFactory
{
public:
  /**
   * \brief Create a raw IPv6 socket.
   */
  virtual Ptr<Socket> CreateSocket ();
};

} /* namespace ns3 */

#endif /* IPV6_RAW_SOCKET_FACTORY_IMPL_H */

