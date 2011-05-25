/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007,2008 INRIA
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
 * Author: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 */

#ifndef SERVICE_FLOW_RECORD_H
#define SERVICE_FLOW_RECORD_H

#include <stdint.h>
#include "ns3/nstime.h"
#include "bs-uplink-scheduler.h"
#include "bs-net-device.h"
#include "ns3/ptr.h"

namespace ns3 {

/**
 * \ingroup wimax
 * \brief this class implements a structure to manage some parameters and statistics related to a service flow
 */
class ServiceFlowRecord
{
public:
  ServiceFlowRecord (void);
  ~ServiceFlowRecord (void);

  /**
   * \brief Set the grant size (only for UGS service flows)
   * \param grantSize the grant size to set
   */
  void SetGrantSize (uint32_t grantSize);
  /**
   * \return the grant size (only for ugs service flows)
   */
  uint32_t GetGrantSize (void) const;
  /**
   * \brief Set the grant time stamp. Used for data alocation for ugs flows, and unicast poll (bw request)
   * for non-UGS flows
   * \param grantTimeStamp the grant time stamp to set
   */
  void SetGrantTimeStamp (Time grantTimeStamp);
  /**
   * \return the grant time stamp. Used for data allocation for ugs flows, and unicast poll (bw request) for non-UGS flows
   */
  Time GetGrantTimeStamp (void) const;
  /**
   * \brief Set the DlTimeStamp.
   * \param dlTimeStamp: time when this service flow's traffic was last sent.
   */
  void SetDlTimeStamp (Time dlTimeStamp);
  /**
   * \return the DlTimeStamp: time when this service flow's traffic was last sent
   */
  Time GetDlTimeStamp (void) const;
  /**
   * \brief set the number of sent packets in this service flow
   * \param pktsSent the number of sent packets
   */
  void SetPktsSent (uint32_t pktsSent);
  /**
   * \brief update the number of sent packets by adding pktsSent
   * \param pktsSent the number of sent packets to add
   */
  void UpdatePktsSent (uint32_t pktsSent);
  /**
   * \return the number of sent packet in this service flow
   */
  uint32_t GetPktsSent (void) const;
  /**
   * \brief Set the number of received packet
   * \param pktsRcvd The number of received packets
   */
  void SetPktsRcvd (uint32_t pktsRcvd);
  /**
   * \brief update the number of received packets by adding pktsRcvd
   * \param pktsRcvd the number of received packets to add
   */
  void UpdatePktsRcvd (uint32_t pktsRcvd);
  /**
   * \return the number of received packet
   */
  uint32_t GetPktsRcvd (void) const;
  /**
   * \brief Set the number of sent bytes
   * \param bytesSent the number of sent bytes
   */
  void SetBytesSent (uint32_t bytesSent);
  /**
   * \brief update the number of sent bytes by adding bytesSent
   * \param bytesSent the number of bytes to add
   */
  void UpdateBytesSent (uint32_t bytesSent);
  /**
   * \return The number of sent bytes
   */
  uint32_t GetBytesSent (void) const;
  /**
   * \brief Set the number of received bytes
   * \param bytesRcvd the number of received bytes
   */
  void SetBytesRcvd (uint32_t bytesRcvd);
  /**
    * \brief update the number of received bytes by adding bytesRcvd
    * \param bytesRcvd the number of bytes to add
    */
  void UpdateBytesRcvd (uint32_t bytesRcvd);
  /**
    * \return The number of received bytes
    */
  uint32_t GetBytesRcvd (void) const;

  void SetRequestedBandwidth (uint32_t requestedBandwidth);
  void UpdateRequestedBandwidth (uint32_t requestedBandwidth);
  uint32_t GetRequestedBandwidth (void);

  void SetGrantedBandwidth (uint32_t grantedBandwidth);
  void UpdateGrantedBandwidth (uint32_t grantedBandwidth);
  uint32_t GetGrantedBandwidth (void);

  void SetGrantedBandwidthTemp (uint32_t grantedBandwidthTemp);
  void UpdateGrantedBandwidthTemp (uint32_t grantedBandwidthTemp);
  uint32_t GetGrantedBandwidthTemp (void);

  void SetBwSinceLastExpiry (uint32_t bwSinceLastExpiry);
  void UpdateBwSinceLastExpiry (uint32_t bwSinceLastExpiry);
  uint32_t GetBwSinceLastExpiry (void);

  void SetLastGrantTime (Time grantTime);
  Time GetLastGrantTime (void) const;

  void SetBacklogged (uint32_t backlogged);
  void IncreaseBacklogged (uint32_t backlogged);
  uint32_t GetBacklogged (void) const;

  void SetBackloggedTemp (uint32_t backloggedTemp);
  void IncreaseBackloggedTemp (uint32_t backloggedTemp);
  uint32_t GetBackloggedTemp (void) const;

private:
  uint32_t m_grantSize; // only used for UGS flow
  Time m_grantTimeStamp; // allocation (for data) for UGS flows and unicast poll (for bandwidth requests) for non-UGS flows
  Time m_dlTimeStamp; // time when this service flow's traffic was last sent

  // stats members
  uint32_t m_pktsSent;
  uint32_t m_pktsRcvd;

  uint32_t m_bytesSent;
  uint32_t m_bytesRcvd;

  uint32_t m_requestedBandwidth;
  uint32_t m_grantedBandwidth;
  uint32_t m_grantedBandwidthTemp; // Temporary variable used to sort list. Necessary to keep original order

  /* bandwidth granted since last expiry of minimum reserved traffic rate interval,
   /only for nrtPS, to make sure minimum reserved traffic rate is maintained */
  uint32_t m_bwSinceLastExpiry;
  Time m_lastGrantTime;
  int32_t m_backlogged;
  int32_t m_backloggedTemp;

};

} // namespace ns3

#endif /* SERVICE_FLOW_RECORD_H */
