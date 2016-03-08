.. include:: replace.txt
.. highlight:: cpp

pfifo_fast queue disc
---------------------

Model Description
*****************

Linux pfifo_fast is the default priority queue enabled on Linux
systems. Packets are enqueued in three FIFO droptail queues according
to three priority bands based on the classification returned by
the configured packet filters.

The system behaves similar to three ns3::DropTail queues operating
together, in which packets from higher priority bands are always
dequeued before a packet from a lower priority band is dequeued.

The queue disc capacity, i.e., the maximum number of packets that can
be enqueued in the queue disc, is set through the limit attribute, which
plays the same role as txqueuelen in Linux. If no internal queue is
provided, three DropTail queues having each a capacity equal to limit are
created by default. User is allowed to provide queues, but they must be
three, operate in packet mode and each have a capacity not less
than limit.

It is necessary to provide at least one packet filter. To simulate the
Linux behavior, the PfifoFastIpv4PacketFilter and/or the PfifoFastIpv6PacketFilter
shall be provided. These filters classify packets based on their Type of
Service bits or DSCP bits. If the filters are unable to classify a packet,
i.e., they return -1 (PF_NO_MATCH), that packet is enqueued into band 1
(normal service).



Attributes
==========

The PfifoFastQueueDisc class holds a single attribute:

* ``Limit:`` The maximum number of packets accepted by the queue disc. The default value is 1000.

Examples
========

The traffic-control example located in ``examples/traffic-control`` shows how to configure
and install a pfifo_fast queue on Ipv4 nodes.

Validation
**********

The pfifo_fast model is tested using :cpp:class:`PfifoFastQueueDiscTestSuite` class defined
in ``src/traffic-control/test/pfifo-fast-queue-disc-test-suite.cc``. The suite includes 4 test cases:

* Test 1: The first test checks whether IPv4 packets are enqueued in the correct band based on the TOS byte
* Test 2: The second test checks whether IPv4 packets are enqueued in the correct band based on the TOS byte
* Test 3: The third test checks that the queue disc cannot enqueue more packets than its limit
* Test 4: The fourth test checks that packets that the filters have not been able to classify are enqueued into the default band of 1
