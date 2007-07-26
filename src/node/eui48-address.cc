#include "eui48-address.h"
#include "address.h"
#include "ns3/assert.h"

namespace ns3 {

#define ASCII_a (0x41)
#define ASCII_z (0x5a)
#define ASCII_A (0x61)
#define ASCII_Z (0x7a)
#define ASCII_COLON (0x3a)
#define ASCII_ZERO (0x30)

static char
AsciiToLowCase (char c)
{
  if (c >= ASCII_a && c <= ASCII_z) {
    return c;
  } else if (c >= ASCII_A && c <= ASCII_Z) {
    return c + (ASCII_a - ASCII_A);
  } else {
    return c;
  }
}


Eui48Address::Eui48Address ()
{
  memset (m_address, 0, 6);
}
Eui48Address::Eui48Address (const char *str)
{
  int i = 0;
  while (*str != 0 && i < 6) 
    {
      uint8_t byte = 0;
      while (*str != ASCII_COLON && *str != 0) 
	{
	  byte <<= 4;
	  char low = AsciiToLowCase (*str);
	  if (low >= ASCII_a) 
	    {
	      byte |= low - ASCII_a + 10;
	    } 
	  else 
	    {
	      byte |= low - ASCII_ZERO;
	    }
	  str++;
	}
      m_address[i] = byte;
      i++;
      if (*str == 0) 
	{
	  break;
	}
      str++;
    }
  NS_ASSERT (i == 6);
}
Address 
Eui48Address::ConvertTo (void) const
{
  return Address (GetType (), m_address, 6);
}
Eui48Address 
Eui48Address::ConvertFrom (const Address &address)
{
  NS_ASSERT (address.CheckCompatible (GetType (), 6));
  Eui48Address retval;
  address.CopyTo (retval.m_address);
  return retval;
}
Eui48Address 
Eui48Address::Allocate (void)
{
  static uint64_t id = 0;
  id++;
  Eui48Address address;
  address.m_address[0] = (id >> 48) & 0xff;
  address.m_address[1] = (id >> 32) & 0xff;
  address.m_address[2] = (id >> 24) & 0xff;
  address.m_address[3] = (id >> 16) & 0xff;
  address.m_address[4] = (id >> 8) & 0xff;
  address.m_address[5] = (id >> 0) & 0xff;
  return address;
}
uint8_t 
Eui48Address::GetType (void)
{
  static uint8_t type = Address::Register ();
  return type;
}


} // namespace ns3
