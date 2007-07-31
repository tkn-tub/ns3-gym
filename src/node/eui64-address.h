#ifndef EUI64_ADDRESS_H
#define EUI64_ADDRESS_H

#include <stdint.h>
#include <ostream>

namespace ns3 {

class Address;

/**
 * \brief an EUI-48 address
 *
 * This class can contain 48 bit IEEE addresses.
 */
class Eui64Address
{
public:
  Eui64Address ();
  /**
   * \param str a string representing the new Eui64Address
   *
   * The format of the string is "xx:xx:xx:xx:xx:xx"
   */
  Eui64Address (const char *str);

  /**
   * \param buffer address in network order
   *
   * Copy the input address to our internal buffer.
   */
  void CopyFrom (const uint8_t buffer[8]);
  /**
   * \param buffer address in network order
   *
   * Copy the internal address to the input buffer.
   */
  void CopyTo (uint8_t buffer[8]) const;
  /**
   * \returns a new Address instance
   *
   * Convert an instance of this class to a polymorphic Address instance.
   */
  Address ConvertTo (void) const;
  /**
   * \returns true if the address matches, false otherwise.
   */
  static bool IsMatchingType (const Address &address);
  /**
   * \param address a polymorphic address
   *
   * Convert a polymorphic address to an Eui64Address instance.
   * The conversion performs a type check.
   */
  static Eui64Address ConvertFrom (const Address &address);
  /**
   * Allocate a new Eui64Address.
   */
  static Eui64Address Allocate (void);
private:
  static uint8_t GetType (void);
  uint8_t m_address[8];
};

bool operator == (const Eui64Address &a, const Eui64Address &b);
bool operator != (const Eui64Address &a, const Eui64Address &b);
std::ostream& operator<< (std::ostream& os, const Eui64Address & address);

} // namespace ns3

#endif /* EUI64_ADDRESS_H */
