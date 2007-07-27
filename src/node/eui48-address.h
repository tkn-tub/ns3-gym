#ifndef EUI48_ADDRESS_H
#define EUI48_ADDRESS_H

#include <stdint.h>

namespace ns3 {

class Address;

/**
 * \brief an EUI-48 address
 *
 * This class can contain 48 bit IEEE addresses.
 */
class Eui48Address
{
public:
  Eui48Address ();
  /**
   * \param str a string representing the new Eui48Address
   *
   * The format of the string is "xx:xx:xx:xx:xx:xx"
   */
  Eui48Address (const char *str);
  /**
   * \returns a new Address instance
   *
   * Convert an instance of this class to a polymorphic Address instance.
   */
  Address ConvertTo (void) const;
  /**
   * \param address a polymorphic address
   *
   * Convert a polymorphic address to an Eui48Address instance.
   * The conversion performs a type check.
   */
  static Eui48Address ConvertFrom (const Address &address);
  /**
   * Allocate a new Eui48Address.
   */
  static Eui48Address Allocate (void);
private:
  static uint8_t GetType (void);
  uint8_t m_address[6];

};

} // namespace ns3

#endif /* EUI48_ADDRESS_H */
