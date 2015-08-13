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
 *         Andrea Sacco <andrea.sacco85@gmail.com>
 */

#include "uan-phy-gen.h"
#include "uan-transducer.h"
#include "uan-channel.h"
#include "uan-net-device.h"
#include "ns3/simulator.h"
#include "ns3/traced-callback.h"
#include "ns3/ptr.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/uan-tx-mode.h"
#include "ns3/node.h"
#include "ns3/uinteger.h"
#include "ns3/energy-source-container.h"
#include "ns3/acoustic-modem-energy-model.h"


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UanPhyGen");

NS_OBJECT_ENSURE_REGISTERED (UanPhyGen);
NS_OBJECT_ENSURE_REGISTERED (UanPhyPerGenDefault);
NS_OBJECT_ENSURE_REGISTERED (UanPhyCalcSinrDefault);
NS_OBJECT_ENSURE_REGISTERED (UanPhyCalcSinrFhFsk);
NS_OBJECT_ENSURE_REGISTERED (UanPhyPerUmodem);


/*************** UanPhyCalcSinrDefault definition *****************/
UanPhyCalcSinrDefault::UanPhyCalcSinrDefault ()
{

}
UanPhyCalcSinrDefault::~UanPhyCalcSinrDefault ()
{

}

TypeId
UanPhyCalcSinrDefault::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanPhyCalcSinrDefault")
    .SetParent<UanPhyCalcSinr> ()
    .SetGroupName ("Uan")
    .AddConstructor<UanPhyCalcSinrDefault> ()
  ;
  return tid;
}

double
UanPhyCalcSinrDefault::CalcSinrDb (Ptr<Packet> pkt,
                                   Time arrTime,
                                   double rxPowerDb,
                                   double ambNoiseDb,
                                   UanTxMode mode,
                                   UanPdp pdp,
                                   const UanTransducer::ArrivalList &arrivalList) const
{
  if (mode.GetModType () == UanTxMode::OTHER)
    {
      NS_LOG_WARN ("Calculating SINR for unsupported modulation type");
    }

  double intKp = -DbToKp (rxPowerDb); // This packet is in the arrivalList
  UanTransducer::ArrivalList::const_iterator it = arrivalList.begin ();
  for (; it != arrivalList.end (); it++)
    {
      intKp += DbToKp (it->GetRxPowerDb ());
    }

  double totalIntDb = KpToDb (intKp + DbToKp (ambNoiseDb));

  NS_LOG_DEBUG ("Calculating SINR:  RxPower = " << rxPowerDb << " dB.  Number of interferers = " << arrivalList.size () << "  Interference + noise power = " << totalIntDb << " dB.  SINR = " << rxPowerDb - totalIntDb << " dB.");
  return rxPowerDb - totalIntDb;
}

/*************** UanPhyCalcSinrFhFsk definition *****************/
UanPhyCalcSinrFhFsk::UanPhyCalcSinrFhFsk ()
{

}
UanPhyCalcSinrFhFsk::~UanPhyCalcSinrFhFsk ()
{

}

TypeId
UanPhyCalcSinrFhFsk::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanPhyCalcSinrFhFsk")
    .SetParent<UanPhyCalcSinr> ()
    .SetGroupName ("Uan")
    .AddConstructor<UanPhyCalcSinrFhFsk> ()
    .AddAttribute ("NumberOfHops",
                   "Number of frequencies in hopping pattern.",
                   UintegerValue (13),
                   MakeUintegerAccessor (&UanPhyCalcSinrFhFsk::m_hops),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}
