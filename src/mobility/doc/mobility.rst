Mobility
--------

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

The mobility support in ns-3 includes:

- a set of mobility models which are used to track and maintain the *current* cartesian position and speed of an object.
- a "course change notifier" trace source which can be used to register listeners to the course changes of a mobility model
- a number of helper classes which are used to place nodes and setup mobility models (including parsers for some mobility definition formats).

Model Description
*****************

The source code for mobility lives in the directory ``src/mobility``.

Design
======

The design includes mobility models, position allocators, and helper
functions.  

In ns-3, special ``MobilityModel`` objects track the evolution of position
with respect to a (cartesian) coordinate system.  The mobility model
is typically aggregated to an ``ns3::Node`` object and queried using
``GetObject<MobilityModel> ()``. The base class ``ns3::MobilityModel``
is subclassed for different motion behaviors.

The initial position of objects is typically set with a PositionAllocator.
These types of objects will lay out the position on a notional canvas.
Once the simulation starts, the position allocator may no longer be
used, or it may be used to pick future mobility "waypoints" for such
mobility models.

Most users interact with the mobility system using mobility helper
classes.  The MobilityHelper combines a mobility model and position
allocator, and can be used with a node container to install mobility
capability on a set of nodes.

We first describe the coordinate system and issues 
surrounding multiple coordinate systems.

Coordinate system
#################

There are many possible coordinate systems and possible translations between
them.  ns-3 uses the Cartesian coordinate system only, at present.

The question has arisen as to how to use the mobility models (supporting
Cartesian coordinates) with different coordinate systems.  This is possible
if the user performs conversion between the ns-3 Cartesian and the
other coordinate system.  One possible library to assist is
the proj4 http://trac.osgeo.org/proj/ library for projections and reverse 
projections.

If we support converting between coordinate systems, we must adopt a
reference.  It has been suggested to use the geocentric Cartesian coordinate
system as a reference.  Contributions are welcome in this regard.

The question has arisen about adding a new mobility model whose motion
is natively implemented in a different coordinate system (such as an
orbital mobility model implemented using spherical coordinate system).
We advise to create a subclass with the APIs desired
(such as Get/SetSphericalPosition), and new position allocators, and 
implement the motion however desired, but must also support the conversion to 
cartesian (by supporting the cartesian Get/SetPosition). 

Coordinates
###########

The base class for a coordinate is called ``ns3::Vector``.  While
positions are normally described as coordinates and not vectors in
the literature, it is possible to reuse the same data structure to
represent position (x,y,z) and velocity (magnitude and direction
from the current position).  ns-3 uses class Vector for both.  
  
There are also some additional related structures used to support
mobility models.

- Rectangle
- Box
- Waypoint

MobilityModel
#############

Describe base class

- GetPosition ()
- Position and Velocity attributes
- GetDistanceFrom ()
- CourseChangeNotification

MobilityModel Subclasses
########################

- ConstantPosition
- ConstantVelocity
- ConstantAcceleration
- GaussMarkov
- Hierarchical
- RandomDirection2D
- RandomWalk2D
- RandomWaypoint
- SteadyStateRandomWaypoint
- Waypoint

PositionAllocator
#################

Position allocators usually used only at beginning, to lay out the nodes
initial position.  However, some mobility models (e.g. RandomWaypoint) will
use a position allocator to pick new waypoints.

- ListPositionAllocator
- GridPositionAllocator
- RandomRectanglePositionAllocator
- RandomBoxPositionAllocator
- RandomDiscPositionAllocator
- UniformDiscPositionAllocator

Helper
######

A special mobility helper is provided that is mainly aimed at supporting
the installation of mobility to a Node container (when using containers
at the helper API level).  The MobilityHelper class encapsulates 
a MobilityModel factory object and a PositionAllocator used for
initial node layout.  

ns-2 MobilityHelper
###################

The ns-2 mobility format is a widely used mobility trace format.  The
documentation is available at: http://www.isi.edu/nsnam/ns/doc/node172.html

Valid trace files use the following ns2 statements: ::

   $node set X_ x1
   $node set Y_ y1
   $node set Z_ z1
   $ns at $time $node setdest x2 y2 speed
   $ns at $time $node set X_ x1
   $ns at $time $node set Y_ Y1
   $ns at $time $node set Z_ Z1

Some examples of external tools that can export in this format include:

- BonnMotion http://net.cs.uni-bonn.de/wg/cs/applications/bonnmotion/
- SUMO http://sourceforge.net/apps/mediawiki/sumo/index.php?title=Main_Page
- TraNS http://trans.epfl.ch/

A special Ns2MobilityHelper object can be used to parse these files
and convert the statements into ns-3 mobility events.

Scope and Limitations
=====================

- only cartesian coordinates are presently supported

References
==========

TBD

Usage
*****

Most ns-3 program authors typically interact with the mobility system
only at configuration time.  However, various ns-3 objects interact
with mobility objects repeatedly during runtime, such as a propagation
model trying to determine the path loss between two mobile nodes.

Helper
======

A typical usage pattern can be found in the ``third.cc`` program in the
tutorial.

First, the user instantiates a ``MobilityHelper`` object and sets some
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
place the nodes.  The first argument is an ns-3 TypeId specifying the
type of mobility model; the remaining attribute/value pairs configure
this position allocator.

Next, the user typically sets the MobilityModel subclass; e.g.:

::

  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
    "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));

Once the helper is configured, it is typically passed a container, such as:

::

  mobility.Install (wifiStaNodes);

A MobilityHelper object may be reconfigured and reused for different
NodeContainers during the configuration of an ns-3 scenario.

Use of Random Variables
=======================

A typical use case is to evaluate protocols on a mobile topology that
involves some randomness in the motion or initial position allocation.
To obtain random motion and positioning that is not affected by
the configuration of the rest of the scenario, it is recommended to
use the "AssignStreams" facility of the random number system.

Class ``MobilityModel`` and class ``PositionAllocator`` both have public
API to assign streams to underlying random variables:

::

  /**
   * Assign a fixed random variable stream number to the random variables
   * used by this model. Return the number of streams (possibly zero) that
   * have been assigned.
   *
   * \param stream first stream index to use
   * \return the number of stream indices assigned by this model
   */
  int64_t AssignStreams (int64_t stream);

The class ``MobilityHelper`` also provides this API.  The typical usage 
pattern when using the helper is:

::

  int64_t streamIndex = /*some positive integer */  
  MobilityHelper mobility;
  ... (configure mobility)
  mobility.Install (wifiStaNodes);
  int64_t streamsUsed = mobility.AssignStreams (wifiStaNodes, streamIndex);

If AssignStreams is called before Install, it will not have any effect.

Advanced Usage
==============

A number of external tools can be used to generate traces read by
the Ns2MobilityHelper.

ns-2 scengen
############

TBD

BonnMotion
##########

http://net.cs.uni-bonn.de/wg/cs/applications/bonnmotion/

SUMO
####

http://sourceforge.net/apps/mediawiki/sumo/index.php?title=Main_Page

TraNS
#####

http://trans.epfl.ch/

Examples
========

- main-random-topology.cc
- main-random-walk.cc
- main-grid-topology.cc
- ns2-mobility-trace.cc

Validation
**********

TBD
