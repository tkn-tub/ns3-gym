.. include:: replace.txt
.. highlight:: cpp

Building Topologies
-------------------

Building a Bus Network Topology
*******************************

In this section we are going to expand our mastery of |ns3| network 
devices and channels to cover an example of a bus network.  |ns3|
provides a net device and channel we call CSMA (Carrier Sense Multiple Access).

The |ns3| CSMA device models a simple network in the spirit of 
Ethernet.  A real Ethernet uses CSMA/CD (Carrier Sense Multiple Access with 
Collision Detection) scheme with exponentially increasing backoff to contend 
for the shared transmission medium.  The |ns3| CSMA device and 
channel models only a subset of this.

Just as we have seen point-to-point topology helper objects when constructing
point-to-point topologies, we will see equivalent CSMA topology helpers in
this section.  The appearance and operation of these helpers should look 
quite familiar to you.

We provide an example script in our examples/tutorial} directory.  This script
builds on the ``first.cc`` script and adds a CSMA network to the 
point-to-point simulation we've already considered.  Go ahead and open 
``examples/tutorial/second.cc`` in your favorite editor.  You will have already seen
enough |ns3| code to understand most of what is going on in this 
example, but we will go over the entire script and examine some of the output.

Just as in the ``first.cc`` example (and in all ns-3 examples) the file
begins with an emacs mode line and some GPL boilerplate.

The actual code begins by loading module include files just as was done in the
``first.cc`` example.

::

  #include "ns3/core-module.h"
  #include "ns3/network-module.h"
  #include "ns3/csma-module.h"
  #include "ns3/internet-module.h"
  #include "ns3/point-to-point-module.h"
  #include "ns3/applications-module.h"
  #include "ns3/ipv4-global-routing-helper.h"

One thing that can be surprisingly useful is a small bit of ASCII art that
shows a cartoon of the network topology constructed in the example.  You will
find a similar "drawing" in most of our examples.

In this case, you can see that we are going to extend our point-to-point
example (the link between the nodes n0 and n1 below) by hanging a bus network
off of the right side.  Notice that this is the default network topology 
since you can actually vary the number of nodes created on the LAN.  If you
set nCsma to one, there will be a total of two nodes on the LAN (CSMA 
channel) --- one required node and one "extra" node.  By default there are
three "extra" nodes as seen below:

::

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 10.1.2.0

Then the ns-3 namespace is ``used`` and a logging component is defined.
This is all just as it was in ``first.cc``, so there is nothing new yet.

::
  
  using namespace ns3;
  
  NS_LOG_COMPONENT_DEFINE ("SecondScriptExample");

The main program begins with a slightly different twist.  We use a verbose
flag to determine whether or not the ``UdpEchoClientApplication`` and
``UdpEchoServerApplication`` logging components are enabled.  This flag
defaults to true (the logging components are enabled) but allows us to turn
off logging during regression testing of this example.

You will see some familiar code that will allow you to change the number
of devices on the CSMA network via command line argument.  We did something
similar when we allowed the number of packets sent to be changed in the section
on command line arguments.  The last line makes sure you have at least one
"extra" node.

The code consists of variations of previously covered API so you should be
entirely comfortable with the following code at this point in the tutorial.

::

  bool verbose = true;
  uint32_t nCsma = 3;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc, argv);

  if (verbose)
    {
      LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;

The next step is to create two nodes that we will connect via the 
point-to-point link.  The ``NodeContainer`` is used to do this just as was
done in ``first.cc``.

::

  NodeContainer p2pNodes;
  p2pNodes.Create (2);

Next, we declare another ``NodeContainer`` to hold the nodes that will be
part of the bus (CSMA) network.  First, we just instantiate the container
object itself.  

::

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma);

The next line of code ``Gets`` the first node (as in having an index of one)
from the point-to-point node container and adds it to the container of nodes
that will get CSMA devices.  The node in question is going to end up with a 
point-to-point device *and* a CSMA device.  We then create a number of 
"extra" nodes that compose the remainder of the CSMA network.  Since we 
already have one node in the CSMA network -- the one that will have both a
point-to-point and CSMA net device, the number of "extra" nodes means the
number nodes you desire in the CSMA section minus one.

The next bit of code should be quite familiar by now.  We instantiate a
``PointToPointHelper`` and set the associated default ``Attributes`` so
that we create a five megabit per second transmitter on devices created using
the helper and a two millisecond delay on channels created by the helper.

::

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

We then instantiate a ``NetDeviceContainer`` to keep track of the 
point-to-point net devices and we ``Install`` devices on the 
point-to-point nodes.

We mentioned above that you were going to see a helper for CSMA devices and
channels, and the next lines introduce them.  The ``CsmaHelper`` works just
like a ``PointToPointHelper``, but it creates and connects CSMA devices and
channels.  In the case of a CSMA device and channel pair, notice that the data
rate is specified by a *channel* ``Attribute`` instead of a device 
``Attribute``.  This is because a real CSMA network does not allow one to mix,
for example, 10Base-T and 100Base-T devices on a given channel.  We first set 
the data rate to 100 megabits per second, and then set the speed-of-light delay
of the channel to 6560 nano-seconds (arbitrarily chosen as 1 nanosecond per foot
over a 100 meter segment).  Notice that you can set an ``Attribute`` using 
its native data type.

::

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

Just as we created a ``NetDeviceContainer`` to hold the devices created by
the ``PointToPointHelper`` we create a ``NetDeviceContainer`` to hold 
the devices created by our ``CsmaHelper``.  We call the ``Install`` 
method of the ``CsmaHelper`` to install the devices into the nodes of the
``csmaNodes NodeContainer``.

We now have our nodes, devices and channels created, but we have no protocol
stacks present.  Just as in the ``first.cc`` script, we will use the
``InternetStackHelper`` to install these stacks.

::

  InternetStackHelper stack;
  stack.Install (p2pNodes.Get (0));
  stack.Install (csmaNodes);

Recall that we took one of the nodes from the ``p2pNodes`` container and
added it to the ``csmaNodes`` container.  Thus we only need to install 
the stacks on the remaining ``p2pNodes`` node, and all of the nodes in the
``csmaNodes`` container to cover all of the nodes in the simulation.

Just as in the ``first.cc`` example script, we are going to use the 
``Ipv4AddressHelper`` to assign IP addresses to our device interfaces.
First we use the network 10.1.1.0 to create the two addresses needed for our
two point-to-point devices.

