.. include:: replace.txt
.. highlight:: cpp

++++++++++++++++++++
Design Documentation
++++++++++++++++++++


|ns3| nodes can contain a collection of NetDevice objects, much like an actual
computer contains separate interface cards for Ethernet, Wifi, Bluetooth, etc.
This chapter describes the |ns3| WifiNetDevice and related models. By adding
WifiNetDevice objects to |ns3| nodes, one can create models of 802.11-based
infrastructure and ad hoc networks.

Overview of the model
*********************

The WifiNetDevice models a wireless network interface controller based
on the IEEE 802.11 standard [ieee80211]_. We will go into more detail below but in brief,
|ns3| provides models for these aspects of 802.11:

* basic 802.11 DCF with **infrastructure** and **adhoc** modes
* **802.11a**, **802.11b**, **802.11g** and **802.11n** (both 2.4 and 5 GHz bands) physical layers
* **MSDU aggregation** and **MPDU aggregation** extensions of 802.11n, and both can be combined together (two-level aggregation)
* QoS-based EDCA and queueing extensions of **802.11e**
* the ability to use different propagation loss models and propagation delay models,
  please see the chapter on :ref:`Propagation` for more detail
* various rate control algorithms including **Aarf, Arf, Cara, Onoe, Rraa,
  ConstantRate, and Minstrel**
* 802.11s (mesh), described in another chapter

The set of 802.11 models provided in |ns3| attempts to provide an accurate
MAC-level implementation of the 802.11 specification and to provide a
not-so-slow PHY-level model of the 802.11a specification and a not-so-slow PHY-level model of the 802.11a/b/g/n specifications.

In |ns3|, nodes can have multiple WifiNetDevices on separate channels, and the
WifiNetDevice can coexist with other device types; this removes an architectural
limitation found in |ns2|. Presently, however, there is no model for
cross-channel interference or coupling.

The source code for the WifiNetDevice lives in the directory
``src/wifi``.

The implementation is modular and provides roughly four levels of models:

* the **PHY layer models**
* the so-called **MAC low models**: they implement DCF and EDCAF
* the so-called **MAC high models**: they implement the MAC-level beacon
  generation, probing, and association state machines, and
* a set of **Rate control algorithms** used by the MAC low models

Next, we provide some overview of each layer.
More detailed information will be discussed later.

MAC high models
===============

