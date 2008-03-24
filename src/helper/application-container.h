#ifndef APPLICATION_CONTAINER_H
#define APPLICATION_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "ns3/application.h"

namespace ns3 {

/**
 * \brief holds a vector of ns3::Application pointers
 *
 */
class ApplicationContainer
{
public:
  typedef std::vector<Ptr<Application> >::const_iterator Iterator;

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
  Ptr<Application> Get (uint32_t i) const;

  /**
   * \param other another netdevice container
   *
   * Append to the end of this container the other input container.
   */
  void Add (ApplicationContainer other);
  /**
   * \param device another netdevice pointer.
   *
   * Append to the end of this container the input netdevice pointer.
   */
  void Add (Ptr<Application> application);

  void Start (Time start);
  void Stop (Time stop);

private:
  std::vector<Ptr<Application> > m_applications;
};

} // namespace ns3

#endif /* APPLICATION_CONTAINER_H */