::

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

Recall that we save the created interfaces in a container to make it easy to
pull out addressing information later for use in setting up the applications.

We now need to assign IP addresses to our CSMA device interfaces.  The 
operation works just as it did for the point-to-point case, except we now
are performing the operation on a container that has a variable number of 
CSMA devices --- remember we made the number of CSMA devices changeable by 
command line argument.  The CSMA devices will be associated with IP addresses 
from network number 10.1.2.0 in this case, as seen below.

::

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

Now we have a topology built, but we need applications.  This section is
going to be fundamentally similar to the applications section of 
``first.cc`` but we are going to instantiate the server on one of the 
nodes that has a CSMA device and the client on the node having only a 
point-to-point device.

First, we set up the echo server.  We create a ``UdpEchoServerHelper`` and
provide a required ``Attribute`` value to the constructor which is the server
port number.  Recall that this port can be changed later using the 
``SetAttribute`` method if desired, but we require it to be provided to
the constructor.

::

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

Recall that the ``csmaNodes NodeContainer`` contains one of the 
nodes created for the point-to-point network and ``nCsma`` "extra" nodes. 
What we want to get at is the last of the "extra" nodes.  The zeroth entry of
the ``csmaNodes`` container will be the point-to-point node.  The easy
way to think of this, then, is if we create one "extra" CSMA node, then it
will be at index one of the ``csmaNodes`` container.  By induction,
if we create ``nCsma`` "extra" nodes the last one will be at index 
``nCsma``.  You see this exhibited in the ``Get`` of the first line of 
code.

The client application is set up exactly as we did in the ``first.cc``
example script.  Again, we provide required ``Attributes`` to the 
``UdpEchoClientHelper`` in the constructor (in this case the remote address
and port).  We tell the client to send packets to the server we just installed
on the last of the "extra" CSMA nodes.  We install the client on the 
leftmost point-to-point node seen in the topology illustration.

::

  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (p2pNodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

Since we have actually built an internetwork here, we need some form of 
internetwork routing.  |ns3| provides what we call global routing to
help you out.  Global routing takes advantage of the fact that the entire 
internetwork is accessible in the simulation and runs through the all of the
nodes created for the simulation --- it does the hard work of setting up routing 
for you without having to configure routers.

Basically, what happens is that each node behaves as if it were an OSPF router
that communicates instantly and magically with all other routers behind the
scenes.  Each node generates link advertisements and communicates them 
directly to a global route manager which uses this global information to 
construct the routing tables for each node.  Setting up this form of routing
is a one-liner:

::

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

Next we enable pcap tracing.  The first line of code to enable pcap tracing 
in the point-to-point helper should be familiar to you by now.  The second
line enables pcap tracing in the CSMA helper and there is an extra parameter
you haven't encountered yet.

::

  pointToPoint.EnablePcapAll ("second");
  csma.EnablePcap ("second", csmaDevices.Get (1), true);

The CSMA network is a multi-point-to-point network.  This means that there 
can (and are in this case) multiple endpoints on a shared medium.  Each of 
these endpoints has a net device associated with it.  There are two basic
alternatives to gathering trace information from such a network.  One way 
is to create a trace file for each net device and store only the packets
that are emitted or consumed by that net device.  Another way is to pick 
one of the devices and place it in promiscuous mode.  That single device
then "sniffs" the network for all packets and stores them in a single
pcap file.  This is how ``tcpdump``, for example, works.  That final 
parameter tells the CSMA helper whether or not to arrange to capture 
packets in promiscuous mode.  

In this example, we are going to select one of the devices on the CSMA
network and ask it to perform a promiscuous sniff of the network, thereby
emulating what ``tcpdump`` would do.  If you were on a Linux machine
you might do something like ``tcpdump -i eth0`` to get the trace.  
In this case, we specify the device using ``csmaDevices.Get(1)``, 
which selects the first device in the container.  Setting the final
parameter to true enables promiscuous captures.

The last section of code just runs and cleans up the simulation just like
the ``first.cc`` example.

::

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
  }

In order to run this example, copy the ``second.cc`` example script into 
the scratch directory and use waf to build just as you did with
the ``first.cc`` example.  If you are in the top-level directory of the
repository you just type,

.. sourcecode:: bash

  $ cp examples/tutorial/second.cc scratch/mysecond.cc
  $ ./waf

Warning:  We use the file ``second.cc`` as one of our regression tests to
verify that it works exactly as we think it should in order to make your
tutorial experience a positive one.  This means that an executable named 
``second`` already exists in the project.  To avoid any confusion
about what you are executing, please do the renaming to ``mysecond.cc``
suggested above.

