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

#include "uan-mac-rc-gw.h"
#include "uan-mac-rc.h"
#include "uan-header-common.h"
#include "uan-header-rc.h"
#include "uan-phy.h"
#include "uan-tx-mode.h"

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/nstime.h"
#include "ns3/double.h"
#include "ns3/uinteger.h"

#include <utility>
#include <set>
#include <map>
#include <vector>
#include <algorithm>

NS_LOG_COMPONENT_DEFINE ("UanMacRcGw");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (UanMacRcGw);

bool
operator < (UanAddress &a, UanAddress &b)
{
  return a.GetAsInt () < b.GetAsInt ();
}


UanMacRcGw::UanMacRcGw ()
  : UanMac (),
    m_state (IDLE),
    m_currentRateNum (0),
    m_cleared (false)
{
  UanHeaderCommon ch;
  UanHeaderRcRts rts;
  UanHeaderRcCts cts;
  UanHeaderRcAck ack;
  UanHeaderRcCtsGlobal ctsg;

  m_rtsSize = ch.GetSerializedSize () + rts.GetSerializedSize ();
  m_ctsSizeN = cts.GetSerializedSize ();
  m_ctsSizeG = ch.GetSerializedSize () + ctsg.GetSerializedSize ();
  m_ackSize = ch.GetSerializedSize () + ack.GetSerializedSize ();

  NS_LOG_DEBUG ("Gateway initialized");
}

UanMacRcGw::~UanMacRcGw ()
{
}

void
UanMacRcGw::Clear ()
{
  if (m_cleared)
    {
      return;
    }
  m_cleared = true;
  if (m_phy)
    {
      m_phy->Clear ();
      m_phy = 0;
    }
  m_propDelay.clear ();
  std::map<UanAddress, AckData>::iterator it = m_ackData.begin ();
  for (; it != m_ackData.end (); it++)
    {
      it->second.rxFrames.clear ();
    }
  m_ackData.clear ();
  m_requests.clear ();
  m_sortedRes.clear ();
}

