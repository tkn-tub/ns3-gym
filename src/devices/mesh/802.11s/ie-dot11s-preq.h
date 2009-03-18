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


#ifndef WIFI_PREQ_INFORMATION_ELEMENT_H
#define WIFI_PREQ_INFORMATION_ELEMENT_H

#include <stdint.h>
#include <vector>

#include "ns3/node.h"
#include "ns3/buffer.h"
#include "ns3/mac48-address.h"
#include "ns3/header.h"
#include "ns3/dot11s-codes.h"
#include "ns3/wifi-information-element.h"

namespace ns3 {
/**
 * \ingroup mesh
 */
class DestinationAddressUnit : public RefCountBase
{
public:
  DestinationAddressUnit();
  void SetFlags(bool doFlag, bool rfflag);
  //void SetPerDestFlags(uint8_t per_dest_flags);
  void SetDestinationAddress(Mac48Address dest_address);
  void SetDestSeqNumber(uint32_t dest_seq_number);
  bool IsDo();
  bool IsRf();
  Mac48Address GetDestinationAddress() const;
  uint32_t GetDestSeqNumber() const;

private:
  bool m_do;
  bool m_rf;
  Mac48Address m_destinationAddress;
  uint32_t m_destSeqNumber;
};
/**
 * \ingroup mesh
 */
class IeDot11sPreq : public WifiInformationElement
{
public:
  IeDot11sPreq();
  ~IeDot11sPreq ();
  static TypeId GetTypeId ();
  virtual TypeId GetInstanceTypeId() const;
  virtual void Print(std::ostream &os) const;
  void AddDestinationAddressElement(
    bool doFlag,
    bool rfFlag,
    Mac48Address dest_address,
    uint32_t dest_seq_number
  );
  void DelDestinationAddressElement(Mac48Address dest_address);
  void ClearDestinationAddressElement();
  std::vector<Ptr<DestinationAddressUnit> > GetDestinationList ();
  //void SetFlags(uint8_t flags);
  void SetUnicastPreq();
  /*
   * \brief In proactive case: need we send PREP
   */
  void SetNeedNotPrep();
  void SetHopcount(uint8_t hopcount);
  void SetTTL(uint8_t ttl);
  void SetPreqID(uint32_t id);
  void SetOriginatorAddress(Mac48Address originator_address);
  void SetOriginatorSeqNumber(uint32_t originator_seq_number);
  void SetLifetime(uint32_t lifetime);
  void SetMetric(uint32_t metric);
  void SetDestCount(uint8_t dest_count);

  //uint8_t  GetFlags() const ;
  bool  IsUnicastPreq() const;
  bool  IsNeedNotPrep() const;
  uint8_t  GetHopCount() const;
  uint8_t  GetTtl()const ;
  uint32_t GetPreqID() const;
  Mac48Address GetOriginatorAddress() const;
  uint32_t GetOriginatorSeqNumber() const;
  uint32_t GetLifetime() const;
  uint32_t GetMetric() const;
  uint8_t  GetDestCount() const;
  void  DecrementTtl();
  void  IncrementMetric(uint32_t metric);
protected:
  WifiElementId ElementId () const{
    return IE11S_PREQ;
  }
  void SerializeInformation(Buffer::Iterator i) const;
  uint8_t DeserializeInformation(Buffer::Iterator i, uint8_t length);
  uint8_t GetInformationSize() const;
  void PrintInformation(std::ostream& os) const;
private:
  //how many destinations we support
  uint8_t m_maxSize; //TODO: make as an attrubute
  //Fields:
  uint8_t m_flags;
  uint8_t m_hopCount;
  uint8_t m_ttl;
  uint32_t m_preqId;
  Mac48Address m_originatorAddress;
  uint32_t m_originatorSeqNumber;
  uint32_t m_lifetime;
  uint32_t m_metric;
  uint8_t  m_destCount;
  std::vector<Ptr<DestinationAddressUnit> >
  m_destinations;
};

} //namespace ns3
#endif

