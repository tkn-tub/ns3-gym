#ifndef EUI48_ADDRESS_H
#define EUI48_ADDRESS_H

#include <stdint.h>

namespace ns3 {

class Address;

class Eui48Address
{
public:
  Eui48Address ();
  Eui48Address (const char *str);
  Address ConvertTo (void) const;
  static Eui48Address ConvertFrom (const Address &address);
  static Eui48Address Allocate (void);
private:
  static uint8_t GetType (void);
  uint8_t m_address[6];

};

} // namespace ns3

#endif /* EUI48_ADDRESS_H */
