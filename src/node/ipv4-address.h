/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef IPV4_ADDRESS_H
#define IPV4_ADDRESS_H

#include <stdint.h>
#include <ostream>

namespace ns3 {

class Ipv4Mask;

/** Ipv4 addresses are stored in host order in
  * this class.
  */
class Ipv4Address {
public:
  Ipv4Address ();
  /**
   * input address is in host order.
   * \param address The host order 32-bit address
   */
  Ipv4Address (uint32_t address);
  /** 
    * \brief Constructs an Ipv4Address by parsing a the input C-string
    *
    * Input address is in format:
    * hhh.xxx.xxx.lll
    * where h is the high byte and l the
    * low byte
    * \param address C-string containing the address as described above
    */
  Ipv4Address (char const *address);
  
  /**
   * \brief Comparison operation between two Ipv4Addresses
   * \param other address to which to compare this address
   * \return True if the addresses are equal. False otherwise.
   */
  bool IsEqual (Ipv4Address other) const;

  /** 
   * \brief Get the host-order 32-bit IP address
   *
   * Using this method is frowned upon.
   * Please, do _not_ use this method.
   * It is there only for chunk-ipv4.
   * \return the host-order 32-bit IP address
   */
  uint32_t GetHostOrder (void) const;
  void SetHostOrder (uint32_t ip);
  /**
   * Serialize this address to a 4-byte buffer
   * \param buf output buffer to which this address gets overwritten with this
   * Ipv4Address
   */
  void Serialize (uint8_t buf[4]) const;
  /**
   * \brief Print this address to the given output stream
   *
   * The print format is in the typical "192.168.1.1"
   * \param os The output stream to which this Ipv4Address is printed
   */
  void Print (std::ostream &os) const;

  bool IsBroadcast (void) const;
  bool IsMulticast (void) const;
  /**
   * \brief Combine this address with a network mask
   *
   * This method returns an IPv4 address that is this address combined
   * (bitwise and) with a network mask, yielding an IPv4 network
   * address.
   *
   * \param a network mask 
   */
  Ipv4Address CombineMask (Ipv4Mask const &mask) const;

  static Ipv4Address GetZero (void);
  static Ipv4Address GetAny (void);
  static Ipv4Address GetBroadcast (void);
  static Ipv4Address GetLoopback (void);
private:
  uint32_t m_address;
};


class Ipv4Mask {
public:
  Ipv4Mask ();
  Ipv4Mask (uint32_t mask);
  Ipv4Mask (char const *mask);

  bool IsMatch (Ipv4Address a, Ipv4Address b) const;

  bool IsEqual (Ipv4Mask other) const;


  /* Using this method is frowned upon.
   * Please, do _not_ use this method.
   */
  uint32_t GetHostOrder (void) const;
  void SetHostOrder (uint32_t value);

  void Print (std::ostream &os) const;

  static Ipv4Mask GetLoopback (void);
  static Ipv4Mask GetZero (void);
private:
  uint32_t m_mask;
};

std::ostream& operator<< (std::ostream& os, Ipv4Address const& address);
std::ostream& operator<< (std::ostream& os, Ipv4Mask const& mask);

bool operator == (Ipv4Address const &a, Ipv4Address const &b);
bool operator != (Ipv4Address const &a, Ipv4Address const &b);
bool operator < (Ipv4Address const &addrA, Ipv4Address const &addrB);

class Ipv4AddressHash : public std::unary_function<Ipv4Address, size_t> {
public:
  size_t operator()(Ipv4Address const &x) const;
};

bool operator == (Ipv4Mask const &a, Ipv4Mask const &b);
bool operator != (Ipv4Mask const &a, Ipv4Mask const &b);

}; // namespace ns3

#endif /* IPV4_ADDRESS_H */
