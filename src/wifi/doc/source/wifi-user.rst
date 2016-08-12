.. include:: replace.txt
.. highlight:: cpp

++++++++++++++++++
User Documentation
++++++++++++++++++

Using the WifiNetDevice
***********************

The modularity provided by the implementation makes low-level configuration of
the WifiNetDevice powerful but complex. For this reason, we provide some helper
classes to perform common operations in a simple matter, and leverage the |ns3|
attribute system to allow users to control the parametrization of the underlying
models.

Users who use the low-level |ns3| API and who wish to add a WifiNetDevice to
their node must create an instance of a WifiNetDevice, plus a number of
constituent objects, and bind them together appropriately (the WifiNetDevice is
very modular in this regard, for future extensibility). At the low-level API,
this can be done with about 20 lines of code (see ``ns3::WifiHelper::Install``,
and ``ns3::YansWifiPhyHelper::Create``). They also must create, at some point, a
WifiChannel, which also contains a number of constituent objects (see
``ns3::YansWifiChannelHelper::Create``).

However, a few helpers are available for users to add these devices and channels
with only a few lines of code, if they are willing to use defaults, and the
helpers provide additional API to allow the passing of attribute values to
change default values.  Commonly used attribute values are listed in the
Attributes section.  The scripts in ``examples/wireless`` can be browsed to
see how this is done.  Next, we describe the common steps to create a WifiNetDevice
from the bottom layer (WifiChannel) up to the device layer (WifiNetDevice).

To create a WifiNetDevice, users need to follow these steps:

* Decide on which physical layer framework, the ``SpectrumWifiPhy`` or 
  ``YansWifiPhy``, to use.  This will affect which Channel and Phy type to use. 
* Configure the WifiChannel: WifiChannel takes care of getting signal
  from one device to other devices on the same wifi channel.
  The main configurations of WifiChannel are propagation loss model and propagation delay model.
* Configure the WifiPhy: WifiPhy takes care of actually sending and receiving wireless
  signal from WifiChannel.  Here, WifiPhy decides whether each frame will be successfully
  decoded or not depending on the received signal strength and noise.  Thus, the main
  configuration of WifiPhy is the error rate model, which is the one that actually 
  calculates the probability of successfully decoding the frame based on the signal.
* Configure WifiMac: this step is more on related to the architecture and device level.
  The users configure the wifi architecture (i.e. ad-hoc or ap-sta) and whether QoS (802.11e), HT (802.11n) and/or VHT (802.11ac) features are supported or not.
* Create WifiDevice: at this step, users configure the desired wifi standard
  (e.g. **802.11b**, **802.11g**, **802.11a**, **802.11n** or **802.11ac**) and rate control algorithm
* Configure mobility: finally, mobility model is (usually) required before WifiNetDevice
  can be used.

The following sample code illustrates a typical configuration using mostly
default values in the simulator, and infrastructure mode::

  NodeContainer wifiStaNode;
  wifiStaNode.Create (10);   // Create 10 station node objects
  NodeContainer wifiApNode;
  wifiApNode.Create (1);   // Create 1 access point node object

  // Create a channel helper and phy helper, and then create the channel
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  // Create a WifiMacHelper, which is reused across STA and AP configurations
  WifiMacHelper mac;

  // Create a WifiHelper, which will use the above helpers to create
  // and install Wifi devices.  Configure a Wifi standard to use, which
  // will align various parameters in the Phy and Mac to standard defaults.
  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
  // Declare NetDeviceContainers to hold the container returned by the helper
  NetDeviceContainer wifiStaDevices;
  NetDeviceContainer wifiApDevice;

  // Perform the installation
  mac.SetType ("ns3::StaWifiMac");
  wifiStaDevices = wifi.Install (phy, mac, wifiStaNodes);
  mac.SetType ("ns3::ApWifiMac");
  wifiApDevice = wifi.Install (phy, mac, wifiApNode);

At this point, the 11 nodes have Wi-Fi devices configured, attached to a 
common channel.  The rest of this section describes how additional 
configuration may be performed.

YansWifiChannelHelper
=====================