double
UanPhyCalcSinrFhFsk::CalcSinrDb (Ptr<Packet> pkt,
                                 Time arrTime,
                                 double rxPowerDb,
                                 double ambNoiseDb,
                                 UanTxMode mode,
                                 UanPdp pdp,
                                 const UanTransducer::ArrivalList &arrivalList) const
{
  if (mode.GetModType () != UanTxMode::FSK)
    {
      NS_LOG_WARN ("Calculating SINR for unsupported mode type");
    }



  double ts = 1.0 / mode.GetPhyRateSps ();
  double clearingTime = (m_hops - 1.0) * ts;
  double csp = pdp.SumTapsFromMaxNc (Seconds (0), Seconds (ts));

  // Get maximum arrival offset
  double maxAmp = -1;
  double maxTapDelay = 0.0;
  UanPdp::Iterator pit = pdp.GetBegin ();
  for (; pit != pdp.GetEnd (); pit++)
    {
      if (std::abs (pit->GetAmp ()) > maxAmp)
        {
          maxAmp = std::abs (pit->GetAmp ());
          maxTapDelay = pit->GetDelay ().GetSeconds ();
        }
    }


  double effRxPowerDb = rxPowerDb + KpToDb (csp);

  double isiUpa = rxPowerDb * pdp.SumTapsFromMaxNc (Seconds (ts + clearingTime), Seconds (ts));
  UanTransducer::ArrivalList::const_iterator it = arrivalList.begin ();
  double intKp = -DbToKp (effRxPowerDb);
  for (; it != arrivalList.end (); it++)
    {
      UanPdp intPdp = it->GetPdp ();
      double tDelta = std::abs (arrTime.GetSeconds () + maxTapDelay - it->GetArrivalTime ().GetSeconds ());
      // We want tDelta in terms of a single symbol (i.e. if tDelta = 7.3 symbol+clearing
      // times, the offset in terms of the arriving symbol power is
      // 0.3 symbol+clearing times.

      int32_t syms = (uint32_t)( (double) tDelta / (ts + clearingTime));
      tDelta = tDelta - syms * (ts + clearingTime);

      // Align to pktRx
      if (arrTime + Seconds (maxTapDelay)  > it->GetArrivalTime ())
        {
          tDelta = ts + clearingTime - tDelta;
        }

      double intPower = 0.0;
      if (tDelta < ts)
        {
          intPower += intPdp.SumTapsNc (Seconds (0), Seconds (ts - tDelta));
          intPower += intPdp.SumTapsNc (Seconds (ts - tDelta + clearingTime),
                                        Seconds (2 * ts - tDelta + clearingTime));
        }
      else
        {
          Time start = Seconds (ts + clearingTime - tDelta);
          Time end = start + Seconds (ts);
          intPower += intPdp.SumTapsNc (start, end);

          start = start + Seconds (ts + clearingTime);
          end = start + Seconds (ts);
          intPower += intPdp.SumTapsNc (start, end);
        }
      intKp += DbToKp (it->GetRxPowerDb ()) * intPower;
    }

  double totalIntDb = KpToDb (isiUpa + intKp + DbToKp (ambNoiseDb));

  NS_LOG_DEBUG ("Calculating SINR:  RxPower = " << rxPowerDb << " dB.  Effective Rx power " << effRxPowerDb << " dB.  Number of interferers = " << arrivalList.size () << "  Interference + noise power = " << totalIntDb << " dB.  SINR = " << effRxPowerDb - totalIntDb << " dB.");
  return effRxPowerDb - totalIntDb;
}

/*************** UanPhyPerGenDefault definition *****************/
UanPhyPerGenDefault::UanPhyPerGenDefault ()
{

}

UanPhyPerGenDefault::~UanPhyPerGenDefault ()
{

}
TypeId
UanPhyPerGenDefault::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanPhyPerGenDefault")
    .SetParent<UanPhyPer> ()
    .SetGroupName ("Uan")
    .AddConstructor<UanPhyPerGenDefault> ()
    .AddAttribute ("Threshold", "SINR cutoff for good packet reception.",
                   DoubleValue (8),
                   MakeDoubleAccessor (&UanPhyPerGenDefault::m_thresh),
                   MakeDoubleChecker<double> ());
  return tid;
}


// Default PER calculation simply compares SINR to a threshold which is configurable
// via an attribute.
double
UanPhyPerGenDefault::CalcPer (Ptr<Packet> pkt, double sinrDb, UanTxMode mode)
{
  if (sinrDb >= m_thresh)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}

