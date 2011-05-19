/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 */
#ifndef QOS_TAG_H
#define QOS_TAG_H

#include "ns3/packet.h"

namespace ns3 {

class Tag;


/**
 * As per IEEE Std. 802.11-2007, Section 6.1.1.1.1, when EDCA is used the
 * the Traffic ID (TID) value corresponds to one of the User Priority (UP)
 * values defined by the IEEE Std. 802.1D-2004, Annex G, table G-2.
 *
 * Note that this correspondence does not hold for HCCA, since in that
 * case the mapping between UPs and TIDs should be determined by a
 * TSPEC element as per IEEE Std. 802.11-2007, Section 7.3.2.30
 */
enum UserPriority
{
  UP_BK = 1, /**< background  */
  UP_BE = 0, /**< best effort (default) */
  UP_EE = 3, /**< excellent effort  */
  UP_CL = 4, /**< controlled load */
  UP_VI = 5, /**< video, < 100ms latency and jitter */
  UP_VO = 6, /**< voice, < 10ms latency and jitter */
  UP_NC = 7  /**< network control */
};



/**
 * \ingroup wifi
 *
 * The aim of the QosTag is to provide means for an Application to
 * specify the TID which will be used by a QoS-aware WifiMac for a
 * given traffic flow. Note that the current QosTag class was
 * designed to be completely mac/wifi specific without any attempt
 * at being generic.
 */
class QosTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Create a QosTag with the default TID = 0 (best effort traffic)
   */
  QosTag ();

  /**
   * Create a QosTag with the given TID
   */
  QosTag (uint8_t tid);

  /**
   * Set the TID to the given value. This assumes that the
   * application is aware of the QoS support provided by the MAC
   * layer, and is therefore able to set the correct TID.
   *
   * @param tid the value of the TID to set
   */
  void SetTid (uint8_t tid);

  /**
   * Set the TID to the value that corresponds to the requested
   * UserPriority. Note that, since the mapping of UserPriority to TID
   * is defined for EDCA only, you should call this method only when
   * EDCA is used. When using HDCA, QosTag(uint8_t tid) should be used
   * instead.
   *
   * @param up the requested UserPriority
   *
   */
  void SetUserPriority (UserPriority up);

  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual uint32_t GetSerializedSize () const;
  virtual void Print (std::ostream &os) const;

  uint8_t GetTid (void) const;

private:
  uint8_t m_tid;
};

} //namespace ns3

#endif /* QOS_TAG_H */
