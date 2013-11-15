.. include:: replace.txt
.. highlight:: cpp

Emu NetDevice
-------------

**Note:** The ``EmuNetDevice`` is being deprecated as of ns-3.17.  The
:doc:`fd-net-device` and ``EmuFdNetDeviceHelper`` class replaces this functionality and API entirely.

Behavior
********

The ``Emu`` net device allows a simulation node to send and receive packets over
a real network. The emulated net device relies on a specified interface being in
promiscuous mode. It opens a raw socket and binds to that interface.  We perform
MAC spoofing to separate simulation network traffic from other 
network traffic that may be flowing to and from the host.

One can use the ``Emu`` net device in a testbed situation where the host on
which the simulation is running has a specific interface of interest which
drives the testbed hardware. You would also need to set this specific interface
into promiscuous mode and provide an appropriate device name to the |ns3|
emulated net device. An example of this environment is the ORBIT testbed as
described above.

The ``Emu`` net device only works if the underlying interface is up and in
promiscuous mode. Packets will be sent out over the device, but we use MAC
spoofing. The MAC addresses will be generated (by default) using the
Organizationally Unique Identifier (OUI) 00:00:00 as a base. This vendor code is
not assigned to any organization and so should not conflict with any real
hardware.  

It is always up to the user to determine that using these MAC addresses is okay
on your network and won't conflict with anything else (including another
simulation using ``Emu`` devices) on your network. If you are using the emulated
net device in separate simulations you must consider global MAC address
assignment issues and ensure that MAC addresses are unique across all
simulations. The emulated net device respects the MAC address provided in the
``SetAddress`` method so you can do this manually. For larger simulations, you
may want to set the OUI in the MAC address allocation function.

IP addresses corresponding to the emulated net devices are the addresses
generated in the simulation, which are generated in the usual way via helper
functions. Since we are using MAC spoofing, there will not be a conflict between
|ns3| network stacks and any native network stacks.

The emulated net device comes with a helper function as all |ns3| devices do.
One unique aspect is that there is no channel associated with the underlying
medium. We really have no idea what this external medium is, and so have not
made an effort to model it abstractly. The primary thing to be aware of is the
implication this has for IPv4 global routing. The global router module attempts
to walk the channels looking for adjacent networks. Since there is no channel,
the global router will be unable to do this and you must then use a dynamic
routing protocol such as OLSR to include routing in ``Emu``-based networks.

Usage
*****

Any mixing of |ns3| objects with real objects will typically require that
|ns3| compute checksums in its protocols. By default, checksums are not
computed by |ns3|. To enable checksums (e.g. UDP, TCP, IP), users must set
the attribute ``ChecksumEnabled`` to true, such as follows::

    GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

The usage of the ``Emu`` net device is straightforward once the network of
simulations has been configured. Since most of the work involved in working with
this device is in network configuration before even starting a simulation, you
may want to take a moment to review a couple of HOWTO pages on the |ns3| wiki
that describe how to set up a virtual test network using VMware and how to run a
set of example (client server) simulations that use ``Emu`` net devices.

* `<http://www.nsnam.org/wiki/HOWTO_use_VMware_to_set_up_virtual_networks_(Windows)>`_
* `<http://www.nsnam.org/wiki/HOWTO_use_ns-3_scripts_to_drive_real_hardware_(experimental)>`_

Once you are over the configuration hurdle, the script changes required to use
an ``Emu`` device are trivial. The main structural difference is that you will
need to create an |ns3| simulation script for each node. In the case of the
HOWTOs above, there is one client script and one server script. The only
"challenge" is to get the addresses set correctly.

Just as with all other |ns3| net devices, we provide a helper class for the
``Emu`` net device. The following code snippet illustrates how one would declare
an EmuHelper and use it to set the "DeviceName" attribute to "eth1" and install
``Emu`` devices on a group of nodes. You would do this on both the client and
server side in the case of the HOWTO seen above.::

  EmuHelper emu;
  emu.SetAttribute ("DeviceName", StringValue ("eth1"));
  NetDeviceContainer d = emu.Install (n);

