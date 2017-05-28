Nix-Vector Routing Documentation
--------------------------------

.. include:: replace.txt
.. highlight:: cpp

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)


Nix-vector routing is a simulation specific routing protocol and is 
intended for large network topologies.  The on-demand nature of this 
protocol as well as the low-memory footprint of the nix-vector provides 
improved performance in terms of memory usage and simulation run time 
when dealing with a large number of nodes.

Model Description
*****************

The source code for the NixVectorRouting module lives in
the directory ``src/nix-vector-routing``.

|ns3| nix-vector-routing performs on-demand route computation using 
a breadth-first search and an efficient route-storage data structure 
known as a nix-vector.

When a packet is generated at a node for transmission, the route is 
calculated, and the nix-vector is built. 
The nix-vector stores an index for each hop along the path, which 
corresponds to the neighbor-index.  This index is used to determine 
which net-device and gateway should be used.  To route a packet, the 
nix-vector must be transmitted with the packet. At each hop, the 
current node extracts the appropriate neighbor-index from the 
nix-vector and transmits the packet through the corresponding 
net-device.  This continues until the packet reaches the destination.

Scope and Limitations
=====================

Currently, the ns-3 model of nix-vector routing supports IPv4 p2p links 
as well as CSMA links.  It does not (yet) provide support for 
efficient adaptation to link failures.  It simply flushes all nix-vector 
routing caches. Finally, IPv6 is not supported.


Usage
*****

The usage pattern is the one of all the Internet routing protocols.
Since NixVectorRouting is not installed by default in the 
Internet stack, it is necessary to set it in the Internet Stack 
helper by using ``InternetStackHelper::SetRoutingHelper``


Examples
========

The examples for the NixVectorRouting module lives in
the directory ``src/nix-vector-routing/examples``.

