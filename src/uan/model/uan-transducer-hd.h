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

#ifndef UAN_TRANSDUCER_HD_H
#define UAN_TRANSDUCER_HD_H

#include "uan-transducer.h"
#include "ns3/simulator.h"
namespace ns3 {

/**
 * \class UanTransducerHd
 * \brief Half duplex implementation of transducer object
 *
 * This class will only allow attached Phy's to receive packets if not in TX mode
 */
class UanTransducerHd : public UanTransducer
{
public:
  UanTransducerHd ();
  virtual ~UanTransducerHd ();

  static TypeId GetTypeId (void);

  // inherited methods
  virtual State GetState (void) const;
  virtual bool IsRx (void) const;
  virtual bool IsTx (void) const;
  virtual const ArrivalList &GetArrivalList (void) const;
  virtual void Receive (Ptr<Packet> packet, double rxPowerDb, UanTxMode txMode, UanPdp pdp);
  virtual void Transmit (Ptr<UanPhy> src, Ptr<Packet> packet, double txPowerDb, UanTxMode txMode);
  virtual void SetChannel (Ptr<UanChannel> chan);
  virtual Ptr<UanChannel> GetChannel (void) const;
  virtual void AddPhy (Ptr<UanPhy>);
  virtual const UanPhyList &GetPhyList (void) const;
  virtual void Clear (void);

private:
  State m_state;
  ArrivalList m_arrivalList;
  UanPhyList m_phyList;
  Ptr<UanChannel> m_channel;
  EventId m_endTxEvent;
  Time m_endTxTime;
  bool m_cleared;

  void RemoveArrival (UanPacketArrival arrival);
  void EndTx (void);
protected:
  virtual void DoDispose ();
};

}

#endif /* UAN_TRANSDUCER_HD_H */
