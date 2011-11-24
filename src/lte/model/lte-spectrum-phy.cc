/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009, 2011 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 *         Giuseppe Piro  <g.piro@poliba.it>
 */


#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <math.h>
#include <ns3/simulator.h>
#include <ns3/trace-source-accessor.h>
#include "lte-spectrum-phy.h"
#include "lte-spectrum-signal-parameters.h"
#include "lte-net-device.h"
#include "lte-mac-tag.h"
#include "lte-sinr-chunk-processor.h"
#include "lte-phy-tag.h"

NS_LOG_COMPONENT_DEFINE ("LteSpectrumPhy");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (LteSpectrumPhy);

LteSpectrumPhy::LteSpectrumPhy ()
  : m_state (IDLE)
{
  NS_LOG_FUNCTION (this);
  m_interference = CreateObject<LteInterference> ();
}


LteSpectrumPhy::~LteSpectrumPhy ()
{
  NS_LOG_FUNCTION (this);
  m_expectedTbs.clear ();
}

void LteSpectrumPhy::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_channel = 0;
  m_mobility = 0;
  m_device = 0;
  m_interference->Dispose ();
  m_interference = 0;
  m_genericPhyTxEndCallback      = MakeNullCallback< void, Ptr<const Packet> > ();
  m_genericPhyRxEndErrorCallback = MakeNullCallback< void > ();
  m_genericPhyRxEndOkCallback    = MakeNullCallback< void, Ptr<Packet> >  ();
  SpectrumPhy::DoDispose ();
} 

std::ostream& operator<< (std::ostream& os, LteSpectrumPhy::State s)
{
  switch (s)
    {
    case LteSpectrumPhy::IDLE:
      os << "IDLE";
      break;
    case LteSpectrumPhy::RX:
      os << "RX";
      break;
    case LteSpectrumPhy::TX:
      os << "TX";
      break;
    default:
      os << "UNKNOWN";
      break;
    }
  return os;
}


TypeId
LteSpectrumPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteSpectrumPhy")
    .SetParent<SpectrumPhy> ()
    .AddTraceSource ("TxStart",
                     "Trace fired when a new transmission is started",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyTxStartTrace))
    .AddTraceSource ("TxEnd",
                     "Trace fired when a previosuly started transmission is finished",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyTxEndTrace))
    .AddTraceSource ("RxStart",
                     "Trace fired when the start of a signal is detected",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyRxStartTrace))
    .AddTraceSource ("RxEndOk",
                     "Trace fired when a previosuly started RX terminates successfully",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyRxEndOkTrace))
    .AddTraceSource ("RxEndError",
                     "Trace fired when a previosuly started RX terminates with an error",
                     MakeTraceSourceAccessor (&LteSpectrumPhy::m_phyRxEndErrorTrace))
  ;
  return tid;
}



Ptr<NetDevice>
LteSpectrumPhy::GetDevice ()
{
  NS_LOG_FUNCTION (this);
  return m_device;
}


Ptr<MobilityModel>
LteSpectrumPhy::GetMobility ()
{
  NS_LOG_FUNCTION (this);
  return m_mobility;
}


void
LteSpectrumPhy::SetDevice (Ptr<NetDevice> d)
{
  NS_LOG_FUNCTION (this << d);
  m_device = d;
}


void
LteSpectrumPhy::SetMobility (Ptr<MobilityModel> m)
{
  NS_LOG_FUNCTION (this << m);
  m_mobility = m;
}


void
LteSpectrumPhy::SetChannel (Ptr<SpectrumChannel> c)
{
  NS_LOG_FUNCTION (this << c);
  m_channel = c;
}

Ptr<const SpectrumModel>
LteSpectrumPhy::GetRxSpectrumModel () const
{
  if (m_txPsd)
    {
      return m_txPsd->GetSpectrumModel ();
    }
  else
    {
      return 0;
    }
}


void
LteSpectrumPhy::SetTxPowerSpectralDensity (Ptr<SpectrumValue> txPsd)
{
  NS_LOG_FUNCTION (this << txPsd);
  NS_ASSERT (txPsd);
  m_txPsd = txPsd;
}


void
LteSpectrumPhy::SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd)
{
  NS_LOG_FUNCTION (this << noisePsd);
  NS_ASSERT (noisePsd);
  m_interference->SetNoisePowerSpectralDensity (noisePsd);
}



void
LteSpectrumPhy::SetGenericPhyTxEndCallback (GenericPhyTxEndCallback c)
{
  NS_LOG_FUNCTION (this);
  m_genericPhyTxEndCallback = c;
}


void
LteSpectrumPhy::SetGenericPhyRxEndErrorCallback (GenericPhyRxEndErrorCallback c)
{
  NS_LOG_FUNCTION (this);
  m_genericPhyRxEndErrorCallback = c;
}


void
LteSpectrumPhy::SetGenericPhyRxEndOkCallback (GenericPhyRxEndOkCallback c)
{
  NS_LOG_FUNCTION (this);
  m_genericPhyRxEndOkCallback = c;
}