The YansWifiChannelHelper has an unusual name. Readers may wonder why it is
named this way. The reference is to the `yans simulator
<http://cutebugs.net/files/wns2-yans.pdf>`_ from which this model is taken. The
helper can be used to create a WifiChannel with a default PropagationLoss and
PropagationDelay model.

Users will typically type code such as::

  YansWifiChannelHelper wifiChannelHelper = YansWifiChannelHelper::Default ();
  Ptr<WifiChannel> wifiChannel = wifiChannelHelper.Create ();

to get the defaults.  Specifically, the default is a channel model with a
propagation delay equal to a constant, the speed of light (``ns3::ConstantSpeedPropagationDelayModel``),
and a propagation loss based on a default log distance model (``ns3::LogDistancePropagationLossModel``)), using a default exponent of 3.
Please note that the default log distance model is configured with a reference
loss of 46.6777 dB at reference distance of 1m.  The reference loss of 46.6777 dB
was calculated using Friis propagation loss model at 5.15 GHz.  The reference loss
must be changed if **802.11b**, **802.11g** or **802.11n** (at 2.4 GHz) are used since they operate at 2.4 Ghz.

Note the distinction above in creating a helper object vs. an actual simulation
object.  In |ns3|, helper objects (used at the helper API only) are created on
the stack (they could also be created with operator new and later deleted).
However, the actual |ns3| objects typically inherit from ``class ns3::Object``
and are assigned to a smart pointer.  See the chapter in the |ns3| manual for
a discussion of the |ns3| object model, if you are not familiar with it.

The following two methods are useful when configuring YansWifiChannelHelper:

* ``YansWifiChannelHelper::AddPropagationLoss`` adds a PropagationLossModel
  to a chain of PropagationLossModel
* ``YansWifiChannelHelper::SetPropagationDelay`` sets a PropagationDelayModel

YansWifiPhyHelper
=================

Physical devices (base class ``ns3::WifiPhy``) connect to ``ns3::WifiChannel`` models in
|ns3|.  We need to create WifiPhy objects appropriate for the YansWifiChannel; here
the ``YansWifiPhyHelper`` will do the work.

The YansWifiPhyHelper class configures an object factory to create instances of
a ``YansWifiPhy`` and adds some other objects to it, including possibly a
supplemental ErrorRateModel and a pointer to a MobilityModel. The user code is
typically::

  YansWifiPhyHelper wifiPhyHelper = YansWifiPhyHelper::Default ();
  wifiPhyHelper.SetChannel (wifiChannel);

The default YansWifiPhyHelper is configured with NistErrorRateModel
(``ns3::NistErrorRateModel``). You can change the error rate model by
calling the ``YansWifiPhyHelper::SetErrorRateModel`` method.

Optionally, if pcap tracing is needed, a user may use the following
command to enable pcap tracing::

  YansWifiPhyHelper::SetPcapDataLinkType (enum SupportedPcapDataLinkTypes dlt)

|ns3| supports RadioTap and Prism tracing extensions for 802.11.

Note that we haven't actually created any WifiPhy objects yet; we've just
prepared the YansWifiPhyHelper by telling it which channel it is connected to.
The Phy objects are created in the next step.

802.11n/ac PHY layer can use either either long (800 ns) or short (400 ns) OFDM guard intervals. To configure this parameter, the following line of code could be used (in this example, it enables the support of a short guard interval)::

 wifiPhyHelper.Set ("ShortGuardEnabled", BooleanValue(true));

In order to enable 802.11n/ac MIMO, the number of Rx antennas as well as the number of Tx antennas need to be configured.
For example, this code enables 2x2 MIMO::

 wifiPhyHelper.Set ("TxAntennas", UintegerValue (2));
 wifiPhyHelper.Set ("RxAntennas", UintegerValue (2));

Furthermore, 802.11n provides an optional mode (GreenField mode) to reduce preamble durations and which is only compatible with 802.11n devices. This mode is enabled as follows::

 wifiPhyHelper.Set ("GreenfieldEnabled",BooleanValue(true));

