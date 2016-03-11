.. include:: replace.txt

Optimized Link State Routing (OLSR)
-----------------------------------

This model implements the base specification of the Optimized
Link State Routing (OLSR) protocol, which is a dynamic mobile ad hoc
unicast routing protocol.  It has been developed at the
University of Murcia (Spain) by Francisco J. Ros for NS-2, and was
ported to NS-3 by Gustavo Carneiro at INESC Porto (Portugal).

The implementation is based on OLSR Version 1 (:rfc:`3626`) and
it is *not* compliant with OLSR Version 2 (:rfc:`7181`) or any 
of the Version 2 extensions.
 
Model Description
*****************

The source code for the OLSR model lives in the directory `src/olsr`.
As stated before, the model is based on :rfc:`3626`. Moreover, many
design choices are based on the previous ns2 model.

Scope and Limitations
+++++++++++++++++++++

The model is for IPv4 only.  

* Mostly compliant with OLSR as documented in :rfc:`3626`, 
* The use of multiple interfaces was not supported by the NS-2 version, but is supported in NS-3;
* OLSR does not respond to the routing event notifications corresponding to dynamic interface up and down (``ns3::RoutingProtocol::NotifyInterfaceUp`` and ``ns3::RoutingProtocol::NotifyInterfaceDown``) or address insertion/removal ``ns3::RoutingProtocol::NotifyAddAddress`` and ``ns3::RoutingProtocol::NotifyRemoveAddress``).
* Unlike the NS-2 version, does not yet support MAC layer feedback as described in :rfc:`3626`;

Host Network Association (HNA) is supported in this implementation
of OLSR. Refer to ``examples/olsr-hna.cc`` to see how the API
is used.

References
++++++++++

.. [rfc3626] :rfc:`3626` *Optimized Link State Routing*
.. [rfc7181] :rfc:`7181` *The Optimized Link State Routing Protocol Version 2*

Usage
*****

The usage pattern is the one of all the Internet routing protocols.
Since OLSR is not installed by default in the Internet stack, it is necessary to
set it in the Internet Stack helper by using ``InternetStackHelper::SetRoutingHelper``

Examples
++++++++

The examples are in the ``src/olsr/examples/`` directory. However, many other examples esists in the
general examples directory, e.g., ``examples/routing/manet-routing-compare.cc``.

For specific examples of the HNA feature, see the examples in ``src/olsr/examples/``.

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

The list of configurabel attributes is:

* HelloInterval (time, default 2s), HELLO messages emission interval.
* TcInterval (time, default 5s), TC messages emission interval.
* MidInterval (time, default 5s), MID messages emission interval.
* HnaInterval (time, default 5s), HNA messages emission interval.
* Willingness (enum, default OLSR_WILL_DEFAULT), Willingness of a node to carry and forward traffic for other nodes.

Tracing
+++++++

The available traces are:

* Rx: Receive OLSR packet.
* Tx: Send OLSR packet.
* RoutingTableChanged: The OLSR routing table has changed.

Caveats
+++++++

The code does not present any known issue.

Validation
**********

The code validationhas been done through Wireshark message compliance and unit testings.