void
LteSpectrumPhy::SetState (State newState)
{
  ChangeState (newState);
}


void
LteSpectrumPhy::ChangeState (State newState)
{
  NS_LOG_LOGIC (this << " state: " << m_state << " -> " << newState);
  m_state = newState;
}


bool
LteSpectrumPhy::StartTx (Ptr<PacketBurst> pb)
{
  NS_LOG_FUNCTION (this << pb);
  NS_LOG_LOGIC (this << " state: " << m_state);

  m_phyTxStartTrace (pb);

  switch (m_state)
    {
    case RX:
      NS_FATAL_ERROR ("cannot TX while RX: according to FDD channel acces, the physical layer for transmission cannot be used for reception");
      break;

    case TX:
      NS_FATAL_ERROR ("cannot TX while already TX: the MAC should avoid this");
      break;

    case IDLE:
      {
        /*
          m_txPsd must be setted by the device, according to
          (i) the available subchannel for transmission
          (ii) the power transmission
        */
        NS_ASSERT (m_txPsd);
        m_txPacketBurst = pb;

        // we need to convey some PHY meta information to the receiver
        // to be used for simulation purposes (e.g., the CellId). This
        // is done by adding an LtePhyTag to the first packet in the
        // burst.
        NS_ASSERT (pb->Begin () != pb->End ());
        LtePhyTag tag (m_cellId);
        Ptr<Packet> firstPacketInBurst = *(pb->Begin ());
        firstPacketInBurst->AddPacketTag (tag);

        ChangeState (TX);
        NS_ASSERT (m_channel);
        double tti = 0.001;
      Ptr<LteSpectrumSignalParameters> txParams = Create<LteSpectrumSignalParameters> ();
      txParams->duration = Seconds (tti);
      txParams->txPhy = GetObject<SpectrumPhy> ();
      txParams->psd = m_txPsd;
      txParams->packetBurst = pb;
      m_channel->StartTx (txParams);
        Simulator::Schedule (Seconds (tti), &LteSpectrumPhy::EndTx, this);
      }
      return false;
      break;

    default:
      NS_FATAL_ERROR ("uknown state");
      return true;
      break;
    }
}


void
LteSpectrumPhy::EndTx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << " state: " << m_state);

  NS_ASSERT (m_state == TX);

  m_phyTxEndTrace (m_txPacketBurst);

  if (!m_genericPhyTxEndCallback.IsNull ())
    {
      for (std::list<Ptr<Packet> >::const_iterator iter = m_txPacketBurst->Begin (); iter
           != m_txPacketBurst->End (); ++iter)
        {
          Ptr<Packet> packet = (*iter)->Copy ();
          m_genericPhyTxEndCallback (packet);
        }
    }

  m_txPacketBurst = 0;
  ChangeState (IDLE);
}


void
LteSpectrumPhy::StartRx (Ptr<SpectrumSignalParameters> spectrumRxParams)
{
  NS_LOG_FUNCTION (this << spectrumRxParams);
  NS_LOG_LOGIC (this << " state: " << m_state);

  // interference will happen regardless of the type of the signal (could be 3G, GSM, whatever)
  Ptr <const SpectrumValue> rxPsd = spectrumRxParams->psd;
  Time duration = spectrumRxParams->duration;
            
  m_interference->AddSignal (rxPsd, duration);

  // the device might start RX only if the signal is of a type
  // understood by this device - in this case, an LTE signal.
  Ptr<LteSpectrumSignalParameters> lteRxParams = DynamicCast<LteSpectrumSignalParameters> (spectrumRxParams);
  if (lteRxParams != 0)
    {
      switch (m_state)
        {
        case TX:
          NS_FATAL_ERROR ("cannot RX while TX: according to FDD channel access, the physical layer for transmission cannot be used for reception");
          break;

        case IDLE:
        case RX:
          // the behavior is similar when
          // we're IDLE or RX because we can receive more signals
          // simultaneously (e.g., at the eNB).
          {
            // To check if we're synchronized to this signal, we check
            // for the CellId which is reported in the LtePhyTag
            NS_ASSERT (lteRxParams->packetBurst->Begin () != lteRxParams->packetBurst->End ());
            LtePhyTag tag;
            Ptr<Packet> firstPacketInBurst = *(lteRxParams->packetBurst->Begin ());
            firstPacketInBurst->RemovePacketTag (tag);
            if (tag.GetCellId () == m_cellId)
              {
                NS_LOG_LOGIC (this << " synchronized with this signal (cellId=" << tag.GetCellId () << ")");
                if (m_rxPacketBurstList.empty ())
                  {
                    NS_ASSERT (m_state == IDLE);
                    // first transmission, i.e., we're IDLE and we
                    // start RX
                    m_firstRxStart = Simulator::Now ();
                    m_firstRxDuration = duration;
                    NS_LOG_LOGIC (this << " scheduling EndRx with delay " << duration);
                    Simulator::Schedule (duration, &LteSpectrumPhy::EndRx, this);
                  }
                else
                  {
                    NS_ASSERT (m_state == RX);
                    // sanity check: if there are multiple RX events, they
                    // should occur at the same time and have the same
                    // duration, otherwise the interference calculation
                    // won't be correct
                    NS_ASSERT ((m_firstRxStart == Simulator::Now ()) 
                               && (m_firstRxDuration == duration));
                  }

                ChangeState (RX);
                m_interference->StartRx (rxPsd);

                m_phyRxStartTrace (lteRxParams->packetBurst);

                m_rxPacketBurstList.push_back (lteRxParams->packetBurst);
 
                NS_LOG_LOGIC (this << " numSimultaneousRxEvents = " << m_rxPacketBurstList.size ());
              }
            else
              {
                NS_LOG_LOGIC (this << " not in sync with this signal (cellId=" 
                                   << tag.GetCellId () << ", m_cellId=" << m_cellId << ")");
              }
          }
          break;

        default:
          NS_FATAL_ERROR ("unknown state");
          break;
        }

      NS_LOG_LOGIC (this << " state: " << m_state);
    }
}

