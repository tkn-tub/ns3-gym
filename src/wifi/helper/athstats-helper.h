/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 */

#ifndef ATHSTATS_HELPER_H
#define ATHSTATS_HELPER_H

#include <string>
#include "ns3/object.h"
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/nstime.h"
#include "ns3/wifi-phy.h"
#include "ns3/double.h"
#include "ns3/mac48-address.h"

namespace ns3 {


class NetDevice;

/**
 * @brief create AthstatsWifiTraceSink instances and connect them to wifi devices
 *
 *
 */
class AthstatsHelper
{
public:
  AthstatsHelper ();
  void EnableAthstats (std::string filename,  uint32_t nodeid, uint32_t deviceid);
  void EnableAthstats (std::string filename, Ptr<NetDevice> nd);
  void EnableAthstats (std::string filename, NetDeviceContainer d);
  void EnableAthstats (std::string filename, NodeContainer n);

private:
  Time m_interval;
};




/**
 * @brief trace sink for wifi device that mimics madwifi's athstats tool.
 *
 * The AthstatsWifiTraceSink class is a trace sink to be connected to several of the traces
 * available within a wifi device. The purpose of AthstatsWifiTraceSink is to
 * mimic the behavior of the athstats tool distributed wih the madwifi
 * driver. In particular, the reproduced behavior is that obtained
 * when executing athstats without parameters: a report written in
 * text format is produced every fixed interval, based on the events
 * observed by the wifi device.
 *
 * Differences with the "real" athstats:
 *
 * - AthstatsWifiTraceSink is expected to write its output to a file
 *   (not to stdout).
 *
 * - only a subset of the metrics supported by athstats is supported
 *   by AthstatsWifiTraceSink
 *
 * - AthstatsWifiTraceSink does never produce a cumulative report.
 */
class AthstatsWifiTraceSink  : public Object
{
public:
  static TypeId GetTypeId (void);
  AthstatsWifiTraceSink ();
  virtual ~AthstatsWifiTraceSink ();


  /**
   * function to be called when the net device transmits a packet
   *
   * @param context
   * @param p the packet being transmitted
   */
  void DevTxTrace (std::string context, Ptr<const Packet> p);

  /**
   * function to be called when the net device receives a packet
   *
   * @param context
   * @param p the packet being received
   */
  void DevRxTrace (std::string context, Ptr<const Packet> p);

  /**
   * Function to be called when a RTS frame transmission by the considered
   * device has failed
   *
   * @param context
   * @param address the MAC address of the remote station
   */
  void TxRtsFailedTrace (std::string context, Mac48Address address);

  /**
   * Function to be called when a data frame transmission by the considered
   * device has failed
   *
   * @param context
   * @param address the MAC address of the remote station
   */
  void TxDataFailedTrace (std::string context, Mac48Address address);

  /**
   * Function to be called when the transmission of a RTS frame has
   * exceeded the retry limit
   *
   * @param context
   * @param address the MAC address of the remote station
   */
  void TxFinalRtsFailedTrace (std::string context, Mac48Address address);

  /**
   * Function to be called when the transmission of a data frame has
   * exceeded the retry limit
   *
   * @param context
   * @param address the MAC address of the remote station
   */
  void TxFinalDataFailedTrace (std::string context, Mac48Address address);

  /**
   * Function to be called when the PHY layer  of the considered
   * device receives a frame
   *
   * @param context
   * @param packet
   * @param snr
   * @param mode
   * @param preamble
   */
  void PhyRxOkTrace (std::string context, Ptr<const Packet> packet, double snr, WifiMode mode, enum WifiPreamble preamble);

  /**
   * Function to be called when a frame reception by the PHY
   * layer  of the considered device resulted in an error due to a failure in the CRC check of
   * the frame
   *
   * @param context
   * @param packet
   * @param snr
   */
  void PhyRxErrorTrace (std::string context, Ptr<const Packet> packet, double snr);

  /**
   * Function to be called when a frame is being transmitted by the
   * PHY layer of the considered device
   *
   * @param context
   * @param packet
   * @param mode
   * @param preamble
   * @param txPower
   */
  void PhyTxTrace (std::string context, Ptr<const Packet> packet, WifiMode mode, WifiPreamble preamble, uint8_t txPower);

  /**
   * Function to be called when the PHY layer of the considered device
   * changes state
   *
   * @param context
   * @param start
   * @param duration
   * @param state
   */
  void PhyStateTrace (std::string context, Time start, Time duration, enum WifiPhy::State state);

  /**
   * Open a file for output
   *
   * @param name the name of the file to be opened.
   */
  void Open (std::string const& name);

private:
  /**
   * @internal
   */
  void WriteStats ();

  /**
   * @internal
   */
  void ResetCounters ();

  uint32_t m_txCount;
  uint32_t m_rxCount;
  uint32_t m_shortRetryCount;
  uint32_t m_longRetryCount;
  uint32_t m_exceededRetryCount;
  uint32_t m_phyRxOkCount;
  uint32_t m_phyRxErrorCount;
  uint32_t m_phyTxCount;

  std::ofstream *m_writer;

  Time m_interval;

}; // class AthstatsWifiTraceSink




} // namespace ns3




#endif /* ATHSTATS_HELPER_H */
