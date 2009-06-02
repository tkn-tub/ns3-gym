/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Authors: Joe Kopena <tjkopena@cs.drexel.edu>
 *
 * These applications are used in the WiFi Distance Test experiment,
 * described and implemented in test02.cc.  That file should be in the
 * same place as this file.  The applications have two very simple
 * jobs, they just generate and receive packets.  We could use the
 * standard Application classes included in the NS-3 distribution.
 * These have been written just to change the behavior a little, and
 * provide more examples.
 *
 */

// #define NS3_LOG_ENABLE // Now defined by Makefile

#include "ns3/core-module.h"
#include "ns3/common-module.h"
#include "ns3/application.h"

#include "ns3/stats-module.h"

using namespace ns3;

//----------------------------------------------------------------------
//------------------------------------------------------
class Sender: public Application {
public:
  static TypeId GetTypeId(void);
  Sender();
  virtual ~Sender();

protected:
  virtual void DoDispose(void);

private:
  virtual void StartApplication(void);
  virtual void StopApplication(void);

  void SendPacket();

  uint32_t        m_pktSize;
  Ipv4Address     m_destAddr;
  uint32_t        m_destPort;
  uint32_t        m_numPkts;
  DataRate        m_cbrRate;      // Rate that data is generated
  Time            nextTime;       // Time till next packet
  
  Ptr<Socket>     m_socket;
  EventId         m_sendEvent;

  TracedCallback<Ptr<const Packet> > m_txTrace;

  uint32_t        m_count;

  // end class Sender
};




//------------------------------------------------------
class Receiver: public Application {
public:
  static TypeId GetTypeId(void);
  Receiver();
  virtual ~Receiver();

  void SetCounter(Ptr<CounterCalculator<> > calc);
  void SetDelayTracker(Ptr<TimeMinMaxAvgTotalCalculator> delay);

protected:
  virtual void DoDispose(void);

private:
  virtual void StartApplication(void);
  virtual void StopApplication(void);

  void Receive(Ptr<Socket> socket);

  Ptr<Socket>     m_socket;

  uint32_t        m_port;

  Ptr<CounterCalculator<> > m_calc;
  Ptr<TimeMinMaxAvgTotalCalculator> m_delay;

  // end class Receiver
};




//------------------------------------------------------
class TimestampTag : public Tag {
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  
  // these are our accessors to our tag structure
  void SetTimestamp(Time time);
  Time GetTimestamp(void) const;

  void Print(std::ostream &os) const;

private:
  Time m_timestamp;

  // end class TimestampTag
};
