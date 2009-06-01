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

  /**
   * \returns the IPv4 address of the j'th address of the interface
   *  corresponding to index i.
   * 
   * If the second parameter is omitted, the zeroth indexed address of 
   * the interface is returned.  Unless IP aliaising is being used on
   * the interface, the second parameter may typically be omitted.
   */
  Ipv4Address GetAddress (uint32_t i, uint32_t j = 0) const;

  void SetMetric (uint32_t i, uint16_t metric);
  /**
   * \param ipv4 pointer to Ipv4 object
   * \param interface interface index of the Ipv4Interface to add to the container
   */
  void Add (Ptr<Ipv4> ipv4, uint32_t interface);
  void Add (std::string ipv4Name, uint32_t interface);

 private:
  
  typedef std::vector<std::pair<Ptr<Ipv4>,uint32_t> > InterfaceVector;
  InterfaceVector m_interfaces;
};

} // namespace ns3

#endif /* IPV4_INTERFACE_CONTAINER_H */