802.11n PHY layer can support both 20 (default) or 40 MHz channel width, and 802.11ac PHY layer can use either 20, 40, 80 (default) or 160 MHz channel width. Since the channel width value is overwritten by ``WifiHelper::SetStandard``, this should be done post-install using ``Config::Set``::

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211ac);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue("VHtMcs9"), "ControlMode", StringValue("VhtMcs0"));

  //Install PHY and MAC
  Ssid ssid = Ssid ("ns3-wifi");
  WifiMacHelper mac;

  mac.SetType ("ns3::StaWifiMac",
  "Ssid", SsidValue (ssid),
  "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevice;
  staDevice = wifi.Install (phy, mac, wifiStaNode);

  mac.SetType ("ns3::ApWifiMac",
  "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  //Once install is done, we overwrite the channel width value
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (160));

Channel, frequency, and channel width configuration
===================================================

There are a few ``ns3::WifiPhy`` parameters that are related, and cannot
be set completely independently, concerning the frequency and channel width
that the device is tuned to.  These are:

* ``WifiPhyStandard``:  For example, 802.11b, 802.11n, etc.
* ``Frequency``
* ``ChannelWidth``
* ``ChannelNumber``

It is possible to set the above to incompatible combinations (e.g. channel
number 1 with 40 MHz channel width on frequency 4915 MHz).  In addition,
the latter three values above are attributes; it is possible to set them
in a number of ways:

* by setting global configuration default; e.g.

::

  Config::SetDefault ("ns3::WifiPhy::ChannelNumber", UintegerValue (3));

* by setting an attribute value in the helper; e.g.

::

  YansWifiPhyHelper wifiPhyHelper = YansWifiPhyHelper::Default ();
  wifiPhyHelper.Set ("ChannelNumber", UintegerValue (3));


* by setting the WifiHelper::SetStandard (enum WifiPhyStandard) method; and

* by performing post-installation configuration of the option, either
  via a Ptr to the WifiPhy object, or through the Config namespace; e.g.:

::

  Config::Set ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/$ns3::WifiPhy/ChannelNumber", UintegerValue (3));

This section provides guidance on how to configure these settings in
a coherent manner, and what happens if non-standard values are chosen.

WifiHelper::SetStandard()
+++++++++++++++++++++++++

``WifiHelper::SetStandard ()`` is a method to set various parameters
in the Mac and Phy to standard values and some reasonable defaults.
For example, ``SetStandard (WIFI_PHY_STANDARD_80211a)`` will set the
WifiPhy to Channel 36 in the 5 GHz band, among other settings.

The following values for WifiPhyStandard are defined in 
``src/wifi/model/wifi-phy-standard.h``:

::

  /** OFDM PHY for the 5 GHz band (Clause 17) */
  WIFI_PHY_STANDARD_80211a,
  /** DSSS PHY (Clause 15) and HR/DSSS PHY (Clause 18) */
  WIFI_PHY_STANDARD_80211b,
  /** ERP-OFDM PHY (Clause 19, Section 19.5) */
  WIFI_PHY_STANDARD_80211g,
  /** OFDM PHY for the 5 GHz band (Clause 17 with 10 MHz channel bandwidth) */
  WIFI_PHY_STANDARD_80211_10MHZ,
  /** OFDM PHY for the 5 GHz band (Clause 17 with 5 MHz channel bandwidth) */
  WIFI_PHY_STANDARD_80211_5MHZ,
  /** This is intended to be the configuration used in this paper:
   *  Gavin Holland, Nitin Vaidya and Paramvir Bahl, "A Rate-Adaptive
   *  MAC Protocol for Multi-Hop Wireless Networks", in Proc. of
   *  ACM MOBICOM, 2001.
   */
  WIFI_PHY_STANDARD_holland,
  /** HT OFDM PHY for the 2.4 GHz band (clause 20) */
  WIFI_PHY_STANDARD_80211n_2_4GHZ,
  /** HT OFDM PHY for the 5 GHz band (clause 20) */
  WIFI_PHY_STANDARD_80211n_5GHZ,
  /** VHT OFDM PHY (clause 22) */
  WIFI_PHY_STANDARD_80211ac

In addition, a value WIFI_PHY_STANDARD_UNSPECIFIED is defined to indicate
that the user has not set a standard.

By default, the WifiPhy will be initialized to WIFI_PHY_STANDARD_UNSPECIFIED,
when it is created directly by ``CreateObject`` (i.e. not by WifiHelper).
However, the WifiHelper (the typical use case for WifiPhy creation) will 
configure the WIFI_PHY_STANDARD_80211a standard by default.  Other values 
for standards should be passed explicitly to the WifiHelper object.

If user has not already separately configured Frequency or ChannelNumber
when SetStandard is called, the user obtains default values, in addition
(e.g. channel 1 for 802.11b/g, or channel 36 for a/n), in addition to
an appropriate ChannelWidth value for the standard (typically, 20 MHz, but
80 MHz for 802.11ac).

WifiPhy attribute interactions
++++++++++++++++++++++++++++++

Users should keep in mind that the two attributes that matter most
within the model code are ``WifiPhy::Frequency`` and 
``WifiPhy::ChannelWidth``; these are the ones directly used to set
transmission parameters.  ``WifiPhy::ChannelNumber`` and 
``WifiHelper::SetStandard ()`` are convenience shorthands for setting
frequency and channel width.  The ``ns3::WifiPhy`` contains code to
keep these values aligned and to generate runtime errors in some cases
if users set these attributes to incompatible values.

The pair (WifiPhyStandard, ChannelNumber) is an alias for a pair of 
(Frequency/ChannelWidth) items.  Valid combinations are stored in 
a map within WifiPhy that is populated with well-known values but that
can be dynamically extended at runtime.  

WifiPhy::Frequency
++++++++++++++++++

The WifiPhy channel center frequency is set by the attribute ``Frequency``
in the class ``WifiPhy``.  It is expressed in units of MHz.  By default,
this attribute is set to the value 0 to indicate that no value is configured.

Note that this is a change in definition from ns-3.25 and earlier releases,
where this attribute referred to the start of the overall frequency band
on which the channel resides, not the specific channel center frequency.

WifiPhy::ChannelWidth
+++++++++++++++++++++

The WifiPhy channel width is set by the attribute ``ChannelWidth``
in the class ``WifiPhy``.  It is expressed in units of MHz.  By default,
this attribute is set to the value 20.  Allowable values are 5, 10, 20,
22, 40, 80, or 160 (MHz).

WifiPhy::ChannelNumber
++++++++++++++++++++++

Several channel numbers are defined and well-known in practice.  However,
valid channel numbers vary by geographical region around the world, and
there is some overlap between the different standards.

In |ns3|, the class ``WifiPhy`` contains an attribute ``ChannelNumber`` that
is, by default, set to the value 0.  The value 0 indicates that no
channel number has been set by the user.

In |ns3|, a ChannelNumber may be defined or unknown.  These terms
are not found in the code; they are just used to describe behavoir herein.

If a ChannelNumber is defined, it means that WifiPhy has stored a
map of ChannelNumber to the center frequency and channel width commonly
known for that channel in practice.  For example:

* Channel 1, when IEEE 802.11b is configured, corresponds to a channel
  width of 22 MHz and a center frequency of 2412 MHz.  

* Channel 36, when IEEE 802.11n is configured at 5GHz, corresponds to 
  a channel width of 20 MHz and a center frequency of 5180 MHz.  

The following channel numbers are well-defined for 2.4 GHz standards:

* channels 1-14 with ChannelWidth of 22 MHz for 802.11b
* channels 1-14 with ChannelWidth of 20 MHz for 802.11n-2.4GHz and 802.11g

The following channel numbers are well-defined for 5 GHz standards:

+------------------+-------------------------------------------+
| ``ChannelWidth`` | ``ChannelNumber``                         |
+------------------+-------------------------------------------+
| 20 MHz           | 36, 40, 44, 48, 52, 56, 60, 64, 100,      |
|                  | 104, 108, 112, 116, 120, 124,             |
|                  | 128, 132, 136, 140, 144,                  |
|                  | 149, 153, 161, 165, 169                   |
+------------------+-------------------------------------------+
| 40 MHz           | 38, 46, 54, 62, 102, 110, 118, 126,       |
|                  | 134, 142, 151, 159                        |
+------------------+-------------------------------------------+
| 80 MHz           | 42, 58, 106, 122, 138, 155                |
+------------------+-------------------------------------------+
| 160 MHz          | 50, 114                                   |
+------------------+-------------------------------------------+
| 10 MHz (802.11p) | 172, 174, 176, 178, 180, 182, 184         |
+------------------+-------------------------------------------+

The channel number may be set either before or after creation of the
WifiPhy object.  

If an unknown channel number (other than zero) is configured, the
simulator will exit with an error; for instance, such as:

::

  Ptr<WifiPhy> wifiPhy = ...;
  wifiPhy->SetAttribute ("ChannelNumber", UintegerValue (1321));

The known channel numbers are defined in the implementation file
``src/wifi/model/wifi-phy.cc``.  Of course, this file may be edited
by users to extend to additional channel numbers.  Below, we describe 
how new channel numbers may be defined dynamically at run-time.

If a known channel number is configured against an incorrect value
of the WifiPhyStandard, the simulator will exit with an error; for instance,
such as:

::

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
  ...
  Ptr<WifiPhy> wifiPhy = ...;
  wifiPhy->SetAttribute ("ChannelNumber", UintegerValue (14));

In the above, while channel number 14 is well-defined in practice for 802.11b
only, it is for 2.4 GHz band, not 5 GHz band.

Defining a new channel number
+++++++++++++++++++++++++++++

Users may define their own channel number so that they can later refer to
the channel by number.  

The method is ``WifiPhy::DefineChannelNumber ()`` and it takes the following
arguments:

* uint16_t channelNumber
* enum WifiPhyStandard standard
* uint32_t frequency
* uint32_t channelWidth

The pair of (channelNumber, standard) are used as an index to a map that
returns a Frequency and ChannelWidth.  By calling this method, one can
dynamically add members to the map.  For instance, let's suppose that you
previously configured WIFI_PHY_STANDARD_80211a, and wanted to deine a new
channel number '34' of width 20 MHz and at center frequency 5160 MHz.

If you try to simply configure ChannelNumber to the value 34, it will fail,
since 34 is undefined.  However, you can use DefineChannelNumber as follows:

::

  Ptr<WifiPhy> wifiPhy = ...;
  wifiPhy->DefineChannelNumber (34, WIFI_PHY_STANDARD_80211a, 5160, 20);

and then later you can refer to channel number 34 in your program, which
will configure a center operating frequency of 5160 MHz and a width of
20 MHz.

The steps can be repeated to explicitly configure the same channel for
multiple standards:

::

  wifiPhy->DefineChannelNumber (34, WIFI_PHY_STANDARD_80211a, 5160, 20);
  wifiPhy->DefineChannelNumber (34, WIFI_PHY_STANDARD_80211n_5GHZ, 5160, 20);

or for a wildcard, unspecified standard:

::

  wifiPhy->DefineChannelNumber (34, WIFI_PHY_STANDARD_UNSPECIFIED, 5160, 20);

Order of operation issues
+++++++++++++++++++++++++

Depending on the default values used and the order of operation in setting
the values for the standard, channel width, frequency, and channel number,
different configurations can be obtained.   Below are some common use cases.

* **(accepting the standard defaults):**  If a user has not already 
  separately configured frequency or channel number when 
  ``WifiHelper::SetStandard ()`` is called, the user gets default values 
  (e.g. channel 1 for 802.11b/g or channel 36 for a/n, with 20 MHz 
  channel widths)

* **(overwriting the standard channel):**  If the user has previously 
  configured (e.g. via SetDefault) either frequency or channel number when 
  SetStandard is called, and the frequency or channel number are appropriate 
  for the standard being configured, they are not overwritten

* **(changing the standard channel after Install):**  The user may also call
  ``WifiHelper::SetStandard ()`` after ``Install ()`` and either configure
  the frequency to something different, or configure the channel number
  to something different.  Note that if the channel number is undefined
  for the standard that was previously set, an error will occur.  

* **(changing to non-standard frequency):**  If the user configures a 
  frequency outside the standardized frequency range for the current 
  WifiPhyStandard, this is OK.  This allows users to experiment with 
  wifi on e.g. whitespace frequencies but still use SetStandard to set 
  all of the other configuration details.

* **(interaction between channel number and frequency):**  If the user 
  the user sets Frequency to a different value than the currently configured
  ChannelNumber (or if ChannelNumber is zero), then the ChannelNumber is 
  set to a new channel number if known, or to zero if unknown. 

  * *example:*  ChannelNumber previously set to 36, user sets Frequency to 5200, then ChannelNumber gets automatically set to 40
  * *example:*  ChannelNumber set to 36, user later sets Frequency to 5185, ChannelNumber gets reset to 0

In summary, ChannelNumber and Frequency follow each other.  ChannelNumber
sets both Frequency and ChannelWidth if the channel number has been defined
for the standard.  Setting ChannelWidth has no effect on Frequency or
ChannelNumber.  Setting Frequency will set ChannelNumber to either the
defined value for that Wi-Fi standard, or to the value 0 if undefined.

SpectrumWifiPhyHelper
=====================

The API for this helper closely tracks the API of the YansWifiPhyHelper,
with the exception that a channel of type ``ns3::SpectrumChannel`` instead
of type ``ns3::WifiChannel`` must be used with it.

WifiMacHelper
=============

The next step is to configure the MAC model. We use WifiMacHelper to accomplish this.
WifiMacHelper takes care of both the MAC low model and MAC high model, and configures an object factory to create instances of a ``ns3::WifiMac``.
It is used to configure MAC parameters like type of MAC, and to select whether 802.11/WMM-style QoS and/or 802.11n-style High throughput (HT) and/or 802.11ac-style Very High throughput (VHT) support are/is required.

By default, it creates an ad-hoc MAC instance that does not have 802.11e/WMM-style QoS nor 802.11n-style High throughput (HT) nor 802.11ac-style Very High throughput (VHT) support enabled.

For example the following user code configures a non-QoS and non-(V)HT MAC that
will be a non-AP STA in an infrastructure network where the AP has
SSID ``ns-3-ssid``::

    WifiMacHelper wifiMacHelper;
    Ssid ssid = Ssid ("ns-3-ssid");
    wifiMacHelper.SetType ("ns3::StaWifiMac",
                          "Ssid", SsidValue (ssid),
                          "ActiveProbing", BooleanValue (false));

The following code shows how to create an AP with QoS enabled::

  WifiMacHelper wifiMacHelper;
  wifiMacHelper.SetType ("ns3::ApWifiMac",
                         "Ssid", SsidValue (ssid),
                         "QosSupported", BooleanValue (true),
                         "BeaconGeneration", BooleanValue (true),
                         "BeaconInterval", TimeValue (Seconds (2.5)));

To create ad-hoc MAC instances, simply use ``ns3::AdhocWifiMac`` instead of ``ns3::StaWifiMac`` or ``ns3::ApWifiMac``.

With QoS-enabled MAC models it is possible to work with traffic belonging to
four different Access Categories (ACs): **AC_VO** for voice traffic,
**AC_VI** for video traffic, **AC_BE** for best-effort
traffic and **AC_BK** for background traffic.

When selecting **802.11n** as the desired wifi standard, both 802.11e/WMM-style QoS and 802.11n-style High throughput (HT) support gets enabled.
Similarly, When selecting **802.11ac** as the desired wifi standard, 802.11e/WMM-style QoS, 802.11n-style High throughput (HT) and 802.11ac-style Very High throughput (VHT) support gets enabled.

For MAC instances that have QoS support enabled, the ``ns3::WifiMacHelper`` can be also used to set:

* block ack threshold (number of packets for which block ack mechanism should be used);
* block ack inactivity timeout.

For example the following user code configures a MAC that will be a non-AP STA with QoS enabled and a block ack threshold for AC_BE set to 2 packets,
in an infrastructure network where the AP has SSID ``ns-3-ssid``::

    WifiMacHelper wifiMacHelper;
    Ssid ssid = Ssid ("ns-3-ssid");
    wifiMacHelper.SetType ("ns3::StaWifiMac",
                          "Ssid", SsidValue (ssid),
                          "QosSupported", BooleanValue (true),
                          "BE_BlockAckThreshold", UintegerValue (2),
                          "ActiveProbing", BooleanValue (false));

For MAC instances that have 802.11n-style High throughput (HT) and/or 802.11ac-style Very High throughput (VHT) support enabled, the ``ns3::WifiMacHelper`` can be also used to set:

* MSDU aggregation parameters for a particular Access Category (AC) in order to use 802.11n/ac A-MSDU feature;
* MPDU aggregation parameters for a particular Access Category (AC) in order to use 802.11n/ac A-MPDU feature.

By defaut, MSDU aggregation feature is disabled for all ACs and MPDU aggregation is enabled for AC_VI and AC_BE, with a maximum aggregation size of 65535 bytes.

For example the following user code configures a MAC that will be a non-AP STA with HT and QoS enabled, MPDU aggregation enabled for AC_VO with a maximum aggregation size of 65535 bytes, and MSDU aggregation enabled for AC_BE with a maximum aggregation size of 7935 bytes,
in an infrastructure network where the AP has SSID ``ns-3-ssid``::

    WifiHelper wifi;
    wifi.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);

    WifiMacHelper wifiMacHelper;
    Ssid ssid = Ssid ("ns-3-ssid");
    wifiMacHelper.SetType ("ns3::StaWifiMac",
                          "Ssid", SsidValue (ssid),
                          "VO_MaxAmpduSize", UintegerValue (65535),
                          "BE_MaxAmsduSize", UintegerValue (7935),
                          "ActiveProbing", BooleanValue (false));

