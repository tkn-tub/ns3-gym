.. include:: replace.txt

DSR Routing
-----------

Dynamic Source Routing (DSR) protocol is a reactive routing protocol designed 
specifically for use in multi-hop wireless ad hoc networks of mobile nodes.

This model was developed by 
`the ResiliNets research group <http://www.ittc.ku.edu/resilinets>`_
at the University of Kansas.  

DSR Routing Overview
********************
 
This model implements the base specification of the Dynamic Source Routing 
(DSR) protocol. Implementation is based on RFC4728.

* ``Class dsr::DsrRouting`` implements all functionality of service packet exchange and inherits IpL4Protocol.
* ``Class dsr::DsrOptions`` implements functionality of packet processing and talks to DsrRouting to send/receive packets
* ``Class dsr::DsrFsHeader`` defines the fixed-size header and identifies the up-layer protocol
* ``Class dsr::DsrOptionHeader`` takes care of different dsr options and processes different header according to the specification from the RFC 
* ``Class dsr::DsrSendBuffer`` is a buffer to save data packets and route error packets when there is no route to forward the packets
* ``Class dsr::DsrMaintainBuffer`` is a buffer to save data packets for next-hop notification when the data packet has already been sent out of send buffer
* ``Class dsr::RouteCache`` is the essential part to save routes found for data packets.  DSR responds to several routes for a single destination
* ``Class dsr::RreqTable`` implements the functions to avoid duplicate route requests and control route request rate for a single destination
 
Protocol operation depends on many adjustable parameters.  We support parameters, with their default values, from RFC and parameters that enable/disable 
protocol features or tune for specific simulation scenarios, such as the max 
size of the send buffer and its timeout value. The full parameter list is 
found in the dsr-routing.cc file.

DSR discovers routes on-demand. Therefore, our DSR model buffers all packets, 
while a route request packet (RREQ) is disseminated. We implement a packet 
buffer in dsr-rsendbuff.cc. The packet queue implements garbage collection 
of old packets and a queue size limit. When the packet is sent out from the 
send buffer, it will be queued in maintenance buffer for next hop 
acknowledgment.

The Route Cache implementation support garbage collection of old entries 
and state machine, as defined in the standard.  It implements as a STL 
map container. The key is the destination IP address.

Protocol operation strongly depends on broken link detection mechanism. 
We implement the three heuristics recommended.
    
First, we use layer 2 feedback when possible. A link is considered to be 
broken if frame transmission results in a transmission failure for all 
retries. This mechanism is meant for active links and works much faster than 
in its absence.  Layer 2 feedback implementation relies on TxErrHeader trace 
source, currently it is supported in AdhocWifiMac only.

Second, passive acknowledgment should be used whenever possible. The node 
turns on "promiscuous" receive mode, in which it can receive packets not 
destined for itself, and when the node assures the delivery of that data 
packet to its destination, it cancels the passive acknowledgment timer.

Last, we use a network layer acknowledge scheme to notify the receipt of 
a packet. Route request packet will not be acknowledged or retransmitted.

The following optional protocol optimizations aren't implemented:

* Flow state
* First Hop External (F), Last Hop External (L) flags
* Handling unknown DSR options
* Two types of error headers:
     1. flow state not supported option
     2. unsupported option (not going to happen in simulation)

DSR operates with direct access to IP header, and operates between 
network and transport layer.

Implementation changes
======================

* The DsrFsHeader has added 3 fields: message type, source id, destination id, 
  and these changes only for post-processing

  * message type is used to identify the data packet from control packet
  * source id is used to identify the real source of the data packet since we have to deliver the packet hop-by-hop and the ipv4header is not carrying the real source and destination ip address as needed
  * destination id is for same reason of above
    
* Route Reply header is not word-aligned in DSR rfc, change it to word-aligned in implementation
* DSR works as a shim header between transport and network protocol, it needs its own forwarding mechanism, we are changing the packet transmission to hop-by-hop delivery, so we added two fields in dsr fixed header to notify packet delivery
  
  # message type to notify the type of this packet: data packet or control one
  # source id to identify the real source address of this packet
  # destination id to identify the real destination

Current Route Cache implementation
==================================

This implementation used "path cache", which is simple to implement and ensures loop-free paths:

* the path cache has automatic expire policy
* the cache saves multiple route entries for a certain destination and sort the entries based on hop counts
* the MaxEntriesEachDst can be tuned to change the maximum entries saved for a single destination
* when adding mulitiple routes for one destination, the route is compared based on hop-count and expire time, the one with less hop count or relatively new route is favored
* Future implementation may include "link cache" as another possibility

DSR Instructions
****************

The following should be kept in mind when running DSR as routing protocol:

* NodeTraversalTime is the time it takes to traverse two neighboring nodes and should be chosen to fit the transmission range
* PassiveAckTimeout is the time a packet in maintenance buffer wait for passive acknowledgment, normally set as two times of NodeTraversalTime
* RouteCacheTimeout should be set smaller value when the nodes' velocity become higher. The default value is 300s.
   
Helper
******

To have a node run DSR, the easiest way would be to use the DsrHelper
and DsrMainHelpers in your simulation script. For instance::

  DsrHelper dsr;
  DsrMainHelper dsrMain;
  dsrMain.Install (dsr, adhocNodes);

The example scripts inside ``src/dsr/examples/`` demonstrate the use of 
DSR based nodes
in different scenarios. The helper source can be found inside ``src/dsr/helper/dsr-main-helper.{h,cc}`` 
and ``src/dsr/helper/dsr-helper.{h,cc}``

Examples
********

The example can be found in ``src/dsr/examples/``:

* dsr.cc use DSR as routing protocol within a traditional MANETs environment[3].

DSR is also built in the routing comparison case in ``examples/routing/``:

* ``manet-routing-compare.cc`` is a comparison case with built in MANET routing protocols and can generate its own results.

Validation
**********

This model has been tested as follows:

* Unit tests have been written to verify the internals of DSR. This can be found in ``src/dsr/test/dsr-test-suite.cc``. These tests verify whether the methods inside DSR module which deal with packet buffer, headers work correctly.
* Simulation cases similar to [3] have been tested and have comparable results.
* manet-routing-compare.cc has been used to compare DSR with three of other routing protocols.

A paper was presented on these results at the Workshop on ns-3 in 2011.
  
References
**********

[1] Link for the `original paper: <http://www.monarch.cs.rice.edu/monarch-papers/dsr-chapter00.pdf>`_

[2] Link for `RFC 4728: <http://www6.ietf.org/rfc/rfc4728.txt>`_

[3] Link for the `Broch's comparison paper: <http://www.monarch.cs.rice.edu/monarch-papers/mobicom98.ps>`_
