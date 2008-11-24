/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007-2008 Louis Pasteur University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Sebastien Vincent <vincent@clarinet.u-strasbg.fr>
 */

#include <string.h>

#include "ns3/log.h"
#include "ipv6-address.h"
#include "ns3/assert.h"
#include "mac48-address.h"

#include <iomanip>

NS_LOG_COMPONENT_DEFINE ("Ipv6Address");

namespace ns3 {

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * \brief Get a hash key.
   * \param k the key
   * \param length the length of the key
   * \param level the previous hash, or an arbitrary value
   * \return hash
   * \note Adpated from Jens Jakobsen implementation (chillispot).
   */
  static uint32_t lookuphash (unsigned char* k, uint32_t length, uint32_t level)
  {
#define mix(a,b,c) \
    { \
      a -= b; a -= c; a ^= (c>>13); \
      b -= c; b -= a; b ^= (a<<8); \
      c -= a; c -= b; c ^= (b>>13); \
      a -= b; a -= c; a ^= (c>>12);  \
      b -= c; b -= a; b ^= (a<<16); \
      c -= a; c -= b; c ^= (b>>5); \
      a -= b; a -= c; a ^= (c>>3);  \
      b -= c; b -= a; b ^= (a<<10); \
      c -= a; c -= b; c ^= (b>>15); \
    }

    typedef uint32_t  ub4;   /* unsigned 4-byte quantities */
    typedef unsigned  char ub1;   /* unsigned 1-byte quantities */
    uint32_t a,b,c,len;

    /* Set up the internal state */
    len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = level;           /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
      a += (k[0] +((ub4)k[1]<<8) +((ub4)k[2]<<16) +((ub4)k[3]<<24));
      b += (k[4] +((ub4)k[5]<<8) +((ub4)k[6]<<16) +((ub4)k[7]<<24));
      c += (k[8] +((ub4)k[9]<<8) +((ub4)k[10]<<16)+((ub4)k[11]<<24));
      mix(a,b,c);
      k += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += length;
    switch(len)              /* all the case statements fall through */
    {
      case 11: c+=((ub4)k[10]<<24);
      case 10: c+=((ub4)k[9]<<16);
      case 9 : c+=((ub4)k[8]<<8);
               /* the first byte of c is reserved for the length */
      case 8 : b+=((ub4)k[7]<<24);
      case 7 : b+=((ub4)k[6]<<16);
      case 6 : b+=((ub4)k[5]<<8);
      case 5 : b+=k[4];
      case 4 : a+=((ub4)k[3]<<24);
      case 3 : a+=((ub4)k[2]<<16);
      case 2 : a+=((ub4)k[1]<<8);
      case 1 : a+=k[0];
               /* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
  }
#ifdef __cplusplus
}
#endif

/**
 * \brief Convert an IPv6 C-string into a 128-bit representation.
 * \return 1 if OK, 0 if failure (bad format, ...)
 * \note This function is strongly inspired by inet_pton6() from Paul Vixie.
 * \todo Handle IPv6 address with decimal value for last four bytes.
 */
static int AsciiToIpv6Host (char const *address, uint8_t addr[16])
{
  static const char xdigits_l[] = "0123456789abcdef",
               xdigits_u[] = "0123456789ABCDEF";
  unsigned char tmp[16 /*NS_IN6ADDRSZ*/], *tp, *endp, *colonp;
  const char *xdigits, *curtok;
  int ch, seen_xdigits;
  unsigned int val;

  memset((tp = tmp), '\0', 16 /* NS_IN6ADDRSZ*/);
  endp = tp + 16 /*NS_IN6ADDRSZ*/;
  colonp = NULL;
  /* Leading :: requires some special handling. */
  if (*address == ':')
    if (*++address != ':')
      return (0);
  curtok = address;
  seen_xdigits = 0;
  val = 0;
  while ((ch = *address++) != '\0')
  {
    const char *pch;

    if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL)
      pch = strchr((xdigits = xdigits_u), ch);
    if (pch != NULL)
    {
      val <<= 4;
      val |= (pch - xdigits);
      if (++seen_xdigits > 4)
        return (0);
      continue;
    }
    if (ch == ':')
    {
      curtok = address;
      if (!seen_xdigits)
      {
        if (colonp)
          return (0);
        colonp = tp;
        continue;
      }
      if (tp + 2 /*NS_INT16SZ*/ > endp)
        return (0);
      *tp++ = (unsigned char) (val >> 8) & 0xff;
      *tp++ = (unsigned char) val & 0xff;
      seen_xdigits = 0;
      val = 0;
      continue;
    }

    /* TODO Handle address like 2001::xxx.xxx.xxx.xxxx */
#if 0
    if (ch == '.' && ((tp + 4 /*NS_INADDRSZ*/) <= endp) &&
        inet_pton4(curtok, tp) > 0)
    {
      tp += 4 /*NS_INADDRSZ*/;
      seen_xdigits = 0;
      break;/* '\0' was seen by inet_pton4(). */
    }
#endif
    return (0);
  }
  if (seen_xdigits)
  {
    if (tp + 2/* NS_INT16SZ*/ > endp)
      return (0);
    *tp++ = (unsigned char) (val >> 8) & 0xff;
    *tp++ = (unsigned char) val & 0xff;
  }
  if (colonp != NULL)
  {
    /*
     * Since some memmove()'s erroneously fail to handle
     * overlapping regions, we'll do the shift by hand.
     */
    const int n = tp - colonp;
    int i;

    if (tp == endp)
      return (0);
    for (i = 1; i <= n; i++)
    {
      endp[- i] = colonp[n - i];
      colonp[n - i] = 0;
    }
    tp = endp;
  }
  if (tp != endp)
    return (0);

  /* memcpy(dst, tmp, NS_IN6ADDRSZ);  */
  memcpy(addr, tmp, 16);
  return (1);
}

Ipv6Address::Ipv6Address ()
{
  memset(m_address, 0x00, 16);
}

Ipv6Address::Ipv6Address (Ipv6Address const& addr)
{
  memcpy(m_address, addr.m_address, 16);
}

Ipv6Address::Ipv6Address (Ipv6Address const* addr)
{
  memcpy(m_address, addr->m_address, 16);
}

Ipv6Address::Ipv6Address (char const* address)
{
  AsciiToIpv6Host (address, m_address);
}

Ipv6Address::Ipv6Address (uint8_t address[16])
{
  /* 128 bit => 16 bytes */
  memcpy(m_address, address, 16);
}

Ipv6Address::~Ipv6Address ()
{
  /* do nothing */
}

void Ipv6Address::Set (char const* address)
{
  AsciiToIpv6Host (address, m_address);
}

void Ipv6Address::Set (uint8_t address[16])
{
  /* 128 bit => 16 bytes */
  memcpy(m_address, address, 16);
}

void Ipv6Address::Serialize (uint8_t buf[16]) const
{
  memcpy(buf, m_address, 16);
}

Ipv6Address Ipv6Address::Deserialize (const uint8_t buf[16])
{
  Ipv6Address ipv6((uint8_t*)buf);
  return ipv6;
}

Ipv6Address Ipv6Address::MakeAutoconfiguredAddress (Mac48Address addr, Ipv6Address prefix)
{
  Ipv6Address ret;
  uint8_t buf[16];
  uint8_t buf2[16];

  addr.CopyTo(buf);
  prefix.GetBytes(buf2);

  memcpy(buf2 + 8, buf, 3);
  buf2[11] = 0xff;
  buf2[12] = 0xfe;
  memcpy(buf2 + 13, buf + 3, 3);
  buf2[8] |= 0x02;

  ret.Set(buf2);
  return ret;
}

Ipv6Address Ipv6Address::MakeAutoconfiguredLinkLocalAddress (Mac48Address addr)
{
  Ipv6Address ret;
  uint8_t buf[16];
  uint8_t buf2[16];

  addr.CopyTo(buf);

  memset(buf2, 0x00, sizeof(buf2));
  buf2[0] = 0xfe;
  buf2[1] = 0x80;
  memcpy(buf2 + 8, buf, 3);
  buf2[11] = 0xff;
  buf2[12] = 0xfe;
  memcpy(buf2 + 13, buf + 3, 3);
  buf2[8] |= 0x02;

  ret.Set(buf2);
  return ret;
}

Ipv6Address Ipv6Address::MakeSolicitedAddress (Ipv6Address addr)
{
  uint8_t buf[16];
  uint8_t buf2[16];
  Ipv6Address ret;

  addr.Serialize(buf2);

  memset(buf, 0x00, sizeof(buf));
  buf[0] = 0xff;
  buf[1] = 0x02;
  buf[11] = 0x01;
  buf[12] = 0xff;
  buf[13] = buf2[13];
  buf[14] = buf2[14];
  buf[15] = buf2[15];

  ret.Set(buf);
  return ret;
}

void Ipv6Address::Print (std::ostream& os) const
{
  os << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[0]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[1] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[2] 
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[3] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[4]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[5] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[6]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[7] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[8]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[9] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[10]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[11] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[12]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[13] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[14]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_address[15];
}

bool Ipv6Address::IsLocalhost () const
{
  static Ipv6Address localhost("::1");
  return (*this == localhost);
}

bool Ipv6Address::IsMulticast () const
{
  if(m_address[0] == 0xff)
  {
    return true;
  }
  return false;
}

Ipv6Address Ipv6Address::CombinePrefix (Ipv6Prefix const & prefix)
{
  Ipv6Address ipv6;
  uint8_t addr[16];
  uint8_t pref[16];
  unsigned int i = 0;

  memcpy(addr, m_address, 16);
  ((Ipv6Prefix)prefix).GetBytes(pref);

  /* a little bit ugly... */
  for(i = 0 ; i < 16 ; i++)
  {
    addr[i] = addr[i] & pref[i];
  }
  ipv6.Set(addr);
  return ipv6;
}

bool Ipv6Address::IsSolicitedMulticast () const
{
  uint8_t buf[16];

  Serialize(buf);

  if(buf[0] == 0xff && 
      buf[1] == 0x02 &&
      buf[11] == 0x01 &&
      buf[12] == 0xff)
  {
    return true;
  }
  return false;
}

bool Ipv6Address::IsAllNodesMulticast () const
{
  static Ipv6Address allnodes("ff02::1");
  return (*this == allnodes);
}

bool Ipv6Address::IsAllRoutersMulticast () const
{
  static Ipv6Address allrouters("ff02::2");
  return (*this == allrouters);
}

bool Ipv6Address::IsAllHostsMulticast () const
{
  static Ipv6Address allhosts("ff02::3");
  return (*this == allhosts);
}

bool Ipv6Address::IsAny () const
{
  static Ipv6Address any("::");
  return (*this == any);
}

bool Ipv6Address::IsMatchingType (const Address& address)
{
  return address.CheckCompatible(GetType(), 16);
}

Ipv6Address::operator Address () const
{
  return ConvertTo ();
}

Address Ipv6Address::ConvertTo (void) const
{
  uint8_t buf[16];
  Serialize (buf);
  return Address(GetType(), buf, 16);
}

Ipv6Address Ipv6Address::ConvertFrom (const Address &address)
{
  NS_ASSERT (address.CheckCompatible (GetType (), 16));
  uint8_t buf[16];
  address.CopyTo (buf);
  return Deserialize (buf);
}

uint8_t Ipv6Address::GetType (void)
{
  static uint8_t type = Address::Register();
  return type;
}

Ipv6Address Ipv6Address::GetZero ()
{
  Ipv6Address zero("::");
  return zero;
}

Ipv6Address Ipv6Address::GetAny ()
{
  Ipv6Address any("::");
  return any;
}

Ipv6Address Ipv6Address::GetAllNodesMulticast ()
{
  Ipv6Address nmc("ff02::1");
  return nmc;
}

Ipv6Address Ipv6Address::GetAllRoutersMulticast ()
{
  Ipv6Address rmc("ff02::2");
  return rmc;
}

Ipv6Address Ipv6Address::GetAllHostsMulticast ()
{
  Ipv6Address hmc("ff02::3");
  return hmc;
}

Ipv6Address Ipv6Address::GetLoopback ()
{
  static Ipv6Address loopback("::1");
  return loopback;
}

void Ipv6Address::GetBytes (uint8_t buf[16]) const
{
  memcpy(buf, m_address, 16);
}

bool Ipv6Address::IsLinkLocal () const
{
  Ipv6Address linkLocal("fe80::0");
  if(!IsMulticast() && ((Ipv6Address*)this)->CombinePrefix(Ipv6Prefix(64))==linkLocal)
  {
    return true;
  }
  return false;
}

bool Ipv6Address::IsEqual (const Ipv6Address& other) const
{
  if(!memcmp(m_address, other.m_address, 16))
  {
    return true;
  }
  return false;
}

std::ostream& operator << (std::ostream& os, Ipv6Address const& address)
{
  address.Print(os);
  return os;
}

std::istream& operator >> (std::istream& is, Ipv6Address& address)
{
  std::string str;
  is >> str;
  address = Ipv6Address (str.c_str ());
  return is;
}

Ipv6Prefix::Ipv6Prefix ()
{
  memset(m_prefix, 0x00, 16);
}

Ipv6Prefix::Ipv6Prefix (char const* prefix)
{
  AsciiToIpv6Host(prefix, m_prefix);
}

Ipv6Prefix::Ipv6Prefix (uint8_t prefix[16])
{
  memcpy(m_prefix, prefix, 16);
}

Ipv6Prefix::Ipv6Prefix (uint8_t prefix)
{
  unsigned int nb=0;
  unsigned int mod=0;
  unsigned int i=0;

  memset(m_prefix, 0x00, 16);

  NS_ASSERT(prefix <= 128);

  nb = prefix / 8;
  mod = prefix % 8;

  memset(m_prefix, 0xff, nb);

  if(mod)
  {
    m_prefix[nb] = 0xff << (8-mod);
  }

  if(nb < 16)
  {
    nb++;
    for(i = nb; i < 16 ; i++)
    {
      m_prefix[i] = 0x00;
    }
  }
}

Ipv6Prefix::Ipv6Prefix (Ipv6Prefix const& prefix)
{
  memcpy(m_prefix, prefix.m_prefix, 16);
}

Ipv6Prefix::Ipv6Prefix (Ipv6Prefix const* prefix)
{
  memcpy(m_prefix, prefix->m_prefix, 16);
}

Ipv6Prefix::~Ipv6Prefix ()
{
  /* do nothing */
}

bool Ipv6Prefix::IsMatch (Ipv6Address a, Ipv6Address b) const
{
  uint8_t addrA[16];
  uint8_t addrB[16];
  unsigned int i = 0;

  a.GetBytes(addrA);
  b.GetBytes(addrB);

  /* a little bit ugly... */
  for(i = 0 ; i < 16 ; i++)
  {
    if((addrA[i] & m_prefix[i]) !=	(addrB[i] & m_prefix[i]))
    {
      return false;
    }
  }
  return true;
}

void Ipv6Prefix::Print (std::ostream &os) const
{
  os << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[0]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[1] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[2]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[3] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[4]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[5] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[6]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[7] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[8]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[9] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[10]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[11] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[12]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[13] << ":"
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[14]
    << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) m_prefix[15];
}

Ipv6Prefix Ipv6Prefix::GetLoopback ()
{
  Ipv6Prefix prefix((uint8_t)128);
  return prefix;
}

Ipv6Prefix Ipv6Prefix::GetZero ()
{
  Ipv6Prefix prefix((uint8_t)0);
  return prefix;
}

void Ipv6Prefix::GetBytes (uint8_t buf[16]) const
{
  memcpy(buf, m_prefix, 16);
}

bool Ipv6Prefix::IsEqual (const Ipv6Prefix& other) const
{
  if(!memcmp(m_prefix, other.m_prefix, 16))
  {
    return true;
  }
  return false;
}

std::ostream& operator<< (std::ostream& os, Ipv6Prefix const& prefix)
{
  prefix.Print (os);
  return os;
}

std::istream& operator >> (std::istream& is, Ipv6Prefix& prefix)
{
  std::string str;
  is >> str;
  prefix = Ipv6Prefix (str.c_str ());
  return is;
}

bool operator == (Ipv6Prefix const &a, Ipv6Prefix const &b)
{
  return a.IsEqual (b);
}

bool operator != (Ipv6Prefix const &a, Ipv6Prefix const &b)
{
  return !a.IsEqual (b);
}

size_t Ipv6AddressHash::operator() (Ipv6Address const &x) const
{
  uint8_t buf[16];

  x.GetBytes(buf);

  return lookuphash(buf, sizeof(buf), 0);
}

ATTRIBUTE_HELPER_CPP (Ipv6Address);
ATTRIBUTE_HELPER_CPP (Ipv6Prefix);

} /* namespace ns3 */

