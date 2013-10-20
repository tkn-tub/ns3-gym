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

#ifndef UAN_MAC_RC_H
#define UAN_MAC_RC_H

#include "uan-mac.h"
#include "uan-address.h"

#include "ns3/nstime.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/traced-callback.h"
#include "ns3/event-id.h"
#include "ns3/random-variable-stream.h"

#include <list>
#include <utility>
#include <vector>


namespace ns3 {

class Address;
class UanTxMode;
class UanHeaderRcRts;
class UanHeaderRcCts;
class UanHeaderRcCtsGlobal;
class UanPhy;


/**
 * \class Reservation
 * \brief Stores reservation info for use in scheduling data channel by reservation channel MAC
 */
class Reservation
{
public:
  /**
   * \brief Create empty object with default values
   */
  Reservation ();
  /**
   * \brief Create Reservation object with given packet list, frame number and max packets
   * \param list List of packets for assigned to reservation
   * \param frameNo Frame number of reservation transmission
   * \param maxPkts Maximum number of packets to assign to reservation from packet list (0 = no maximum)
   */
  Reservation (std::list<std::pair <Ptr<Packet>, UanAddress > > &list, uint8_t frameNo, uint32_t maxPkts = 0);
  ~Reservation ();
  /**
   * \returns number of frames in reservation
   */
  uint32_t GetNoFrames () const;
  /**
   * \returns total byte length of reservation (sum of all packets with headers)
   */
  uint32_t GetLength () const;
  /**
   * \returns reference to list of packets in this reservation
   */
  const std::list<std::pair <Ptr<Packet>, UanAddress > > &GetPktList (void) const;
  /**
   * \returns Frame number of reservation
   */
  uint8_t GetFrameNo () const;
  /**
   * \returns Retry number of reservation
   */
  uint8_t GetRetryNo () const;
  /**
   * \returns Timestamp nth RTS send for this reservation
   * \param n Timestamp for retry number n to return
   */
  Time GetTimestamp (uint8_t n) const;
  /**
   * \returns True if reservation packets have been transmitted
   */
  bool IsTransmitted () const;
  /**
   * \param fn Frame number to set reservation to
   */
  void SetFrameNo (uint8_t fn);
  /**
   * \param t Time of latest RTS send
   */
  void AddTimestamp (Time t);
  /**
   * \brief Increments retry count by 1
   */
  void IncrementRetry ();
  /**
   * \param t True if resevation has been transmitted
   * \brief Sets reservation transmitted state
   */
  void SetTransmitted (bool t = true);
private:
  std::list<std::pair <Ptr<Packet>, UanAddress > > m_pktList;
  uint32_t m_length;
  uint8_t m_frameNo;
  std::vector<Time> m_timestamp;
  uint8_t m_retryNo;
  bool m_transmitted;

};


/**
 * \class UanMacRc
 * \brief Non-gateway node MAC for reservation channel MAC protocol
 *
 * This MAC protocol assumes a network topology where all traffic
 * is destined for a set of GW nodes which are connected via
 * some out of band (RF?) means.  This particular implementation
 * assumes that there is only a single gateway.
 *
 * For more information on class operation email
 * lentracy@u.washington.edu
 * (This work is, as of yet, unpublished)
 */
class UanMacRc : public UanMac
{
public:
  enum {
    TYPE_DATA, TYPE_GWPING, TYPE_RTS, TYPE_CTS, TYPE_ACK
  };
  UanMacRc ();
  virtual ~UanMacRc ();

  static TypeId GetTypeId (void);

  // Inherited virtual functions
  virtual Address GetAddress (void);
  virtual void SetAddress (UanAddress addr);
  virtual bool Enqueue (Ptr<Packet> pkt, const Address &dest, uint16_t protocolNumber);
  virtual void SetForwardUpCb (Callback<void, Ptr<Packet>, const UanAddress&> cb);
  virtual void AttachPhy (Ptr<UanPhy> phy);
  virtual Address GetBroadcast (void) const;
  virtual void Clear (void);

 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  int64_t AssignStreams (int64_t stream);

private:
  enum State {
    UNASSOCIATED, GWPSENT, IDLE, RTSSENT, DATATX
  };

  State m_state;
  bool m_rtsBlocked;

  EventId m_startAgain;
  UanAddress m_address;
  double m_retryRate;
  UanAddress m_assocAddr;
  Ptr<UanPhy> m_phy;
  uint32_t m_numRates;
  uint32_t m_currentRate;
  uint32_t m_maxFrames;
  uint32_t m_queueLimit;
  uint8_t m_frameNo;
  Time m_sifs;
  Time m_learnedProp;

  double m_minRetryRate;
  double m_retryStep;

  uint32_t m_ctsSizeN;
  uint32_t m_ctsSizeG;

  bool m_cleared;

  std::list<std::pair <Ptr<Packet>, UanAddress > > m_pktQueue;
  std::list<Reservation> m_resList;

  Callback<void, Ptr<Packet>, const UanAddress& > m_forwardUpCb;

  TracedCallback<Ptr<const Packet>, UanTxMode > m_rxLogger;
  TracedCallback<Ptr<const Packet>, uint16_t > m_enqueueLogger;
  TracedCallback<Ptr<const Packet>, uint16_t > m_dequeueLogger;

  EventId m_rtsEvent;
  void ReceiveOkFromPhy (Ptr<Packet>, double sinr, UanTxMode mode);

  void Associate (void);
  void AssociateTimeout (void);
  void SendRts (void);
  void RtsTimeout (void);
  UanHeaderRcRts CreateRtsHeader (const Reservation &res);
  void ScheduleData (const UanHeaderRcCts &ctsh, const UanHeaderRcCtsGlobal &ctsg, uint32_t ctsBytes);
  void ProcessAck (Ptr<Packet> ack);
  void SendPacket (Ptr<Packet> pkt, uint32_t rate);
  bool IsPhy1Ok (void);
  void BlockRtsing (void);

  static uint32_t m_cntrlSends;

  /// Provides exponential random variables.
  Ptr<ExponentialRandomVariable> m_ev;

protected:
  void DoDispose ();
};

}

#endif /* UAN_MAC_RC_H */
