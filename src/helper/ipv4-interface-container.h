#ifndef IPV4_INTERFACE_CONTAINER_H
#define IPV4_INTERFACE_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "ns3/ipv4.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

/**
 * \brief keep track of a set of ipv4 interfaces.
 *
 */
class Ipv4InterfaceContainer
{
public:
  /**
   * Create an empty Ipv4InterfaceContainer.
   */
  Ipv4InterfaceContainer ();

  /**
   * \returns the number of interfaces stored in this Ipv4InterfaceContainer.
   */
  uint32_t GetN (void) const;

  Ipv4Address GetAddress (uint32_t i) const;
  void SetMetric (uint32_t i, uint16_t metric);

  void Add (Ptr<Ipv4> ipv4, uint32_t interface);

 private:
  std::vector<std::pair<Ptr<Ipv4>,uint32_t> > m_interfaces;
};

} // namespace ns3

#endif /* IPV4_INTERFACE_CONTAINER_H */
