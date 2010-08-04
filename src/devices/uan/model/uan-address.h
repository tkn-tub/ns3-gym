/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 */

#ifndef UANADDRESS_H
#define UANADDRESS_H

#include "ns3/address.h"
#include <iostream>

namespace ns3 {

/**
 * \class UanAddress
 *
 * A class used for addressing UAN MAC's.  This implementation uses a simple 8 bit
 * flat addressing scheme.  It is unlikely that perceived underwater networks will
 * soon exceed 200 nodes (or the overlapping of two underwater networks - the ocean is big),
 * so this should provide adequate addressing for most applications.
 */
class UanAddress
{
public:
  UanAddress ();
  /**
   * \param addr byte address to assign to this address
   * \brief Create UanAddress object with address addr
   */
  UanAddress (uint8_t addr);
  virtual ~UanAddress ();

  /**
   * \param address  Address to convert to UAN address
   * \returns UanAddress from Address
   */
  static UanAddress ConvertFrom (const Address &address);

  /**
   * \param address  Address to test
   * \returns True if address given is consistant with UanAddress
   */
  static bool IsMatchingType  (const Address &address);

  /**
   * \brief Create UanAddress object with broadcast address
   */
  operator Address () const;

  /**
   * \param pBuffer Buffer to extract address from
   * \brief Sets address to address stored in parameter
   */
  void CopyFrom (const uint8_t *pBuffer);

  /**
   * \param pBuffer
   * \brief Writes address to buffer parameter
   */
  void CopyTo (uint8_t *pBuffer);

  /**
   * \returns 8 bit integer version of address
   */
  uint8_t GetAsInt (void) const;

  /**
   * \returns Broadcast address: 255
   */
  static UanAddress GetBroadcast (void);

  /**
   * \brief Allocates UanAddress from 0-254 (will wrap back to 0 if more than 255 are allocated)
   */
  static UanAddress Allocate ();


private:
  uint8_t m_address;

  static uint8_t GetType (void);
  Address ConvertTo (void) const;

  friend bool operator == (const UanAddress &a, const UanAddress &b);
  friend bool operator != (const UanAddress &a, const UanAddress &b);
  friend std::ostream& operator<< (std::ostream& os, const UanAddress & address);
  friend std::istream& operator>> (std::istream& is, UanAddress & address);

};


/**
 * \brief Returns true if byte address a is equal to byte address b
 */
bool operator == (const UanAddress &a, const UanAddress &b);

/**
 * \brief Returns true if byte address a is unequal to byte address b
 */
bool operator != (const UanAddress &a, const UanAddress &b);

/**
 * \brief Writes param address to stream os as 8 bit integer
 */
std::ostream& operator<< (std::ostream& os, const UanAddress & address);

/**
 * \brief Inputs param address from stream is as 8 bit integer
 */
std::istream& operator>> (std::istream& is, UanAddress & address);

} // namespace ns3

#endif // UANADDRESS_H
