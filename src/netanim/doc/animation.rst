.. include:: replace.txt

Animation
---------

Animation is an important tool for network simulation. While |ns3| does not
contain a default graphical animation tool, we currently have two ways to provide
animation, namely using the PyViz method or the NetAnim method.
The PyViz method is described in http://www.nsnam.org/wiki/index.php/PyViz.

We will describe the NetAnim method briefly here.

NetAnim
*******

NetAnim is a standalone, Qt4-based software executable that uses a trace file generated during 
an |ns3| simulation to display the topology and animate the packet flow between nodes.

.. figure:: figures/Dumbbell.*
   :align: center
   :width: 500px
   :height: 400px

   An example of packet animation on wired-links

.. figure:: figures/Wireless.*
   :align: center
   :width: 480px
   :height: 400px

   An example of packet animation on wireless-links

In addition, NetAnim also provides useful features such as tables to display meta-data of packets like the image below

.. figure:: figures/PacketStatistics.*
   :align: center
   :width: 500px

   An example of tables for packet meta-data with protocol filters

and a way to visualize the trajectory of a mobile node

.. figure:: figures/Trajectory.*
   :align: center
   :width: 500px

   An example of the trajectory of a mobile node

Methodology
===========
The class ns3::AnimationInterface is responsible for the creation the trace XML file.
AnimationInterface uses the tracing infrastructure to track packet flows between nodes.
AnimationInterface registers itself as a trace hook for tx and rx events before the simulation
begins. When a packet is scheduled for transmission or reception, the corresponding tx and rx 
trace hooks in AnimationInterface are called. When the rx hooks are called, AnimationInterface will be aware of the two endpoints between which a packet has flowed, and adds this information
to the trace file, in XML format along with the corresponding tx and rx timestamps. The XML format 
will be discussed in a later section. It is important to note that AnimationInterface records a 
packet only if the rx trace hooks are called. Every tx event must be matched by an rx event.

Downloading NetAnim
===================
If NetAnim is not already available in the |ns3| package you downloaded, you can do the following:

Please ensure that you have installed mercurial.
The latest version of NetAnim can be downloaded using mercurial with the following command::

  hg clone http://code.nsnam.org/netanim

Building NetAnim
================
Prerequisites
~~~~~~~~~~~~~
Qt4 (4.7 and over) is required to build NetAnim. This can be obtained using the following ways:

For Debian/Ubuntu Linux distributions::

  apt-get install qt4-dev-tools

For Red Hat/Fedora based distribution::

  yum install qt4
  yum install qt4-devel

For Mac/OSX::

  http://qt.nokia.com/downloads/

Build steps
~~~~~~~~~~~

To build NetAnim use the following commands::

  cd netanim
  make clean
  qmake NetAnim.pro  (For MAC Users: qmake -spec macx-g++ NetAnim.pro)
  make


Note: qmake could be "qmake-qt4" in some systems

This should create an executable named "NetAnim" in the same directory::

 john@john-VirtualBox:~/netanim$ ls -l NetAnim
 -rwxr-xr-x 1 john john 390395 2012-05-22 08:32 NetAnim

Usage
=====
Using NetAnim is a two-step process

Step 1:Generate the animation XML trace file during simulation using "ns3::AnimationInterface" in the |ns3| code base.

Step 2:Load the XML trace file generated in Step 1 with the offline Qt4-based animator named NetAnim.

Step 1: Generate XML animation trace file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The class "AnimationInterface" under "src/netanim" uses underlying |ns3| trace sources 
to construct a timestamped ASCII file in XML format.

Examples are found under src/netanim/examples
Example::

  ./waf -d debug configure --enable-examples
  ./waf --run "dumbbell-animation"

The above will create an XML file dumbbell-animation.xml

Mandatory
#########

