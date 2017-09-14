Queue disciplines
--------------------------------------------------------------

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

Model Description
*****************

Packets received by the Traffic Control layer for transmission to a netdevice
can be passed to a queueing discipline (queue disc) to perform scheduling and
policing.  The |ns3| term "queue disc" corresponds to what Linux calls a "qdisc".
A netdevice can have a single (root) queue disc installed on it.
Installing a queue disc on a netdevice is not mandatory. If a netdevice does
not have a queue disc installed on it, the traffic control layer sends the packets
directly to the netdevice. This is the case, for instance, of the loopback netdevice.

As in Linux, queue discs may be simple queues or may be complicated hierarchical
structures.  A queue disc may contain distinct elements:

* queues, which actually store the packets waiting for transmission
* classes, which permit the definition of different treatments for different subdivisions of traffic
* filters, which determine the queue or class which a packet is destined to

Linux uses the terminology "classful qdiscs" or "classless qdiscs" to describe
how packets are handled.  This use of the term "class" should be distinguished
from the C++ language "class".  In general, the below discussion uses "class"
in the Linux, not C++, sense, but there are some uses of the C++ term, so
please keep in mind the dual use of this term in the below text.

Notice that a child queue disc must be attached to every class and a packet
filter is only able to classify packets of a single protocol. Also, while in Linux
some queue discs (e.g., fq-codel) use an internal classifier and do not make use of
packet filters, in ns-3 every queue disc including multiple queues or multiple classes
needs an external filter to classify packets (this is to avoid having the traffic-control
module depend on other modules such as internet).

Queue disc configuration vary from queue disc to queue disc. A typical taxonomy divides
queue discs in classful (i.e., support classes) and classless (i.e., do not support
classes). More recently, after the appearance of multi-queue devices (such as Wi-Fi),
some multi-queue aware queue discs have been introduced. Multi-queue aware queue discs
handle as many queues (or queue discs -- without using classes) as the number of
transmission queues used by the device on which the queue disc is installed.
An attempt is made, also, to enqueue each packet in the "same" queue both within the
queue disc and within the device.

The traffic control layer interacts with a queue disc in a simple manner: after requesting
to enqueue a packet, the traffic control layer requests the qdisc to "run", i.e., to
dequeue a set of packets, until a predefined number ("quota") of packets is dequeued
or the netdevice stops the queue disc.  A netdevice shall
stop the queue disc when its transmission queue does not have room for another
packet. Also, a netdevice shall wake the queue disc when it detects that there
is room for another packet in its transmission queue, but the transmission queue
is stopped. Waking a queue disc is equivalent to make it run.

Every queue disc collects statistics about the total number of packets/bytes
received from the upper layers (in case of root queue disc) or from the parent
queue disc (in case of child queue disc), enqueued, dequeued, requeued, dropped,
dropped before enqueue, dropped after dequeue, stored in the queue disc and
sent to the netdevice or to the parent queue disc. Note that packets that are
dequeued may be requeued, i.e., retained by the traffic control infrastructure,
if the netdevice is not ready to receive them. Requeued packets are not part
of the queue disc. The following identities hold:

* dropped = dropped before enqueue + dropped after dequeue
* received = dropped before enqueue + enqueued
* queued = enqueued - dequeued
* sent = dequeued - dropped after dequeue (- 1 if there is a requeued packet)

Separate counters are also kept for each possible reason to drop a packet.
When a packet is dropped by an internal queue, e.g., because the queue is full,
the reason is "Dropped by internal queue". When a packet is dropped by a child
queue disc, the reason is "(Dropped by child queue disc) " followed by the
reason why the child queue disc dropped the packet.

The QueueDisc base class provides the SojournTime trace source, which provides
the sojourn time of every packet dequeued from a queue disc, including packets
that are dropped or requeued after being dequeued. The sojourn time is taken
when the packet is dequeued from the queue disc, hence it does not account for
the additional time the packet is retained within the queue disc in case it is
requeued.


