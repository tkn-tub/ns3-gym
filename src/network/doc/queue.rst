Queues
------

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

This section documents the queue object, which is typically used by NetDevices
and QueueDiscs to store packets.

Packets stored in a queue can be managed according to different policies.
Currently, the following policies are available:

* DropTail

Model Description
*****************

The source code for the new module lives in the directory ``src/network/utils``.

ns-3 provides the classic droptail queue model and the ability to
trace certain queue operations such as enqueuing, dequeuing, and dropping.
These may be added to certain NetDevice objects that take a Ptr<Queue>
pointer.

Note that not all device models use these queue models.  
In particular, WiFi, WiMax, and LTE use specialized device queues.
The queue models described here are more often used with simpler ns-3 
device models such as PointToPoint and Csma.

All the queuing disciplines, instead, make use of the queue model defined here.

Design
======

An abstract base class, class Queue, is typically used and subclassed
for specific scheduling and drop policies. A class QueueItem is introduced
to model the items stored in a queue. The base class QueueItem only contains
a pointer to a packet. Subclasses may be defined to store additional information.
Common operations provided by the base class Queue include:

* ``bool Enqueue (Ptr<QueueItem> item)``:  Enqueue a packet
* ``Ptr<QueueItem> Dequeue (void)``:  Dequeue a packet
* ``uint32_t GetNPackets (void)``:  Get the queue depth, in packets
* ``uint32_t GetNBytes (void)``:  Get the queue depth, in packets

as well as tracking some statistics on queue operations.

There are three trace sources that may be hooked:

* ``Enqueue``
* ``Dequeue``
* ``Drop``

Also, three attributes are defined in the Queue base class:

* ``Mode``: whether the capacity of the queue is measured in packets or bytes
* ``MaxPackets``: the maximum number of packets accepted by the queue in packet mode
* ``MaxBytes``: the maximum number of bytes accepted by the queue in byte mode

The Enqueue method does not allow to store a packet if the queue capacity is exceeded.

DropTail
########

This is a basic first-in-first-out (FIFO) queue that performs a tail drop
when the queue is full.

Usage
*****

Helpers
=======

A typical usage pattern is to create a device helper and to configure
the queue type and attributes from the helper, such as this example:

.. sourcecode:: cpp

  PointToPointHelper p2p;

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer devn0n2 = p2p.Install (n0n2);

  p2p.SetQueue ("ns3::DropTailQueue");
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("3ms"));
  NetDeviceContainer devn1n2 = p2p.Install (n1n2);

  p2p.SetQueue ("ns3::DropTailQueue",
                "LinkBandwidth", StringValue (linkDataRate),
                "LinkDelay", StringValue (linkDelay));
  p2p.SetDeviceAttribute ("DataRate", StringValue (linkDataRate));
  p2p.SetChannelAttribute ("Delay", StringValue (linkDelay));
  NetDeviceContainer devn2n3 = p2p.Install (n2n3);


Output
======

The ns-3 ascii trace helpers used by many of the NetDevices will hook
the Enqueue, Dequeue, and Drop traces of these queues and print out 
trace statements, such as the following from ``examples/udp/udp-echo.cc``:

.. sourcecode:: text

  + 2 /NodeList/0/DeviceList/1/$ns3::CsmaNetDevice/TxQueue/Enqueue ns3::EthernetHeader 
  ( length/type=0x806, source=00:00:00:00:00:01, destination=ff:ff:ff:ff:ff:ff) 
  ns3::ArpHeader (request source mac: 00-06-00:00:00:00:00:01 source ipv4: 10.1.1.1 
  dest ipv4: 10.1.1.2) Payload (size=18) ns3::EthernetTrailer (fcs=0)
  - 2 /NodeList/0/DeviceList/1/$ns3::CsmaNetDevice/TxQueue/Dequeue ns3::EthernetHeader 
  ( length/type=0x806, source=00:00:00:00:00:01, destination=ff:ff:ff:ff:ff:ff) 
  ns3::ArpHeader (request source mac: 00-06-00:00:00:00:00:01 source ipv4: 10.1.1.1 
  dest ipv4: 10.1.1.2) Payload (size=18) ns3::EthernetTrailer (fcs=0)

which shows an enqueue "+" and dequeue "-" event at time 2 seconds.

Users are, of course, free to define and hook their own trace sinks to
these trace sources.

Examples
========

The drop-tail queue is used in several examples, such as 
``examples/udp/udp-echo.cc``.

