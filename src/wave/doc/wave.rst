WAVE models
-----------
WAVE is a system architecture for wireless-based vehicular communications, 
specified by the IEEE.  This chapter documents available models for WAVE
within |ns3|.  The focus is on the MAC layer and MAC extension layer
defined by [ieee80211p]_.

.. include:: replace.txt

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

Model Description
*****************

WAVE is an overall system architecture for vehicular communications.
The standards for specifying WAVE include a set of extensions to the IEEE
802.11 standard, found in IEEE Std 802.11p-2010 [ieee80211p]_, and
the IEEE 1609 standard set, consisting of four documents:  
resource manager:  IEEE 1609.1 [ieee1609dot1]_,
security services:  IEEE 1609.2 [ieee1609dot2]_,
network and transport layer services:  IEEE 1609.3 [ieee1609dot3]_,
and multi-channel coordination:  IEEE 1609.4 [ieee1609dot4]_.

In |ns3|, the focus of the ``wave`` module is on the MAC layer.
The key design aspect of WAVE-compilant MACs is that they allow
communications outside the context of a basic service set (BSS).
The literature uses the acronym OCB to denote "outside the context
of a BSS", and the class ``ns3::OcbWifiMac`` models this in |ns3|. 
Many management frames will not be used, but when used, the BSSID field 
needs to be set to a wildcard BSSID value. 
Management information is transmitted by what is called a vendor specific 
action frame. 

With these changes, the packet transmissions (for a moving vehicle) can
be fast with small delay.  At the physical layer, the biggest difference is 
to use the 5.9 GHz band with a channel bandwidth of 10 MHz.  These physical
layer changes can make the wireless signal relatively more stable,
without degrading throughput too much (ranging from 3 Mbps to 27 Mbps), 
although 20 MHz channel bandwidth is still supported.

The source code for the WAVE MAC models lives in the directory
``src/wave``.

The current code represents an interim capability to realize an
802.11p-compliant device, but without the WAVE extensions (which
are planned for a later patch).  In vehicular communications using
WAVE, radios have the capability of switching between control and
service channels, or using multiple radios.  These aspects are not
yet modelled.  The WaveNetDevice modelled herein enforces that
a WAVE-compliant physical layer (at 5.9 GHz) is selected, and 
does not require any association between devices (similar to an 
adhoc WiFi MAC), but is otherwise similar (at this time) to a
WifiNetDevice.

Design
======

In |ns3|, support for 802.11p involves the MAC and PHY layers.
To use an 802.11p NetDevice, ``ns3::Wifi80211pHelper`` is suggested.

MAC layer
#########

The classes used to model the MAC layer are ``ns3::OrganizationIdentifier``,
``ns3::VendorSpecificActionHeader``, ``ns3::HigherDataTxVectorTag``,
``ns3::WaveMacLow``, and ``ns3::OcbWifiMac``.

The OrganizationIdentifier and VendorSpecificActionHeader are used to support 
the sending of a Vendor Specific Action frame. The HigherDataTxVectorTag 
and WaveMacLow are used to support higher control transmission parameters. 
These classes are all used in OcbWifiMac.

OcbWifiMac is very similar to AdhocWifiMac, with some modifications. 
(|ns3| AdhocWifiMac class is implemented very close to the 802.11p OCB 
mode rather than a real 802.11 ad-hoc mode. The AdhocWifiMac has no BSS 
context that is defined in 802.11 standard, so it will not take time to 
send beacon and authenticate.)

1. SetBssid, GetBssid, SetSsid, GetSsid
   these methods are related to 802.11 BSS context which is unused in OCB context.
2. SetLinkUpCallback, SetLinkDownCallback
   WAVE device can send packets directly, so the WiFi link is never down.
3. SendVsc, AddReceiveVscCallback
   WAVE management information shall be sent by vendor specific action frame, 
   and it will be called by upper layer 1609.4 standard to send WSA
   (WAVE Service Advertisement) packets or other vendor specific information.
4. SendTimingAdvertisement (not implemented)
   Although Timing Advertisement is very important and specifically defined in 
   802.11p standard, it is not useful in a simulation environment. 
   Every node in |ns3| vehicular simulation is assumed to be already time 
   synchronized (perhaps by GPS).
