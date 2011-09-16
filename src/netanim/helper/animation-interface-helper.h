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
   * \param fbRx First-bit Receive time
   * \param nd NetDevice where packet was received
   * \param rxRange Reception range 
   *
   */
  AnimRxInfo (const Time& fbRx, Ptr <const NetDevice> nd ,double rxRange)
    : m_fbRx (fbRx.GetSeconds ()), m_lbRx (0), m_rxnd (nd), rxRange (rxRange), m_PhyRxComplete (false){}

  /** 
   * \brief First bit receive time
   * \param m_fbRx First bit receive time
   *
   */
  double m_fbRx;            
  
  /** 
   * \brief Last bit receive time
   * \param m_lbRx Last bit receive time
   *
   */
  double m_lbRx;             

  /**
   * \brief Ptr to receiving NetDevice
   * \param m_rxnd Ptr to receiving NetDevice
   *
   */
  Ptr <const NetDevice> m_rxnd;

  /** 
   * \brief Reception range
   * \param rxRange Reception range
   *
   */
  double rxRange;
  
  /**
   * \brief Check if Wifi Phy Rx is Complete
   * \returns true if Wifi Phy Rx is complete
   *
   */
  bool IsPhyRxComplete ();

  /**
   * \brief Set Wifi Phy Rx Complete
   *
   */
  void SetPhyRxComplete ();

private:
  bool m_PhyRxComplete;

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
   *
   */
  AnimPacketInfo ();
  
  /**
   * \brief Constructor
   * \param tx_nd Ptr to NetDevice that is transmitting
   * \param fbTx First bit transmit time
   * \param lbTx Last bit transmit time
   * \param txLoc Transmitter Location
   *
   */
  AnimPacketInfo(Ptr <const NetDevice> tx_nd, const Time& fbTx, const Time& lbTx,Vector txLoc);
  
  /**
   * \brief Ptr to NetDevice that is transmitting
   * \param m_txnd NetDevice that is transmitting
   *
   */ 
  Ptr <const NetDevice> m_txnd;

  /** 
   * \brief First bit transmission time
   * \param m_fbTx First bit transmission time
   *
   */
  double   m_fbTx;     

  /**
   * \brief Last bit transmission time
   * \param m_lbTx Last bit transmission time
   *
   */
  double   m_lbTx;     

  /**
   * \brief Transmitter's location
   * \param m_txLoc Transmitter's Location
   *
   */
  Vector   m_txLoc;


  /**
   * \brief Collection of receivers
   * \param m_rx Collection of receivers
   *
   */
  std::map<uint32_t,AnimRxInfo> m_rx;

  /**
   * \brief Process RxBegin notifications
   * \param nd Ptr to NetDevice where packet was received
   * \param fbRx First bit receive time
   *
   */
  void ProcessRxBegin (Ptr <const NetDevice> nd, const Time& fbRx);

  /**
   * \brief Process RxEnd notifications
   * \param nd Ptr to NetDevice where packet was received
   * \param fbRx First bit receive time
   * \param rxLoc Location of receiver
   * \returns true if RxEnd notification was expected and processed
   *
   */
  bool ProcessRxEnd (Ptr <const NetDevice> nd, const Time& fbRx, Vector rxLoc);

  /**
   * \brief Process RxDrop notifications
   * \param nd Ptr to NetDevice where packet was dropped on reception
   *
   */
  void ProcessRxDrop (Ptr <const NetDevice> nd);
  
  /**
   * \brief GetRxInfo
   * \param nd Ptr to NetDevice where packet was received
   * \returns AnimRxInfo object
   *
   */ 
  AnimRxInfo GetRxInfo (Ptr <const NetDevice> nd);

  /**
   * \brief RemoveRxInfo
   * \param nd Ptr to NetDevice where packet was received
   *
   */ 
  void RemoveRxInfo (Ptr <const NetDevice> nd);

  /**
   * \brief Time delta between First bit Rx and Last bit Rx
   * \param firstlastbitDelta Time delta between First bit Rx and Last bit Rx
   */
   double firstlastbitDelta;

};

} // namespace ns3

#endif
