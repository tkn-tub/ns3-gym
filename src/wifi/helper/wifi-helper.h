/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 *          Mirko Banchi <mk.banchi@gmail.com>
 */

#ifndef WIFI_HELPER_H
#define WIFI_HELPER_H

#include <string>
#include "ns3/attribute.h"
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/wifi-phy-standard.h"
#include "ns3/trace-helper.h"
#include "ns3/wifi-mac-helper.h"
#include "ns3/wifi-phy.h"

namespace ns3 {

class WifiPhy;
class WifiNetDevice;
class Node;

/**
 * \brief create PHY objects
 *
 * This base class must be implemented by new PHY implementation which wish to integrate
 * with the \ref ns3::WifiHelper class.
 */
class WifiPhyHelper : public PcapHelperForDevice,
                      public AsciiTraceHelperForDevice
{
public:
  WifiPhyHelper ();
  virtual ~WifiPhyHelper ();

  /**
   * \param node the node on which the PHY object will reside
   * \param device the device within which the PHY object will reside
   *
   * \returns a new PHY object.
   *
   * Subclasses must implement this method to allow the ns3::WifiHelper class
   * to create PHY objects from ns3::WifiHelper::Install.
   *
   * Typically the device type will be of class WifiNetDevice but the
   * type of the pointer is generalized so that this method may be used
   * by other Wifi device variants such as WaveNetDevice.
   */
  virtual Ptr<WifiPhy> Create (Ptr<Node> node, Ptr<NetDevice> device) const = 0;
  
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
  PcapHelper::DataLinkType GetPcapDataLinkType (void) const;

protected:
  /**
   * \param file the pcap file wrapper
   * \param packet the packet
   * \param channelFreqMhz the channel frequency
   * \param channelNumber the channel number
   * \param rate the PHY bitrate
   * \param preamble the preamble type
   * \param txVector the TXVECTOR
   * \param aMpdu the A-MPDU information
   *
   * Handle tx pcap.
   */
  static void PcapSniffTxEvent (Ptr<PcapFileWrapper> file,
                                Ptr<const Packet> packet,
                                uint16_t channelFreqMhz,
                                uint16_t channelNumber,
                                uint32_t rate,
                                WifiPreamble preamble,
                                WifiTxVector txVector,
                                struct mpduInfo aMpdu);
  /**
   * \param file the pcap file wrapper
   * \param packet the packet
   * \param channelFreqMhz the channel frequency
   * \param channelNumber the channel number
   * \param rate the PHY bitrate
   * \param preamble the preamble type
   * \param txVector the TXVECTOR
   * \param aMpdu the A-MPDU information
   * \param signalNoise the rx signal and noise information
   *
   * Handle rx pcap.
   */
  static void PcapSniffRxEvent (Ptr<PcapFileWrapper> file,
                                Ptr<const Packet> packet,
                                uint16_t channelFreqMhz,
                                uint16_t channelNumber,
                                uint32_t rate,
                                WifiPreamble preamble,
                                WifiTxVector txVector,
                                struct mpduInfo aMpdu,
                                struct signalNoiseDbm signalNoise);
    
  ObjectFactory m_phy;
  ObjectFactory m_errorRateModel;
    
private:
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
    
  PcapHelper::DataLinkType m_pcapDlt;
};


/**
 * \brief helps to create WifiNetDevice objects
 *
 * This class can help to create a large set of similar
 * WifiNetDevice objects and to configure a large set of
 * their attributes during creation.
 */
class WifiHelper
{
public:
  virtual ~WifiHelper ();

  /**
   * Create a Wifi helper in an empty state: all its parameters
   * must be set before calling ns3::WifiHelper::Install
   *
   * The default state is defined as being an Adhoc MAC layer with an ARF rate control algorithm
   * and both objects using their default attribute values. 
   * By default, configure MAC and PHY for 802.11a.
   */
  WifiHelper ();

  /**
   * \returns a new WifiHelper in a default state
   *
   * The default state is defined as being an Adhoc MAC layer with an ARF rate control algorithm
   * and both objects using their default attribute values. By default, configure MAC and PHY
   * for 802.11a.
   *
   * \deprecated This method will go away in future versions of ns-3.
   * The constructor of the class is now performing the same job, which makes this function useless.
   */
  NS_DEPRECATED
  static WifiHelper Default (void);

  /**
   * \param type the type of ns3::WifiRemoteStationManager to create.
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
   * All the attributes specified in this method should exist
   * in the requested station manager.
   */
  void SetRemoteStationManager (std::string type,
                                std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
                                std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
                                std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
                                std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
                                std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
                                std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
                                std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
                                std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());
  /**
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param c the set of nodes on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  virtual NetDeviceContainer Install (const WifiPhyHelper &phy,
                                      const WifiMacHelper &mac, NodeContainer c) const;
  /**
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param node the node on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  virtual NetDeviceContainer Install (const WifiPhyHelper &phy,
                                      const WifiMacHelper &mac, Ptr<Node> node) const;
  /**
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param nodeName the name of node on which a wifi device must be created
   * \returns a device container which contains all the devices created by this method.
   */
  virtual NetDeviceContainer Install (const WifiPhyHelper &phy,
                                      const WifiMacHelper &mac, std::string nodeName) const;
  /**
   * \param standard the phy standard to configure during installation
   *
   * This method sets standards-compliant defaults for WifiMac
   * parameters such as sifs time, slot time, timeout values, etc.,
   * based on the standard selected.  It results in
   * WifiMac::ConfigureStandard(standard) being called on each
   * installed mac object.
   *
   * The default standard of 802.11a will be applied if SetStandard()
   * is not called.
   *
   * Note that WifiMac::ConfigureStandard () will overwrite certain
   * defaults in the attribute system, so if a user wants to manipulate
   * any default values affected by ConfigureStandard() while using this
   * helper, the user should use a post-install configuration such as
   * Config::Set() on any objects that this helper creates, such as:
   * \code
   * Config::Set ("/NodeList/0/DeviceList/0/$ns3::WifiNetDevice/Mac/Slot", TimeValue (MicroSeconds (slot)));
   * \endcode
   *
   * \sa WifiMac::ConfigureStandard
   * \sa Config::Set
   */
  virtual void SetStandard (enum WifiPhyStandard standard);

  /**
   * Helper to enable all WifiNetDevice log components with one statement
   */
  static void EnableLogComponents (void);

  /**
  * Assign a fixed random variable stream number to the random variables
  * used by the Phy and Mac aspects of the Wifi models.  Each device in
  * container c has fixed stream numbers assigned to its random variables.
  * The Wifi channel (e.g. propagation loss model) is excluded.
  * Return the number of streams (possibly zero) that
  * have been assigned. The Install() method should have previously been
  * called by the user.
  *
  * \param c NetDeviceContainer of the set of net devices for which the
  *          WifiNetDevice should be modified to use fixed streams
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this helper
  */
  int64_t AssignStreams (NetDeviceContainer c, int64_t stream);


protected:
  ObjectFactory m_stationManager;
  enum WifiPhyStandard m_standard;
};

} //namespace ns3

#endif /* WIFI_HELPER_H */