If you are following the tutorial religiously (you are, aren't you) you will
still have the NS_LOG variable set, so go ahead and clear that variable and
run the program.

.. sourcecode:: bash

  $ export NS_LOG=
  $ ./waf --run scratch/mysecond

Since we have set up the UDP echo applications to log just as we did in 
``first.cc``, you will see similar output when you run the script.

.. sourcecode:: text

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (0.415s)
  Sent 1024 bytes to 10.1.2.4
  Received 1024 bytes from 10.1.1.1
  Received 1024 bytes from 10.1.2.4

Recall that the first message, "``Sent 1024 bytes to 10.1.2.4``," is the 
UDP echo client sending a packet to the server.  In this case, the server
is on a different network (10.1.2.0).  The second message, "``Received 1024 
bytes from 10.1.1.1``," is from the UDP echo server, generated when it receives
the echo packet.  The final message, "``Received 1024 bytes from 10.1.2.4``,"
is from the echo client, indicating that it has received its echo back from
the server.

If you now go and look in the top level directory, you will find three trace 
files:

.. sourcecode:: text

  second-0-0.pcap  second-1-0.pcap  second-2-0.pcap

Let's take a moment to look at the naming of these files.  They all have the 
same form, ``<name>-<node>-<device>.pcap``.  For example, the first file
in the listing is ``second-0-0.pcap`` which is the pcap trace from node 
zero, device zero.  This is the point-to-point net device on node zero.  The 
file ``second-1-0.pcap`` is the pcap trace for device zero on node one,
also a point-to-point net device; and the file ``second-2-0.pcap`` is the
pcap trace for device zero on node two.

If you refer back to the topology illustration at the start of the section, 
you will see that node zero is the leftmost node of the point-to-point link
and node one is the node that has both a point-to-point device and a CSMA 
device.  You will see that node two is the first "extra" node on the CSMA
network and its device zero was selected as the device to capture the 
promiscuous-mode trace.

Now, let's follow the echo packet through the internetwork.  First, do a 
tcpdump of the trace file for the leftmost point-to-point node --- node zero.

.. sourcecode:: bash

  $ tcpdump -nn -tt -r second-0-0.pcap

You should see the contents of the pcap file displayed:

.. sourcecode:: text

  reading from file second-0-0.pcap, link-type PPP (PPP)
  2.000000 IP 10.1.1.1.49153 > 10.1.2.4.9: UDP, length 1024
  2.007602 IP 10.1.2.4.9 > 10.1.1.1.49153: UDP, length 1024

The first line of the dump indicates that the link type is PPP (point-to-point)
which we expect.  You then see the echo packet leaving node zero via the 
device associated with IP address 10.1.1.1 headed for IP address
10.1.2.4 (the rightmost CSMA node).  This packet will move over the 
point-to-point link and be received by the point-to-point net device on node 
one.  Let's take a look:

.. sourcecode:: bash

  $ tcpdump -nn -tt -r second-1-0.pcap

You should now see the pcap trace output of the other side of the point-to-point
link:

.. sourcecode:: text

  reading from file second-1-0.pcap, link-type PPP (PPP)
  2.003686 IP 10.1.1.1.49153 > 10.1.2.4.9: UDP, length 1024
  2.003915 IP 10.1.2.4.9 > 10.1.1.1.49153: UDP, length 1024

Here we see that the link type is also PPP as we would expect.  You see the
packet from IP address 10.1.1.1 (that was sent at 2.000000 seconds) headed 
toward IP address 10.1.2.4 appear on this interface.  Now, internally to this 
node, the packet will be forwarded to the CSMA interface and we should see it 
pop out on that device headed for its ultimate destination.  

Remember that we selected node 2 as the promiscuous sniffer node for the CSMA
network so let's then look at second-2-0.pcap and see if its there.

.. sourcecode:: bash

  $ tcpdump -nn -tt -r second-2-0.pcap

You should now see the promiscuous dump of node two, device zero:

.. sourcecode:: text

  reading from file second-2-0.pcap, link-type EN10MB (Ethernet)
  2.003696 arp who-has 10.1.2.4 (ff:ff:ff:ff:ff:ff) tell 10.1.2.1
  2.003707 arp reply 10.1.2.4 is-at 00:00:00:00:00:06
  2.003801 IP 10.1.1.1.49153 > 10.1.2.4.9: UDP, length 1024
  2.003811 arp who-has 10.1.2.1 (ff:ff:ff:ff:ff:ff) tell 10.1.2.4
  2.003822 arp reply 10.1.2.1 is-at 00:00:00:00:00:03
  2.003915 IP 10.1.2.4.9 > 10.1.1.1.49153: UDP, length 1024

As you can see, the link type is now "Ethernet".  Something new has appeared,
though.  The bus network needs ``ARP``, the Address Resolution Protocol.
Node one knows it needs to send the packet to IP address 10.1.2.4, but it
doesn't know the MAC address of the corresponding node.  It broadcasts on the
CSMA network (ff:ff:ff:ff:ff:ff) asking for the device that has IP address
10.1.2.4.  In this case, the rightmost node replies saying it is at MAC address
00:00:00:00:00:06.  Note that node two is not directly involved in this 
exchange, but is sniffing the network and reporting all of the traffic it sees.

This exchange is seen in the following lines,

.. sourcecode:: text

  2.003696 arp who-has 10.1.2.4 (ff:ff:ff:ff:ff:ff) tell 10.1.2.1
  2.003707 arp reply 10.1.2.4 is-at 00:00:00:00:00:06

Then node one, device one goes ahead and sends the echo packet to the UDP echo
server at IP address 10.1.2.4. 

.. sourcecode:: text

  2.003801 IP 10.1.1.1.49153 > 10.1.2.4.9: UDP, length 1024

The server receives the echo request and turns the packet around trying to send
it back to the source.  The server knows that this address is on another network
that it reaches via IP address 10.1.2.1.  This is because we initialized global
routing and it has figured all of this out for us.  But, the echo server node
doesn't know the MAC address of the first CSMA node, so it has to ARP for it
just like the first CSMA node had to do.

.. sourcecode:: text

  2.003811 arp who-has 10.1.2.1 (ff:ff:ff:ff:ff:ff) tell 10.1.2.4
  2.003822 arp reply 10.1.2.1 is-at 00:00:00:00:00:03

The server then sends the echo back to the forwarding node.

.. sourcecode:: text

  2.003915 IP 10.1.2.4.9 > 10.1.1.1.49153: UDP, length 1024

Looking back at the rightmost node of the point-to-point link,

.. sourcecode:: bash

  $ tcpdump -nn -tt -r second-1-0.pcap

You can now see the echoed packet coming back onto the point-to-point link as
the last line of the trace dump.

.. sourcecode:: text

  reading from file second-1-0.pcap, link-type PPP (PPP)
  2.003686 IP 10.1.1.1.49153 > 10.1.2.4.9: UDP, length 1024
  2.003915 IP 10.1.2.4.9 > 10.1.1.1.49153: UDP, length 1024

Lastly, you can look back at the node that originated the echo

.. sourcecode:: bash

  $ tcpdump -nn -tt -r second-0-0.pcap

and see that the echoed packet arrives back at the source at 2.007602 seconds,

.. sourcecode:: text

  reading from file second-0-0.pcap, link-type PPP (PPP)
  2.000000 IP 10.1.1.1.49153 > 10.1.2.4.9: UDP, length 1024
  2.007602 IP 10.1.2.4.9 > 10.1.1.1.49153: UDP, length 1024

Finally, recall that we added the ability to control the number of CSMA devices
in the simulation by command line argument.  You can change this argument in
the same way as when we looked at changing the number of packets echoed in the
``first.cc`` example.  Try running the program with the number of "extra" 
devices set to four:

.. sourcecode:: bash

  $ ./waf --run "scratch/mysecond --nCsma=4"

You should now see,

.. sourcecode:: text

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (0.405s)
  Sent 1024 bytes to 10.1.2.5
  Received 1024 bytes from 10.1.1.1
  Received 1024 bytes from 10.1.2.5

Notice that the echo server has now been relocated to the last of the CSMA
nodes, which is 10.1.2.5 instead of the default case, 10.1.2.4.

It is possible that you may not be satisfied with a trace file generated by
a bystander in the CSMA network.  You may really want to get a trace from
a single device and you may not be interested in any other traffic on the 
network.  You can do this fairly easily.

Let's take a look at ``scratch/mysecond.cc`` and add that code enabling us
to be more specific.  ``ns-3`` helpers provide methods that take a node
number and device number as parameters.  Go ahead and replace the 
``EnablePcap`` calls with the calls below.

::

  pointToPoint.EnablePcap ("second", p2pNodes.Get (0)->GetId (), 0);
  csma.EnablePcap ("second", csmaNodes.Get (nCsma)->GetId (), 0, false);
  csma.EnablePcap ("second", csmaNodes.Get (nCsma-1)->GetId (), 0, false);

We know that we want to create a pcap file with the base name "second" and
we also know that the device of interest in both cases is going to be zero,
so those parameters are not really interesting.

In order to get the node number, you have two choices:  first, nodes are 
numbered in a monotonically increasing fashion starting from zero in the 
order in which you created them.  One way to get a node number is to figure 
this number out "manually" by contemplating the order of node creation.  
If you take a look at the network topology illustration at the beginning of 
the file, we did this for you and you can see that the last CSMA node is 
going to be node number ``nCsma + 1``.  This approach can become 
annoyingly difficult in larger simulations.  

An alternate way, which we use here, is to realize that the
``NodeContainers`` contain pointers to |ns3| ``Node`` Objects.
The ``Node`` Object has a method called ``GetId`` which will return that
node's ID, which is the node number we seek.  Let's go take a look at the 
Doxygen for the ``Node`` and locate that method, which is further down in 
the |ns3| core code than we've seen so far; but sometimes you have to
search diligently for useful things.

Go to the Doxygen documentation for your release (recall that you can find it
on the project web site).  You can get to the ``Node`` documentation by
looking through at the "Classes" tab and scrolling down the "Class List" 
until you find ``ns3::Node``.  Select ``ns3::Node`` and you will be taken
to the documentation for the ``Node`` class.  If you now scroll down to the
``GetId`` method and select it, you will be taken to the detailed 
documentation for the method.  Using the ``GetId`` method can make 
determining node numbers much easier in complex topologies.

Let's clear the old trace files out of the top-level directory to avoid confusion
about what is going on,

.. sourcecode:: bash

  $ rm *.pcap
  $ rm *.tr

If you build the new script and run the simulation setting ``nCsma`` to 100,

.. sourcecode:: bash

  $ ./waf --run "scratch/mysecond --nCsma=100"

you will see the following output:

.. sourcecode:: text

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (0.407s)
  Sent 1024 bytes to 10.1.2.101
  Received 1024 bytes from 10.1.1.1
  Received 1024 bytes from 10.1.2.101

Note that the echo server is now located at 10.1.2.101 which corresponds to
having 100 "extra" CSMA nodes with the echo server on the last one.  If you
list the pcap files in the top level directory you will see,

.. sourcecode:: text

  second-0-0.pcap  second-100-0.pcap  second-101-0.pcap

The trace file ``second-0-0.pcap`` is the "leftmost" point-to-point device
which is the echo packet source.  The file ``second-101-0.pcap`` corresponds
to the rightmost CSMA device which is where the echo server resides.  You may 
have noticed that the final parameter on the call to enable pcap tracing on the 
echo server node was false.  This means that the trace gathered on that node
was in non-promiscuous mode.

To illustrate the difference between promiscuous and non-promiscuous traces, we
also requested a non-promiscuous trace for the next-to-last node.  Go ahead and
take a look at the ``tcpdump`` for ``second-100-0.pcap``.

.. sourcecode:: bash

  $ tcpdump -nn -tt -r second-100-0.pcap

You can now see that node 100 is really a bystander in the echo exchange.  The
only packets that it receives are the ARP requests which are broadcast to the
entire CSMA network.

.. sourcecode:: text

  reading from file second-100-0.pcap, link-type EN10MB (Ethernet)
  2.003696 arp who-has 10.1.2.101 (ff:ff:ff:ff:ff:ff) tell 10.1.2.1
  2.003811 arp who-has 10.1.2.1 (ff:ff:ff:ff:ff:ff) tell 10.1.2.101

Now take a look at the ``tcpdump`` for ``second-101-0.pcap``.

.. sourcecode:: bash

  $ tcpdump -nn -tt -r second-101-0.pcap

You can now see that node 101 is really the participant in the echo exchange.

.. sourcecode:: text

  reading from file second-101-0.pcap, link-type EN10MB (Ethernet)
  2.003696 arp who-has 10.1.2.101 (ff:ff:ff:ff:ff:ff) tell 10.1.2.1
  2.003696 arp reply 10.1.2.101 is-at 00:00:00:00:00:67
  2.003801 IP 10.1.1.1.49153 > 10.1.2.101.9: UDP, length 1024
  2.003801 arp who-has 10.1.2.1 (ff:ff:ff:ff:ff:ff) tell 10.1.2.101
  2.003822 arp reply 10.1.2.1 is-at 00:00:00:00:00:03
  2.003822 IP 10.1.2.101.9 > 10.1.1.1.49153: UDP, length 1024

Models, Attributes and Reality
******************************

This is a convenient place to make a small excursion and make an important
point.  It may or may not be obvious to you, but whenever one is using a 
simulation, it is important to understand exactly what is being modeled and
what is not.  It is tempting, for example, to think of the CSMA devices 
and channels used in the previous section as if they were real Ethernet 
devices; and to expect a simulation result to directly reflect what will 
happen in a real Ethernet.  This is not the case.  

A model is, by definition, an abstraction of reality.  It is ultimately the 
responsibility of the simulation script author to determine the so-called
"range of accuracy" and "domain of applicability" of the simulation as
a whole, and therefore its constituent parts.

In some cases, like ``Csma``, it can be fairly easy to determine what is 
*not* modeled.  By reading the model description (``csma.h``) you 
can find that there is no collision detection in the CSMA model and decide
on how applicable its use will be in your simulation or what caveats you 
may want to include with your results.  In other cases, it can be quite easy
to configure behaviors that might not agree with any reality you can go out
and buy.  It will prove worthwhile to spend some time investigating a few
such instances, and how easily you can swerve outside the bounds of reality
in your simulations.

As you have seen, |ns3| provides ``Attributes`` which a user
can easily set to change model behavior.  Consider two of the ``Attributes``
of the ``CsmaNetDevice``:  ``Mtu`` and ``EncapsulationMode``.  
The ``Mtu`` attribute indicates the Maximum Transmission Unit to the 
device.  This is the size of the largest Protocol Data Unit (PDU) that the
device can send.  

The MTU defaults to 1500 bytes in the ``CsmaNetDevice``.  This default
corresponds to a number found in RFC 894, "A Standard for the Transmission
of IP Datagrams over Ethernet Networks."  The number is actually derived 
from the maximum packet size for 10Base5 (full-spec Ethernet) networks -- 
1518 bytes.  If you subtract the DIX encapsulation overhead for Ethernet 
packets (18 bytes) you will end up with a maximum possible data size (MTU) 
of 1500 bytes.  One can also find that the ``MTU`` for IEEE 802.3 networks
is 1492 bytes.  This is because LLC/SNAP encapsulation adds an extra eight 
bytes of overhead to the packet.  In both cases, the underlying hardware can
only send 1518 bytes, but the data size is different.

In order to set the encapsulation mode, the ``CsmaNetDevice`` provides
an ``Attribute`` called ``EncapsulationMode`` which can take on the 
values ``Dix`` or ``Llc``.  These correspond to Ethernet and LLC/SNAP
framing respectively.

If one leaves the ``Mtu`` at 1500 bytes and changes the encapsulation mode
to ``Llc``, the result will be a network that encapsulates 1500 byte PDUs
with LLC/SNAP framing resulting in packets of 1526 bytes, which would be 
illegal in many networks, since they can transmit a maximum of 1518 bytes per
packet.  This would most likely result in a simulation that quite subtly does
not reflect the reality you might be expecting.

Just to complicate the picture, there exist jumbo frames (1500 < MTU <= 9000 bytes)
and super-jumbo (MTU > 9000 bytes) frames that are not officially sanctioned
by IEEE but are available in some high-speed (Gigabit) networks and NICs.  One
could leave the encapsulation mode set to ``Dix``, and set the ``Mtu``
``Attribute`` on a ``CsmaNetDevice`` to 64000 bytes -- even though an 
associated ``CsmaChannel DataRate`` was set at 10 megabits per second.  
This would essentially model an Ethernet switch made out of vampire-tapped
1980s-style 10Base5 networks that support super-jumbo datagrams.  This is
certainly not something that was ever made, nor is likely to ever be made,
but it is quite easy for you to configure.

In the previous example, you used the command line to create a simulation that
had 100 ``Csma`` nodes.  You could have just as easily created a simulation
with 500 nodes.  If you were actually modeling that 10Base5 vampire-tap network,
the maximum length of a full-spec Ethernet cable is 500 meters, with a minimum 
tap spacing of 2.5 meters.  That means there could only be 200 taps on a 
real network.  You could have quite easily built an illegal network in that
way as well.  This may or may not result in a meaningful simulation depending
on what you are trying to model.

Similar situations can occur in many places in |ns3| and in any
simulator.  For example, you may be able to position nodes in such a way that
they occupy the same space at the same time, or you may be able to configure
amplifiers or noise levels that violate the basic laws of physics.

|ns3| generally favors flexibility, and many models will allow freely
setting ``Attributes`` without trying to enforce any arbitrary consistency
or particular underlying spec.

The thing to take home from this is that |ns3| is going to provide a
super-flexible base for you to experiment with.  It is up to you to understand
what you are asking the system to do and to  make sure that the simulations you
create have some meaning and some connection with a reality defined by you.

Building a Wireless Network Topology
************************************

In this section we are going to further expand our knowledge of |ns3|
network devices and channels to cover an example of a wireless network.  
|ns3| provides a set of 802.11 models that attempt to provide an 
accurate MAC-level implementation of the 802.11 specification and a 
"not-so-slow" PHY-level model of the 802.11a specification.

Just as we have seen both point-to-point and CSMA topology helper objects when
constructing point-to-point topologies, we will see equivalent ``Wifi``
topology helpers in this section.  The appearance and operation of these 
helpers should look quite familiar to you.

We provide an example script in our ``examples/tutorial`` directory.  This script
builds on the ``second.cc`` script and adds a Wifi network.  Go ahead and
open ``examples/tutorial/third.cc`` in your favorite editor.  You will have already
seen enough |ns3| code to understand most of what is going on in 
this example, but there are a few new things, so we will go over the entire 
script and examine some of the output.

Just as in the ``second.cc`` example (and in all |ns3| examples)
the file begins with an emacs mode line and some GPL boilerplate.

Take a look at the ASCII art (reproduced below) that shows the default network
topology constructed in the example.  You can see that we are going to 
further extend our example by hanging a wireless network off of the left side.
Notice that this is a default network topology since you can actually vary the
number of nodes created on the wired and wireless networks.  Just as in the 
``second.cc`` script case, if you change ``nCsma``, it will give you a 
number of "extra" CSMA nodes.  Similarly, you can set ``nWifi`` to 
control how many ``STA`` (station) nodes are created in the simulation.
There will always be one ``AP`` (access point) node on the wireless 
network.  By default there are three "extra" CSMA nodes and three wireless 
``STA`` nodes.

The code begins by loading module include files just as was done in the
``second.cc`` example.  There are a couple of new includes corresponding
to the Wifi module and the mobility module which we will discuss below.

::

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"

The network topology illustration follows:

::

  // Default Network Topology
  //
  //   Wifi 10.1.3.0
  //                 AP
  //  *    *    *    *
  //  |    |    |    |    10.1.1.0
  // n5   n6   n7   n0 -------------- n1   n2   n3   n4
  //                   point-to-point  |    |    |    |
  //                                   ================
  //                                     LAN 10.1.2.0

You can see that we are adding a new network device to the node on the left 
side of the point-to-point link that becomes the access point for the wireless
network.  A number of wireless STA nodes are created to fill out the new 
10.1.3.0 network as shown on the left side of the illustration.

After the illustration, the ``ns-3`` namespace is ``used`` and a logging
component is defined.  This should all be quite familiar by now.

::

  using namespace ns3;
  
  NS_LOG_COMPONENT_DEFINE ("ThirdScriptExample");

The main program begins just like ``second.cc`` by adding some command line
parameters for enabling or disabling logging components and for changing the 
number of devices created.

::

  bool verbose = true;
  uint32_t nCsma = 3;
  uint32_t nWifi = 3;

  CommandLine cmd;
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

Just as in all of the previous examples, the next step is to create two nodes
that we will connect via the point-to-point link.  

::

  NodeContainer p2pNodes;
  p2pNodes.Create (2);

Next, we see an old friend.  We instantiate a ``PointToPointHelper`` and 
set the associated default ``Attributes`` so that we create a five megabit 
per second transmitter on devices created using the helper and a two millisecond 
delay on channels created by the helper.  We then ``Intall`` the devices
on the nodes and the channel between them.

::

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);

