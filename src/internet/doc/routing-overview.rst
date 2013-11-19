.. include:: replace.txt
.. highlight:: cpp

Routing overview
----------------

|ns3| is intended to support traditional routing approaches and protocols,
support ports of open source routing implementations, and facilitate research
into unorthodox routing techniques. The overall routing architecture is
described below in :ref:`Routing-architecture`. Users who wish to just read
about how to configure global routing for wired topologies can read 
:ref:`Global-centralized-routing`. Unicast routing protocols are described in 
:ref:`Unicast-routing`.  Multicast routing is documented in 
:ref:`Multicast-routing`.

.. _Routing-architecture:

Routing architecture
********************

.. _fig-routing:

.. figure:: figures/routing.*

    Overview of routing


:ref:`fig-routing` shows the overall routing architecture for Ipv4. The key
objects are Ipv4L3Protocol, Ipv4RoutingProtocol(s) (a class to which all
routing/forwarding has been delegated from Ipv4L3Protocol), and Ipv4Route(s).

Ipv4L3Protocol must have at least one Ipv4RoutingProtocol added to it at
simulation setup time. This is done explicitly by calling
Ipv4::SetRoutingProtocol ().

The abstract base class Ipv4RoutingProtocol () declares a minimal interface,
consisting of two methods:  RouteOutput () and RouteInput ().  For packets
traveling outbound from a host, the transport protocol will query Ipv4 for the
Ipv4RoutingProtocol object interface, and will request a route via
Ipv4RoutingProtocol::RouteOutput ().  A Ptr to Ipv4Route object is returned.
This is analagous to a dst_cache entry in Linux. The Ipv4Route is carried down
to the Ipv4L3Protocol to avoid a second lookup there. However, some cases (e.g.
Ipv4 raw sockets) will require a call to RouteOutput()
directly from Ipv4L3Protocol.

For packets received inbound for forwarding or delivery, 
the following steps occur. Ipv4L3Protocol::Receive() calls
Ipv4RoutingProtocol::RouteInput(). This passes the packet ownership to the
Ipv4RoutingProtocol object. There are four callbacks associated with this call:

* LocalDeliver 
* UnicastForward
* MulticastForward
* Error

The Ipv4RoutingProtocol must eventually call one of these callbacks for each
packet that it takes responsibility for. This is basically how the input routing
process works in Linux.

.. _routing-specialization:

.. figure:: figures/routing-specialization.*

   Ipv4Routing specialization. 

This overall architecture is designed to support different routing approaches,
including (in the future) a Linux-like policy-based routing implementation,
proactive and on-demand routing protocols, and simple routing protocols for when
the simulation user does not really care about routing.

:ref:`routing-specialization` illustrates how multiple routing protocols derive
from this base class. A class Ipv4ListRouting (implementation class
Ipv4ListRoutingImpl) provides the existing list routing approach in |ns3|. Its
API is the same as base class Ipv4Routing except for the ability to add multiple
prioritized routing protocols (Ipv4ListRouting::AddRoutingProtocol(),
Ipv4ListRouting::GetRoutingProtocol()).

The details of these routing protocols are described below in
:ref:`Unicast-routing`.  For now, we will first start with a basic
unicast routing capability that is intended to globally build routing
tables at simulation time t=0 for simulation users who do not care
about dynamic routing.

.. _Global-centralized-routing:

Global centralized routing
**************************

Global centralized routing is sometimes called "God" routing; it is a special
implementation that walks the simulation topology and runs a shortest path
algorithm, and populates each node's routing tables. No actual protocol overhead
(on the simulated links) is incurred with this approach. It does have a few
constraints:

* **Wired only:**  It is not intended for use in wireless networks.
* **Unicast only:** It does not do multicast.
* **Scalability:**  Some users of this on large topologies (e.g. 1000 nodes)
  have noticed that the current implementation is not very scalable. The global
  centralized routing will be modified in the future to reduce computations and
  runtime performance.

Presently, global centralized IPv4 unicast routing over both point-to-point and
shared (CSMA) links is supported.

