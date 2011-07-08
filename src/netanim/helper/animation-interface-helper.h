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
 * Author: John Abraham <john.abraham@gatech.edu>
 */

// Animation Interface helpers

#ifndef _ANIMATION_INTERFACE_HELPER_H_
#define _ANIMATION_INTERFACE_HELPER_H_

#include "ns3/node.h"
#include "ns3/mobility-model.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include <vector>
#include <map>

namespace ns3 {


/**
 * \ingroup netanim
 *
 * \brief AnimRxInfo helper class 
 *
 * This helper class keeps of wireless packets received
 * including info about the first bit received time,
 * last bit received time and NetDevice received on
 * It is intended only for use by the AnimationInterface
 * class.
 * 
 */

class AnimRxInfo
{
public:

  /**
   * \brief Default constructor
   *
   */
  AnimRxInfo () {};

  /**
   * \brief Constructor
   * \param First-bit Receive time
   * \param Ptr to NetDevice used for reception
   *
   */
  AnimRxInfo (const Time& fbRx, Ptr <const NetDevice> nd)
    : m_fbRx (fbRx.GetSeconds ()), m_lbRx (0), m_rxnd (nd) {}

  /* 
   * First bit receive time
   */
  double m_fbRx;            
  
  /* 
   * Last bit receive time
   */
  double m_lbRx;             

  /*
   * Ptr to receiving Net Device
   */
  Ptr <const NetDevice> m_rxnd;
};

/**
 * \ingroup netanim
 *
 * \brief AnimPacketInfo helper class
 *
 * This helper class keeps of wireless packets transmitted and
 * received
 * including info about the last bit transmit time, first bit 
 * transmit time, location of the transmitter and
 * NetDevice transmited on
 * It is intended only for use by the AnimationInterface
 * class.
 *
 */

class AnimPacketInfo

{
public:

  /**
   * \brief Default constructor
   */
  AnimPacketInfo ();
  
  /**
   * \brief Constructor
   * \param Ptr to NetDevice transmitted on
   * \param First bit transmit time
   * \param Last bit transmit time
   * \param Transmitter Location
   *
   */
  AnimPacketInfo(Ptr<const NetDevice> nd,
                 const Time& fbTx, const Time& lbTx,Vector txLoc);

  /**
   * Ptr to NetDevice used for transmission
   */
  Ptr<const NetDevice> m_txnd;

  /**
   * Number of receivers
   */
  uint32_t m_nRx;      

  /** 
   * Number of RxEnd trace callbacks
   */
  uint32_t m_nRxEnd;   

  /** 
   * First bit transmission time
   */
  double   m_fbTx;     

  /**
   * Last bit transmission time
   */
  double   m_lbTx;     

  /**
   * Transmitter's location
   */
  Vector   transmitter_loc;

  /** 
   * Receptiion range
   */
  double reception_range;

  /**
   * Collection of receivers
   */
  std::map<uint32_t,AnimRxInfo> m_rx;

  /**
   * \brief Process RxBegin notifications
   * \param Ptr to NetDevice where packet was received
   * \param First bit receive time
   * \param Location of the transmitter
   *
   */
  void ProcessRxBegin (Ptr <const NetDevice> nd, const Time& fbRx,
                   Vector rxLoc);

  /**
   * \brief Process RxEnd notifications
   * \param Ptr to NetDevice where packet was received
   * \param First bit receive time
   *
   */
  bool ProcessRxEnd (Ptr <const NetDevice> nd, const Time& fbRx);

  /**
   * \brief Process RxDrop notifications
   * \param Ptr to NetDevice where packet was dropped on reception
   *
   */
  void ProcessRxDrop (Ptr <const NetDevice> nd);

  /**
   * \brief Time delta between First bit Rx and Last bit Rx
   *
   */
   double firstlastbitDelta;

};

} // namespace ns3

#endif
