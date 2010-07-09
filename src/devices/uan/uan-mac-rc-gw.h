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

#ifndef UANMACRCGW_H
#define UANMACRCGW_H

#include "uan-mac.h"
#include "uan-address.h"

#include "ns3/nstime.h"
#include "ns3/traced-callback.h"




#include <set>
#include <map>

namespace ns3 {

class UanTxMode;

/**
 * \class UanMacRcGw
 *
 * \brief Gateway side of RC-MAC
 *
 *
 *
 * This MAC protocol assumes a network topology where all traffic
 * is destined for a set of GW nodes which are connected via
 * some out of band (RF?) means.  UanMacRcGw is the protocol
 * which runs on the gateway nodes.
 *
 * This particular implementation
 * assumes that there is only a single gateway.
 *
 * For more information on class operation email
 * lentracy@u.washington.edu
 * (This work is, as of yet, unpublished)
 */

class UanMacRcGw : public UanMac
{
public:
  UanMacRcGw ();
  virtual ~UanMacRcGw ();

  static TypeId GetTypeId (void);

  // Inherited virtual functions
  virtual Address GetAddress (void);
  virtual void SetAddress (UanAddress addr);
  virtual bool Enqueue (Ptr<Packet> pkt, const Address &dest, uint16_t protocolNumber);
  virtual void SetForwardUpCb (Callback<void, Ptr<Packet>, const UanAddress&> cb);
  virtual void AttachPhy (Ptr<UanPhy> phy);
  virtual Address GetBroadcast (void) const;
  virtual void Clear (void);

private:
  enum State {
    IDLE, INCYCLE, CTSING
  };
  State m_state;

  struct Request
  {
    uint8_t numFrames;
    uint8_t frameNo;
    uint8_t retryNo;
    uint16_t length;
    Time rxTime;
  };

  struct AckData
  {
    uint8_t frameNo;
    std::set<uint8_t> rxFrames;
    uint8_t expFrames;
  };
  Callback<void, Ptr<Packet>, const UanAddress& > m_forwardUpCb;
  Ptr<UanPhy> m_phy;
  UanAddress m_address;
  Time m_maxDelta;
  Time m_sifs;
  uint32_t m_maxRes;
  uint32_t m_numRates;
  uint32_t m_rtsSize;
  uint32_t m_ctsSizeN;
  uint32_t m_ctsSizeG;
  uint32_t m_ackSize;
  double m_retryRate;
  uint16_t m_currentRetryRate;
  uint32_t m_currentRateNum;
  uint32_t m_numNodes;
  uint32_t m_totalRate;
  uint32_t m_rateStep;
  uint32_t m_frameSize;

  uint16_t m_numRetryRates;
  double m_minRetryRate;
  double m_retryStep;

  std::map<UanAddress, Time> m_propDelay;

  std::map<UanAddress, AckData> m_ackData;


  std::map<UanAddress, Request> m_requests;
  std::set<std::pair<Time, UanAddress> > m_sortedRes;

  bool m_cleared;

  TracedCallback<Ptr<const Packet>, UanTxMode > m_rxLogger;

  // Start time, min p-delay, reservations, frames, bytes, window size, ctl rate, retry rate
  TracedCallback<Time, Time, uint32_t, uint32_t, double, uint32_t, double> m_cycleLogger;

  void ReceivePacket (Ptr<Packet> pkt, double sinr, UanTxMode mode);
  void StartCycle (void);
  void EndCycle (void);
  void SendPacket (Ptr<Packet> pkt, uint32_t rate);
  void CycleStarted (void);
  void ReceiveError (Ptr<Packet> pkt, double sinr);

  // Stuff for computing exp throughput
  double ComputeAlpha (uint32_t totalFrames, uint32_t totalBytes, uint32_t n, uint32_t a, double deltaK);
  std::vector<double>  GetExpPdk (void);
  double ComputeExpS (uint32_t a, uint32_t ld, std::vector<double> exppdk);
  double ComputeExpS (uint32_t a, uint32_t ld);
  uint32_t CompExpMinIndex (uint32_t n, uint32_t k);
  double ComputePiK (uint32_t a, uint32_t n, uint32_t k);
  double ComputeExpBOverA (uint32_t n, uint32_t a, uint32_t ldlh, std::vector<double> deltaK);
  uint64_t NchooseK (uint32_t n, uint32_t k);
  uint32_t FindOptA (void);
protected:
  virtual void DoDispose ();

};

/**
 * \brief Defined for use in UanMacRcGw
 * \param a Address to compare
 * \param b Address to compare
 */
bool operator < (UanAddress &a, UanAddress &b);

}

#endif // UANMACRCGW_H
