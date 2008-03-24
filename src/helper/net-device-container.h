#ifndef NET_DEVICE_CONTAINER_H
#define NET_DEVICE_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "ns3/net-device.h"

namespace ns3 {

/**
 * \brief holds a vector of ns3::NetDevice pointers
 *
 */
class NetDeviceContainer
{
public:
  typedef std::vector<Ptr<NetDevice> >::const_iterator Iterator;

  /**
   * \returns an iterator which points to the start of the array of pointers.
   */
  Iterator Begin (void) const;
  /**
   * \returns an iterator which points to the end of the array of pointers.
   */
  Iterator End (void) const;

  /**
   * \returns the number of netdevice pointers stored in this container.
   */
  uint32_t GetN (void) const;
  /**
   * \param i the index of the requested netdevice pointer.
   * \returns the requested netdevice pointer.
   */
  Ptr<NetDevice> Get (uint32_t i) const;

  /**
   * \param other another netdevice container
   *
   * Append to the end of this container the other input container.
   */
  void Add (NetDeviceContainer other);
  /**
   * \param device another netdevice pointer.
   *
   * Append to the end of this container the input netdevice pointer.
   */
  void Add (Ptr<NetDevice> device);

private:
  std::vector<Ptr<NetDevice> > m_devices;
};

} // namespace ns3

#endif /* NET_DEVICE_CONTAINER_H */