/*************** UanPhyPerUmodem definition *****************/
UanPhyPerUmodem::UanPhyPerUmodem ()
{

}
UanPhyPerUmodem::~UanPhyPerUmodem ()
{

}

TypeId UanPhyPerUmodem::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UanPhyPerUmodem")
    .SetParent<UanPhyPer> ()
    .SetGroupName ("Uan")
    .AddConstructor<UanPhyPerUmodem> ()
  ;
  return tid;
}

double
UanPhyPerUmodem::NChooseK (uint32_t n, uint32_t k)
{
  double result;

  result = 1.0;

  for (uint32_t i = std::max (k,n - k) + 1; i <= n; ++i)
    {
      result *= i;
    }

  for (uint32_t i = 2; i <= std::min (k,n - k); ++i)
    {
      result /= i;
    }

  return result;
}

double
UanPhyPerUmodem::CalcPer (Ptr<Packet> pkt, double sinr, UanTxMode mode)
{
  uint32_t d[] =
  { 12, 14, 16, 18, 20, 22, 24, 26, 28 };
  double Bd[] =
  {
    33, 281, 2179, 15035LLU, 105166LLU, 692330LLU, 4580007LLU, 29692894LLU,
    190453145LLU
  };

  // double Rc = 1.0 / 2.0;
  double ebno = std::pow (10.0, sinr / 10.0);
  double perror = 1.0 / (2.0 + ebno);
  double P[9];

  if (sinr >= 10)
    {
      return 0;
    }
  if (sinr <= 6)
    {
      return 1;
    }

  for (uint32_t r = 0; r < 9; r++)
    {
      double sumd = 0;
      for (uint32_t k = 0; k < d[r]; k++)
        {
          sumd = sumd + NChooseK (d[r] - 1 + k, k) * std::pow (1 - perror, (double) k);
        }
      P[r] = std::pow (perror, (double) d[r]) * sumd;

    }

  double Pb = 0;
  for (uint32_t r = 0; r < 8; r++)
    {
      Pb = Pb + Bd[r] * P[r];
    }

  // cout << "Pb = " << Pb << endl;
  uint32_t bits = pkt->GetSize () * 8;

  double Ppacket = 1;
  double temp = NChooseK (bits, 0);
  temp *= std::pow ( (1 - Pb), (double) bits);
  Ppacket -= temp;
  temp = NChooseK (288, 1) * Pb * std::pow ( (1 - Pb), bits - 1.0);
  Ppacket -= temp;

  if (Ppacket > 1)
    {
      return 1;
    }
  else
    {
      return Ppacket;
    }
}

/*************** UanPhyGen definition *****************/
UanPhyGen::UanPhyGen ()
  : UanPhy (),
    m_state (IDLE),
    m_channel (0),
    m_transducer (0),
    m_device (0),
    m_mac (0),
    m_rxGainDb (0),
    m_txPwrDb (0),
    m_rxThreshDb (0),
    m_ccaThreshDb (0),
    m_pktRx (0),
    m_cleared (false),
    m_disabled (false)
{
  m_pg = CreateObject<UniformRandomVariable> ();

  m_energyCallback.Nullify ();
}

UanPhyGen::~UanPhyGen ()
{

}

void
UanPhyGen::Clear ()
{
  if (m_cleared)
    {
      return;
    }
  m_cleared = true;
  m_listeners.clear ();
  if (m_channel)
    {
      m_channel->Clear ();
      m_channel = 0;
    }
  if (m_transducer)
    {
      m_transducer->Clear ();
      m_transducer = 0;
    }
  if (m_device)
    {
      m_device->Clear ();
      m_device = 0;
    }
  if (m_mac)
    {
      m_mac->Clear ();
      m_mac = 0;
    }
  if (m_per)
    {
      m_per->Clear ();
      m_per = 0;
    }
  if (m_sinr)
    {
      m_sinr->Clear ();
      m_sinr = 0;
    }
  m_pktRx = 0;
}