Next, we declare another ``NodeContainer`` to hold the nodes that will be
part of the bus (CSMA) network.

::

  NodeContainer csmaNodes;
  csmaNodes.Add (p2pNodes.Get (1));
  csmaNodes.Create (nCsma);

The next line of code ``Gets`` the first node (as in having an index of one)
from the point-to-point node container and adds it to the container of nodes
that will get CSMA devices.  The node in question is going to end up with a 
point-to-point device and a CSMA device.  We then create a number of "extra"
nodes that compose the remainder of the CSMA network.

We then instantiate a ``CsmaHelper`` and set its ``Attributes`` as we did
in the previous example.  We create a ``NetDeviceContainer`` to keep track of
the created CSMA net devices and then we ``Install`` CSMA devices on the 
selected nodes.

::

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

Next, we are going to create the nodes that will be part of the Wifi network.
We are going to create a number of "station" nodes as specified by the 
command line argument, and we are going to use the "leftmost" node of the 
point-to-point link as the node for the access point.

::

  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);
  NodeContainer wifiApNode = p2pNodes.Get (0);

The next bit of code constructs the wifi devices and the interconnection
channel between these wifi nodes. First, we configure the PHY and channel
helpers:

::

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();

For simplicity, this code uses the default PHY layer configuration and
channel models which are documented in the API doxygen documentation for
the ``YansWifiChannelHelper::Default`` and ``YansWifiPhyHelper::Default``
methods. Once these objects are created, we create a channel object
and associate it to our PHY layer object manager to make sure
that all the PHY layer objects created by the ``YansWifiPhyHelper``
share the same underlying channel, that is, they share the same
wireless medium and can communication and interfere:

::

  phy.SetChannel (channel.Create ());

Once the PHY helper is configured, we can focus on the MAC layer. Here we choose to
work with non-Qos MACs so we use a NqosWifiMacHelper object to set MAC parameters. 

::

  WifiHelper wifi = WifiHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();

The ``SetRemoteStationManager`` method tells the helper the type of 
rate control algorithm to use.  Here, it is asking the helper to use the AARF
algorithm --- details are, of course, available in Doxygen.

Next, we configure the type of MAC, the SSID of the infrastructure network we
want to setup and make sure that our stations don't perform active probing:

::

  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
    "Ssid", SsidValue (ssid),
    "ActiveProbing", BooleanValue (false));

This code first creates an 802.11 service set identifier (SSID) object
that will be used to set the value of the "Ssid" ``Attribute`` of
the MAC layer implementation.  The particular kind of MAC layer that
will be created by the helper is specified by ``Attribute`` as
being of the "ns3::StaWifiMac" type.  The use of
``NqosWifiMacHelper`` will ensure that the "QosSupported"
``Attribute`` for created MAC objects is set false. The combination
of these two configurations means that the MAC instance next created
will be a non-QoS non-AP station (STA) in an infrastructure BSS (i.e.,
a BSS with an AP).  Finally, the "ActiveProbing" ``Attribute`` is
set to false.  This means that probe requests will not be sent by MACs
created by this helper.