void
UanMacRcGw::DoDispose ()
{
  Clear ();
  UanMac::DoDispose ();
}
TypeId
UanMacRcGw::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanMacRcGw")
    .SetParent<UanMac> ()
    .AddConstructor<UanMacRcGw> ()
    .AddAttribute ("MaxReservations",
                   "Maximum number of reservations to accept per cycle",
                   UintegerValue (10),
                   MakeUintegerAccessor (&UanMacRcGw::m_maxRes),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("NumberOfRates",
                   "Number of rates per Phy layer",
                   UintegerValue (1023),
                   MakeUintegerAccessor (&UanMacRcGw::m_numRates),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("RetryRate",
                   "Number of retry rates per second at non-gateway nodes",
                   DoubleValue (1 / 10.0),
                   MakeDoubleAccessor (&UanMacRcGw::m_retryRate),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("MaxPropDelay",
                   "Maximum propagation delay between gateway and non-gateway nodes",
                   TimeValue (Seconds (2)),
                   MakeTimeAccessor (&UanMacRcGw::m_maxDelta),
                   MakeTimeChecker ())
    .AddAttribute ("SIFS",
                   "Spacing between frames to account for timing error and processing delay",
                   TimeValue (Seconds (0.2)),
                   MakeTimeAccessor (&UanMacRcGw::m_sifs),
                   MakeTimeChecker ())
    .AddAttribute ("NumberOfNodes",
                   "Number of non-gateway nodes in this gateway's neighborhood",
                   UintegerValue (10),
                   MakeUintegerAccessor (&UanMacRcGw::m_numNodes),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("MinRetryRate",
                   "Smallest allowed RTS retry rate",
                   DoubleValue (0.01),
                   MakeDoubleAccessor (&UanMacRcGw::m_minRetryRate),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RetryStep",
                   "Retry rate increment",
                   DoubleValue (0.01),
                   MakeDoubleAccessor (&UanMacRcGw::m_retryStep),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("NumberOfRetryRates",
                   "Number of retry rates",
                   UintegerValue (100),
                   MakeUintegerAccessor (&UanMacRcGw::m_numRetryRates),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("TotalRate",
                   "Total available channel rate in bps (for a single channel, without splitting reservation channel)",
                   UintegerValue (4096),
                   MakeUintegerAccessor (&UanMacRcGw::m_totalRate),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("RateStep",
                   "Increments available for rate assignment in bps",
                   UintegerValue (4),
                   MakeUintegerAccessor (&UanMacRcGw::m_rateStep),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("FrameSize",
                   "Size of data frames in bytes",
                   UintegerValue (1000),
                   MakeUintegerAccessor (&UanMacRcGw::m_frameSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddTraceSource ("RX",
                     "A packet was destined for and received at this MAC layer",
                     MakeTraceSourceAccessor (&UanMacRcGw::m_rxLogger))
    .AddTraceSource ("Cycle",
                     "Trace cycle statistics",
                     MakeTraceSourceAccessor (&UanMacRcGw::m_cycleLogger))

  ;

  return tid;
}

Address
UanMacRcGw::GetAddress (void)
{
  return m_address;
}

void
UanMacRcGw::SetAddress (UanAddress addr)
{
  m_address = addr;
}

bool
UanMacRcGw::Enqueue (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{
  NS_LOG_WARN ("RCMAC Gateway transmission to acoustic nodes is not yet implemented");
  return false;
}

void
UanMacRcGw::SetForwardUpCb (Callback<void, Ptr<Packet>, const UanAddress&> cb)
{
  m_forwardUpCb = cb;
}

void
UanMacRcGw::AttachPhy (Ptr<UanPhy> phy)
{
  m_phy = phy;
  phy->SetReceiveOkCallback (MakeCallback (&UanMacRcGw::ReceivePacket, this));
  phy->SetReceiveErrorCallback (MakeCallback (&UanMacRcGw::ReceiveError, this));
}

void
UanMacRcGw::ReceiveError (Ptr<Packet> pkt, double sinr)
{
}

Address
UanMacRcGw::GetBroadcast (void) const
{
  return UanAddress::GetBroadcast ();
}

void
UanMacRcGw::ReceivePacket (Ptr<Packet> pkt, double sinr, UanTxMode mode)
{
  UanHeaderCommon ch;
  pkt->PeekHeader (ch);

  if (ch.GetDest () == m_address || ch.GetDest () == UanAddress::GetBroadcast ())
    {
      m_rxLogger (pkt, mode);
    }
  else
    {
      return;
    }

  pkt->RemoveHeader (ch);

  switch (ch.GetType ())
    {
    case UanMacRc::TYPE_DATA:
      {
        UanHeaderRcData dh;
        pkt->RemoveHeader (dh);
        m_propDelay[ch.GetSrc ()] = dh.GetPropDelay ();
        if (m_ackData.find (ch.GetSrc ()) == m_ackData.end ())
          {
            NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " GATEWAY Received unexpected data packet");
          }
        else
          {
            NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " GW Received data packet from " << ch.GetSrc () << " length = " << pkt->GetSize ());
            m_ackData[ch.GetSrc ()].rxFrames.insert (dh.GetFrameNo ());
          }
        m_forwardUpCb (pkt, ch.GetSrc ());
      }
      break;
    case UanMacRc::TYPE_GWPING:
    case UanMacRc::TYPE_RTS:
      if (m_state == CTSING)
        {
          return;
        }

      {
        UanHeaderRcRts rh;
        pkt->RemoveHeader (rh);

        if (m_requests.find (ch.GetSrc ()) == m_requests.end ())
          {
            Request req;
            req.numFrames = rh.GetNoFrames ();
            req.rxTime = Simulator::Now ();
            req.frameNo = rh.GetFrameNo ();
            req.retryNo = rh.GetRetryNo ();
            req.length = rh.GetLength ();
            NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " GW storing reservation from " << ch.GetSrc () << " with length " << req.length);
            m_requests.insert (std::make_pair (ch.GetSrc (), req));
            std::map<UanAddress, Time>::iterator it = m_propDelay.find (ch.GetSrc ());
            if (it == m_propDelay.end ())
              {
                m_sortedRes.insert (std::make_pair (m_maxDelta, ch.GetSrc ()));
              }
            else
              {
                m_sortedRes.insert (std::make_pair ( (*it).second, ch.GetSrc ()));
              }
          }
      }
      if (m_state == IDLE)
        {
          StartCycle ();
        }
      break;
    case UanMacRc::TYPE_CTS:
      NS_FATAL_ERROR ("Received CTS at GW.  Currently only support single GW network!");
      break;
    case UanMacRc::TYPE_ACK:
      NS_FATAL_ERROR ("Received ACK at GW.  Currently only support single GW network!");
      break;
    default:
      NS_FATAL_ERROR ("Received unknown packet at GW!");
    }
}

void
UanMacRcGw::StartCycle (void)
{
  uint32_t numRts = m_sortedRes.size ();

  if (numRts)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Simulator starting non-empty cycle");
    }
  else
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Simulator starting EMPTY cycle");
    }

  // Calculate dataRate
  uint32_t totalBytes = 0;
  uint32_t totalFrames = 0;
  double pDelay = 0;
  if (numRts > 0)
    {
      std::map<UanAddress, Request>::iterator rit = m_requests.begin ();
      for (; rit != m_requests.end (); rit++)
        {
          totalBytes += (*rit).second.length;
          totalFrames += (*rit).second.numFrames;
        }
      pDelay = 2 * m_sortedRes.begin ()->first.GetSeconds ();
    }


  double minRate = m_phy->GetMode (m_numRates).GetDataRateBps ();

  uint32_t optA = m_maxRes;
  if (m_maxRes == 0)
    {
      optA = FindOptA ();
    }
  double thAlpha = ComputeAlpha (totalFrames, totalBytes, m_numNodes, optA, pDelay / 2.0);

  double thCtlRate = m_totalRate * thAlpha;

  double temprate = (thCtlRate - minRate) / ((double) m_rateStep) + 0.5;
  m_currentRateNum =  (uint32_t) temprate;
  if (m_currentRateNum >= m_numRates)
    {
      m_currentRateNum = m_numRates - 1;
    }

  NS_LOG_DEBUG ("Found theoretical alpha: " << thAlpha << " Found associated rate = " << thCtlRate << " Giving rate number: " << temprate);
  double thX = thAlpha * m_totalRate / (2.0 * m_numNodes * m_rtsSize * 8.0);

  double dataRate = m_phy->GetMode (m_currentRateNum).GetDataRateBps ();


  if (thX < m_minRetryRate)
    {
      NS_LOG_WARN ("Gateway found optimum RTS retry rate is below minimum");
      m_currentRetryRate = 0;
    }
  else
    {
      m_currentRetryRate = (uint16_t)((thX - m_minRetryRate) / m_retryStep + 0.5);
    }

  double actualX = m_currentRetryRate * m_retryStep + m_minRetryRate;

  uint32_t ctlRate =  m_phy->GetMode (m_currentRateNum + m_numRates).GetDataRateBps ();


  double winSize = (double)(totalBytes) * 8.0 / dataRate + m_sifs.GetSeconds () * totalFrames + pDelay;
  if (numRts == 0)
    {
      winSize = (optA * std::exp (1.0) + 0.5) * 2.0 * 8.0 * m_rtsSize / (thAlpha * m_totalRate) + 2 * m_maxDelta.GetSeconds ();
    }
  double effWinSize = winSize - m_rtsSize * 8 / ctlRate  - 2 * m_maxDelta.GetSeconds ();


  // Before fast CTS/ACK(below)
  double cycleSeconds = winSize + (totalFrames + 1.0) * m_sifs.GetSeconds () + m_ctsSizeG * 8.0 / dataRate + (m_ctsSizeN + m_ackSize) * 8.0 * numRts / dataRate;

  Time ctsTxTimeG = Seconds (m_ctsSizeG * 8.0 / dataRate);
  Time ctsTxTimeTotal = Seconds (m_ctsSizeN * 8.0 * numRts / dataRate) + ctsTxTimeG;
  if (numRts == 0)
    {
      UanHeaderRcCtsGlobal ctsg;
      ctsg.SetWindowTime (Seconds (effWinSize));
      ctsg.SetRateNum (m_currentRateNum);
      ctsg.SetRetryRate (m_currentRetryRate);
      ctsg.SetTxTimeStamp (Simulator::Now ());

      UanHeaderCommon ch (m_address, UanAddress::GetBroadcast (), UanMacRc::TYPE_CTS);
      Ptr<Packet> p = Create<Packet> ();
      p->AddHeader (ctsg);
      p->AddHeader (ch);
      SendPacket (p, m_currentRateNum);


      Simulator::Schedule (Seconds (cycleSeconds), &UanMacRcGw::StartCycle, this);
      m_state = INCYCLE;
      m_cycleLogger (Simulator::Now (), Seconds (0), numRts, totalBytes, effWinSize, ctlRate, actualX);
      return;
    }

  Time nextEarliest = ctsTxTimeTotal + m_sifs;

  m_state = CTSING;
  Simulator::Schedule (nextEarliest, &UanMacRcGw::CycleStarted, this);

  std::set<std::pair<Time, UanAddress> >::iterator it = m_sortedRes.begin ();
  Time minPdelay = (*it).first;
  Ptr<Packet> cts = Create<Packet> ();

  for (; it != m_sortedRes.end (); it++)
    {
      Request req = m_requests[(*it).second];
      Time pdelay = (*it).first;

      AckData newData;
      newData.expFrames = req.numFrames;
      newData.frameNo = req.frameNo;
      UanAddress dest = (*it).second;
      m_ackData.insert (std::make_pair (dest, newData));

      Time earliestArr = ctsTxTimeTotal + pdelay + pdelay + m_sifs;
      Time arrivalTime = std::max (earliestArr, nextEarliest);
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " GW: Scheduling request for prop. delay " << pdelay.GetSeconds () << " for " << (*it).second << " Earliest possible arrival=" << earliestArr.GetSeconds () << "  Next arrival time=" << nextEarliest.GetSeconds ());
      nextEarliest = arrivalTime + Seconds (req.length * 8.0 / dataRate) + Seconds (m_sifs.GetSeconds () * req.numFrames);

      UanHeaderRcCts ctsh;
      ctsh.SetAddress (dest);
      ctsh.SetRtsTimeStamp (req.rxTime);
      ctsh.SetFrameNo (req.frameNo);
      ctsh.SetRetryNo (req.retryNo);
      ctsh.SetDelayToTx (arrivalTime);
      cts->AddHeader (ctsh);

      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " GW Scheduling reception for " << (uint32_t) req.numFrames << " frames at " << (Simulator::Now () + arrivalTime).GetSeconds () << "  (delaytiltx of " << arrivalTime.GetSeconds () << ")  Total length is " << req.length << " with txtime " << req.length * 8 / dataRate << " seconds");
    }

  UanHeaderRcCtsGlobal ctsg;
  ctsg.SetRateNum (m_currentRateNum);
  ctsg.SetRetryRate (m_currentRetryRate);
  ctsg.SetWindowTime (Seconds (effWinSize));
  ctsg.SetTxTimeStamp (Simulator::Now ());
  UanHeaderCommon ch;
  ch.SetDest (UanAddress::GetBroadcast ());
  ch.SetSrc (m_address);
  ch.SetType (UanMacRc::TYPE_CTS);
  cts->AddHeader (ctsg);
  cts->AddHeader (ch);
  SendPacket (cts, m_currentRateNum);

  m_requests.clear ();
  m_sortedRes.clear ();
  Simulator::Schedule (nextEarliest, &UanMacRcGw::EndCycle, this);


  m_cycleLogger (Simulator::Now (), minPdelay, numRts, totalBytes, cycleSeconds, ctlRate, actualX);
}

void
UanMacRcGw::CycleStarted ()
{
  m_state = INCYCLE;
}
void
UanMacRcGw::EndCycle ()
{

  NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " GW Ending cycle");

  Time nextAck = Seconds (0);

  Time ackTime = Seconds (m_ackSize * 8.0 / m_phy->GetMode (m_currentRateNum).GetDataRateBps ());

  std::map<UanAddress, AckData>::iterator it = m_ackData.begin ();
  for (; it != m_ackData.end (); it++)
    {
      UanAddress dest = (*it).first;
      AckData &data = (*it).second;

      std::list<uint32_t> toNack;
      for (uint32_t i = 0; i < data.expFrames; i++)
        {
          if (data.rxFrames.find (i) == data.rxFrames.end ())
            {
              toNack.push_back (i);
            }
        }
      UanHeaderCommon ch;
      ch.SetDest (dest);
      ch.SetSrc (m_address);
      ch.SetType (UanMacRc::TYPE_ACK);
      UanHeaderRcAck ah;
      ah.SetFrameNo (data.frameNo);
      std::list<uint32_t>::iterator nit = toNack.begin ();
      for (; nit != toNack.end (); nit++)
        {
          ah.AddNackedFrame (*nit);
        }

      Ptr<Packet> ack = Create<Packet> ();
      ack->AddHeader (ah);
      ack->AddHeader (ch);
      Simulator::Schedule (nextAck, &UanMacRcGw::SendPacket, this, ack, m_currentRateNum);
      nextAck = nextAck + ackTime + m_sifs;
    }
  m_ackData.clear ();
  Simulator::Schedule (nextAck, &UanMacRcGw::StartCycle, this);

}
void
UanMacRcGw::SendPacket (Ptr<Packet> pkt, uint32_t rate)
{
  UanHeaderCommon ch;
  pkt->PeekHeader (ch);
  std::string type;
  switch (ch.GetType ())
    {
    case UanMacRc::TYPE_DATA:
      type = "DATA";
      break;
    case UanMacRc::TYPE_RTS:
      type = "RTS";
      break;
    case UanMacRc::TYPE_CTS:
      type = "CTS";
      break;
    case UanMacRc::TYPE_ACK:
      type = "ACK";
      break;
    case UanMacRc::TYPE_GWPING:
      type = "GWPING";
      break;
    default:
      type = "UNKNOWN";
      break;
    }
  NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " GW sending " << type << " packet with size " << pkt->GetSize () << " to " << ch.GetDest () << " at rate " << rate);
  m_phy->SendPacket (pkt, rate);
}


double
UanMacRcGw::ComputeAlpha (uint32_t totalFrames, uint32_t totalBytes, uint32_t n, uint32_t a, double deltaK)
{

  double alpha;
  double lrae = m_rtsSize * 8.0 * a * std::exp (1.0);
  if (totalFrames == 0)
    {

      alpha =  (2.0 * lrae + 8.0 * m_rtsSize - std::sqrt (m_ctsSizeG * 8.0 * 8.0 * m_rtsSize + 2 * 8.0 * m_ctsSizeG * 8.0 * m_rtsSize * a * std::exp (1.0)) ) /
        (2 * lrae + 8.0 * m_rtsSize - 8.0 * m_ctsSizeG);
    }
  else
    {
      double w = totalBytes * 8.0 + totalFrames*m_sifs.GetSeconds () * m_totalRate;
      double v = m_rtsSize * 8.0 + 2 * lrae;
      double u = (2 * m_maxDelta.GetSeconds () - 2 * deltaK) * m_totalRate;

      double gamma = (w - u + v) / (2 * (u - totalFrames * m_sifs.GetSeconds () * m_totalRate));

      alpha = -gamma + std::sqrt (gamma * gamma + v / (u - totalFrames * m_sifs.GetSeconds () * m_totalRate));

      if (alpha < 0 || alpha > 1)
        {
          alpha = -gamma - std::sqrt (gamma * gamma + v / (u - totalFrames * m_sifs.GetSeconds () * m_totalRate));
        }
    }
  NS_ASSERT_MSG (alpha > 0 && alpha < 1, "Error computing alpha.  Alpha out of valid range!");
  return alpha;
}

std::vector<double>
UanMacRcGw::GetExpPdk (void)
{
  uint32_t n = m_numNodes;
  std::vector<double> pds;
  std::map<UanAddress, Time>::iterator pdit = m_propDelay.begin ();

  for (; pdit != m_propDelay.end (); pdit++)
    {
      pds.push_back (pdit->second.GetSeconds ());
    }
  while (pds.size () < m_numNodes)
    {
      pds.push_back (m_maxDelta.GetSeconds ());
    }

  std::sort (pds.begin (), pds.end ());
  // Find expected min. prop. delay for k nodes
  std::vector<double> exppdk;
  exppdk.push_back (m_maxDelta.GetSeconds ());
  for (uint32_t k = 1; k <= n; k++)
    {
      uint32_t ind = CompExpMinIndex (n,k) - 1;
      exppdk.push_back (pds[ind]);
    }
  return exppdk;
}

double
UanMacRcGw::ComputeExpS (uint32_t a, uint32_t ld, std::vector<double> exppdk)
{
  UanHeaderCommon ch;
  uint32_t lh = ch.GetSerializedSize ();

  uint32_t n = m_numNodes;
  double expk = n * (1 - std::exp (-((double) a) / (double) n));
  NS_LOG_DEBUG ("expk = " << expk);

  // Compute expected data per cycle
  double expdata = 8 * ld * expk;

  // Compute expected time per cycle
  double alpha0 = ComputeAlpha (0,0,n,a,exppdk[0]);
  double c0 = 8.0 * m_ctsSizeG / ( m_totalRate * (1 - alpha0)) + 2 * m_maxDelta.GetSeconds () + (a * std::exp (1.0) + 0.5) * 2 * m_rtsSize * 8.0 / (alpha0 * m_totalRate);
  double exptime = ComputePiK (a,n,0) * c0;
  double expp = 0;
  for (uint32_t i = 1; i <= n; i++)
    {
      expp += ComputePiK (a,n,i) * exppdk[i - 1];
    }

  exptime += ComputeExpBOverA (n,a,ld + lh,exppdk) + expk * 2 * m_sifs.GetSeconds () + m_sifs.GetSeconds () + 2 * expp;
  double s = (1.0 / m_totalRate) * expdata / exptime;

  return s;
}

double
UanMacRcGw::ComputeExpS (uint32_t a, uint32_t ld)
{
  return ComputeExpS (a, ld, GetExpPdk ());
}

uint32_t
UanMacRcGw::CompExpMinIndex (uint32_t n, uint32_t k)
{
  double sum = 0;
  for (uint32_t i = 1; i <= n - k + 1; i++)
    {
      double p = (double) NchooseK (n - i, k - 1) / NchooseK (n, k);
      sum += p * i;
    }
  return (uint32_t)(sum + 0.5);
}

double
UanMacRcGw::ComputePiK (uint32_t a, uint32_t n, uint32_t k)
{
  double nck = (double) NchooseK (n, k);
  return nck * std::pow ( (std::exp ( (double) a / (double) n) - 1.0), (double) k) * std::exp (-( (double) a));
}

double
UanMacRcGw::ComputeExpBOverA (uint32_t n, uint32_t a, uint32_t ldlh, std::vector<double> deltaK)
{

  double sum = 0;
  uint32_t lt = 8 * (m_ctsSizeN + ldlh + m_ackSize);
  for (uint32_t k = 1; k <= n; k++)
    {
      double num = 8.0 * m_ctsSizeG + k * lt;
      double denom = (1.0 - ComputeAlpha (k, k * ldlh, n, a, deltaK[k])) * m_totalRate;
      double pik = ComputePiK (a, n, k);
      double term = pik * num / denom;

      sum += term;
    }

  return sum;
}

uint64_t
UanMacRcGw::NchooseK (uint32_t n, uint32_t k)
{
  if (k > n)
    {
      return 0;
    }

  if (k > n / 2)
    {
      k = n - k;
    }

  double accum = 1;
  for (uint32_t i = 1; i <= k; i++)
    {
      accum = accum * (n - k + i) / i;
    }

  return (uint64_t)(accum + 0.5);

}

uint32_t
UanMacRcGw::FindOptA (void)
{
  double tput = 0;
  uint32_t a = 1;
  while (1)
    {

      double newtput = ComputeExpS (a, m_frameSize);
      if (newtput < tput)
        {
          a--;
          break;
        }
      else
        {
          tput = newtput;
          a++;
        }
    }
  NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " GW: Found optimum a = " << a);
  return a;
}

int64_t
UanMacRcGw::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  return 0;
}

} // namespace ns3
