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

#ifndef UANHEADERCOMMON_H
#define UANHEADERCOMMON_H

#include "ns3/header.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "uan-address.h"

namespace ns3 {

/**
 * \class UanHeaderCommon
 *
 * Header includes 1 byte src address, 1 byte dest address,
 * and a 1 byte type field.
 */
class UanHeaderCommon : public Header
{
public:
  UanHeaderCommon ();
  /**
   * \brief Create UanHeaderCommon object with given source and destination address and header type
   * \param src Source address defined in header
   * \param dest Destination address defined in header
   * \param type Header type
   */
  UanHeaderCommon (const UanAddress src, const UanAddress dest, uint8_t type);
  virtual ~UanHeaderCommon ();

  static TypeId GetTypeId (void);

  /**
   * \param dest Address of destination node
   */
  void SetDest (UanAddress dest);
  /**
   * \param src Address of packet source node
   */
  void SetSrc (UanAddress src);
  /**
   * \param type Value to set 1 byte type field to.  Value has protocol specific meaning
   */
  void SetType (uint8_t type);

  /**
   * \returns UanAddress in destination field
   */
  UanAddress GetDest (void) const;
  /**
   * \returns UanAddress in source field
   */
  UanAddress GetSrc (void) const;
  /**
   * \returns value of type field
   */
  uint8_t GetType (void) const;



  // Inherrited methods
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  virtual TypeId GetInstanceTypeId (void) const;
private:
  UanAddress m_dest;
  UanAddress m_src;
  uint8_t m_type;


};

}

#endif // UANHEADERCOMMON_H
