/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef YANS_WIFI_HELPER_H
#define YANS_WIFI_HELPER_H

#include "wifi-helper.h"
#include "ns3/trace-helper.h"
#include "ns3/yans-wifi-channel.h"

namespace ns3 {

/**
 * \brief manage and create wifi channel objects for the yans model.
 *
 * The intent of this class is to make it easy to create a channel object
 * which implements the yans channel model. The yans channel model is described
 * in "Yet Another Network Simulator", http://cutebugs.net/files/wns2-yans.pdf
 */
class YansWifiChannelHelper
{
public:
  /**
   * Create a channel helper without any parameter set. The user must set
   * them all to be able to call Create later.
   */
  YansWifiChannelHelper ();

  /**
   * Create a channel helper in a default working state. By default, we create
   * a channel model with a propagation delay equal to a constant, the speed of light,
   * and a propagation loss based on a log distance model with a reference loss of 46.6777 dB
   * at reference distance of 1m.
   */
  static YansWifiChannelHelper Default (void);

  /**
   * \param name the name of the model to add
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
   * Add a propagation loss model to the set of currently-configured loss models.
   * This method is additive to allow you to construct complex propagation loss models
   * such as a log distance + jakes model, etc.
   *
   * The order in which PropagationLossModels are added may be significant. Some
   * propagation models are dependent of the "txPower" (eg. Nakagami model), and
   * are therefore not commutative. The final receive power (excluding receiver
   * gains) are calculated in the order the models are added.
   */
  void AddPropagationLoss (std::string name,
                           std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                           std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                           std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                           std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                           std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                           std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                           std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                           std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  /**
   * \param name the name of the model to set
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
   * Configure a propagation delay for this channel.
   */
  void SetPropagationDelay (std::string name,
                            std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                            std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                            std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                            std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                            std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                            std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                            std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                            std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

  /**
   * \returns a new channel
   *
   * Create a channel based on the configuration parameters set previously.
   */
  Ptr<YansWifiChannel> Create (void) const;

  /**
  * Assign a fixed random variable stream number to the random variables
  * used by the channel.  Typically this corresponds to random variables
  * used in the propagation loss models.  Return the number of streams
  * (possibly zero) that have been assigned.
  *
  * \param c NetDeviceContainer of the set of net devices for which the
  *          WifiNetDevice should be modified to use fixed streams
  * \param stream first stream index to use
  *
  * \return the number of stream indices assigned by this helper
  */
  int64_t AssignStreams (Ptr<YansWifiChannel> c, int64_t stream);


private:
  std::vector<ObjectFactory> m_propagationLoss;
  ObjectFactory m_propagationDelay;
};


/**
 * \brief Make it easy to create and manage PHY objects for the yans model.
 *
 * The yans PHY model is described in "Yet Another Network Simulator",
 * http://cutebugs.net/files/wns2-yans.pdf
 *
 * The Pcap and ascii traces generated by the EnableAscii and EnablePcap methods defined
 * in this class correspond to PHY-level traces and come to us via WifiPhyHelper
 *
 */
class YansWifiPhyHelper : public WifiPhyHelper,
                          public PcapHelperForDevice,
                          public AsciiTraceHelperForDevice
{
public:
  /**
   * Create a phy helper without any parameter set. The user must set
   * them all to be able to call Install later.
   */
  YansWifiPhyHelper ();

  /**
   * Create a phy helper in a default working state.
   */
  static YansWifiPhyHelper Default (void);

  /**
   * \param channel the channel to associate to this helper
   *
   * Every PHY created by a call to Install is associated to this channel.
   */
  void SetChannel (Ptr<YansWifiChannel> channel);
  /**
   * \param channelName The name of the channel to associate to this helper
   *
   * Every PHY created by a call to Install is associated to this channel.
   */
  void SetChannel (std::string channelName);
  /**
   * \param name the name of the attribute to set
   * \param v the value of the attribute
   *
   * Set an attribute of the underlying PHY object.
   */
  void Set (std::string name, const AttributeValue &v);
  /**
   * \param name the name of the error rate model to set.
   * \param n0 the name of the attribute to set
   * \param v0 the value of the attribute to set
   * \param n1 the name of the attribute to set
   * \param v1 the value of the attribute to set
   * \param n2 the name of the attribute to set
   * \param v2 the value of the attribute to set
   * \param n3 the name of the attribute to set
   * \param v3 the value of the attribute to set
   * \param n4 the name of the attribute to set
   * \param v4 the value of the attribute to set
   * \param n5 the name of the attribute to set
   * \param v5 the value of the attribute to set
   * \param n6 the name of the attribute to set
   * \param v6 the value of the attribute to set
   * \param n7 the name of the attribute to set
   * \param v7 the value of the attribute to set
   *
   * Set the error rate model and its attributes to use when Install is called.
   */
  void SetErrorRateModel (std::string name,
                          std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                          std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                          std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                          std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                          std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                          std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                          std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                          std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

  /**
   * An enumeration of the pcap data link types (DLTs) which this helper
   * supports.  See http://wiki.wireshark.org/Development/LibpcapFileFormat
   * for more information on these formats.
   */
  enum SupportedPcapDataLinkTypes
  {
    DLT_IEEE802_11       = PcapHelper::DLT_IEEE802_11,       /**< IEEE 802.11 Wireless LAN headers on packets */
    DLT_PRISM_HEADER     = PcapHelper::DLT_PRISM_HEADER,     /**< Include Prism monitor mode information */
    DLT_IEEE802_11_RADIO = PcapHelper::DLT_IEEE802_11_RADIO  /**< Include Radiotap link layer information */
  };

  /**
   * Set the data link type of PCAP traces to be used. This function has to be
   * called before EnablePcap(), so that the header of the pcap file can be
   * written correctly.
   *
   * @see SupportedPcapDataLinkTypes
   *
   * @param dlt The data link type of the pcap file (and packets) to be used
   */
  void SetPcapDataLinkType (enum SupportedPcapDataLinkTypes dlt);

  /**
   * Get the data link type of PCAP traces to be used.
   *
   * @see SupportedPcapDataLinkTypes
   *
   * @returns The data link type of the pcap file (and packets) to be used
   */
  uint32_t GetPcapDataLinkType (void) const;

private:
  /**
   * \param node the node on which we wish to create a wifi PHY
   * \param device the device within which this PHY will be created
   * \returns a newly-created PHY object.
   *
   * This method implements the pure virtual method defined in \ref ns3::WifiPhyHelper.
   */
  virtual Ptr<WifiPhy> Create (Ptr<Node> node, Ptr<NetDevice> device) const;

  /**
   * @brief Enable pcap output the indicated net device.
   *
   * NetDevice-specific implementation mechanism for hooking the trace and
   * writing to the trace file.
   *
   * @param prefix Filename prefix to use for pcap files.
   * @param nd Net device for which you want to enable tracing.
   * @param promiscuous If true capture all possible packets available at the device.
   * @param explicitFilename Treat the prefix as an explicit filename if true
   */
  virtual void EnablePcapInternal (std::string prefix,
                                   Ptr<NetDevice> nd,
                                   bool promiscuous,
                                   bool explicitFilename);

  /**
   * \brief Enable ascii trace output on the indicated net device.
   *
   * NetDevice-specific implementation mechanism for hooking the trace and
   * writing to the trace file.
   *
   * \param stream The output stream object to use when logging ascii traces.
   * \param prefix Filename prefix to use for ascii trace files.
   * \param nd Net device for which you want to enable tracing.
   * \param explicitFilename Treat the prefix as an explicit filename if true
   */
  virtual void EnableAsciiInternal (Ptr<OutputStreamWrapper> stream,
                                    std::string prefix,
                                    Ptr<NetDevice> nd,
                                    bool explicitFilename);

  ObjectFactory m_phy;
  ObjectFactory m_errorRateModel;
  Ptr<YansWifiChannel> m_channel;
  uint32_t m_pcapDlt;
};

} //namespace ns3

#endif /* YANS_WIFI_HELPER_H */
