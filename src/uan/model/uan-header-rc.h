/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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


#ifndef UAN_HEADER_RC_H
#define UAN_HEADER_RC_H

#include "ns3/header.h"
#include "ns3/nstime.h"
#include "ns3/uan-address.h"

#include <set>

namespace ns3 {

/**
 * \class UanHeaderRcData
 *
 * \brief Extra data header information
 *
 * Adds prop. delay measure, and frame number info to
 * transmitted data packet
 */
class UanHeaderRcData : public Header
{
public:
  UanHeaderRcData ();
  /**
   * \param frameNum Data frame # of reservation being transmitted
   * \param propDelay  Measured propagation delay found in handshaking
   * \note Prop. delay is transmitted with 16 bits and ms accuracy
   */
  UanHeaderRcData (uint8_t frameNum, Time propDelay);
  virtual ~UanHeaderRcData ();

  static TypeId GetTypeId (void);

  /**
   * \param frameNum Data frame # of reservation being transmitted
   */
  void SetFrameNo (uint8_t frameNum);
  /**
   * \param propDelay Measured propagation delay found in handshaking
   * \note Prop. delay is transmitted with 16 bits and ms accuracy
   */
  void SetPropDelay (Time propDelay);
  /**
   * \returns Data frame # of reservation being transmitted
   */
  uint8_t GetFrameNo (void) const;
  /**
   * \returns Measured propagation delay found in handshaking
   * \note Prop. delay is transmitted with 16 bits and ms accuracy
   */
  Time GetPropDelay (void) const;

  // Inherrited methods
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  virtual TypeId GetInstanceTypeId (void) const;

private:
  uint8_t m_frameNo;
  Time m_propDelay;
};

/**
 * \class UanHeaderRcRts
 *
 * \brief RTS header
 *
 * Contains frame #, retry #, # frames, length, and timestamp
 */
class UanHeaderRcRts : public Header
{
public:
  UanHeaderRcRts ();
  /**
   * \param frameNo Reservation frame #
   * \param retryNo Retry # of RTS packet
   * \param noFrames # of data frames in reservation
   * \param length # of bytes (including headers) in data
   * \param ts RTS TX timestamp
   * \note Timestamp is serialized into 32 bits with ms accuracy
   */
  UanHeaderRcRts (uint8_t frameNo, uint8_t retryNo, uint8_t noFrames, uint16_t length, Time ts);
  virtual ~UanHeaderRcRts ();

  static TypeId GetTypeId (void);

  /**
   * \param fno TX frame #
   */
  void SetFrameNo (uint8_t fno);
  /**
   * \param no Number of data frames included in this reservation request
   */
  void SetNoFrames (uint8_t no);
  /**
   * \param timeStamp RTS transmission time
   */
  void SetTimeStamp (Time timeStamp);
  /**
   * \param length Total number of data bytes in reservation (including headers)
   * \note Timestamp is serialized with 32 bits in ms precision
   */
  void SetLength (uint16_t length);
  /**
   * \param no Retry number of this RTS (Used to match timestamp to correctly received RTS)
   */
  void SetRetryNo (uint8_t no);

  /**
   * \returns Frame #
   */
  uint8_t GetFrameNo (void) const;
  /**
   * \returns # of data frames in reservation
   */
  uint8_t GetNoFrames (void) const;
  /**
   * \returns TX time of the RTS packet
   * \note Timestamp is serialized with 32 bits in ms precision
   */
  Time GetTimeStamp (void) const;
  /**
   * \returns Total # of bytes in data packets for reservation (including headers)
   */
  uint16_t GetLength (void) const;
  /**
   * \returns Retry number of this RTS packet
   */
  uint8_t GetRetryNo (void) const;

  // Inherrited methods
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  virtual TypeId GetInstanceTypeId (void) const;

private:
  uint8_t m_frameNo;
  uint8_t m_noFrames;
  uint16_t m_length;
  Time m_timeStamp;
  uint8_t m_retryNo;
};

/**
 * \class UanHeaderRcCtsGlobal
 *
 * \brief Cycle broadcast information for
 */


class UanHeaderRcCtsGlobal : public Header
{
public:
  /**
   * \brief Create UanHeaderRcCtsGlobal with fields zeroed out
   */
  UanHeaderRcCtsGlobal ();
  /**
   * \brief Create object with given window time, time stamp, rate, and retry rate.
   * \param wt Window time
   * \param ts Timestamp
   * \param rate Rate number
   * \param retryRate  Retry rate value
   */
  UanHeaderRcCtsGlobal (Time wt, Time ts, uint16_t rate, uint16_t retryRate);
  ~UanHeaderRcCtsGlobal ();

