#ifndef NET_DEVICE_CONTAINER_H
#define NET_DEVICE_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "net-device.h"

namespace ns3 {

class NetDeviceContainer
{
public:
  typedef std::vector<Ptr<NetDevice> >::const_iterator Iterator;

  Iterator Begin (void) const;
  Iterator End (void) const;

  uint32_t GetN (void) const;
  Ptr<NetDevice> Get (uint32_t i) const;

  void Create (uint32_t n);
  void Add (NetDeviceContainer other);
  void Add (Ptr<NetDevice> node);

private:
  std::vector<Ptr<NetDevice> > m_nodes;
};

} // namespace ns3

#endif /* NET_DEVICE_CONTAINER_H */
