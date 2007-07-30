#ifndef IPV4_TRANSPORT_ADDRESS_H
#define IPV4_TRANSPORT_ADDRESS_H

#include "address.h"
#include "ipv4-address.h"
#include <stdint.h>

namespace ns3 {


/**
 * \brief an Inet address class
 *
 * This class is similar to inet_sockaddr in the BSD socket
 * API. i.e., this class holds an Ipv4Address and a port number
 * to form an ipv4 transport endpoint.
 */
class InetSocketAddress
{
public:
  /**
   * \param ipv4 the ipv4 address
   * \param port the port number
   */
  InetSocketAddress (Ipv4Address ipv4, uint16_t port);
  /**
   * \param ipv4 the ipv4 address
   *
   * The port number is set to zero by default.
   */
  InetSocketAddress (Ipv4Address ipv4);
  /**
   * \param port the port number
   *
   * The ipv4 address is set to the "Any" address by default.
   */
  InetSocketAddress (uint16_t port);
  /**
   * \param ipv4 string which represents an ipv4 address
   * \param port the port number
   */
  InetSocketAddress (const char *ipv4, uint16_t port);
  /**
   * \param ipv4 string which represents an ipv4 address
   *
   * The port number is set to zero.
   */
  InetSocketAddress (const char *ipv4);
  /**
   * \returns the port number
   */
  uint16_t GetPort (void) const;
  /**
   * \returns the ipv4 address
   */
  Ipv4Address GetIpv4 (void) const;

  /**
   * \param port the new port number.
   */
  void SetPort (uint16_t port);
  /**
   * \param address the new ipv4 address
   */
  void SetIpv4 (Ipv4Address address);

  /**
   * \returns true if the address matches, false otherwise.
   */
  static bool IsMatchingType (const Address &address);

  /**
   * \returns an Address instance which represents this
   * InetSocketAddress instance.
   */
  Address ConvertTo (void) const;
  /**
   * \param address the Address instance to convert from.
   * \returns an InetSocketAddress which corresponds to the input
   * Address
   */
  static InetSocketAddress ConvertFrom (const Address &address);
private:
  static uint8_t GetType (void);
  Ipv4Address m_ipv4;
  uint16_t m_port;
};

} // namespace ns3


#endif /* IPV4_TRANSPORT_ADDRESS_H */
