#include "ns3/assert.h"
#include "address.h"
#include <iostream>
#include <iomanip>

namespace ns3 {

Address::Address ()
  : m_type (0),
    m_len (0)
{
  memset (m_data, 0, m_len);
}

Address::Address (uint8_t type, const uint8_t *buffer, uint8_t len)
  : m_type (type),
    m_len (len)
{
  NS_ASSERT (m_len <= MAX_SIZE);
  memset (m_data, 0, m_len);
  memcpy (m_data, buffer, m_len);
}
Address::Address (const Address & address)
  : m_type (address.m_type),
    m_len (address.m_len)
{
  NS_ASSERT (m_len <= MAX_SIZE);
  memset (m_data, 0, m_len);
  memcpy (m_data, address.m_data, m_len);
}
Address &
Address::operator = (const Address &address)
{
  NS_ASSERT (m_len <= MAX_SIZE);
  m_type = address.m_type;
  m_len = address.m_len;
  NS_ASSERT (m_len <= MAX_SIZE);
  memset (m_data, 0, m_len);
  memcpy (m_data, address.m_data, m_len);
  return *this;
}

bool
Address::IsInvalid (void) const
{
  return m_len == 0 && m_type == 0;
}

uint8_t 
Address::GetLength (void) const
{
  NS_ASSERT (m_len <= MAX_SIZE);
  return m_len;
}
uint32_t
Address::CopyTo (uint8_t buffer[MAX_SIZE]) const
{
  NS_ASSERT (m_len <= MAX_SIZE);
  memcpy (buffer, m_data, m_len);
  return m_len;
}
uint32_t
Address::CopyAllTo (uint8_t *buffer, uint8_t len) const
{
  NS_ASSERT (len >= m_len + 2);
  buffer[0] = m_type;
  buffer[1] = m_len;
  memcpy (buffer + 2, m_data, m_len);
  return m_len + 2;
}

uint32_t
Address::CopyFrom (const uint8_t *buffer, uint8_t len)
{
  NS_ASSERT (len <= MAX_SIZE);
  memcpy (m_data, buffer, len);
  m_len = len;
  return m_len;
}
uint32_t
Address::CopyAllFrom (const uint8_t *buffer, uint8_t len)
{
  NS_ASSERT (len >= 2);
  m_type = buffer[0];
  m_len = buffer[1];
  NS_ASSERT (len >= m_len + 2);
  memcpy (m_data, buffer + 2, m_len);
  return m_len + 2;
}
bool 
Address::CheckCompatible (uint8_t type, uint8_t len) const
{
  NS_ASSERT (len <= MAX_SIZE);
  return m_len == len && (m_type == type || m_type == 0);
}
bool 
Address::IsMatchingType (uint8_t type) const
{
  return m_type == type;
}

uint8_t 
Address::Register (void)
{
  static uint8_t type = 1;
  type++;
  return type;
}

ATTRIBUTE_HELPER_CPP (Address);


bool operator == (const Address &a, const Address &b)
{
  /* Two addresses can be equal even if their types are 
   * different if one of the two types is zero. a type of 
   * zero identifies an Address which might contain meaningful 
   * payload but for which the type field could not be set because
   * we did not know it. This can typically happen in the ARP
   * layer where we receive an address from an ArpHeader but
   * we do not know its type: we really want to be able to
   * compare addresses without knowing their real type.
   */
  if (a.m_type != b.m_type &&
      a.m_type != 0 && 
      b.m_type != 0)
    {
      return false;
    }
  if (a.m_len != b.m_len)
    {
      return false;
    }
  return memcmp (a.m_data, b.m_data, a.m_len) == 0;
}
bool operator != (const Address &a, const Address &b)
{
  return !(a == b);
}
bool operator < (const Address &a, const Address &b)
{
  // XXX: it is not clear to me how to order based on type.
  // so, we do not compare the types here but we should.
  if (a.m_len < b.m_len)
    {
      return true;
    }
  else if (a.m_len > b.m_len)
    {
      return false;
    }
  NS_ASSERT (a.GetLength() == b.GetLength());
  for (uint8_t i = 0; i < a.GetLength(); i++) 
    {
      if (a.m_data[i] < b.m_data[i]) 
	{
	  return true;
	} 
      else if (a.m_data[i] > b.m_data[i]) 
	{
	  return false;
	}
    }
  return false;
}

std::ostream& operator<< (std::ostream& os, const Address & address)
{
  if (address.m_len == 0) 
    {
      os << "NULL-ADDRESS";
      return os;
    }
  os.setf (std::ios::hex, std::ios::basefield);
  os.fill('0');
  for (uint8_t i=0; i < (address.m_len-1); i++) 
    {
      os << std::setw(2) << (uint32_t)address.m_data[i] << ":";
    }
  // Final byte not suffixed by ":"
  os << std::setw(2) << (uint32_t)address.m_data[address.m_len-1];
  os.setf (std::ios::dec, std::ios::basefield);
  os.fill(' ');
  return os;
}

std::istream& operator>> (std::istream& is, Address & address)
{
  // XXX: need to be able to parse this.
  return is;
}



} // namespace ns3