void
UanPhyGen::DoDispose ()
{
  Clear ();
  m_energyCallback.Nullify ();
  UanPhy::DoDispose ();
}

UanModesList
UanPhyGen::GetDefaultModes (void)
{
  UanModesList l;
  l.AppendMode (UanTxModeFactory::CreateMode (UanTxMode::FSK,80,80,22000,4000,13,"FSK"));
  l.AppendMode (UanTxModeFactory::CreateMode (UanTxMode::PSK,200, 200, 22000, 4000, 4, "QPSK"));
  return l;
}
TypeId
UanPhyGen::GetTypeId (void)
{

  static TypeId tid = TypeId ("ns3::UanPhyGen")
    .SetParent<UanPhy> ()
    .SetGroupName ("Uan")
    .AddConstructor<UanPhyGen> ()
    .AddAttribute ("CcaThreshold",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&UanPhyGen::m_ccaThreshDb),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxThreshold",
                   "Required SNR for signal acquisition in dB.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&UanPhyGen::m_rxThreshDb),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPower",
                   "Transmission output power in dB.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&UanPhyGen::m_txPwrDb),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGain",
                   "Gain added to incoming signal at receiver.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&UanPhyGen::m_rxGainDb),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SupportedModes",
                   "List of modes supported by this PHY.",
                   UanModesListValue (UanPhyGen::GetDefaultModes ()),
                   MakeUanModesListAccessor (&UanPhyGen::m_modes),
                   MakeUanModesListChecker () )
    .AddAttribute ("PerModel",
                   "Functor to calculate PER based on SINR and TxMode.",
                   StringValue ("ns3::UanPhyPerGenDefault"),
                   MakePointerAccessor (&UanPhyGen::m_per),
                   MakePointerChecker<UanPhyPer> ())
    .AddAttribute ("SinrModel",
                   "Functor to calculate SINR based on pkt arrivals and modes.",
                   StringValue ("ns3::UanPhyCalcSinrDefault"),
                   MakePointerAccessor (&UanPhyGen::m_sinr),
                   MakePointerChecker<UanPhyCalcSinr> ())
    .AddTraceSource ("RxOk",
                     "A packet was received successfully.",
                     MakeTraceSourceAccessor (&UanPhyGen::m_rxOkLogger),
                     "ns3::UanPhy::TracedCallback")
    .AddTraceSource ("RxError",
                     "A packet was received unsuccessfully.",
                     MakeTraceSourceAccessor (&UanPhyGen::m_rxErrLogger),
                     "ns3::UanPhy::TracedCallback")
    .AddTraceSource ("Tx",
                     "Packet transmission beginning.",
                     MakeTraceSourceAccessor (&UanPhyGen::m_txLogger),
                     "ns3::UanPhy::TracedCallback")
  ;
  return tid;

}

void
UanPhyGen::SetEnergyModelCallback (DeviceEnergyModel::ChangeStateCallback cb)
{
  NS_LOG_FUNCTION (this);
  m_energyCallback = cb;
}

void
UanPhyGen::UpdatePowerConsumption (const State state)
{
  NS_LOG_FUNCTION (this);

  if (!m_energyCallback.IsNull ())
    {
      m_energyCallback (state);
    }
}

void
UanPhyGen::EnergyDepletionHandler ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("Energy depleted at node " << m_device->GetNode ()->GetId () <<
                ", stopping rx/tx activities");

  m_disabled = true;
}