5. ConfigureEdca
   This method will allow the user to set EDCA parameters of WAVE channeles 
   including CCH ans SCHs. And the OcbWifiMac itself also uses this method 
   to configure default 802.11p EDCA parameters.
6. WILDCARD BSSID
   The WILDCARD BSSID is set to "ff:ff:ff:ff:ff:ff".
   As defined in 802.11-2007, a wildcard BSSID shall not be used in the
   BSSID field except for management frames of subtype probe request. But Adhoc 
   mode of |ns3| simplifies this mechanism:  when stations receive packets, 
   packets regardless of BSSID will be forwarded up to the higher layer. 
   This process is very close 
   to OCB mode as defined in 802.11p-2010, in which stations use the wildcard 
   BSSID to allow the higher layer of other stations to hear directly.
7. Enqueue, Receive
   The most important methods are send and receive methods. According to the 
   standard, we should filter the frames that are not permitted. However here we 
   just identify the frames we care about; the other frames will be discarded.

PHY layer
#########
Actually, no modification or extension happens in the |ns3| PHY layer
corresponding to this model.
In the 802.11p standard, the PHY layer wireless technology is still 80211a OFDM with 10MHz channel width,
so Wifi80211pHelper will only allow the user to set the standard 
to WIFI_PHY_STANDARD_80211_10MHZ or WIFI_PHY_STANDARD_80211_20MHZ
(WIFI_PHY_STANDARD_80211a with 20MHz is supported, but not recommended.)
The maximum station transmit power and maximum permitted EIRP defined in 
802.11p is larger 
than that of WiFi, so transmit range can normally become longer than 
usual WiFi.  However, this feature will 
not be implemented. Users who want to obtain longer range should configure 
attributes "TxPowerStart", 
"TxPowerEnd" and "TxPowerLevels" of the YansWifiPhy class.

Scope and Limitations
=====================

1. Does the model involve vehicular mobility of some sort?

Vehicular networks involve not only communication protocols, but also 
a communication environment 
including vehicular mobility and propagation models. Because of specific 
features of the latter, the protocols need to change. The MAC layer model 
in this 
project just adapts MAC changes to vehicular environment. However this model 
does not involve any
vehicular mobility with time limit. Users can use any mobility model in |ns3|, 
but should understand that vehicular mobility is out of scope for the
current WAVE module.

2. Is this model going to use different propagation models?

Referring to the first issue, some more realistic propagation models 
for vehicualr environment
are suggested and welcome. And some existing propagation models in |ns3| are 
also suitable. 
Normally, users can use Friis, Two-Ray Ground, and Nakagami models. 

3. Are there any vehicular application models to drive the code?

About vehicular application models, SAE J2375 depends on WAVE architecture and
is an application message set in US; CAM and DENM in Europe between network 
and application layer, but is very close to application model. The BSM in 
J2375 and CAM send alert messages that every 
vehicle node will sent periodicity about its status information to 
cooperate with others. 
Fow now, a vehicular application model is not within scope.

References
==========

.. [ieee80211p] IEEE Std 802.11p-2010 "IEEE Standard for Information technology-- Local and metropolitan area networks-- Specific requirements-- Part 11: Wireless LAN Medium Access Control (MAC) and Physical Layer (PHY) Specifications Amendment 6: Wireless Access in Vehicular Environments"

.. [ieee1609dot1] IEEE Std 1609.1-2010 "IEEE Standard for Wireless Access in Vehicular Environments (WAVE) - Resource Manager, 2010"

.. [ieee1609dot2] IEEE Std 1609.2-2010 "IEEE Standard for Wireless Access in Vehicular Environments (WAVE) - Security Services for Applications and Management Messages, 2010"

.. [ieee1609dot3] IEEE Std 1609.3-2010 "IEEE Standard for Wireless Access in Vehicular Environments (WAVE) - Networking Services, 2010"

.. [ieee1609dot4] IEEE Std 1609.4-2010 "IEEE Standard for Wireless Access in Vehicular Environments (WAVE) - Multi-Channel Operation, 2010"

Usage
*****