The only other change that may be required is to make sure that the address
spaces (MAC and IP) on the client and server simulations are compatible. First
the MAC address is set to a unique well-known value in both places (illustrated
here for one side).::

  //
  // We've got the devices in place.  Since we're using MAC address 
  // spoofing under the sheets, we need to make sure that the MAC addresses
  // we have assigned to our devices are unique.  Ns-3 will happily
  // automatically assign the same MAC address to the devices in both halves
  // of our two-script pair, so let's go ahead and just manually change them
  // to something we ensure is unique.
  //
  Ptr<NetDevice> nd = d.Get (0);
  Ptr<EmuNetDevice> ed = nd->GetObject<EmuNetDevice> ();
  ed->SetAddress ("00:00:00:00:00:02");

And then the IP address of the client or server is set in the usual way using
helpers.::

  //
  // We've got the "hardware" in place.  Now we need to add IP addresses.
  // This is the server half of a two-script pair.  We need to make sure
  // that the addressing in both of these applications is consistent, so
  // we use provide an initial address in both cases.  Here, the client 
  // will reside on one machine running ns-3 with one node having ns-3
  // with IP address "10.1.1.2" and talk to a server script running in 
  // another ns-3 on another computer that has an ns-3 node with IP 
  // address "10.1.1.3"
  //
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0", "0.0.0.2");
  Ipv4InterfaceContainer i = ipv4.Assign (d);

You will use application helpers to generate traffic exactly as you do in any
|ns3| simulation script. Note that the server address shown below in a snippet
from the client, must correspond to the IP address assigned to the server node
similarly to the snippet above.::

  uint32_t packetSize = 1024;
  uint32_t maxPacketCount = 2000;
  Time interPacketInterval = Seconds (0.001);
  UdpEchoClientHelper client ("10.1.1.3", 9);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (packetSize));
  ApplicationContainer apps = client.Install (n.Get (0));
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (2.0));

The ``Emu`` net device and helper provide access to ASCII and pcap tracing
functionality just as other |ns3| net devices to. You enable tracing similarly
to these other net devices::

  EmuHelper::EnablePcapAll ("emu-udp-echo-client");

For examples that use the ``Emu`` net device, see
``src/emu/examples/emu-udp-echo.cc`` and ``src/emu/examples/emu-ping.cc``
in the repository.

Implementation
**************

Perhaps the most unusual part of the ``Emu`` and ``Tap`` device implementation
relates to the requirement for executing some of the code with super-user
permissions. Rather than force the user to execute the entire simulation as
root, we provide a small "creator" program that runs as root and does any
required high-permission sockets work.

We do a similar thing for both the ``Emu`` and the ``Tap`` devices.  The
high-level view is that the ``CreateSocket`` method creates a local interprocess
(Unix) socket, forks, and executes the small creation program. The small
program, which runs as suid root, creates a raw socket and sends back the raw
socket file descriptor over the Unix socket that is passed to it as a parameter.
The raw socket is passed as a control message (sometimes called ancillary data)
of type SCM_RIGHTS.

The ``Emu`` net device uses the |ns3| threading and multithreaded real-time
scheduler extensions. The interesting work in the ``Emu`` device is done when
the net device is started (``EmuNetDevice::StartDevice ()``). An attribute
("Start") provides a simulation time at which to spin up the net device. At this
specified time (which defaults to t=0), the socket creation function is called
and executes as described above. You may also specify a time at which to stop
the device using the "Stop" attribute.

Once the (promiscuous mode) socket is created, we bind it to an interface name 
also provided as an attribute ("DeviceName") that is stored internally as 
``m_deviceName``::

  struct ifreq ifr;
  bzero (&ifr, sizeof(ifr));
  strncpy ((char *)ifr.ifr_name, m_deviceName.c_str (), IFNAMSIZ);

  int32_t rc = ioctl (m_sock, SIOCGIFINDEX, &ifr);

  struct sockaddr_ll ll;
  bzero (&ll, sizeof(ll));

  ll.sll_family = AF_PACKET;
  ll.sll_ifindex = m_sll_ifindex;
  ll.sll_protocol = htons(ETH_P_ALL);

  rc = bind (m_sock, (struct sockaddr *)&ll, sizeof (ll));

After the promiscuous raw socket is set up, a separate thread is spawned to do 
reads from that socket and the link state is set to ``Up``.::

  m_readThread = Create<SystemThread> (
    MakeCallback (&EmuNetDevice::ReadThread, this));
  m_readThread->Start ();

  NotifyLinkUp ();