By default, when using the |ns3| helper API and the default InternetStackHelper,
global routing capability will be added to the node, and global routing will be
inserted as a routing protocol with lower priority than the static routes (i.e.,
users can insert routes via Ipv4StaticRouting API and they will take precedence
over routes found by global routing).

Global Unicast Routing API
++++++++++++++++++++++++++

The public API is very minimal. User scripts include the following::

    #include "ns3/internet-module.h"

If the default InternetStackHelper is used, then an instance of global routing
will be aggregated to each node.  After IP addresses are configured, the
following function call will cause all of the nodes that have an Ipv4 interface
to receive forwarding tables entered automatically by the GlobalRouteManager::

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

*Note:* A reminder that the wifi NetDevice will work but does not take any
wireless effects into account. For wireless, we recommend OLSR dynamic routing
described below.

It is possible to call this function again in the midst of a simulation using
the following additional public function::

  Ipv4GlobalRoutingHelper::RecomputeRoutingTables ();

which flushes the old tables, queries the nodes for new interface information,
and rebuilds the routes.

For instance, this scheduling call will cause the tables to be rebuilt
at time 5 seconds::

  Simulator::Schedule (Seconds (5),
                       &Ipv4GlobalRoutingHelper::RecomputeRoutingTables);


There are two attributes that govern the behavior. The first is
Ipv4GlobalRouting::RandomEcmpRouting. If set to true, packets are randomly
routed across equal-cost multipath routes. If set to false (default), only one
route is consistently used. The second is
Ipv4GlobalRouting::RespondToInterfaceEvents. If set to true, dynamically
recompute the global routes upon Interface notification events (up/down, or
add/remove address). If set to false (default), routing may break unless the
user manually calls RecomputeRoutingTables() after such events. The default is
set to false to preserve legacy |ns3| program behavior.

Global Routing Implementation
+++++++++++++++++++++++++++++

This section is for those readers who care about how this is implemented.  A
singleton object (GlobalRouteManager) is responsible for populating the static
routes on each node, using the public Ipv4 API of that node.  It queries each
node in the topology for a "globalRouter" interface.  If found, it uses the API
of that interface to obtain a "link state advertisement (LSA)" for the router.
Link State Advertisements are used in OSPF routing, and we follow their
formatting.

It is important to note that all of these computations are done before 
packets are flowing in the network.  In particular, there are no
overhead or control packets being exchanged when using this implementation.
Instead, this global route manager just walks the list of nodes to
build the necessary information and configure each node's routing table.

The GlobalRouteManager populates a link state database with LSAs gathered from
the entire topology. Then, for each router in the topology, the
GlobalRouteManager executes the OSPF shortest path first (SPF) computation on
the database, and populates the routing tables on each node.

The quagga (`<http://www.quagga.net>`_) OSPF implementation was used as the
basis for the routing computation logic. One benefit of following an existing
OSPF SPF implementation is that OSPF already has defined link state
advertisements for all common types of network links:

* point-to-point (serial links)
* point-to-multipoint (Frame Relay, ad hoc wireless)
* non-broadcast multiple access (ATM)
* broadcast (Ethernet)

Therefore, we think that enabling these other link types will be more
straightforward now that the underlying OSPF SPF framework is in place.

Presently, we can handle IPv4 point-to-point, numbered links, as well as shared
broadcast (CSMA) links.  Equal-cost multipath is also supported.  Although
wireless link types are supported by the implementation, note that due
to the nature of this implementation, any channel effects will not be
considered and the routing tables will assume that every node on the
same shared channel is reachable from every other node (i.e. it will
be treated like a broadcast CSMA link).

The GlobalRouteManager first walks the list of nodes and aggregates
a GlobalRouter interface to each one as follows::

  typedef std::vector < Ptr<Node> >::iterator Iterator;
  for (Iterator i = NodeList::Begin (); i != NodeList::End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<GlobalRouter> globalRouter = CreateObject<GlobalRouter> (node);
      node->AggregateObject (globalRouter);
    }

