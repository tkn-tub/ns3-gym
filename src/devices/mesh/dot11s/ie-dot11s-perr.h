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
#include "ns3/wifi-information-element.h"

namespace ns3 {
namespace dot11s {
/**
 * \ingroup dot11s
 * \brief See 7.3.2.98 of 802.11s draft 2.07
 */
class IePerr : public WifiInformationElement
{
public:
  IePerr ();
  ~IePerr ();
  struct FailedDestination
  {
    Mac48Address destination;
    uint32_t seqnum;
  };
  uint8_t   GetNumOfDest ();

  void AddAddressUnit (struct FailedDestination unit);
  std::vector<FailedDestination> GetAddressUnitVector () const;
  void DeleteAddressUnit (Mac48Address address);
  void Merge(const IePerr perr);
  void ResetPerr ();
private:
  WifiElementId ElementId () const{
    return IE11S_PERR;
  };
  void  SerializeInformation (Buffer::Iterator i) const;
  uint8_t  DeserializeInformation (Buffer::Iterator start, uint8_t length);
  void PrintInformation (std::ostream& os) const;
  uint8_t  GetInformationSize () const;
private:
  uint8_t   m_numOfDest;
  std::vector<FailedDestination> m_addressUnits;
  friend bool operator== (const IePerr & a, const IePerr & b);
};
  bool operator== (const IePerr & a, const IePerr & b);
} // namespace dot11s
} //namespace ns3
#endif