Once all the station-specific parameters are fully configured, both at the
MAC and PHY layers, we can invoke our now-familiar ``Install`` method to 
create the wifi devices of these stations:

::

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

We have configured Wifi for all of our STA nodes, and now we need to 
configure the AP (access point) node.  We begin this process by changing
the default ``Attributes`` of the ``NqosWifiMacHelper`` to reflect the 
requirements of the AP.

::

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

In this case, the ``NqosWifiMacHelper`` is going to create MAC
layers of the "ns3::ApWifiMac", the latter specifying that a MAC
instance configured as an AP should be created, with the helper type
implying that the "QosSupported" ``Attribute`` should be set to
false - disabling 802.11e/WMM-style QoS support at created APs.  

The next lines create the single AP which shares the same set of PHY-level
``Attributes`` (and channel) as the stations:

::

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);

Now, we are going to add mobility models.  We want the STA nodes to be mobile,
wandering around inside a bounding box, and we want to make the AP node 
stationary.  We use the ``MobilityHelper`` to make this easy for us.
First, we instantiate a ``MobilityHelper`` object and set some 
``Attributes`` controlling the "position allocator" functionality.

::

  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
    "MinX", DoubleValue (0.0),
    "MinY", DoubleValue (0.0),
    "DeltaX", DoubleValue (5.0),
    "DeltaY", DoubleValue (10.0),
    "GridWidth", UintegerValue (3),
    "LayoutType", StringValue ("RowFirst"));

This code tells the mobility helper to use a two-dimensional grid to initially
place the STA nodes.  Feel free to explore the Doxygen for class 
``ns3::GridPositionAllocator`` to see exactly what is being done.

We have arranged our nodes on an initial grid, but now we need to tell them
how to move.  We choose the ``RandomWalk2dMobilityModel`` which has the 
nodes move in a random direction at a random speed around inside a bounding 
box.

::

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
    "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));

We now tell the ``MobilityHelper`` to install the mobility models on the 
STA nodes.

::

  mobility.Install (wifiStaNodes);

We want the access point to remain in a fixed position during the simulation.
We accomplish this by setting the mobility model for this node to be the 
``ns3::ConstantPositionMobilityModel``:

::

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiApNode);

We now have our nodes, devices and channels created, and mobility models 
chosen for the Wifi nodes, but we have no protocol stacks present.  Just as 
we have done previously many times, we will use the ``InternetStackHelper``
to install these stacks.