Design
==========

A C++ abstract base class, class QueueDisc, is subclassed to implement a specific
queue disc. A subclass is required to implement the following methods:

* ``bool DoEnqueue (Ptr<QueueDiscItem> item)``:  Enqueue a packet
* ``Ptr<QueueDiscItem> DoDequeue (void)``:  Dequeue a packet
* ``Ptr<const QueueDiscItem> DoPeek (void) const``: Peek a packet
* ``bool CheckConfig (void) const``: Check if the configuration is correct
* ``void InitializeParams (void)``: Initialize queue disc parameters

The C++ base class QueueDisc implements:

* methods to add/get a single queue, class or filter and methods to get the number \
  of installed queues, classes or filters
* a ``Classify`` method which classifies a packet by processing the list of filters \
  until a filter able to classify the packet is found
* methods to extract multiple packets from the queue disc, while handling transmission \
  (to the device) failures by requeuing packets

The base class QueueDisc provides many trace sources:

* ``Enqueue``
* ``Dequeue``
* ``Requeue``
* ``Drop``
* ``PacketsInQueue``
* ``BytesInQueue``

The C++ base class QueueDisc holds the list of attached queues, classes and filter
by means of three vectors accessible through attributes (InternalQueueList,
QueueDiscClassList and PacketFilterList).

Internal queues are implemented as (subclasses of) Queue objects. A Queue stores
QueueItem objects, which consist of just a Ptr<Packet>. Since a queue disc has to
store at least the destination address and the protocol number for each enqueued
packet, a new C++ class, QueueDiscItem, is derived from QueueItem to store such
additional information for each packet. Thus, internal queues are implemented as
Queue objects storing QueueDiscItem objects. Also, there could be the need to store
further information depending on the network layer protocol of the packet. For
instance, for IPv4 and IPv6 packets it is needed to separately store the header
and the payload, so that header fields can be manipulated, e.g., to support
Explicit Congestion Notification as defined in RFC 3168.  To this end,
subclasses ``Ipv4QueueDiscItem`` and ``Ipv6QueueDiscItem`` are derived from
``QueueDiscItem`` to additionally store the IP header and provide protocol
specific operations such as ECN marking.

Classes (in the Linux sense of the term) are implemented via the QueueDiscClass class, which consists of a pointer
to the attached queue disc. Such a pointer is accessible through the QueueDisc attribute.
Classful queue discs needing to set parameters for their classes can subclass
QueueDiscClass and add the required parameters as attributes.

An abstract base class, PacketFilter, is subclassed to implement specific filters.
Subclasses are required to implement two virtual private pure methods:

* ``bool CheckProtocol (Ptr<QueueDiscItem> item) const``: check whether the filter \
  is able to classify packets of the same protocol as the given packet
* ``int32_t DoClassify (Ptr<QueueDiscItem> item) const``: actually classify the packet

PacketFilter provides a public method, ``Classify``, which first calls ``CheckProtocol``
to check that the protocol of the packet matches the protocol of the filter and then
calls ``DoClassify``. Specific filters subclassed from PacketFilter should not be
placed in the traffic-control module but in the module corresponding to the protocol
of the classified packets.


Usage
*****

By default, the InternetStackHelper aggregates a TrafficControlLayer object to every
node. When invoked to assign an IPv{4,6} address to a device, the Ipv{4,6}AddressHelper,
besides creating a Ipv{4,6}Interface, also installs the default qdisc, PfifoFastQueueDisc,
on the device, unless a queue disc has been already installed. Thus, devices get the default
queue disc installed even if they are added to the node after the Internet stack has been
installed on the node.

To install a queue disc other than the default one, it is necessary to install such queue
disc before an IP address is assigned to the device. Alternatively, the default queue disc
can be removed from the device after assigning an IP address, by using the convenient
Uninstall method of the TrafficControlHelper C++ class, and then installing a different
queue disc on the device. Clearly, it is also possible to have no queue disc installed on a device.