This interface is later queried and used to generate a Link State
Advertisement for each router, and this link state database is
fed into the OSPF shortest path computation logic. The Ipv4 API
is finally used to populate the routes themselves. 

.. _Unicast-routing:

Unicast routing
***************

There are presently seven unicast routing protocols defined for IPv4 and two for
IPv6:

* class Ipv4StaticRouting (covering both unicast and multicast)
* IPv4 Optimized Link State Routing (OLSR) (a MANET protocol defined in 
  `RFC 3626 <http://www.ietf.org/rfc/rfc3626.txt>`_)
* IPv4 Ad Hoc On Demand Distance Vector (AODV) (a MANET protocol defined in
  `RFC 3561 <http://www.ietf.org/rfc/rfc3561.txt>`_)
* IPv4 Destination Sequenced Distance Vector (DSDV) (a MANET protocol)
* class Ipv4ListRouting (used to store a prioritized list of routing protocols)
* class Ipv4GlobalRouting (used to store routes computed by the global route
  manager, if that is used)
* class Ipv4NixVectorRouting (a more efficient version of global routing that
  stores source routes in a packet header field)
* class Ipv6ListRouting (used to store a prioritized list of routing protocols)
* class Ipv6StaticRouting 

In the future, this architecture should also allow someone to implement a
Linux-like implementation with routing cache, or a Click modular router, but
those are out of scope for now.

Ipv4ListRouting
+++++++++++++++

This section describes the current default |ns3| Ipv4RoutingProtocol. Typically,
multiple routing protocols are supported in user space and coordinate to write a
single forwarding table in the kernel. Presently in |ns3|, the implementation
instead allows for multiple routing protocols to build/keep their own routing
state, and the IPv4 implementation will query each one of these routing
protocols (in some order determined by the simulation author) until a route is
found.  

We chose this approach because it may better facilitate the integration of
disparate routing approaches that may be difficult to coordinate the writing to
a single table, approaches where more information than destination IP address
(e.g., source routing) is used to determine the next hop, and on-demand routing
approaches where packets must be cached.  

Ipv4ListRouting::AddRoutingProtocol
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Class Ipv4ListRouting provides a pure virtual function declaration for the
method that allows one to add a routing protocol::

  void AddRoutingProtocol (Ptr<Ipv4RoutingProtocol> routingProtocol,
                           int16_t priority);

This method is implemented by class Ipv4ListRoutingImpl in the internet-stack
module.

The priority variable above governs the priority in which the routing protocols
are inserted. Notice that it is a signed int.  By default in |ns3|, the helper
classes will instantiate a Ipv4ListRoutingImpl object, and add to it an
Ipv4StaticRoutingImpl object at priority zero.  Internally, a list of
Ipv4RoutingProtocols is stored, and and the routing protocols are each consulted
in decreasing order of priority to see whether a match is found. Therefore, if
you want your Ipv4RoutingProtocol to have priority lower than the static
routing, insert it with priority less than 0; e.g.::

  Ptr<MyRoutingProtocol> myRoutingProto = CreateObject<MyRoutingProtocol> ();
  listRoutingPtr->AddRoutingProtocol (myRoutingProto, -10);

Upon calls to RouteOutput() or RouteInput(), the list routing object will search
the list of routing protocols, in priority order, until a route is found. Such
routing protocol will invoke the appropriate callback and no further routing
protocols will be searched.  

Optimized Link State Routing (OLSR)
+++++++++++++++++++++++++++++++++++

This IPv4 routing protocol was originally ported from the OLSR-UM implementation
for ns-2. The implementation is found in the src/olsr directory, and an
example script is in examples/simple-point-to-point-olsr.cc.

Typically, OLSR is enabled in a main program by use of an OlsrHelper class that
installs OLSR into an Ipv4ListRoutingProtocol object. The following sample
commands will enable OLSR in a simulation using this helper class along with
some other routing helper objects. The setting of priority value 10, ahead of
the staticRouting priority of 0, means that OLSR will be consulted for a route
before the node's static routing table.::

  NodeContainer c:
  ...
  // Enable OLSR
  NS_LOG_INFO ("Enabling OLSR Routing.");
  OlsrHelper olsr;

  Ipv4StaticRoutingHelper staticRouting;

  Ipv4ListRoutingHelper list;
  list.Add (staticRouting, 0);
  list.Add (olsr, 10);

  InternetStackHelper internet;
  internet.SetRoutingHelper (list);
  internet.Install (c);

