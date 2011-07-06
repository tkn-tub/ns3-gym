.. include:: replace.txt

Optimized Link State Routing (OLSR)
-----------------------------------

This model implements the base specification of the Optimized
Link State Routing (OLSR) protocol, which is a dynamic mobile ad hoc
unicast routing protocol.  It has been developed at the
University of Murcia (Spain) by Francisco J. Ros for NS-2, and was
ported to NS-3 by Gustavo Carneiro at INESC Porto (Portugal).

Model Description
*****************

The source code for the OLSR model lives in the directory `src/olsr`.

Design
++++++

Scope and Limitations
+++++++++++++++++++++

The model is for IPv4 only.  

* Mostly compliant with OLSR as documented in [rfc3626]_, 
* The use of multiple interfaces was not supported by the NS-2 version, but is supported in NS-3;
* OLSR does not respond to the routing event notifications corresponding to dynamic interface up and down (``ns3::RoutingProtocol::NotifyInterfaceUp`` and ``ns3::RoutingProtocol::NotifyInterfaceDown``) or address insertion/removal ``ns3::RoutingProtocol::NotifyAddAddress`` and ``ns3::RoutingProtocol::NotifyRemoveAddress``).
* Unlike the NS-2 version, does not yet support MAC layer feedback as described in RFC 3626;

Host Network Association (HNA) is supported in this implementation
of OLSR. Refer to ``examples/olsr-hna.cc`` to see how the API
is used.

References
++++++++++

.. [rfc3626] RFC 3626 *Optimized Link State Routing*

Usage
*****

Examples
++++++++

Helpers
+++++++

A helper class for OLSR has been written.  After an IPv4 topology
has been created and unique IP addresses assigned to each node, the
simulation script writer can call one of three overloaded functions
with different scope to enable OLSR: ``ns3::OlsrHelper::Install
(NodeContainer container)``; ``ns3::OlsrHelper::Install (Ptr<Node>
node)``; or ``ns3::OlsrHelper::InstallAll (void)``

Attributes
++++++++++

In addition, the behavior of OLSR can be modified by changing certain
attributes.  The method ``ns3::OlsrHelper::Set ()`` can be used
to set OLSR attributes.  These include HelloInterval, TcInterval,
MidInterval, Willingness.  Other parameters are defined as macros
in ``olsr-routing-protocol.cc``.

Tracing
+++++++

Logging
+++++++

Caveats
+++++++

Validation
**********

Unit tests
++++++++++

Larger-scale performance tests
++++++++++++++++++++++++++++++