Selection of the Access Category (AC)
+++++++++++++++++++++++++++++++++++++

The selection of the Access Category (AC) for an MSDU is based on the value of
the DS field in the IP header of the packet (ToS field in case of IPv4, Traffic
Class field in case of IPv6) and is performed similarly to what is done by the
Linux mac80211 subsystem. Basically, the :cpp:func:`ns3::WifiNetDevice::SelectQueue()`
method sets the user priority (UP) of an MSDU to the three most significant
bits of the DS field. The Access Category is then determined according to the
following table:

===  ===============
UP   Access Category
===  ===============
 7     AC_VO
 6     AC_VO
 5     AC_VI
 4     AC_VI
 3     AC_BE
 0     AC_BE
 2     AC_BK
 1     AC_BK
===  ===============

Readers can refer to the doxygen documentation of the
:cpp:func:`ns3::WifiNetDevice::SelectQueue()` method for more information,
including how DSCP values map onto user priorities and access categories.

Note that :cpp:func:`ns3::WifiNetDevice::SelectQueue()` also sets the packet
priority to the user priority, thus overwriting the value determined by the
socket priority (users can read :ref:`Socket-options` for details on how to
set the packet priority). Also, given that the Traffic Control layer calls
:cpp:func:`ns3::WifiNetDevice::SelectQueue()` before enqueuing the packet
into a queue disc, it turns out that queuing disciplines (such as the default
PfifoFastQueueDisc) that classifies packets based on their priority will
use the user priority instead of the socket priority.