Once installed,the OLSR "main interface" can be set with the SetMainInterface()
command. If the user does not specify a main address, the protocol will select
the first primary IP address that it finds, starting first the loopback
interface and then the next non-loopback interface found, in order of Ipv4
interface index. The loopback address of 127.0.0.1 is not selected. In addition,
a number of protocol constants are defined in olsr-routing-protocol.cc.

Olsr is started at time zero of the simulation, based on a call to
Object::Start() that eventually calls OlsrRoutingProtocol::DoStart(). Note:  a
patch to allow the user to start and stop the protocol at other times would be
welcome.

Presently, OLSR is limited to use with an Ipv4ListRouting object, and does not
respond to dynamic changes to a device's IP address or link up/down
notifications; i.e. the topology changes are due to loss/gain of connectivity
over a wireless channel.

.. _Multicast-routing:

Multicast routing
*****************

The following function is used to add a static multicast route
to a node::

    void 
    Ipv4StaticRouting::AddMulticastRoute (Ipv4Address origin,
                                          Ipv4Address group,
                                          uint32_t inputInterface,
                                          std::vector<uint32_t> outputInterfaces);

A multicast route must specify an origin IP address, a multicast group and an
input network interface index as conditions and provide a vector of output
network interface indices over which packets matching the conditions are sent.

Typically there are two main types of multicast routes:  routes of the first
kind are used during forwarding. All of the conditions must be explicitly
provided. The second kind of routes are used to get packets off of a local node.
The difference is in the input interface. Routes for forwarding will always
have an explicit input interface specified. Routes off of a node will always
set the input interface to a wildcard specified by the index
Ipv4RoutingProtocol::IF\_INDEX\_ANY.

For routes off of a local node wildcards may be used in the origin and multicast
group addresses. The wildcard used for Ipv4Adresses is that address returned by
Ipv4Address::GetAny () -- typically "0.0.0.0". Usage of a wildcard allows one to
specify default behavior to varying degrees.

For example, making the origin address a wildcard, but leaving the multicast
group specific allows one (in the case of a node with multiple interfaces) to
create different routes using different output interfaces for each multicast
group.

If the origin and multicast addresses are made wildcards, you have created
essentially a default multicast address that can forward to multiple 
interfaces. Compare this to the actual default multicast address that is
limited to specifying a single output interface for compatibility with
existing functionality in other systems.

Another command sets the default multicast route::

    void 
    Ipv4StaticRouting::SetDefaultMulticastRoute (uint32_t outputInterface);

This is the multicast equivalent of the unicast version SetDefaultRoute. We
tell the routing system what to do in the case where a specific route to a
destination multicast group is not found. The system forwards packets out the
specified interface in the hope that "something out there" knows better how to
route the packet. This method is only used in initially sending packets off of a
host. The default multicast route is not consulted during forwarding -- exact
routes must be specified using AddMulticastRoute for that case.

Since we're basically sending packets to some entity we think may know better
what to do, we don't pay attention to "subtleties" like origin address, nor do
we worry about forwarding out multiple  interfaces. If the default multicast
route is set, it is returned as the selected route from LookupStatic
irrespective of origin or multicast group if another specific route is not
found.

Finally, a number of additional functions are provided to fetch and remove
multicast routes::

  uint32_t GetNMulticastRoutes (void) const;

  Ipv4MulticastRoute *GetMulticastRoute (uint32_t i) const;

  Ipv4MulticastRoute *GetDefaultMulticastRoute (void) const;

  bool RemoveMulticastRoute (Ipv4Address origin,
                             Ipv4Address group,
                             uint32_t inputInterface);

  void RemoveMulticastRoute (uint32_t index);