void
UanPhyGen::SendPacket (Ptr<Packet> pkt, uint32_t modeNum)
{
  NS_LOG_DEBUG ("PHY " << m_mac->GetAddress () << ": Transmitting packet");
  if (m_disabled)
    {
      NS_LOG_DEBUG ("Energy depleted, node cannot transmit any packet. Dropping.");
      return;
    }

  if (m_state == TX)
    {
      NS_LOG_DEBUG ("PHY requested to TX while already Transmitting.  Dropping packet.");
      return;
    }
  else if (m_state == SLEEP)
    {
      NS_LOG_DEBUG ("PHY requested to TX while sleeping.  Dropping packet.");
      return;
    }

  UanTxMode txMode = GetMode (modeNum);

  if (m_pktRx != 0)
    {
      m_minRxSinrDb = -1e30;
      m_pktRx = 0;
    }

  m_transducer->Transmit (Ptr<UanPhy> (this), pkt, m_txPwrDb, txMode);
  m_state = TX;
  UpdatePowerConsumption (TX);
  double txdelay = pkt->GetSize () * 8.0 / txMode.GetDataRateBps ();
  Simulator::Schedule (Seconds (txdelay), &UanPhyGen::TxEndEvent, this);
  NS_LOG_DEBUG ("PHY " << m_mac->GetAddress () << " notifying listeners");
  NotifyListenersTxStart (Seconds (txdelay));
  m_txLogger (pkt, m_txPwrDb, txMode);
}

void
UanPhyGen::TxEndEvent ()
{
  if (m_state == SLEEP || m_disabled == true)
    {
      NS_LOG_DEBUG ("Transmission ended but node sleeping or dead");
      return;
    }

  NS_ASSERT (m_state == TX);
  if (GetInterferenceDb ( (Ptr<Packet>) 0) > m_ccaThreshDb)
    {
      m_state = CCABUSY;
      NotifyListenersCcaStart ();
    }
  else
    {
      m_state = IDLE;
    }
  UpdatePowerConsumption (IDLE);
}

void
UanPhyGen::RegisterListener (UanPhyListener *listener)
{
  m_listeners.push_back (listener);
}


void
UanPhyGen::StartRxPacket (Ptr<Packet> pkt, double rxPowerDb, UanTxMode txMode, UanPdp pdp)
{
  if (m_disabled)
    {
      NS_LOG_DEBUG ("Energy depleted, node cannot receive any packet. Dropping.");
      NotifyRxDrop(pkt);    // traced source netanim
      return;
    }

  switch (m_state)
    {
    case TX:
      NotifyRxDrop(pkt);    // traced source netanim
      NS_ASSERT (false);
      break;
    case RX:
      {
        NS_ASSERT (m_pktRx);
        double newSinrDb = CalculateSinrDb (m_pktRx, m_pktRxArrTime, m_rxRecvPwrDb, m_pktRxMode, m_pktRxPdp);
        m_minRxSinrDb  =  (newSinrDb < m_minRxSinrDb) ? newSinrDb : m_minRxSinrDb;
        NS_LOG_DEBUG ("PHY " << m_mac->GetAddress () << ": Starting RX in RX mode.  SINR of pktRx = " << m_minRxSinrDb);
        NotifyRxBegin(pkt);    // traced source netanim
      }
      break;

    case CCABUSY:
    case IDLE:
      {
        NS_ASSERT (!m_pktRx);
        bool hasmode = false;
        for (uint32_t i = 0; i < GetNModes (); i++)
          {
            if (txMode.GetUid () == GetMode (i).GetUid ())
              {
                hasmode = true;
                break;
              }
          }
        if (!hasmode)
          {
            break;
          }


        double newsinr = CalculateSinrDb (pkt, Simulator::Now (), rxPowerDb, txMode, pdp);
        NS_LOG_DEBUG ("PHY " << m_mac->GetAddress () << ": Starting RX in IDLE mode.  SINR = " << newsinr);
        if (newsinr > m_rxThreshDb)
          {
            m_state = RX;
            UpdatePowerConsumption (RX);
            NotifyRxBegin(pkt);    // traced source netanim
            m_rxRecvPwrDb = rxPowerDb;
            m_minRxSinrDb = newsinr;
            m_pktRx = pkt;
            m_pktRxArrTime = Simulator::Now ();
            m_pktRxMode = txMode;
            m_pktRxPdp = pdp;
            double txdelay = pkt->GetSize () * 8.0 / txMode.GetDataRateBps ();
            Simulator::Schedule (Seconds (txdelay), &UanPhyGen::RxEndEvent, this, pkt, rxPowerDb, txMode);
            NotifyListenersRxStart ();
          }

      }
      break;
    case SLEEP:
      NS_LOG_DEBUG ("Sleep mode. Dropping packet.");
      NotifyRxDrop(pkt);    // traced source netanim
      break;
    }

  if (m_state == IDLE && GetInterferenceDb ( (Ptr<Packet>) 0) > m_ccaThreshDb)
    {
      m_state = CCABUSY;
      NotifyListenersCcaStart ();
    }

}