The ``EmuNetDevice::ReadThread`` function basically just sits in an infinite
loop reading from the promiscuous mode raw socket and scheduling packet 
receptions using the real-time simulator extensions.::

  for (;;)
    {
      ...

      len = recvfrom (m_sock, buf, bufferSize, 0, (struct sockaddr *)&addr, 
        &addrSize);

      ...

      DynamicCast<RealtimeSimulatorImpl> (Simulator::GetImplementation ())->
        ScheduleRealtimeNow (
          MakeEvent (&EmuNetDevice::ForwardUp, this, buf, len));

      ...
    }

The line starting with our templated DynamicCast function probably deserves a
comment. It gains access to the simulator implementation object using the
``Simulator::GetImplementation`` method and then casts to the real-time
simulator implementation to use the real-time schedule method
``ScheduleRealtimeNow``. This function will cause a handler for the  newly
received packet to be scheduled for execution at the current real time clock
value. This will, in turn cause the simulation clock to be advanced to that real
time value when the scheduled event (``EmuNetDevice::ForwardUp``) is fired.

The ``ForwardUp`` function operates as most other similar |ns3| net device
methods do. The packet is first filtered based on the destination address. In
the case of the ``Emu`` device, the MAC destination address will be the address
of the ``Emu`` device and not the hardware address of the real device. Headers
are then stripped off and the trace hooks are hit. Finally, the packet is passed
up the |ns3| protocol stack using the receive callback function of the net
device.

Sending a packet is equally straightforward as shown below. The first thing we
do is to add the ethernet header and trailer to the |ns3| ``Packet`` we are
sending. The source address corresponds to the address of the ``Emu`` device and
not the underlying native device MAC address. This is where the MAC address
spoofing is done. The trailer is added and we enqueue and dequeue the packet
from the net device queue to hit the trace hooks.::

  header.SetSource (source);
  header.SetDestination (destination);
  header.SetLengthType (packet->GetSize ());
  packet->AddHeader (header);

  EthernetTrailer trailer;
  trailer.CalcFcs (packet);
  packet->AddTrailer (trailer);

  m_queue->Enqueue (packet);
  packet = m_queue->Dequeue ();

  struct sockaddr_ll ll;
  bzero (&ll, sizeof (ll));

  ll.sll_family = AF_PACKET;
  ll.sll_ifindex = m_sll_ifindex;
  ll.sll_protocol = htons(ETH_P_ALL);

  rc = sendto (m_sock, packet->PeekData (), packet->GetSize (), 0, 
    reinterpret_cast<struct sockaddr *> (&ll), sizeof (ll));

Finally, we simply send the packet to the raw socket which puts it out on the
real network.

From the point of view of tracing in the net device, there are several
interesting points to insert trace hooks.  A convention inherited from other
simulators is that packets destined for transmission onto attached networks
pass through a single "transmit queue" in the net device.  We provide trace
hooks at this point in packet flow, which corresponds (abstractly) only to a
transition from the network to data link layer, and call them collectively
the device MAC hooks.

When a packet is sent to the Emu net device for transmission it always
passes through the transmit queue.  The transmit queue in the EmuNetDevice
inherits from Queue, and therefore inherits three trace sources:

* An Enqueue operation source (see Queue::m_traceEnqueue);
* A Dequeue operation source (see Queue::m_traceDequeue);
* A Drop operation source (see Queue::m_traceDrop).

The upper-level (MAC) trace hooks for the EmuNetDevice are, in fact,
exactly these three trace sources on the single transmit queue of the device. 

The m_traceEnqueue event is triggered when a packet is placed on the transmit
queue.  This happens at the time that ns3::EmuNetDevice::Send or
``ns3::EmuNetDevice::SendFrom`` is called by a higher layer to queue a 
packet for transmission.

The m_traceDequeue event is triggered when a packet is removed from the
transmit queue.  Dequeues from the transmit queue happen immediately after
the packet was enqueued and only indicate that the packet is about to be
sent to an underlying raw socket.  The actual time at which the packet is
sent out on the wire is not available.

Similar to the upper level trace hooks, there are trace hooks available at
the lower levels of the net device.  We call these the PHY hooks.  These
events fire from the device methods that talk directly to the underlying
raw socket.

The trace source m_dropTrace is not used in the Emu net device since that
is really the business of the underlying "real" device driver.

The other low-level trace source fires on reception of an accepted packet
(see ``ns3::EmuNetDevice::m_rxTrace``).  A packet is accepted if it is destined
for the broadcast address, a multicast address, or to the MAC address
assigned to the Emu net device.