::

  InternetStackHelper stack;
  stack.Install (csmaNodes);
  stack.Install (wifiApNode);
  stack.Install (wifiStaNodes);

Just as in the ``second.cc`` example script, we are going to use the 
``Ipv4AddressHelper`` to assign IP addresses to our device interfaces.
First we use the network 10.1.1.0 to create the two addresses needed for our
two point-to-point devices.  Then we use network 10.1.2.0 to assign addresses
to the CSMA network and then we assign addresses from network 10.1.3.0 to
both the STA devices and the AP on the wireless network.

::

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  address.Assign (staDevices);
  address.Assign (apDevices);

We put the echo server on the "rightmost" node in the illustration at the
start of the file.  We have done this before.

::

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

And we put the echo client on the last STA node we created, pointing it to
the server on the CSMA network.  We have also seen similar operations before.

::

  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps =
    echoClient.Install (wifiStaNodes.Get (nWifi - 1));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

Since we have built an internetwork here, we need to enable internetwork routing
just as we did in the ``second.cc`` example script.

::

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

One thing that can surprise some users is the fact that the simulation we just
created will never "naturally" stop.  This is because we asked the wireless
access point to generate beacons.  It will generate beacons forever, and this
will result in simulator events being scheduled into the future indefinitely,
so we must tell the simulator to stop even though it may have beacon generation
events scheduled.  The following line of code tells the simulator to stop so that 
we don't simulate beacons forever and enter what is essentially an endless
loop.

::

  Simulator::Stop (Seconds (10.0));

We create just enough tracing to cover all three networks:

::

  pointToPoint.EnablePcapAll ("third");
  phy.EnablePcap ("third", apDevices.Get (0));
  csma.EnablePcap ("third", csmaDevices.Get (0), true);

These three lines of code will start pcap tracing on both of the point-to-point
nodes that serves as our backbone, will start a promiscuous (monitor) mode 
trace on the Wifi network, and will start a promiscuous trace on the CSMA 
network.  This will let us see all of the traffic with a minimum number of 
trace files.

Finally, we actually run the simulation, clean up and then exit the program.

::

    Simulator::Run ();
    Simulator::Destroy ();
    return 0;
  }

In order to run this example, you have to copy the ``third.cc`` example
script into the scratch directory and use Waf to build just as you did with
the ``second.cc`` example.  If you are in the top-level directory of the
repository you would type,

.. sourcecode:: bash

  $ cp examples/third.cc scratch/mythird.cc
  $ ./waf
  $ ./waf --run scratch/mythird

Again, since we have set up the UDP echo applications just as we did in the 
``second.cc`` script, you will see similar output.

.. sourcecode:: text

  Waf: Entering directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
  'build' finished successfully (0.407s)
  Sent 1024 bytes to 10.1.2.4
  Received 1024 bytes from 10.1.3.3
  Received 1024 bytes from 10.1.2.4

Recall that the first message, ``Sent 1024 bytes to 10.1.2.4``," is the 
UDP echo client sending a packet to the server.  In this case, the client
is on the wireless network (10.1.3.0).  The second message, 
"``Received 1024 bytes from 10.1.3.3``," is from the UDP echo server, 
generated when it receives the echo packet.  The final message, 
"``Received 1024 bytes from 10.1.2.4``," is from the echo client, indicating
that it has received its echo back from the server.

If you now go and look in the top level directory, you will find four trace 
files from this simulation, two from node zero and two from node one:

.. sourcecode:: text

  third-0-0.pcap  third-0-1.pcap  third-1-0.pcap  third-1-1.pcap

The file "third-0-0.pcap" corresponds to the point-to-point device on node
zero -- the left side of the "backbone".  The file "third-1-0.pcap" 
corresponds to the point-to-point device on node one -- the right side of the
"backbone".  The file "third-0-1.pcap" will be the promiscuous (monitor
mode) trace from the Wifi network and the file "third-1-1.pcap" will be the
promiscuous trace from the CSMA network.  Can you verify this by inspecting
the code?

Since the echo client is on the Wifi network, let's start there.  Let's take
a look at the promiscuous (monitor mode) trace we captured on that network.

.. sourcecode:: bash

  $ tcpdump -nn -tt -r third-0-1.pcap

You should see some wifi-looking contents you haven't seen here before:

.. sourcecode:: text

  reading from file third-0-1.pcap, link-type IEEE802_11 (802.11)
  0.000025 Beacon (ns-3-ssid) [6.0* 9.0 12.0 18.0 24.0 36.0 48.0 54.0 Mbit] IBSS
  0.000263 Assoc Request (ns-3-ssid) [6.0 9.0 12.0 18.0 24.0 36.0 48.0 54.0 Mbit]
  0.000279 Acknowledgment RA:00:00:00:00:00:09 
  0.000552 Assoc Request (ns-3-ssid) [6.0 9.0 12.0 18.0 24.0 36.0 48.0 54.0 Mbit]
  0.000568 Acknowledgment RA:00:00:00:00:00:07 
  0.000664 Assoc Response AID(0) :: Succesful
  0.001001 Assoc Response AID(0) :: Succesful
  0.001145 Acknowledgment RA:00:00:00:00:00:0a 
  0.001233 Assoc Response AID(0) :: Succesful
  0.001377 Acknowledgment RA:00:00:00:00:00:0a 
  0.001597 Assoc Request (ns-3-ssid) [6.0 9.0 12.0 18.0 24.0 36.0 48.0 54.0 Mbit]
  0.001613 Acknowledgment RA:00:00:00:00:00:08 
  0.001691 Assoc Response AID(0) :: Succesful
  0.001835 Acknowledgment RA:00:00:00:00:00:0a 
  0.102400 Beacon (ns-3-ssid) [6.0* 9.0 12.0 18.0 24.0 36.0 48.0 54.0 Mbit] IBSS
  0.204800 Beacon (ns-3-ssid) [6.0* 9.0 12.0 18.0 24.0 36.0 48.0 54.0 Mbit] IBSS
  0.307200 Beacon (ns-3-ssid) [6.0* 9.0 12.0 18.0 24.0 36.0 48.0 54.0 Mbit] IBSS

You can see that the link type is now 802.11 as you would expect.  You can 
probably understand what is going on and find the IP echo request and response
packets in this trace.  We leave it as an exercise to completely parse the 
trace dump.