Helpers
=======
The helpers include ``ns3::NqosWaveMacHelper``, ``ns3::QosWaveMacHelper``, 
``ns3::Wifi80211pHelper`` and ``ns3::WaveHelper``. Wifi80211pHelper is used 
to create 
802.11p devices that follow the 802.11p-2010 standard. WaveHelper is 
used to create WAVE devices that follow the 802.11p-2010 and 1609.4-2010 
standards which are the MAC and PHY layers of the WAVE architecture. 
The relation of them is described as below:

::

    WifiHelper ----------use---------->  WifiMacHelper
        ^                             ^       ^
        |                             |       |
        |                          inherit  inherit
        |                             |       |
        |                 QosWifiMacHelper  NqosWifiMacHelper
        |                             ^       ^
        |                             |       |
      inherit                     inherit  inherit
        |                             |       |
      Wifi80211pHelper     QosWaveMacHelper  NqosWaveHelper

Although Wifi80211Helper can use any subclasses inheriting from 
WifiMacHelper, we force users to use subclasses inheriting from 
QosWaveMacHelper or NqosWaveHelper.

Although the functions of WiFi 802.11p device can be achieved by 
WaveNetDevice's ContinuousAccess assignment, Wifi80211pHelper is recommeneded
if there is no need for multiple channel operation.
Usage is as follows:

::

    NodeContainer nodes;
    NetDeviceContainer devices;
    nodes.Create (2);
    YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
    wifiPhy.SetChannel (wifiChannel.Create ());
    NqosWave80211pMacHelper wifi80211pMac = NqosWaveMacHelper::Default();
    Wifi80211pHelper 80211pHelper = Wifi80211pHelper::Default ();
    devices = 80211pHelper.Install (wifiPhy, wifi80211pMac, nodes);

APIs
====

The 802.11p device can allow the upper layer to send different information
over Vendor Specific Action management frames by using different
OrganizationIdentifier fields to identify differences.

1. already create a Node object and WifiNetDevice object
2. define an OrganizationIdentifier

::

   uint8_t oi_bytes[5] = {0x00, 0x50, 0xC2, 0x4A, 0x40};
   OrganizationIdentifier oi(oi_bytes,5);

3. define a Callback for the defined OrganizationIdentifier

::

   VscCallback vsccall = MakeCallback (&VsaExample::GetWsaAndOi, this);

4. OcbWifiMac of 802.11p device registers this identifier and function

::

      Ptr<WifiNetDevice> device1 = DynamicCast<WifiNetDevice>(nodes.Get (i)->GetDevice (0));
      Ptr<OcbWifiMac> ocb1 = DynamicCast<OcbWifiMac>(device->GetMac ());
      ocb1->AddReceiveVscCallback (oi, vsccall);

5. now one can send management packets over VSA frames

::

      Ptr<Packet> vsc = Create<Packet> ();
      ocb2->SendVsc (vsc, Mac48Address::GetBroadcast (), m_16093oi);

6. then registered callbacks in other devices will be called.

Attributes
==========

The current classes do not provide any additional attributes beyond those
in the WiFi module.

Output
======

The current classes provide output of the same type as WiFi devices;
namely, ASCII and pcap traces, and logging output.  The WAVE logging
components can be enabled globally via the call to

::
  
  Wifi80211pHelper::EnableLogComponents ();


Advanced Usage
==============

To be defined.

Examples
========

A basic example exists called ``wave-simple-80211p.cc``.
This example shows basic construction of an 802.11p node.  Two nodes
are constructed with 802.11p devices, and by default, one node sends a single
packet to another node (the number of packets and interval between
them can be configured by command-line arguments).  The example shows
typical usage of the helper classes for this mode of WiFi.

Troubleshooting
===============

To be defined.

Validation
**********

A single test suite named ``wifi-80211p-ocb`` is defined.  This test
case consists of a stationary node and a mobile node.  The mobile
node moves towards the stationary mode, and time points are checked
at which time the physical layer starts to receive packets (and
whether the MAC becomes associated, if applicable).  The same physical
experiment is repeated for normal WiFi NetDevices in AP/STA mode, in
Adhoc mode, and the new OCB mode.