void
UanPhyGen::RxEndEvent (Ptr<Packet> pkt, double rxPowerDb, UanTxMode txMode)
{
  if (pkt != m_pktRx)
    {
      return;
    }

  if (m_disabled || m_state == SLEEP)
    {
      NS_LOG_DEBUG ("Sleep mode or dead. Dropping packet");
      m_pktRx = 0;
      NotifyRxDrop(pkt);    // traced source netanim
      return;
    }

  NotifyRxEnd(pkt);    // traced source netanim
  if (GetInterferenceDb ( (Ptr<Packet>) 0) > m_ccaThreshDb)
    {
      m_state = CCABUSY;
      NotifyListenersCcaStart ();
    }
  else
    {
      m_state = IDLE;
      UpdatePowerConsumption (IDLE);
    }

  if (m_pg->GetValue (0, 1) > m_per->CalcPer (m_pktRx, m_minRxSinrDb, txMode))
    {
      m_rxOkLogger (pkt, m_minRxSinrDb, txMode);
      NotifyListenersRxGood ();
      if (!m_recOkCb.IsNull ())
        {
          m_recOkCb (pkt, m_minRxSinrDb, txMode);
        }

    }
  else
    {
      m_rxErrLogger (pkt, m_minRxSinrDb, txMode);
      NotifyListenersRxBad ();
      if (!m_recErrCb.IsNull ())
        {
          m_recErrCb (pkt, m_minRxSinrDb);
        }
    }

  m_pktRx = 0;
}

void
UanPhyGen::SetReceiveOkCallback (RxOkCallback cb)
{
  m_recOkCb = cb;
}

void
UanPhyGen::SetReceiveErrorCallback (RxErrCallback cb)
{
  m_recErrCb = cb;
}
bool
UanPhyGen::IsStateSleep (void)
{
  return m_state == SLEEP;
}
bool
UanPhyGen::IsStateIdle (void)
{
  return m_state == IDLE;
}
bool
UanPhyGen::IsStateBusy (void)
{
  return !IsStateIdle () && !IsStateSleep ();
}
bool
UanPhyGen::IsStateRx (void)
{
  return m_state == RX;
}
bool
UanPhyGen::IsStateTx (void)
{
  return m_state == TX;
}

bool
UanPhyGen::IsStateCcaBusy (void)
{
  return m_state == CCABUSY;
}


void
UanPhyGen::SetRxGainDb (double gain)
{
  m_rxGainDb = gain;

}
void
UanPhyGen::SetTxPowerDb (double txpwr)
{
  m_txPwrDb = txpwr;
}
void
UanPhyGen::SetRxThresholdDb (double thresh)
{
  m_rxThreshDb = thresh;
}
void
UanPhyGen::SetCcaThresholdDb (double thresh)
{
  m_ccaThreshDb = thresh;
}
double
UanPhyGen::GetRxGainDb (void)
{
  return m_rxGainDb;
}
double
UanPhyGen::GetTxPowerDb (void)
{
  return m_txPwrDb;

}
double
UanPhyGen::GetRxThresholdDb (void)
{
  return m_rxThreshDb;
}
double
UanPhyGen::GetCcaThresholdDb (void)
{
  return m_ccaThreshDb;
}

Ptr<UanChannel>
UanPhyGen::GetChannel (void) const
{
  return m_channel;
}

Ptr<UanNetDevice>
UanPhyGen::GetDevice (void) const
{
  return m_device;
}

