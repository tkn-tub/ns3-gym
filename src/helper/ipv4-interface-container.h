#ifndef IPV4_INTERFACE_CONTAINER_H
#define IPV4_INTERFACE_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "ns3/ipv4.h"
#include "ns3/ipv4-address.h"

namespace ns3 {

/**
 * \brief holds a vector of std::pair of Ptr<Ipv4> and interface index.
 *
 * Typically ns-3 Ipv4Interfaces are installed on devices using an Ipv4 address
 * helper.  The helper's Assign() method takes a NetDeviceContainer which holds 
 * some number of Ptr<NetDevice>.  For each of the NetDevices in the 
 * NetDeviceContainer the helper will find the associated Ptr<Node> and
 * Ptr<Ipv4>.  It makes sure that an interface exists on the node for the 
 * device and then adds an Ipv4Address according to the address helper settings
 * (incrementing the Ipv4Address somehow as it goes).  The helper then converts
 * the Ptr<Ipv4> and the interface index to a std::pair and adds them to a 
 * container -- a container of this type.
 *
 * The point is then to be able to implicitly associate an index into the 
 * original NetDeviceContainer (that identifies a particular net device) with
 * an identical index into the Ipv4InterfaceContainer that has a std::pair with
 * the Ptr<Ipv4> and interface index you need to play with the interface.
 *
 * @see Ipv4AddressHelper
 * @see Ipv4
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
   * \param other container
   */
  void Add (Ipv4InterfaceContainer other);

  /**
   * \returns the number of Ptr<Ipv4> and interface pairs stored in this 
   * Ipv4InterfaceContainer.
   */
  uint32_t GetN (void) const;

  /**
   * \param i index of ipInterfacePair in container
   * \param j interface address index (if interface has multiple addresses)
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
   * Manually add an entry to the container consisting of the individual parts
   * of an entry std::pair.
   *
   * \param ipv4 pointer to Ipv4 object
   * \param interface interface index of the Ipv4Interface to add to the container
   *
   * @see Ipv4InterfaceContainer
   */
  void Add (Ptr<Ipv4> ipv4, uint32_t interface);

  /**
   * Manually add an entry to the container consisting of a previously composed 
   * entry std::pair.
   *
   * \param ipInterfacePair the pair of a pointer to Ipv4 object and interface index of the Ipv4Interface to add to the container
   *
   * @see Ipv4InterfaceContainer
   */
  void Add (std::pair<Ptr<Ipv4>, uint32_t> ipInterfacePair);

  /**
   * Manually add an entry to the container consisting of the individual parts
   * of an entry std::pair.
   *
   * \param ipv4Name std:string referring to the saved name of an Ipv4 Object that
   *        has been previously named using the Object Name Service.
   * \param interface interface index of the Ipv4Interface to add to the container
   *
   * @see Ipv4InterfaceContainer
   */
  void Add (std::string ipv4Name, uint32_t interface);

  /**
   * Get the std::pair of an Ptr<Ipv4> and interface stored at the location 
   * specified by the index.
   *
   * \param i the index of the entery to retrieve.
   *
   * @see Ipv4InterfaceContainer
   */
  std::pair<Ptr<Ipv4>, uint32_t> Get (uint32_t i) const;

 private:
  
  typedef std::vector<std::pair<Ptr<Ipv4>,uint32_t> > InterfaceVector;
  InterfaceVector m_interfaces;
};

} // namespace ns3

#endif /* IPV4_INTERFACE_CONTAINER_H */
