/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 * Author: Raj Bhattacharjea <raj.b@gatech.edu>
 * typedefs for tcp state machine
 */ 

#include <vector>
#include <map>
#include "sequence-number.h"

#ifndef TCP_TYPEDEFS_H
#define TCP_TYPEDEFS_H

namespace ns3 {

typedef enum { MAX_FLAGS = 0x40 } TCPMaxFlags_t;  // Flags are 6 bits

typedef enum {
  CLOSED,       // 0
  LISTEN,       // 1
  SYN_SENT,     // 2
  SYN_RCVD,     // 3
  ESTABLISHED,  // 4
  CLOSE_WAIT,   // 5
  LAST_ACK,     // 6
  FIN_WAIT_1,   // 7
  FIN_WAIT_2,   // 8
  CLOSING,      // 9
  TIMED_WAIT,   // 10
  LAST_STATE } States_t;

typedef enum {
  APP_LISTEN,   // 0
  APP_CONNECT,  // 1
  APP_SEND,     // 2
  SEQ_RECV,     // 3
  APP_CLOSE,    // 4
  TIMEOUT,      // 5
  ACK_RX,       // 6
  SYN_RX,       // 7
  SYN_ACK_RX,   // 8
  FIN_RX,       // 9
  FIN_ACK_RX,   // 10
  RST_RX,       // 11
  BAD_FLAGS,    // 12
  LAST_EVENT } Events_t;

typedef enum {
  NO_ACT,       // 0
  ACK_TX,       // 1
  ACK_TX_1,     // ACK response to syn
  RST_TX,       // 2
  SYN_TX,       // 3
  SYN_ACK_TX,   // 4
  FIN_TX,       // 5
  FIN_ACK_TX,   // 6
  NEW_ACK,      // 7
  NEW_SEQ_RX,   // 8
  RETX,         // 9
  TX_DATA,      // 10
  PEER_CLOSE,   // 11
  APP_CLOSED,   // 12
  CANCEL_TM,    // 13
  APP_NOTIFY,   // 14 - Notify app that connection failed
  SERV_NOTIFY,  // 15 - Notify server tcp that connection completed
  LAST_ACTION } Actions_t;

class SA  // State/Action pair
{
public:
  SA () : state (LAST_STATE), action (LAST_ACTION) { }
  SA (States_t s, Actions_t a) : state (s), action (a) { }
public:
  States_t  state;
  Actions_t action;
};
typedef std::vector<SA>  StateActionVec_t;
typedef std::vector<StateActionVec_t> StateActions_t;  // One per current state
typedef std::vector<Events_t> EventVec_t;      // For flag events lookup

//type for managing buffered out of sequence data
typedef std::map<SequenceNumber, Ptr<Packet> > UnAckData_t;

class TcpStateMachine {
  public:
    TcpStateMachine ();
    SA Lookup (States_t, Events_t);
    Events_t FlagsEvent (uint8_t); // Lookup event from flags

  public:
    StateActions_t aT; // Action table
    EventVec_t     eV; // Flags event lookup  
};

}//namespace ns3
#endif //TCP_TYPEDEFS_H
