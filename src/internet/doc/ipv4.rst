.. include:: replace.txt
.. highlight:: cpp

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)


IPv4
----

Tracing in the IPv4 Stack
*************************

The internet stack provides a number of trace sources in its various
protocol implementations.  These trace sources can be hooked using your own
custom trace code, or you can use our helper functions in some cases to
arrange for tracing to be enabled.

Tracing in ARP
==============

ARP provides two trace hooks, one in the cache, and one in the layer three
protocol.  The trace accessor in the cache is given the name "Drop."  When
a packet is transmitted over an interface that requires ARP, it is first
queued for transmission in the ARP cache until the required MAC address is
resolved.  There are a number of retries that may be done trying to get the
address, and if the maximum retry count is exceeded the packet in question
is dropped by ARP.  The single trace hook in the ARP cache is called,

- If an outbound packet is placed in the ARP cache pending address resolution
  and no resolution can be made within the maximum retry count, the outbound
  packet is dropped and this trace is fired;

A second trace hook lives in the ARP L3 protocol (also named "Drop") and may
be called for a  number of reasons.

- If an ARP reply is received for an entry that is not waiting for a reply,
  the ARP reply packet is dropped and this trace is fired;
- If an ARP reply is received for a non-existant entry, the ARP reply packet
  is dropped and this trace is fired;
- If an ARP cache entry is in the DEAD state (has timed out) and an ARP reply
  packet is received, the reply packet is dropped and this trace is fired.
- Each ARP cache entry has a queue of pending packets.  If the size of the
  queue is exceeded, the outbound packet is dropped and this trace is fired.

Tracing in IPv4
===============

The IPv4 layer three protocol provides three trace hooks.  These are the
"Tx" (ns3::Ipv4L3Protocol::m_txTrace), "Rx" (ns3::Ipv4L3Protocol::m_rxTrace)
and "Drop" (ns3::Ipv4L3Protocol::m_dropTrace) trace sources.

The "Tx" trace is fired in a number of situations, all of which indicate that
a given packet is about to be sent down to a given ns3::Ipv4Interface.

- In the case of a packet destined for the broadcast address, the
  Ipv4InterfaceList is iterated and for every interface that is up and can
  fragment the packet or has a large enough MTU to transmit the packet,
  the trace is hit.  See ns3::Ipv4L3Protocol::Send.

- In the case of a packet that needs routing, the "Tx" trace may be fired
  just before a packet is sent to the interface appropriate to the default
  gateway.  See ns3::Ipv4L3Protocol::SendRealOut.

- Also in the case of a packet that needs routing, the "Tx" trace may be
  fired just before a packet is sent to the outgoing interface appropriate
  to the discovered route.  See ns3::Ipv4L3Protocol::SendRealOut.

The "Rx" trace is fired when a packet is passed from the device up to the
ns3::Ipv4L3Protocol::Receive function.

- In the receive function, the Ipv4InterfaceList is iterated, and if the
  Ipv4Interface corresponding to the receiving device is fount to be in the
  UP state, the trace is fired.

The "Drop" trace is fired in any case where the packet is dropped (in both
the transmit and receive paths).

- In the ns3::Ipv4Interface::Receive function, the packet is dropped and the
  drop trace is hit if the interface corresponding to the receiving device
  is in the DOWN state.

- Also in the ns3::Ipv4Interface::Receive function, the packet is dropped and
  the drop trace is hit if the checksum is found to be bad.

- In ns3::Ipv4L3Protocol::Send, an outgoing packet bound for the broadcast
  address is dropped and the "Drop" trace is fired if the "don't fragment"
  bit is set and fragmentation is available and required.

- Also in ns3::Ipv4L3Protocol::Send, an outgoing packet destined for the
  broadcast address is dropped and the "Drop" trace is hit if fragmentation
  is not available and is required (MTU < packet size).

- In the case of a broadcast address, an outgoing packet is cloned for each
  outgoing interface.  If any of the interfaces is in the DOWN state, the
  "Drop" trace event fires with a reference to the copied packet.

- In the case of a packet requiring a route, an outgoing packet is dropped
  and the "Drop" trace event fires if no route to the remote host is found.

- In ns3::Ipv4L3Protocol::SendRealOut, an outgoing packet being routed
  is dropped and the "Drop" trace is fired if the "don't fragment" bit is
  set and fragmentation is available and required.

- Also in ns3::Ipv4L3Protocol::SendRealOut, an outgoing packet being routed
  is dropped and the "Drop" trace is hit if fragmentation is not available
  and is required (MTU < packet size).

- An outgoing packet being routed is dropped and the "Drop" trace event fires
  if the required Ipv4Interface is in the DOWN state.

- If a packet is being forwarded, and the TTL is exceeded (see
  ns3::Ipv4L3Protocol::DoForward), the packet is dropped and the "Drop" trace
  event is fired.
