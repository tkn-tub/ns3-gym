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
   * Concatenate the entries in the other container with ours.
   */
  void Add (Ipv4InterfaceContainer other);

  /**
   * \returns the number of interfaces stored in this Ipv4InterfaceContainer.
   */
  uint32_t GetN (void) const;

  Ipv4Address GetAddress (uint32_t i) const;
  void SetMetric (uint32_t i, uint16_t metric);

  void Add (Ptr<Ipv4> ipv4, uint32_t interface);
  void Add (std::string ipv4Name, uint32_t interface);

 private:
  
  typedef std::vector<std::pair<Ptr<Ipv4>,uint32_t> > InterfaceVector;
  InterfaceVector m_interfaces;
};

} // namespace ns3

#endif /* IPV4_INTERFACE_CONTAINER_H */
