/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Author: Kirill Andreev <andreev@iitp.ru>
 */


#ifndef PERR_INFORMATION_ELEMENT_H
#define PERR_INFORMATION_ELEMENT_H

#include <stdint.h>
#include "ns3/node.h"
#include "ns3/buffer.h"
#include "ns3/mac48-address.h"
#include "ns3/hwmp-rtable.h"
#include "ns3/header.h"
#include "dot11s-codes.h"

namespace ns3 {
/**
 * \ingroup mesh
 */
class WifiPerrInformationElement : public Header
{
public:
  WifiPerrInformationElement();
  ~WifiPerrInformationElement();
  static  TypeId   GetTypeId();
  virtual TypeId   GetInstanceTypeId() const;
  virtual void     Print(std::ostream &os) const;
  virtual void     Serialize(Buffer::Iterator i) const;
  virtual uint32_t Deserialize(Buffer::Iterator start);
  virtual uint32_t GetSerializedSize() const;
#if 0
  //RESERVED in D2.07
  uint8_t   GetModeFlags();
  void   SetModeFlags(uint8_t flags);
#endif
  uint8_t   GetNumOfDest();

  void   AddAddressUnit(struct HwmpRtable::FailedDestination unit);
  std::vector<HwmpRtable::FailedDestination>
  GetAddressUnitVector();
  void   DeleteAddressUnit(Mac48Address address);
  void   ResetPerr();
private:
  static uint8_t ElementId() {
    return (uint8_t)IE11S_PERR;
  }
  uint8_t   m_numOfDest;
  std::vector<HwmpRtable::FailedDestination>
  m_addressUnits;
};

}
#endif
