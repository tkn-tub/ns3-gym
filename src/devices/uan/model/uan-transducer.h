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
#ifndef UANTRANSDUCER_H_
#define UANTRANSDUCER_H_

#include "ns3/object.h"
#include "ns3/packet.h"
#include "uan-tx-mode.h"
#include "ns3/uan-prop-model.h"

#include <list>
namespace ns3 {

class UanPhy;
class UanChannel;


/**
 * \class UanPacketArrival
 *
 * \brief Class consisting of packet arrival information (Time, RxPower, mode, PDP)
 */
class UanPacketArrival
{
public:

  /* default constructor so python scan works */
  UanPacketArrival ()
  {
  }

  /**
   * \param packet Packet arriving
   * \param rxPowerDb RX signal power in dB of arriving packet
   * \param txMode TX mode of arriving packet
   * \param pdp Power delay profile of arriving packet
   * \param arrTime Arrival time of packet
   */
  UanPacketArrival (Ptr<Packet> packet, double rxPowerDb, UanTxMode txMode, UanPdp pdp, Time arrTime)
    : m_packet (packet),
      m_rxPowerDb (rxPowerDb),
      m_txMode (txMode),
      m_pdp (pdp),
      m_arrTime (arrTime)
  {
  }

  ~UanPacketArrival()
  {
    m_packet = 0;
  }

  /**
   * \returns Pointer to packet that arrived
   */
  inline Ptr<Packet> GetPacket (void) const
  {
    return m_packet;
  }
  /**
   * \returns Received signal strength in dB re 1uPa
   */
  inline double GetRxPowerDb (void) const
  {
    return m_rxPowerDb;
  }
  /**
   * \returns UanTxMode used to transmit packet
   */
  inline const UanTxMode &GetTxMode (void) const
  {
    return m_txMode;
  }
  /**
   * \returns Arrival time of packet
   */
  inline Time GetArrivalTime (void) const
  {
    return m_arrTime;
  }
  /**
   * \returns PDP of arriving signal
   */
  inline UanPdp GetPdp (void) const
  {
    return m_pdp;
  }
private:
  Ptr<Packet> m_packet;
  double m_rxPowerDb;
  UanTxMode m_txMode;
  UanPdp m_pdp;
  Time m_arrTime;
};

/**
 * \class UanTransducer
 * \brief Virtual base for Transducer objects
 *
 * The Transducer was added to support classes such as UanPhyDual.
 * In a generic Phy setting, this class functions to hold information about all
 * possibly interfering packets.
 */
class UanTransducer : public Object
{
public:
  /**
   * \brief Transducer state (receiving or transmitting)
   */
  enum State {
    TX, RX
  };

  /**
   * \brief Arrival list is a standard template library list of UanPacketArrivals objects
   */
  typedef std::list<UanPacketArrival> ArrivalList;
  /**
   * \brief UanPhyList is a standard template library list of UanPhy objects
   */
  typedef std::list<Ptr<UanPhy> > UanPhyList;

  /**
   * \returns State (TX or RX) of this transducer
   */
  virtual State GetState (void) const = 0;

  /**
   * \returns True if this transducer is available for receiving an incoming packet.
   */
  virtual bool IsRx (void) const = 0;
  /**
   * \returns True if there is a packet being transmitted from this transducer.
   */
  virtual bool IsTx (void) const = 0;
  /**
   * \returns List of all packets currently crossing this node in the water.
   */
  virtual const ArrivalList &GetArrivalList (void) const = 0;
  /**
   * \brief Receive Notify this object that a new packet has arrived at this nodes location
   * \param packet Packet arriving
   * \param rxPowerDb Signal power in dB of arriving packet
   * \param txMode Mode arriving packet is using
   * \param pdp PDP of arriving signal
   */
  virtual void Receive (Ptr<Packet> packet, double rxPowerDb, UanTxMode txMode, UanPdp pdp) = 0;
  /**
   * \brief Transmit a packet from this transducer
   * \param src Source PHY
   * \param packet Packet to transmit
   * \param txPowerDb Outgoing Tx power of packet
   * \param txMode Mode to transmit packet with.
   */
  virtual void Transmit (Ptr<UanPhy> src, Ptr<Packet> packet, double txPowerDb, UanTxMode txMode) = 0;
  /**
   * \param chan Channel this transducer is attached to
   */
  virtual void SetChannel (Ptr<UanChannel> chan) = 0;
  /**
   * \returns Channel this transducer is attached to
   */
  virtual Ptr<UanChannel> GetChannel (void) const = 0;
  /**
   * \param phy Add phy above this transducer (may connect > 1 Phy to a transducer)
   */
  virtual void AddPhy (Ptr<UanPhy> phy) = 0;
  /**
   * \returns List of all Phy's this transducer sends packets to.
   */
  virtual const UanPhyList &GetPhyList (void) const = 0;
  /**
   * Clears all pointer references
   */
  virtual void Clear (void) = 0;

};

} // namespace ns3

#endif /*UANTRANSDUCER_H_*/
