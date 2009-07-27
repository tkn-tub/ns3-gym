#ifndef RAW_SOCKET_FACTORY_IMPL_H
#define RAW_SOCKET_FACTORY_IMPL_H

#include "ns3/raw-socket-factory.h"
#include "ipv4-l3-protocol.h"


namespace ns3 {

class Ipv4L3Protocol;

class RawSocketFactoryImpl : public RawSocketFactory
{
protected:
  virtual void DoDispose (void);
public:
  virtual Ptr<Socket> CreateSocket (void);
private:
  Ptr<Ipv4L3Protocol> m_ipv4;

};


} // namespace ns3

#endif /* RAW_SOCKET_FACTORY_IMPL_H */