void
LteSpectrumPhy::UpdateSinrPerceived (const SpectrumValue& sinr)
{
  NS_LOG_FUNCTION (this << sinr);
  m_sinrPerceived = sinr;
}


void
LteSpectrumPhy::AddExpectedTb (uint16_t  rnti, uint16_t size, uint8_t mcs, std::vector<int> map)
{
  NS_LOG_LOGIC (this << " rnti: " << rnti << " size " << size << " mcs " << mcs);
  expectedTbs_t::iterator it;
  it = m_expectedTbs.find (rnti);
  if (it == m_expectedTbs.end ())
  {
    // insert new entry
    tbInfo_t tbInfo = {size, mcs, map, false};
    m_expectedTbs.insert (std::pair<uint16_t, tbInfo_t> (rnti,tbInfo ));
  }
  else
  {
    NS_FATAL_ERROR ("Expectd two TBs from the same UE");
  }
}


void
LteSpectrumPhy::EndRx ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_LOGIC (this << " state: " << m_state);

  NS_ASSERT (m_state == RX);

  // this will trigger CQI calculation and Error Model evaluation
  // as a side effect, the error model should update the error status of all TBs
  m_interference->EndRx ();
  NS_LOG_INFO (this << " No. of burts " << m_rxPacketBurstList.size ());
  NS_LOG_DEBUG (this << " Expected TBs " << m_expectedTbs.size ());
  expectedTbs_t::iterator itTb = m_expectedTbs.begin ();
  while (itTb!=m_expectedTbs.end ())
    {
      NS_LOG_DEBUG (this << "RNTI " << (*itTb).first << " size " << (*itTb).second.size << " mcs " << (uint32_t)(*itTb).second.mcs << " bitmap " << (*itTb).second.rbBitmap.size ());
      for (uint16_t i = 0; i < (*itTb).second.rbBitmap.size (); i++)
        {
          NS_LOG_DEBUG (this << " RB " << (*itTb).second.rbBitmap.at (i) << " SINR " << m_sinrPerceived[(*itTb).second.rbBitmap.at (i)]);
        }
      itTb++;
    }
  m_expectedTbs.clear ();  // DEBUG
  for (std::list<Ptr<PacketBurst> >::const_iterator i = m_rxPacketBurstList.begin (); 
       i != m_rxPacketBurstList.end (); ++i)
    {
      // here we should determine whether this TB has been received
      // correctly or not
      bool tbRxOk = true;
      NS_LOG_INFO (this << " Burst of " << (*i)->GetNPackets ());
      if (tbRxOk)
        {
          m_phyRxEndOkTrace (*i);

          // forward each PDU in the PacketBurst separately to the MAC 
          // WILD HACK: we currently don't model properly the aggregation
          // of PDUs into TBs. In reality, the PHY is concerned only with
          // TBs, and it should be left to the MAC to decompose the TB into PDUs
          for (std::list<Ptr<Packet> >::const_iterator j = (*i)->Begin (); 
               j != (*i)->End (); ++j)
            {
              if (!m_genericPhyRxEndOkCallback.IsNull ())
                {
                  m_genericPhyRxEndOkCallback (*j);
                }
            }
        }
      else
        {
          // TB received with errors
          m_phyRxEndErrorTrace (*i);
        }
    }
  ChangeState (IDLE);
  m_rxPacketBurstList.clear ();
}

void 
LteSpectrumPhy::SetCellId (uint16_t cellId)
{
  m_cellId = cellId;
}

void
LteSpectrumPhy::AddSinrChunkProcessor (Ptr<LteSinrChunkProcessor> p)
{
  m_interference->AddSinrChunkProcessor (p);
}


} // namespace ns3