Helpers
=======

A typical usage pattern is to create a traffic control helper and to configure type
and attributes of queue discs, queues, classes and filters from the helper, For example,
the default pfifo_fast can be configured as follows:

.. sourcecode:: cpp

  TrafficControlHelper tch;
  uint16_t handle = tch.SetRootQueueDisc ("ns3::PfifoFastQueueDisc");
  tch.AddInternalQueues (handle, 3, "ns3::DropTailQueue", "MaxPackets", UintegerValue (1000));
  QueueDiscContainer qdiscs = tch.Install (devices);

The above code adds three internal queues and a packet filter to the root queue disc of type PfifoFast.
With the above configuration, the config path of the root queue disc installed on the j-th
device of the i-th node (the index of a device is the same as in DeviceList) is:

/NodeList/[i]/$ns3::TrafficControlLayer/RootQueueDiscList/[j]

and the config path of the second internal queue is:

/NodeList/[i]/$ns3::TrafficControlLayer/RootQueueDiscList/[j]/InternalQueueList/1

Implementation details
**********************

In Linux, the struct netdev_queue is used to store information about a single 
transmission queue of a device: status (i.e., whether it has been stopped or not),
data used by techniques such as Byte Queue Limits and a qdisc pointer field that
is mainly used to solve the following problems:

* if a device transmission queue is (almost) empty, identify the queue disc to wake
* if a packet will be enqueued in a given device transmission queue, identify the \
  queue disc which the packet must be enqueued into

The latter problem arises because Linux attempts to determine the device transmission
queue which a packet will be enqueued into before passing the packet to a queue disc.
This is done by calling a specific function of the device driver, if implemented, or
by employing fallback mechanisms (such as hashing of the addresses) otherwise. The
identifier of the selected device transmission queue is stored in the queue_mapping \
field of the struct sk_buff, so that both the queue disc and the device driver can
get the same information. In ns-3, such identifier is stored in a member of the
QueueDiscItem class.

The NetDeviceQueue class in ns-3 is the equivalent of the Linux struct netdev_queue.
The qdisc field of the Linux struct netdev_queue, however, cannot be
similarly stored in a NetDeviceQueue object, because it would make the network module
depend on the traffic-control module. Instead, this information is stored in the
TrafficControlLayer object aggregated to each node. In particular, a TrafficControlLayer
object holds a struct NetDeviceInfo which stores, for each NetDevice, a pointer to the
root queue disc installed on the device, a pointer to the netdevice queue interface
(see below) aggregated to the device, and a vector of pointers (one for each device
transmission queue) to the queue discs to activate when the above
problems occur. The traffic control layer takes care of configuring such a vector
at initialization time, based on the "wake mode" of the root queue disc. If the
wake mode of the root queue disc is WAKE_ROOT, then all the elements of the vector
are pointers to the root queue disc. If the wake mode of the root queue disc is
WAKE_CHILD, then each element of the vector is a pointer to a distinct child queue
disc. This requires that the number of child queue discs matches the number of
netdevice queues. It follows that the wake mode of a classless queue disc must
necessarily be WAKE_ROOT. These two configurations are illustrated by the figures below.

:ref:`fig-classful-queue-disc` below shows how the TrafficControlLayer map looks like in
case of a classful root queue disc whose wake mode is WAKE_ROOT.

.. _fig-classful-queue-disc:

.. figure:: figures/classful-queue-disc.*

    Setup of a queue disc (wake mode: WAKE_ROOT)

:ref:`fig-multi-queue-aware-queue-disc` below shows instead how the TrafficControlLayer
map looks like in case of a classful root queue disc whose wake mode is WAKE_CHILD.

.. _fig-multi-queue-aware-queue-disc:

.. figure:: figures/multi-queue-aware-queue-disc.*

    Setup of a multi-queue aware queue disc