There are presently three **MAC high models** that provide for the three
(non-mesh; the mesh equivalent, which is a sibling of these with common
parent ``ns3::RegularWifiMac``, is not discussed here) Wi-Fi topological
elements - Access Point (AP) (``ns3::ApWifiMac``), 
non-AP Station (STA) (``ns3::StaWifiMac``), and STA in an Independent
Basic Service Set (IBSS - also commonly referred to as an ad hoc
network (``ns3::AdhocWifiMac``).

The simplest of these is ``ns3::AdhocWifiMac``, which implements a
Wi-Fi MAC that does not perform any kind of beacon generation,
probing, or association. The ``ns3::StaWifiMac`` class implements
an active probing and association state machine that handles automatic
re-association whenever too many beacons are missed. Finally,
``ns3::ApWifiMac`` implements an AP that generates periodic
beacons, and that accepts every attempt to associate.

These three MAC high models share a common parent in
``ns3::RegularWifiMac``, which exposes, among other MAC
configuration, an attribute ``QosSupported`` that allows
configuration of 802.11e/WMM-style QoS support and an attribute ``HtSupported`` that allows configuration of 802.11n High Throughput style support.

MAC low layer
==============

The **MAC low layer** is split into three components:

#. ``ns3::MacLow`` which takes care of RTS/CTS/DATA/ACK transactions.
#. ``ns3::DcfManager`` and ``ns3::DcfState`` which implements the DCF and EDCAF
   functions.
#. ``ns3::DcaTxop`` and ``ns3::EdcaTxopN`` which handle the packet queue,
   packet fragmentation, and packet retransmissions if they are needed.
   The ``ns3::DcaTxop`` object is used high MACs that are not QoS-enabled,
   and for transmission of frames (e.g., of type Management)
   that the standard says should access the medium using the DCF. 
   ``ns3::EdcaTxopN`` is is used by QoS-enabled high MACs and also
   performs 802.11n-style MSDU aggregation.

Rate control algorithms
=======================

There are also several **rate control algorithms** that can be used by the
MAC low layer.  A complete list of available rate control algorithms is 
provided in a separate section.

PHY layer models
================

The PHY layer implements a single model in the ``ns3::WifiPhy`` class: the
physical layer model implemented there is described fully in a paper entitled
`Yet Another Network Simulator <http://cutebugs.net/files/wns2-yans.pdf>`_
Validation results for 802.11b are available in this
`technical report <http://www.nsnam.org/~pei/80211b.pdf>`_

.. _wifi-architecture:

.. figure:: figures/WifiArchitecture.*
   
   WifiNetDevice architecture.

The WifiChannel and WifiPhy models
**********************************

The WifiChannel subclass can be used to connect together a set of
``ns3::WifiNetDevice`` network interfaces. The class ``ns3::WifiPhy`` is the
object within the WifiNetDevice that receives bits from the channel.  
For the channel propagation modeling, the propagation module is used; see section :ref:`Propagation` for details.


This section summarizes the description of the BER calculations found in the
yans paper taking into account the Forward Error Correction present in 802.11a
and describes the algorithm we implemented to decide whether or not a packet can
be successfully received. See `"Yet Another Network Simulator"
<http://cutebugs.net/files/wns2-yans.pdf>`_ for more details.

The PHY layer can be in one of five states:

#. TX: the PHY is currently transmitting a signal on behalf of its associated
   MAC
#. RX: the PHY is synchronized on a signal and is waiting until it has received
   its last bit to forward it to the MAC.
#. IDLE: the PHY is not in the TX, RX, or CCA BUSY states.
#. CCA Busy: the PHY is not in TX or RX state but the measured energy is higher than the energy detection threshold.
#. SLEEP: the PHY is in a power save mode and cannot send nor receive frames.

When the first bit of a new packet is received while the PHY is not IDLE (that
is, it is already synchronized on the reception of another earlier packet or it
is sending data itself), the received packet is dropped. Otherwise, if the PHY
is IDLE or CCA Busy, we calculate the received energy of the first bit of this new signal
and compare it against our Energy Detection threshold (as defined by the Clear
Channel Assessment function mode 1). If the energy of the packet k is higher,
then the PHY moves to RX state and schedules an event when the last bit of the
packet is expected to be received. Otherwise, the PHY stays in IDLE 
or CCA Busy state and drops the packet.

The energy of the received signal is assumed to be zero outside of the reception
interval of packet k and is calculated from the transmission power with a
path-loss propagation model in the reception interval.  where the path loss
exponent, :math:`n`, is chosen equal to :math:`3`, the reference distance,
:math:`d_0` is choosen equal to :math:`1.0m` and the reference energy is based
based on a Friis propagation model.

When the last bit of the packet upon which the PHY is synchronized is received,
we need to calculate the probability that the packet is received with any error
to decide whether or not the packet on which we were synchronized could be
successfully received or not: a random number is drawn from a uniform
distribution and is compared against the probability of error.

To evaluate the probability of error, we start from the piecewise linear 
functions shown in Figure :ref:`snir` and calculate the 
SNIR function. 

.. _snir:

.. figure:: figures/snir.*
   
   *SNIR function over time.*

From the SNIR function we can derive the Bit Error Rate (BER) and Packet Error Rate (PER) for
the modulation and coding scheme being used for the transmission.  Please refer to [pei80211ofdm]_, [pei80211b]_, [lacage2006yans]_, [Haccoun]_ and [Frenger]_ for a detailed description of the available BER/PER models.

WifiChannel configuration
=========================

The WifiChannel implementation uses the propagation loss and delay models provided within the |ns3| :ref:`Propagation` module.

The MAC model
*************

The 802.11 Distributed Coordination Function is used to calculate when to grant
access to the transmission medium. While implementing the DCF would have been
particularly easy if we had used a recurring timer that expired every slot, we
chose to use the method described in [ji2004sslswn]_
where the backoff timer duration is lazily calculated whenever needed since it
is claimed to have much better performance than the simpler recurring timer
solution.

The backoff procedure of DCF is described in section 9.2.5.2 of [ieee80211]_.

*  “The backoff procedure shall be invoked for a STA to transfer a frame 
   when finding the medium busy as indicated by either the physical or 
   virtual CS mechanism.”
*  “A backoff procedure shall be performed immediately after the end of 
   every transmission with the More Fragments bit set to 0 of an MPDU of 
   type Data, Management, or Control with subtype PS-Poll, even if no 
   additional transmissions are currently queued.”

Thus, if the queue is empty, a newly arrived packet should be transmitted 
immediately after channel is sensed idle for DIFS.  If queue is not empty 
and after a successful MPDU that has no more fragments, a node should 
also start the backoff timer.

Some users have observed that the 802.11 MAC with an empty queue on an 
idle channel will transmit the first frame arriving to the model 
immediately without waiting for DIFS or backoff, and wonder whether this 
is compliant.  According to the standard, “The backoff procedure shall 
be invoked for a STA to transfer a frame when finding the medium busy 
as indicated by either the physical or virtual CS mechanism.”  So in 
this case, the medium is not found to be busy in recent past and the 
station can transmit immediately. 

The higher-level MAC functions are implemented in a set of other C++ classes and
deal with:

* packet fragmentation and defragmentation,
* use of the RTS/CTS protocol,
* rate control algorithm,
* connection and disconnection to and from an Access Point,
* the MAC transmission queue,
* beacon generation,
* MSDU aggregation,
* etc.

Rate control algorithms
***********************

Multiple rate control algorithms are available in |ns3|.
Some rate control algorithms are modeled after real algorithms used in real devices;
others are found in literature.
The following rate control algorithms can be used by the MAC low layer:

Algorithms found in real devices:

* ``ArfWifiManager`` (default for ``WifiHelper``)
* ``OnoeWifiManager``
* ``ConstantRateWifiManager``
* ``MinstrelWifiManager``

Algorithms in literature:

* ``IdealWifiManager``
* ``AarfWifiManager`` [lacage2004aarfamrr]_
* ``AmrrWifiManager`` [lacage2004aarfamrr]_
* ``CaraWifiManager`` [kim2006cara]_
* ``RraaWifiManager`` [wong2006rraa]_
* ``AarfcdWifiManager`` [maguolo2008aarfcd]_
* ``ParfWifiManager`` [akella2007parf]_
* ``AparfWifiManager`` [chevillat2005aparf]_

ConstantRateWifiManager
=======================

The constant rate control algorithm always uses the same
transmission mode for every packet. Users can set a desired
'DataMode' for all 'unicast' packets and 'ControlMode' for all
'request' control packets (e.g. RTS).

To specify different data mode for non-unicast packets, users
must set the 'NonUnicastMode' attribute of the
WifiRemoteStationManager.  Otherwise, WifiRemoteStationManager
will use a mode with the lowest rate for non-unicast packets.

The 802.11 standard is quite clear on the rules for selection
of transmission parameters for control response frames (e.g.
CTS and ACK).  |ns3| follows the standard and selects the rate
of control response frames from the set of basic rates or
mandatory rates. This means that control response frames may
be sent using different rate even though the ConstantRateWifiManager
is used.  The ControlMode attribute of the ConstantRateWifiManager
is used for RTS frames only.  The rate of CTS and ACK frames are
selected according to the 802.11 standard.  However, users can still
manually add WifiMode to the basic rate set that will allow control
response frames to be sent at other rates.  Please consult the
`project wiki <http://www.nsnam.org/wiki>`_ on how to do this.

Available attributes:

* DataMode (default WifiMode::OfdmRate6Mbps): specify a mode for
  all non-unicast packets
* ControlMode (default WifiMode::OfdmRate6Mbps): specify a mode for
  all 'request' control packets

IdealWifiManager
================

The ideal rate control algorithm selects the best
mode according to the SNR of the previous packet sent.
Consider node *A* sending a unicast packet to node *B*.
When *B* successfully receives the packet sent from *A*,
*B* records the SNR of the received packet into a ``ns3::SnrTag``
and adds the tag to an ACK back to *A*.
By doing this, *A* is able to learn the SNR of the packet sent to *B*
using an out-of-band mechanism (thus the name 'ideal').
*A* then uses the SNR to select a transmission mode based
on a set of SNR thresholds, which was built from a target BER and
mode-specific SNR/BER curves.

Available attribute:

* BerThreshold (default 10e-6): The maximum Bit Error Rate
  that is used to calculate the SNR threshold for each mode.

MinstrelWifiManager
===================

The minstrel rate control algorithm is a rate control algorithm originated from
madwifi project.  It is currently the default rate control algorithm of the Linux kernel.

Minstrel keeps track of the probability of successfully sending a frame of each available rate.
Minstrel then calculates the expected throughput by multiplying the probability with the rate.
This approach is chosen to make sure that lower rates are not selected in favor of the higher
rates (since lower rates are more likely to have higher probability).

In minstrel, roughly 10 percent of transmissions are sent at the so-called lookaround rate.
The goal of the lookaround rate is to force minstrel to try higher rate than the currently used rate.

For a more detailed information about minstrel, see [linuxminstrel]_.

Modifying Wifi model
********************

Modifying the default wifi model is one of the common tasks when performing research.
We provide an overview of how to make changes to the default wifi model in this section.
Depending on your goal, the common tasks are (in no particular order):

* Creating or modifying the default Wi-Fi frames/headers by making changes to ``wifi-mac-header.*``.
* MAC low modification. For example, handling new/modified control frames (think RTS/CTS/ACK/Block ACK),
  making changes to two-way transaction/four-way transaction.  Users usually make changes to 
  ``mac-low.*`` to accomplish this.  Handling of control frames is performed in
  ``MacLow::ReceiveOk``.
* MAC high modification. For example, handling new management frames (think beacon/probe), 
  beacon/probe generation.  Users usually make changes to ``regular-wifi-mac.*``, 
  ``sta-wifi-mac.*``, ``ap-wifi-mac.*``, or ``adhoc-wifi-mac.*`` to accomplish this.
* Wi-Fi queue management.  The files ``dca-txop.*`` and ``edca-txop-n.*`` are of interested for this task.
* Channel access management.  Users should modify the files ``dcf-manager.*``, which grant access to
  ``DcaTxop`` and ``EdcaTxopN``.
* Fragmentation and RTS threholds are handled by Wi-Fi remote station manager.  Note that Wi-Fi remote
  station manager simply indicates if fragmentation and RTS are needed.  Fragmentation is handled by
  ``DcaTxop`` or ``EdcaTxopN`` while RTS/CTS transaction is hanled by ``MacLow``.
* Modifying or creating new rate control algorithms can be done by creating a new child class of Wi-Fi remote
  station manager or modifying the existing ones.

Note on the current implementation
**********************************

* 802.11g does not support 9 microseconds slot
* PHY_RXSTART is not supported
* 802.11e TXOP is not supported
* 802.11n MIMO is not supported
* PLCP preamble reception is not modeled
* BSSBasicRateSet for 802.11b has been assumed to be 1-2 Mbit/s
* BSSBasicRateSet for 802.11a has been assumed to be 6-12-24 Mbit/s
* cases where RTS/CTS and ACK are transmitted using HT formats are not supported
* rate control algorithms are not aware of HT rates; they will only use the 802.11a BSSBasicRateSet when 802.11n is configured