  static TypeId GetTypeId (void);

  /**
   * \param rate Rate number corresponding to data rate of current cycle
   */
  void SetRateNum (uint16_t rate);
  /**
   * \param rate Retry rate number for current cycle
   */
  void SetRetryRate (uint16_t rate);
  /**
   * \param t Time duration following blocking time to allow RTS transmissions
   */
  void SetWindowTime (Time t);

  /**
   * \param timeStamp Time of CTS transmission
   */
  void SetTxTimeStamp (Time timeStamp);

  /**
   * \returns Rate # corresponding to data rate of current cycle
   */
  uint16_t GetRateNum (void) const;
  /**
   * \returns retry rate # of retry rate for current cycle
   */
  uint16_t GetRetryRate (void) const;
  /**
   * \returns Time duration after blocking time allowed for RTS transmissions
   */
  Time GetWindowTime (void) const;
  /**
   * \returns Timestamp of CTS transmission
   */
  Time GetTxTimeStamp (void) const;

  // Inherrited methods
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  virtual TypeId GetInstanceTypeId (void) const;

private:
  Time m_timeStampTx;
  Time m_winTime;
  uint16_t m_retryRate;
  uint16_t m_rateNum;
};
/**
 * \class UanHeaderRcCts
 *
 * \brief CTS header
 *
 * Includes RTS RX time, CTS TX time, delay until TX, RTS blocking period,
 * RTS tx period, rate #, and retry rate #
 */

class UanHeaderRcCts : public Header
{
public:
  UanHeaderRcCts ();
  /**
   * \param frameNo Resrvation frame # being cleared
   * \param retryNo Retry # of received RTS packet
   * \param rtsTs RX time of RTS packet at gateway
   * \param delay Delay until transmission
   * \param addr Destination of CTS packet
   * \note Times are serialized, with ms precission, into 32 bit fields.
   */
  UanHeaderRcCts (uint8_t frameNo, uint8_t retryNo, Time rtsTs, Time delay, UanAddress addr);
  virtual ~UanHeaderRcCts ();

  static TypeId GetTypeId (void);

  /**
   * \param frameNo Frame # of RTS being cleared
   */
  void SetFrameNo (uint8_t frameNo);
  /**
   * \param timeStamp Time of RTS reception
   */
  void SetRtsTimeStamp (Time timeStamp);
  /**
   * \param delay Time duration, from CTS TX, before first data frame arrival
   */
  void SetDelayToTx (Time delay);
  /**
   * \param no Retry number of RTS frame being cleared
   */
  void SetRetryNo (uint8_t no);
  /**
   * \param addr Destination for scheduling info
   */
  void SetAddress (UanAddress addr);

  /**
   * \returns Frame # of RTS being cleared
   */
  uint8_t GetFrameNo (void) const;
  /**
   * \returns RX time of RTS being cleared
   */
  Time GetRtsTimeStamp (void) const;
  /**
   * \returns Delay from TX time of CTS packet until arrival of first data frame
   */
  Time GetDelayToTx (void) const;
  /**
   * \returns Retry # of RTS packet being cleared
   */
  uint8_t GetRetryNo (void) const;
  /**
   * \returns Destination for scheduling info
   */
  UanAddress GetAddress (void) const;

  // Inherrited methods
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  virtual TypeId GetInstanceTypeId (void) const;

private:
  uint8_t m_frameNo;
  Time m_timeStampRts;
  uint8_t m_retryNo;
  Time m_delay;
  UanAddress m_address;
};

/**
 * \class UanHeaderRcAck
 * \brief Header used for ACK packets by protocol ns3::UanMacRc
 */
class UanHeaderRcAck : public Header
{
public:
  UanHeaderRcAck ();
  virtual ~UanHeaderRcAck ();

  static TypeId GetTypeId (void);

  /**
   * \param frameNo Frame # of reservation being acknowledged
   */
  void SetFrameNo (uint8_t frameNo);
  /**
   * \param frame Data frame # being nacked
   */
  void AddNackedFrame (uint8_t frame);

  /**
   * \returns Set of nacked frames
   */
  const std::set<uint8_t> &GetNackedFrames (void) const;
  /**
   * \returns Reservation frame # being acknowledged.
   */
  uint8_t GetFrameNo (void) const;
  /**
   * \returns Number of data frames being NACKED
   */
  uint8_t GetNoNacks (void) const;

  // Inherrited methods
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  virtual TypeId GetInstanceTypeId (void) const;

private:
  uint8_t m_frameNo;
  std::set<uint8_t> m_nackedFrames;

};

}

#endif /* UAN_HEADER_RC_H */