WifiHelper
==========

We're now ready to create WifiNetDevices. First, let's create
a WifiHelper with default settings::

  WifiHelper wifiHelper;

What does this do?  It sets the default wifi standard to **802.11a** and sets the RemoteStationManager to
``ns3::ArfWifiManager``.  You can change the RemoteStationManager by calling the
``WifiHelper::SetRemoteStationManager`` method. To change the wifi standard, call the
``WifiHelper::SetStandard`` method with the desired standard.

Now, let's use the wifiPhyHelper and wifiMacHelper created above to install WifiNetDevices
on a set of nodes in a NodeContainer "c"::

  NetDeviceContainer wifiContainer = WifiHelper::Install (wifiPhyHelper, wifiMacHelper, c);

This creates the WifiNetDevice which includes also a WifiRemoteStationManager, a
WifiMac, and a WifiPhy (connected to the matching WifiChannel).

The ``WifiHelper::SetStandard`` method set various default timing parameters as defined in the selected standard version, overwriting values that may exist or have been previously configured.
In order to change parameters that are overwritten by ``WifiHelper::SetStandard``, this should be done post-install using ``Config::Set``::

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue("HtMcs7"), "ControlMode", StringValue("HtMcs0"));

  //Install PHY and MAC
  Ssid ssid = Ssid ("ns3-wifi");

  WifiMacHelper mac;
  mac.SetType ("ns3::StaWifiMac",
  "Ssid", SsidValue (ssid),
  "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevice;
  staDevice = wifi.Install (phy, mac, wifiStaNode);

  mac.SetType ("ns3::ApWifiMac",
  "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevice;
  apDevice = wifi.Install (phy, mac, wifiApNode);

  //Once install is done, we overwrite the standard timing values
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/Slot", TimeValue (MicroSeconds (slot)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/Sifs", TimeValue (MicroSeconds (sifs)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/AckTimeout", TimeValue (MicroSeconds (ackTimeout)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/CtsTimeout", TimeValue (MicroSeconds (ctsTimeout)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/Rifs", TimeValue (MicroSeconds (rifs)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/BasicBlockAckTimeout", TimeValue (MicroSeconds (basicBlockAckTimeout)));
  Config::Set ("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/CompressedBlockAckTimeout", TimeValue (MicroSeconds (compressedBlockAckTimeout)));

There are many |ns3| attributes that can be set on the above helpers to
deviate from the default behavior; the example scripts show how to do some of
this reconfiguration.

Mobility configuration
======================

Finally, a mobility model must be configured on each node with Wi-Fi device.
Mobility model is used for calculating propagation loss and propagation delay.
Two examples are provided in the next section.
Users are referred to the chapter on :ref:`Mobility` module for detailed information.

Example configuration
=====================

We provide two typical examples of how a user might configure a Wi-Fi network --
one example with an ad-hoc network and one example with an infrastructure network.
The two examples were modified from the two examples in the ``examples/wireless`` folder
(``wifi-simple-adhoc.cc`` and ``wifi-simple-infra.cc``).
Users are encouraged to see examples in the ``examples/wireless`` folder.

AdHoc WifiNetDevice configuration
+++++++++++++++++++++++++++++++++

In this example, we create two ad-hoc nodes equipped with 802.11a Wi-Fi devices.
We use the ``ns3::ConstantSpeedPropagationDelayModel`` as the propagation delay model and
``ns3::LogDistancePropagationLossModel`` with the exponent of 3.0 as the propagation loss model.
Both devices are configured with ``ConstantRateWifiManager`` at the fixed rate of 12Mbps.
Finally, we manually place them by using the ``ns3::ListPositionAllocator``::

  std::string phyMode ("OfdmRate12Mbps");

  NodeContainer c;
  c.Create (2);

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211a);

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); 

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel",
                                  "Exponent", DoubleValue (3.0));
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper mac, and disable rate control (i.e. ConstantRateWifiManager)
  WifiMacHelper wifiMac;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));
  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c);

  // Configure mobility
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);

  // other set up (e.g. InternetStack, Application)

