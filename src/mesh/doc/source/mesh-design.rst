.. include:: replace.txt

.. heading hierarchy:
   ------------- Document Title
   ************* Chapter (#)
   ============= Section (#.#)
   ############# Subsection (#.#.#)
   ~~~~~~~~~~~~~ Paragraph (no number)

Design Documentation
********************

Overview
========

The |ns3| `mesh` module extends the |ns3| `wifi` module to provide mesh
networking capabilities according to the IEEE 802.11s standard [ieee80211s]_.

The basic purpose of IEEE 802.11s is to define a mode of operation for 
Wi-Fi that permits frames to be forwarded over multiple radio hops 
transparent to higher layer protocols such as IP.  To accomplish this,
mesh-capable stations form a `Mesh Basic Service Set` (MBSS) by running
a pair-wise peering protocol to establish forwarding associations, and
by running a routing protocol to find paths through the network.  A
special gateway device called a `mesh gate` allows a MBSS to interconnect
with a Distribution System (DS).

The basic enhancements defined by IEEE 802.11s include:

* discovery services
* peering management
* security 
* beaconing and synchronization
* the Mesh Coordination Function (MCF)
* power management
* channel switching
* extended frame formats
* path selection and forwarding
* interworking (proxy mesh gateways)
* intra-mesh congestion control, and
* emergency service support.

The |ns3| models implement only a subset of the above service extensions,
focusing mainly on those items related to peering and routing/forwarding
of data frames through the mesh.

The Mesh NetDevice based on 802.11s was added in *ns-3.6*. An overview
presentation by Kirill Andreev was published at the wns-3 workshop
in 2009 [And09]_.  An overview paper is available at [And10]_.

As of ns-3.23 release, the model has been slightly updated to the 802.11s-2012
standard with regard to packet formats.
