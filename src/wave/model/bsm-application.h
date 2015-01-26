/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 North Carolina State University
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
 * Author: Scott E. Carpenter <scarpen@ncsu.edu>
 *
 */

#ifndef BSM_APPLICATION_H
#define BSM_APPLICATION_H

#include "ns3/application.h"
#include "ns3/wave-bsm-stats.h"
#include "ns3/random-variable-stream.h"
#include "ns3/internet-stack-helper.h"

namespace ns3 {
/**
 * \ingroup wave
 * \brief The BsmApplication class sends and receives the
 * IEEE 1609 WAVE (Wireless Access in Vehicular Environments)
 * Basic Safety Messages (BSMs) and uses the WaveBsmStats class
 * to manage statistics about BSMs transmitted and received
 * The BSM is a ~200-byte packet that is
 * generally broadcast from every vehicle at a nominal rate of 10 Hz.
 */
class BsmApplication : public Application
{
public:
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   * \return none
   */
  BsmApplication ();
  virtual ~BsmApplication ();

  /**
   * \brief Setup BSM generation parameters for a node
   * \param i IPv4 interface container
   * \param nodeId identifier of the node (index into container)
   * \param totalTime total amount of time that BSM packets should be transmitted
   * \param wavePacketSize the size, in bytes, of a WAVE BSM
   * \param waveInterval the time, in seconds, between each WAVE BSM transmission,
   * typically 10 Hz (0.1 second)
   * \param gpsAccuracy the timing synchronization accuracy of GPS time, in seconds.
   * GPS time-sync is ~40-100 ns.  Universally synchronized time among all vehicles
   * will result in all vehicles transmitting safety messages simultaneously, leading
   * to excessive wireless collisions.
   * \param range the expected transmission range, in m ^ 2.
   * \param collection class for WAVE BSM statistics
   * \param indicators of whether or not node(s) are moving
   * \return none
   */
  void Setup (Ipv4InterfaceContainer & i,
              int nodeId,
              Time totalTime,
              uint32_t wavePacketSize, // bytes
              Time waveInterval,
              double gpsAccuracyNs,
              std::vector <double> rangesSq,          // m ^ 2
              Ptr<WaveBsmStats> waveBsmStats,
              std::vector<int> * nodesMoving,
              int mode,
              Time txDelay);

  /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.  The Install() method should have previously been
  * called by the user.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this helper
  */
  int64_t AssignStreams (int64_t streamIndex);

  /**
  * (Arbitrary) port number that is used to create a socket for transmitting WAVE BSMs.
  */
  static int wavePort;

protected:
  virtual void DoDispose (void);

private:
  // inherited from Application base class.
  virtual void StartApplication (void);    // Called at time specified by Start
  virtual void StopApplication (void);     // Called at time specified by Stop

  /**
   * \brief Creates and transmits a WAVE BSM packet
   * \param socket socket to use for transmission
   * \param pktSize the size, in bytes, of the WAVE BSM packet
   * \param pktCount the number of remaining WAVE BSM packets to be transmitted
   * \param pktInterval the interval, in seconds, until the next packet
   * should be transmitted
   * \return none
   */
  void GenerateWaveTraffic (Ptr<Socket> socket, uint32_t pktSize,
                            uint32_t pktCount, Time pktInterval,
                            uint32_t sendingNodeId);

  /**
   * \brief Receive a WAVE BSM packet
   * \param socket the receiving socket
   * \return none
   */
  void ReceiveWavePacket (Ptr<Socket> socket);

  /**
   * \brief Handle the receipt of a WAVE BSM packet from sender to receiver
   * \param txNode the sending node
   * \param rxNode the receiving node
   * \return none
   */
  void HandleReceivedBsmPacket (Ptr<Node> txNode,
                                Ptr<Node> rxNode);

  /**
   * \brief Get the node for the desired id
   * \param id the id of the desired node
   * \return ptr to the desired node
   */
  Ptr<Node> GetNode (int id);

  /**
   * \brief Get the net device for the desired id
   * \param id the id of the desired net device
   * \return ptr to the desired net device
   */
  Ptr<NetDevice> GetNetDevice (int id);

  Ptr<WaveBsmStats> m_waveBsmStats;
  // tx safety range squared, for optimization
  std::vector <double> m_txSafetyRangesSq;
  Time m_TotalSimTime;
  uint32_t m_wavePacketSize; // bytes
  uint32_t m_numWavePackets;
  Time m_waveInterval;
  double m_gpsAccuracyNs;
  Ipv4InterfaceContainer * m_adhocTxInterfaces;
  std::vector<int> * m_nodesMoving;
  Ptr<UniformRandomVariable> m_unirv;
  int m_nodeId;
  // WAVE channel access mode.  0=continuous PHY; 1=channel-switching
  int m_chAccessMode;
  // When transmitting at a default rate of 10 Hz,
  // the subsystem shall transmit every 100 ms +/-
  // a random value between 0 and 5 ms. [MPR-BSMTX-TXTIM-002]
  // Source: CAMP Vehicle Safety Communications 4 Consortium
  // On-board Minimum Performance Requirements
  // for V2V Safety Systems Version 1.0, December 17, 2014
  // max transmit delay (default 10ms)
  Time m_txMaxDelay;
  Time m_prevTxDelay;
};

} // namespace ns3

#endif /* BSM_APPLICATION_H*/
