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

#include "ns3/mac48-address.h"
#include "ns3/hwmp-rtable.h"
#include "ns3/dot11s-codes.h"
#include "ns3/wifi-information-element.h"

namespace ns3 {
/**
 * \ingroup mesh
 */
class IeDot11sPerr : public WifiInformationElement
{
public:
  IeDot11sPerr();
  ~IeDot11sPerr();
  static  TypeId   GetTypeId();
  virtual TypeId   GetInstanceTypeId() const;
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
protected:
  WifiElementId ElementId() const{
    return IE11S_PERR;
  };
  void  SerializeInformation(Buffer::Iterator i) const;
  uint8_t  DeserializeInformation(Buffer::Iterator start, uint8_t length);
  void PrintInformation(std::ostream& os) const;
  uint8_t  GetInformationSize() const;
private:
  uint8_t   m_numOfDest;
  std::vector<HwmpRtable::FailedDestination>
  m_addressUnits;
};

}
#endif