Ptr<UanTransducer>
UanPhyGen::GetTransducer (void)
{
  return m_transducer;
}
void
UanPhyGen::SetChannel (Ptr<UanChannel> channel)
{
  m_channel = channel;
}

void
UanPhyGen::SetDevice (Ptr<UanNetDevice> device)
{
  m_device = device;
}

void
UanPhyGen::SetMac (Ptr<UanMac> mac)
{
  m_mac = mac;
}

void
UanPhyGen::SetTransducer (Ptr<UanTransducer> trans)
{
  m_transducer = trans;
  m_transducer->AddPhy (this);
}

void
UanPhyGen::SetSleepMode (bool sleep)
{
  if (sleep)
    {
      m_state = SLEEP;
      if (!m_energyCallback.IsNull ())
        {
          m_energyCallback (SLEEP);
        }
    }
  else if (m_state == SLEEP)
    {
      if (GetInterferenceDb ((Ptr<Packet>) 0) > m_ccaThreshDb)
        {
          m_state = CCABUSY;
          NotifyListenersCcaStart ();
        }
      else
        {
          m_state = IDLE;
        }

      if (!m_energyCallback.IsNull ())
        {
          m_energyCallback (IDLE);
        }
    }
}

int64_t
UanPhyGen::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_pg->SetStream (stream);
  return 1;
}

void
UanPhyGen::NotifyTransStartTx (Ptr<Packet> packet, double txPowerDb, UanTxMode txMode)
{
  if (m_pktRx)
    {
      m_minRxSinrDb = -1e30;
    }
}

void
UanPhyGen::NotifyIntChange (void)
{
  if (m_state == CCABUSY && GetInterferenceDb (Ptr<Packet> ()) < m_ccaThreshDb)
    {
      m_state = IDLE;
      NotifyListenersCcaEnd ();
    }
}

double
UanPhyGen::CalculateSinrDb (Ptr<Packet> pkt, Time arrTime, double rxPowerDb, UanTxMode mode, UanPdp pdp)
{
  double noiseDb = m_channel->GetNoiseDbHz ( (double) mode.GetCenterFreqHz () / 1000.0) + 10 * std::log10 (mode.GetBandwidthHz ());
  return m_sinr->CalcSinrDb (pkt, arrTime, rxPowerDb, noiseDb, mode, pdp, m_transducer->GetArrivalList ());
}

double
UanPhyGen::GetInterferenceDb (Ptr<Packet> pkt)
{

  const UanTransducer::ArrivalList &arrivalList = m_transducer->GetArrivalList ();

  UanTransducer::ArrivalList::const_iterator it = arrivalList.begin ();

  double interfPower = 0;

  for (; it != arrivalList.end (); it++)
    {
      if (pkt != it->GetPacket ())
        {
          interfPower += DbToKp (it->GetRxPowerDb ());
        }
    }

  return KpToDb (interfPower);

}

double
UanPhyGen::DbToKp (double db)
{
  return std::pow (10, db / 10.0);
}
double
UanPhyGen::KpToDb (double kp)
{
  return 10 * std::log10 (kp);
}

void
UanPhyGen::NotifyListenersRxStart (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyRxStart ();
    }

}
void
UanPhyGen::NotifyListenersRxGood (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyRxEndOk ();
    }
}
void
UanPhyGen::NotifyListenersRxBad (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyRxEndError ();
    }
}
void
UanPhyGen::NotifyListenersCcaStart (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyCcaStart ();
    }
}
void
UanPhyGen::NotifyListenersCcaEnd (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyCcaEnd ();
    }
}

void
UanPhyGen::NotifyListenersTxStart (Time duration)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyTxStart (duration);
    }
}

uint32_t
UanPhyGen::GetNModes (void)
{
  return m_modes.GetNModes ();
}

UanTxMode
UanPhyGen::GetMode (uint32_t n)
{
  NS_ASSERT (n < m_modes.GetNModes ());

  return m_modes[n];
}

Ptr<Packet>
UanPhyGen::GetPacketRx (void) const
{
  return m_pktRx;
}


} // namespace ns3
