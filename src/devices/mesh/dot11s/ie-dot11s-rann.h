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


#ifndef RANN_INFORMATION_ELEMENT_H
#define RANN_INFORMATION_ELEMENT_H

#include "ns3/mac48-address.h"
#include "ns3/dot11s-codes.h"
#include "ns3/wifi-information-element.h"

namespace ns3 {
namespace dot11s {
/**
 * \ingroup dot11s
 * \brief See 7.3.2.95 of 802.11s draft 2.07
 */
class IeRann
{
public:
  IeRann ();
  virtual ~IeRann ();
  static TypeId GetTypeId ();
  virtual TypeId GetInstanceTypeId () const;
  void SetFlags (uint8_t flags);
  void SetHopcount (uint8_t hopcount);
  void SetTTL (uint8_t ttl);
  void SetOriginatorAddress (Mac48Address originator_address);
  void SetDestSeqNumber (uint32_t dest_seq_number);
  void SetMetric (uint32_t metric);
  uint8_t GetFlags ();
  uint8_t GetHopcount ();
  uint8_t GetTTL ();
  Mac48Address GetOriginatorAddress ();
  uint32_t GetDestSeqNumber ();
  uint32_t GetMetric ();
  void DecrementTtl ();
  void IncrementMetric (uint32_t metric);
private:
  WifiElementId ElementId () const{
    return IE11S_RANN;
  }
  void SerializeInformation (Buffer::Iterator i) const;
  uint8_t DeserializeInformation (Buffer::Iterator start, uint8_t length);
  uint8_t GetInformationSize () const;
  uint8_t m_flags;
  uint8_t m_hopcount;
  uint8_t m_ttl;
  Mac48Address m_originatorAddress;
  uint32_t m_destSeqNumber;
  uint32_t m_metric;
};
  
} // namespace dot11s
} //namespace ns3
#endif