1. Ensure that your program's wscript includes the "netanim" module. An example of such a wscript is at src/netanim/examples/wscript. 
2. Include the header [#include "ns3/netanim-module.h"] in your test program
3. Add the statement

::

  AnimationInterface anim ("animation.xml"); 
  where "animation.xml" is any arbitrary filename

[for versions before ns-3.13 you also have to use the line "anim.SetXMLOutput() to set the XML mode and also use anim.StartAnimation();]


Optional
########
The following are optional but useful steps::

  1.anim.SetMobilityPollInterval (Seconds (1));

AnimationInterface records the position of all nodes every 250 ms by default. The statement above sets 
the periodic interval at which AnimationInterface records the position of all nodes. If the nodes are 
expected to move very little, it is useful to set a high mobility poll interval to avoid large XML files.

::

  2. anim.SetConstantPosition (Ptr< Node > n, double x, double y);

AnimationInterface requires that the position of all nodes be set. In |ns3| this is done by setting an associated MobilityModel. "SetConstantPosition" is a quick way to set the x-y coordinates of a node which is stationary.

::

  3. anim.SetStartTime (Seconds(150)); and anim.SetStopTime (Seconds(150));

AnimationInterface can generate large XML files. The above statements restricts the window between which AnimationInterface does tracing. Restricting the window serves to focus only on relevant portions of the simulation and creating manageably small XML files

::

  4. AnimationInterface anim ("animation.xml", 50000);

Using the above constructor ensures that each animation XML trace file has only 50000 packets. For example, if AnimationInterface captures 150000 packets, using the above constructor splits the capture into 3 files

animation.xml - containing the packet range 1-50000

animation.xml-1 - containing the packet range 50001-100000 

animation.xml-2 - containing the packet range 100001-150000 

::

  5. anim.EnablePacketMetadata (true);

With the above statement, AnimationInterface records the meta-data of each packet in the xml trace file. Metadata can be used by NetAnim to provide better statistics and filter, along with providing some brief information about the packet such as TCP sequence number or source & destination IP address during packet animation.

CAUTION: Enabling this feature will result in larger XML trace files.
Please do NOT enable this feature when using Wimax links.

Step 2: Loading the XML in NetAnim
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. Assuming NetAnim was built, use the command "./NetAnim" to launch NetAnim. Please review the section "Building NetAnim" if NetAnim is not available.
2. When NetAnim is opened, click on the File open button at the top-left corner, select the XML file generated during Step 1.
3. Hit the green play button to begin animation.

Here is a video illustrating this
http://www.youtube.com/watch?v=tz_hUuNwFDs

Essential settings of NetAnim
=============================

Persist combobox
~~~~~~~~~~~~~~~~
.. figure:: figures/Persist.*
   :width: 150px

   The persist combobox

When packets are transmitted and received very quickly, they can be almost invisible. The persist time setting
allows the user to control the duration for which a packet should be visible on the animation canvas. 

Fast-forward button
~~~~~~~~~~~~~~~~~~~
.. figure:: figures/FastForward.*
   :width: 50px

   The Fast-forward button

This setting is ON by default. With this setting ON, the animation progresses in simulation time rather than
wall-clock time. This means, if there were three intervals of time, A to B , B to C and C to D, and if all packets are transmitted and received only in the intervals A to B and C to D , while B to C is a 
20 second idle period with no packet tranmission or reception or node mobility, NetAnim will skip over B to C, instantly without waiting for 20 seconds. The user can turn OFF Fast-forward when they want the animation to proceed like wall-clock time. 

Update-interval slider
~~~~~~~~~~~~~~~~~~~~~~
.. figure:: figures/UpdateRateInterval.*
   :width: 150px

   The update-interval slider

If Fast-forward (discussed above) is turned OFF, the update-interval slider controls the rate at which 
NetAnim refreshes the canvas screen. For instance, for the setting above, NetAnim, updates the position
of nodes and packets only once in 250 ms.

Precision button
~~~~~~~~~~~~~~~~
.. figure:: figures/Precision.*
   :width: 50px

   The precision button

This setting is turned OFF by default. When using purely point-to-point topologies precision can be turned ON, 
to distinguish between small and large packets travelling on a link. For instance a small packet such as a 
TCP ACK segment occupies only a small fraction of the length of the link , which provides a realistic animation.

CAUTION: Precision should be turned ON only for completely point-to-point topologies.

The below two images show the difference between packet link animation for the case without precision and for the
case with precision. The one with precision ON, shows that the SYN segment does not occupy the full link, because 
they are small packets. This is provides a better visualization.

.. figure:: figures/WithoutPrecision.*
   :width: 150px

   Without precision

.. figure:: figures/WithPrecision.*
   :width: 150px

   With precision

Sim-time spinbox
~~~~~~~~~~~~~~~~
.. figure:: figures/SimTime.*
   :width: 150px

   The Sim-time spinbox

The Sim-time spinbox can be used to go forward or backward in simulation time.


Parts of the XML
================
The XML trace files has the following main sections

1. Topology

   - Nodes
   - Links
2. packets (packets over wired-links)
3. wpackets (packets over wireless-links)

XML tags
~~~~~~~~
Nodes are identified by their unique Node id. The XML begins with the "information" element describing the rest of the elements

1. <anim> element

This is the XML root element. All other elements fall within this element.
  Attributes are::

    lp = Logical Processor Id (Used for distributed simulations only)

2. <topology> element

This elements contains the Node and Link elements.It describes, the co-ordinates of the canvas used for animation.
   Attributes are::

     minX = minimum X coordinate of the animation canvas
     minY = minimum Y coordinate of the animation canvas
     maxX = maximum X coordinate of the animation canvas
     maxY = maximum Y coordinate of the animation canvas

Example::

 <topology minX = "-6.42025" minY = "-6.48444" maxX = "186.187" maxY = "188.049">

3. <node> element

This element describes each Node's Id and X,Y co-ordinate (position).
  Attributes are::

   id = Node Id
   locX = X coordinate
   locY = Y coordinate

Example::

  <node id = "8" locX = "107.599" locY = "96.9366" />

4. <link> element

This element describes wired links between two nodes.
 Attributes are::

   fromId = From Node Id (first node id)
   toId   = To Node Id (second node id)
 
Example::

  <link fromId="0" toId="1"/>

5. <packet> element

This element describes a packet over wired links being transmitted at some node and received at another. 

The reception details are described in its associated rx element
 Attributes are::

   fromId = Node Id transmitting the packet
   fbTx = First bit transmit time of the packet
   lbTx = Last bit transmit time of the packet

Example::

  <packet fromId="1" fbTx="1" lbTx="1.000067199"><rx toLp="0" toId="0" fbRx="1.002" lbRx="1.002067199"/>

A packet over wired-links from Node 1 was received at Node 0. The first bit of the packet was transmitted at  the 1st second, the last bit was transmitted at the 1.000067199th second of the simulation Node 0 received the first bit of the packet at the 1.002th second and the last bit of the packet at the 1.002067199th second of the simulation
NOTE: A packet with fromId == toId is a dummy packet used internally by the AnimationInterface. Please ignore this packet

6. <rx> element

This element describes the reception of a packet at a node.
 Attributes are::

   toId = Node Id receiving the packet
   fbRx = First bit Reception Time of the packet
   lbRx = Last bit Reception Time of the packet

7. <wpacket> element

This element describes a packet over wireless links being transmitted at some node and received at another. 

The reception details are described in its associated rx element.
 Attributes are::

   fromId = Node Id transmitting the packet
   fbTx = First bit transmit time of the packet
   lbTx = Last bit transmit time of the packet
   range = Range of the transmission

Example::

  <wpacket fromId = "20" fbTx = "0.003" lbTx = "0.003254" range = "59.68176982">
  <rx toLp="0" toId="32" fbRx="0.003000198" lbRx="0.003254198"/>

A packet over wireless-links from Node 20 was received at Node 32. The first bit of the packet was transmitted at  the 0.003th second, the last bit was transmitted at the 0.003254 second of the simulation Node 0 received the first bit of the packet at the 0.003000198 second and the last bit of the packet at the 0.003254198 second of the simulation

Wiki
====
For detailed instructions on installing "NetAnim", F.A.Qs and loading the XML trace file 
(mentioned earlier) using NetAnim please refer:
http://www.nsnam.org/wiki/index.php/NetAnim
