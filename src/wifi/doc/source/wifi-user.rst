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

To create a WifiNetDevice, users need to configure mainly five steps:

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

Furthermore, 802.11n provides an optional mode (GreenField mode) to reduce preamble durations and which is only compatible with 802.11n devices. This mode is enabled as follows::

 wifiPhyHelper.Set ("GreenfieldEnabled",BooleanValue(true));

802.11n PHY layer can support both 20 (default) or 40 MHz channel width, and 802.11ac PHY layer can use either 20, 40, 80 (default) or 160 MHz channel width. Since the channel width value is overwritten by ``WifiHelper::SetStandard``, this should be done post-install using ``Config::Set``::

  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211ac);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue("VHtMcs9"), "ControlMode", StringValue("VhtMcs0"));
  VhtWifiMacHelper mac = VhtWifiMacHelper::Default ();

  //Install PHY and MAC
  Ssid ssid = Ssid ("ns3-wifi");
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

WifiMacHelper
=============

The next step is to configure the MAC model.
We use WifiMacHelper to accomplish this.
WifiMacHelper takes care of both the MAC low model and MAC high model.
A user must decide if 802.11/WMM-style QoS and/or 802.11n-style High throughput (HT) and/or 802.11ac-style Very High throughput (VHT) support is required.

NqosWifiMacHelper and QosWifiMacHelper
++++++++++++++++++++++++++++++++++++++

The ``ns3::NqosWifiMacHelper`` and ``ns3::QosWifiMacHelper`` configure an
object factory to create instances of a ``ns3::WifiMac``. They are used to
configure MAC parameters like type of MAC.  

The former, ``ns3::NqosWifiMacHelper``, supports creation of MAC
instances that do not have 802.11e/WMM-style QoS nor 802.11n-style High throughput (HT) nor 802.11ac-style Very High throughput (VHT) support enabled.

For example the following user code configures a non-QoS and non-HT MAC that
will be a non-AP STA in an infrastructure network where the AP has
SSID ``ns-3-ssid``::

    NqosWifiMacHelper wifiMacHelper = NqosWifiMacHelper::Default ();
    Ssid ssid = Ssid ("ns-3-ssid");
    wifiMacHelper.SetType ("ns3::StaWifiMac",
                          "Ssid", SsidValue (ssid),
                          "ActiveProbing", BooleanValue (false));

To create MAC instances with QoS support enabled,
``ns3::QosWifiMacHelper`` is used in place of
``ns3::NqosWifiMacHelper``.

The following code shows an example use of ``ns3::QosWifiMacHelper`` to 
create an AP with QoS enabled::

  QosWifiMacHelper wifiMacHelper = QosWifiMacHelper::Default ();
  wifiMacHelper.SetType ("ns3::ApWifiMac",
                         "Ssid", SsidValue (ssid),
                         "BeaconGeneration", BooleanValue (true),
                         "BeaconInterval", TimeValue (Seconds (2.5)));

With QoS-enabled MAC models it is possible to work with traffic belonging to
four different Access Categories (ACs): **AC_VO** for voice traffic,
**AC_VI** for video traffic, **AC_BE** for best-effort
traffic and **AC_BK** for background traffic.  In order for the
MAC to determine the appropriate AC for an MSDU, packets forwarded
down to these MAC layers should be marked using **ns3::QosTag** in
order to set a TID (traffic id) for that packet otherwise it will be
considered belonging to **AC_BE**.

To create ad-hoc MAC instances, simply use ``ns3::AdhocWifiMac`` instead of ``ns3::StaWifiMac`` or ``ns3::ApWifiMac``.

HtWifiMacHelper
+++++++++++++++

The ``ns3::HtWifiMacHelper`` configures an
object factory to create instances of a ``ns3::WifiMac``. It is used to
supports creation of MAC instances that have 802.11n-style High throughput (HT) and QoS support enabled. In particular, this object can be also used to set:

* an MSDU aggregator for a particular Access Category (AC) in order to use 
  802.11n MSDU aggregation feature;
* block ack parameters like threshold (number of packets for which block ack
  mechanism should be used) and inactivity timeout.

For example the following user code configures a HT MAC that
will be a non-AP STA with QoS enabled, aggregation on AC_VO, and Block Ack on AC_BE, in an infrastructure network where the AP has
SSID ``ns-3-ssid``::

    HtWifiMacHelper wifiMacHelper = HtWifiMacHelper::Default ();
    Ssid ssid = Ssid ("ns-3-ssid");
    wifiMacHelper.SetType ("ns3::StaWifiMac",
                          "Ssid", SsidValue (ssid),
                          "ActiveProbing", BooleanValue (false));

    wifiMacHelper.SetMsduAggregatorForAc (AC_VO, "ns3::MsduStandardAggregator",
                                          "MaxAmsduSize", UintegerValue (3839));
    wifiMacHelper.SetBlockAckThresholdForAc (AC_BE, 10);
    wifiMacHelper.SetBlockAckInactivityTimeoutForAc (AC_BE, 5);

This object can be also used to set in the same way as ``ns3::QosWifiMacHelper``.

VhtWifiMacHelper
+++++++++++++++

The ``ns3::VhtWifiMacHelper`` configures an
object factory to create instances of a ``ns3::WifiMac``. It is used to
supports creation of MAC instances that have 802.11ac-style Very High throughput (VHT) and QoS support enabled. This object is similar to ``HtWifiMacHelper``.

WifiHelper
==========

We're now ready to create WifiNetDevices. First, let's create
a WifiHelper with default settings::

  WifiHelper wifiHelper = WifiHelper::Default ();

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

  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211n_2_4GHZ);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue("HtMcs7"), "ControlMode", StringValue("HtMcs0"));
  HtWifiMacHelper mac = HtWifiMacHelper::Default ();

  //Install PHY and MAC
  Ssid ssid = Ssid ("ns3-wifi");
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
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
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
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
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

Note on the current implementation
**********************************

* 802.11g does not support 9 microseconds slot
* PHY_RXSTART is not supported
* 802.11e TXOP is not supported
* 802.11n/ac MIMO is not supported
* 802.11n/ac beamforming is not supported
* PLCP preamble reception is not modeled
* BSSBasicRateSet for 802.11b has been assumed to be 1-2 Mbit/s
* BSSBasicRateSet for 802.11a/g has been assumed to be 6-12-24 Mbit/s
* cases where RTS/CTS and ACK are transmitted using HT formats are not supported
* Only ``ConstantRateWifiManager`` is supported by **802.11n** or **802.11ac**