A NetDeviceQueueInterface object is used by the traffic control layer to access the
information stored in the NetDeviceQueue objects, retrieve the number of transmission
queues of the device and get the transmission queue selected for the transmission of a
given packet. A NetDeviceQueueInterface object must be therefore aggregated to all the
devices having an interface supporting the traffic control layer (i.e., an IPv4 or IPv6
interface). In particular:

* a NetDeviceQueueInterface object is aggregated to all the devices as soon as an IPv4/v6 \
  interface is added to the device. This is because Ipv{4,6}AddressHelper::Assign calls \
  Ipv{4,6}L3Protocol::AddInterface, which calls TrafficControlLayer::SetupDevice, which \
  creates the queue interface and aggregates it to device.

* when notified that a netdevice queue interface has been aggregated, traffic control \
  aware devices can cache the pointer to the \
  netdevice queue interface created by the traffic contol layer into a member variable. \
  Also, multi-queue devices can set the number of device transmission queues and set the \
  select queue callback through the netdevice queue interface

* at initialization time, the traffic control (after calling device->Initialize () to ensure \
  that the netdevice has set the number of device transmission queues, if it has to do so) \
  completes the installation of the queue discs by setting the wake callbacks on the device \
  transmission queues (through the netdevice queue interface). Also, the traffic control \
  calls the Initialize method of the root queue discs.

Requeue
========
In Linux, a packet dequeued from a queue disc can be requeued (i.e., stored somewhere
and sent to the device at a later time) in some circumstances. Firstly, the function
used to dequeue a packet (dequeue_skb) actually dequeues a packet only if the device
is multi-queue or the (unique) device queue is not stopped. If a packet has been
dequeued from the queue disc, it is passed to the sch_direct_xmit function for
transmission to the device. This function checks whether the device queue the packet is destined
to is stopped, in which case the packet is requeued. Otherwise, the packet is sent to the device.
If the device returns NETDEV_TX_BUSY, the packet is requeued. However, it is advised that
the function called to send a packet to the device (ndo_start_xmit) should always
return NETDEV_TX_OK, which means that the packet is consumed by the device driver
and thus needs not to be requeued. However, the ndo_start_xmit function of the device
driver is allowed to return NETDEV_TX_BUSY (and hence the packet is requeued) when
there is no room for the received packet in the device queue, despite the queue is
not stopped. This case is considered as a corner case or an hard error, and should be avoided.

ns-3 implements the requeue mechanism in a similar manner, the only difference being
that packets are not requeued when such corner cases occur. Basically, the method used
to dequeue a packet (QueueDisc::DequeuePacket) actually dequeues a packet only if the
device is multi-queue or the (unique) device queue is not stopped. If a packet has been
dequeued from the queue disc, it is passed to the QueueDisc::Transmit method for
transmission to the device. This method checks whether the device queue the packet is destined
to is stopped, in which case the packet is requeued. Otherwise, the packet is sent to the device.
We request netdevices to stop a device queue when it is not able to store another packet,
so as to avoid the situation in which a packet is received that cannot be enqueued while
the device queue is not stopped. Should such a corner case occur, the netdevice drops
the packet but, unlike Linux, the value returned by NetDevice::Send is ignored and the
packet is not requeued.


The way the requeue mechanism is implemented in ns-3 has the following implications:

* if the underlying device has a single queue, no packet will ever be requeued. Indeed, \
  if the device queue is not stopped when QueueDisc::DequeuePacket is called, it will \
  not be stopped also when QueueDisc::Transmit is called, hence the packet is not requeued \
  (recall that a packet is not requeued after being sent to the device, as the value \
  returned by NetDevice::Send is ignored).
* if the underlying device does not implement flow control, i.e., it does not stop its queue(s), \
  no packet will ever be requeued (recall that a packet is only requeued by QueueDisc::Transmit \
  when the device queue the packet is destined to is stopped)

It turns out that packets may only be requeued when the underlying device is multi-queue
and supports flow control.