Infrastructure (access point and clients) WifiNetDevice configuration 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

This is a typical example of how a user might configure an access point and a set of clients.
In this example, we create one access point and two clients.
Each node is equipped with 802.11b Wi-Fi device::

  std::string phyMode ("DsssRate1Mbps");

  NodeContainer ap;
  ap.Create (1);
  NodeContainer sta;
  sta.Create (2);

  WifiHelper wifi;
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default (); 
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); 

  YansWifiChannelHelper wifiChannel;
  // reference loss must be changed since 802.11b is operating at 2.4GHz
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::LogDistancePropagationLossModel",
                                  "Exponent", DoubleValue (3.0),
				  "ReferenceLoss", DoubleValue (40.0459));
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper mac, and disable rate control
  WifiMacHelper wifiMac;
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));

  // Setup the rest of the upper mac
  Ssid ssid = Ssid ("wifi-default");
  // setup ap.
  wifiMac.SetType ("ns3::ApWifiMac",
                   "Ssid", SsidValue (ssid));
  NetDeviceContainer apDevice = wifi.Install (wifiPhy, wifiMac, ap);
  NetDeviceContainer devices = apDevice;

  // setup sta.
  wifiMac.SetType ("ns3::StaWifiMac",
                   "Ssid", SsidValue (ssid),
                   "ActiveProbing", BooleanValue (false));
  NetDeviceContainer staDevice = wifi.Install (wifiPhy, wifiMac, sta);
  devices.Add (staDevice);

  // Configure mobility
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
  positionAlloc->Add (Vector (0.0, 5.0, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ap);
  mobility.Install (sta);

  // other set up (e.g. InternetStack, Application)