Now, look at the pcap file of the right side of the point-to-point link,

.. sourcecode:: bash

  $ tcpdump -nn -tt -r third-0-0.pcap

Again, you should see some familiar looking contents:

.. sourcecode:: text

  reading from file third-0-0.pcap, link-type PPP (PPP)
  2.002160 IP 10.1.3.3.49153 > 10.1.2.4.9: UDP, length 1024
  2.009767 IP 10.1.2.4.9 > 10.1.3.3.49153: UDP, length 1024

This is the echo packet going from left to right (from Wifi to CSMA) and back
again across the point-to-point link.

Now, look at the pcap file of the right side of the point-to-point link,

.. sourcecode:: bash

  $ tcpdump -nn -tt -r third-1-0.pcap

Again, you should see some familiar looking contents:

.. sourcecode:: text

  reading from file third-1-0.pcap, link-type PPP (PPP)
  2.005846 IP 10.1.3.3.49153 > 10.1.2.4.9: UDP, length 1024
  2.006081 IP 10.1.2.4.9 > 10.1.3.3.49153: UDP, length 1024

This is also the echo packet going from left to right (from Wifi to CSMA) and 
back again across the point-to-point link with slightly different timings
as you might expect.

The echo server is on the CSMA network, let's look at the promiscuous trace 
there:

.. sourcecode:: bash

  $ tcpdump -nn -tt -r third-1-1.pcap

You should see some familiar looking contents:

.. sourcecode:: text

  reading from file third-1-1.pcap, link-type EN10MB (Ethernet)
  2.005846 ARP, Request who-has 10.1.2.4 (ff:ff:ff:ff:ff:ff) tell 10.1.2.1, length 50
  2.005870 ARP, Reply 10.1.2.4 is-at 00:00:00:00:00:06, length 50
  2.005870 IP 10.1.3.3.49153 > 10.1.2.4.9: UDP, length 1024
  2.005975 ARP, Request who-has 10.1.2.1 (ff:ff:ff:ff:ff:ff) tell 10.1.2.4, length 50
  2.005975 ARP, Reply 10.1.2.1 is-at 00:00:00:00:00:03, length 50
  2.006081 IP 10.1.2.4.9 > 10.1.3.3.49153: UDP, length 1024

This should be easily understood.  If you've forgotten, go back and look at
the discussion in ``second.cc``.  This is the same sequence.

Now, we spent a lot of time setting up mobility models for the wireless network
and so it would be a shame to finish up without even showing that the STA
nodes are actually moving around during the simulation.  Let's do this by hooking
into the ``MobilityModel`` course change trace source.  This is just a sneak
peek into the detailed tracing section which is coming up, but this seems a very
nice place to get an example in.

As mentioned in the "Tweaking ns-3" section, the |ns3| tracing system 
is divided into trace sources and trace sinks, and we provide functions to 
connect the two.  We will use the mobility model predefined course change 
trace source to originate the trace events.  We will need to write a trace 
sink to connect to that source that will display some pretty information for 
us.  Despite its reputation as being difficult, it's really quite simple.
Just before the main program of the ``scratch/mythird.cc`` script (i.e.,
just after the ``NS_LOG_COMPONENT_DEFINE`` statement), add the 
following function:

::

  void
  CourseChange (std::string context, Ptr<const MobilityModel> model)
  {
    Vector position = model->GetPosition ();
    NS_LOG_UNCOND (context << 
      " x = " << position.x << ", y = " << position.y);
  }

This code just pulls the position information from the mobility model and 
unconditionally logs the x and y position of the node.  We are
going to arrange for this function to be called every time the wireless
node with the echo client changes its position.  We do this using the 
``Config::Connect`` function.  Add the following lines of code to the
script just before the ``Simulator::Run`` call.

::

  std::ostringstream oss;
  oss <<
    "/NodeList/" << wifiStaNodes.Get (nWifi - 1)->GetId () <<
    "/$ns3::MobilityModel/CourseChange";

  Config::Connect (oss.str (), MakeCallback (&CourseChange));

What we do here is to create a string containing the tracing namespace path
of the event to which we want to connect.  First, we have to figure out which 
node it is we want using the ``GetId`` method as described earlier.  In the
case of the default number of CSMA and wireless nodes, this turns out to be 
node seven and the tracing namespace path to the mobility model would look
like,

::

  /NodeList/7/$ns3::MobilityModel/CourseChange

Based on the discussion in the tracing section, you may infer that this trace 
path references the seventh node in the global NodeList.  It specifies
what is called an aggregated object of type ``ns3::MobilityModel``.  The 
dollar sign prefix implies that the MobilityModel is aggregated to node seven.
The last component of the path means that we are hooking into the 
"CourseChange" event of that model.  

We make a connection between the trace source in node seven with our trace 
sink by calling ``Config::Connect`` and passing this namespace path.  Once 
this is done, every course change event on node seven will be hooked into our 
trace sink, which will in turn print out the new position.

If you now run the simulation, you will see the course changes displayed as 
they happen.

.. sourcecode:: text

  Build finished successfully (00:00:01)
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 10, y = 0
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 9.41539, y = -0.811313
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 8.46199, y = -1.11303
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.52738, y = -1.46869
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 6.67099, y = -1.98503
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 5.6835, y = -2.14268
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 4.70932, y = -1.91689
  Sent 1024 bytes to 10.1.2.4
  Received 1024 bytes from 10.1.3.3
  Received 1024 bytes from 10.1.2.4
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 5.53175, y = -2.48576
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 4.58021, y = -2.17821
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 4.18915, y = -1.25785
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 4.7572, y = -0.434856
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 4.62404, y = 0.556238
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 4.74127, y = 1.54934
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 5.73934, y = 1.48729
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 6.18521, y = 0.59219
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 6.58121, y = 1.51044
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.27897, y = 2.22677
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 6.42888, y = 1.70014
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.40519, y = 1.91654
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 6.51981, y = 1.45166
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.34588, y = 2.01523
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.81046, y = 2.90077
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 6.89186, y = 3.29596
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.46617, y = 2.47732
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.05492, y = 1.56579
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 8.00393, y = 1.25054
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.00968, y = 1.35768
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.33503, y = 2.30328
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.18682, y = 3.29223
  /NodeList/7/$ns3::MobilityModel/CourseChange x = 7.96865, y = 2.66873
