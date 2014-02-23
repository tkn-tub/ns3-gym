/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 * Authors:
 *  Gary Pei <guangyu.pei@boeing.com>
 *  Tom Henderson <thomas.r.henderson@boeing.com>
 */
#ifndef LR_WPAN_HELPER_H
#define LR_WPAN_HELPER_H

#include <ns3/node-container.h>
#include <ns3/lr-wpan-phy.h>
#include <ns3/lr-wpan-mac.h>
#include <ns3/trace-helper.h>

namespace ns3 {

class SingleModelSpectrumChannel;
class MobilityModel;

/**
 * \ingroup lr-wpan
 *
 * \brief helps to manage and create IEEE 802.15.4 NetDevice objects
 *
 * This class can help to create IEEE 802.15.4 NetDevice objects
 * and to configure their attributes during creation.  It also contains
 * additional helper functions used by client code.
 */

class LrWpanHelper : public PcapHelperForDevice,
                     public AsciiTraceHelperForDevice
{
public:
  /**
   * \brief Create a LrWpan helper in an empty state.
   */
  LrWpanHelper (void);
  virtual ~LrWpanHelper (void);

  /**
  * \brief Add mobility model to a physical device
  * \param phy the physical device
  * \param m the mobility model
  */
  void AddMobility (Ptr<LrWpanPhy> phy, Ptr<MobilityModel> m);

  /**
   * \param c a set of nodes
   */
  NetDeviceContainer Install (NodeContainer c);

  /**
   * Helper to enable all LrWpan log components with one statement
   */
  void EnableLogComponents (void);

  static std::string LrWpanPhyEnumerationPrinter (LrWpanPhyEnumeration);
  static std::string LrWpanMacStatePrinter (LrWpanMacState e);

private:
  /**
   * \internal
   *
   * \brief Enable pcap output on the indicated net device.
   * \internal
   *
   * NetDevice-specific implementation mechanism for hooking the trace and
   * writing to the trace file.
   *
   * \param prefix Filename prefix to use for pcap files.
   * \param nd Net device for which you want to enable tracing.
   * \param promiscuous If true capture all possible packets available at the device.
   * \param explicitFilename Treat the prefix as an explicit filename if true
   */
  virtual void EnablePcapInternal (std::string prefix, Ptr<NetDevice> nd, bool promiscuous, bool explicitFilename);

  /**
   * \brief Enable ascii trace output on the indicated net device.
   * \internal
   *
   * NetDevice-specific implementation mechanism for hooking the trace and
   * writing to the trace file.
   *
   * \param stream The output stream object to use when logging ascii traces.
   * \param prefix Filename prefix to use for ascii trace files.
   * \param nd Net device for which you want to enable tracing.
   */
  virtual void EnableAsciiInternal (Ptr<OutputStreamWrapper> stream,
                                    std::string prefix,
                                    Ptr<NetDevice> nd,
                                    bool explicitFilename);

private:
  Ptr<SingleModelSpectrumChannel> m_channel;

};

}

#endif /* LR_WPAN_HELPER_H */
