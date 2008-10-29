#ifndef IPV4_RAW_SOCKET_FACTORY_IMPL_H
#define IPV4_RAW_SOCKET_FACTORY_IMPL_H

#include "ns3/ipv4-raw-socket-factory.h"

namespace ns3 {

class Ipv4RawSocketFactoryImpl : public Ipv4RawSocketFactory
{
public:
  virtual Ptr<Socket> CreateSocket (void);
};


} // namespace ns3

#endif /* IPV4_RAW_SOCKET_FACTORY_IMPL_H */
