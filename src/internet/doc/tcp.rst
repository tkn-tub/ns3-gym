.. include:: replace.txt

TCP models in ns-3
------------------

This chapter describes the TCP models available in |ns3|.

Generic support for TCP
***********************

|ns3| was written to support multiple TCP implementations. The implementations
inherit from a few common header classes in the ``src/network`` directory, so that
user code can swap out implementations with minimal changes to the scripts.

There are two important abstract base classes:

* class :cpp:class:`TcpSocket`:  This is defined in
  ``src/internet/model/tcp-socket.{cc,h}``. This class exists for hosting TcpSocket
  attributes that can be reused across different implementations. For instance,
  the attribute ``InitialCwnd`` can be used for any of the implementations
  that derive from class :cpp:class:`TcpSocket`.
* class :cpp:class:`TcpSocketFactory`:  This is used by the layer-4 protocol
  instance to create TCP sockets of the right type.

There are presently two implementations of TCP available for |ns3|.

* a natively implemented TCP for ns-3
* support for the `Network Simulation Cradle (NSC) <http://www.wand.net.nz/~stj2/nsc/>`_

It should also be mentioned that various ways of combining virtual machines
with |ns3| makes available also some additional TCP implementations, but
those are out of scope for this chapter.

ns-3 TCP
********

Until ns-3.10 release, |ns3| contained a port of the TCP model from `GTNetS
<http://www.ece.gatech.edu/research/labs/MANIACS/GTNetS/index.html>`_. 
This implementation was substantially rewritten by Adriam Tam for ns-3.10.
The model is a full TCP, in that it is bidirectional and attempts to model the
connection setup and close logic. 

The implementation of TCP is contained in the following files:::

    src/internet/model/tcp-header.{cc,h}
    src/internet/model/tcp-l4-protocol.{cc,h}
    src/internet/model/tcp-socket-factory-impl.{cc,h}
    src/internet/model/tcp-socket-base.{cc,h}
    src/internet/model/tcp-tx-buffer.{cc,h}
    src/internet/model/tcp-rx-buffer.{cc,h}
    src/internet/model/tcp-rfc793.{cc,h}
    src/internet/model/tcp-tahoe.{cc,h}
    src/internet/model/tcp-reno.{cc,h}
    src/internet/model/tcp-newreno.{cc,h}
    src/internet/model/rtt-estimator.{cc,h}
    src/network/model/sequence-number.{cc,h}

Different variants of TCP congestion control are supported by subclassing
the common base class :cpp:class:`TcpSocketBase`.  Several variants
are supported, including RFC 793 (no congestion control), Tahoe, Reno,
and NewReno.  NewReno is used by default.

Usage
+++++

In many cases, usage of TCP is set at the application layer by telling
the |ns3| application which kind of socket factory to use.

Using the helper functions defined in ``src/applications/helper`` and
``src/network/helper``, here is how one would create a TCP receiver:::

  // Create a packet sink on the star "hub" to receive these packets
  uint16_t port = 50000;
  Address sinkLocalAddress(InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
  ApplicationContainer sinkApp = sinkHelper.Install (serverNode);
  sinkApp.Start (Seconds (1.0));
  sinkApp.Stop (Seconds (10.0));

Similarly, the below snippet configures OnOffApplication traffic source to use
TCP:::

  // Create the OnOff applications to send TCP to the server
  OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address ());

The careful reader will note above that we have specified the TypeId of an
abstract base class :cpp:class:`TcpSocketFactory`. How does the script tell
|ns3| that it wants the native |ns3| TCP vs. some other one?  Well, when
internet stacks are added to the node, the default TCP implementation that is
aggregated to the node is the |ns3| TCP.  This can be overridden as we show
below when using Network Simulation Cradle. So, by default, when using the |ns3|
helper API, the TCP that is aggregated to nodes with an Internet stack is the
native |ns3| TCP.

To configure behavior of TCP, a number of parameters are exported through the
:ref:`Attributes <ns-3 attribute system>`. These are documented in the `Doxygen
<http://www.nsnam.org/doxygen/classns3_1_1_tcp_socket.html>` for class
:cpp:class:`TcpSocket`.  For example, the maximum segment size is a
settable attribute.

For users who wish to have a pointer to the actual socket (so that
socket operations like Bind(), setting socket options, etc. can be
done on a per-socket basis), Tcp sockets can be created by using the 
``Socket::CreateSocket()`` method and passing in the TypeId 
corresponding to the type of socket desired; e.g.::: 

      // Create the socket if not already created
      TypeId tid = TypeId::LookupByName ("ns3::TcpTahoe");
      Ptr<Socket> localSocket = Socket::CreateSocket (node, tid);

The parameter ``tid`` controls the TypeId of the actual TCP Socket
implementation that is instantiated. This way, the application can be written
generically and different socket implementations can be swapped out by
specifying the TypeId.

Once a TCP socket is created, one will want to follow conventional socket logic
and either connect() and send() (for a TCP client) or bind(), listen(), and
accept() (for a TCP server). :ref:`Sockets API <Sockets APIs>` for a review of
how sockets are used in |ns3|.

Validation
++++++++++

Several TCP validation test results can be found in the
`wiki page <http://www.nsnam.org/wiki/index.php/New_TCP_Socket_Architecture>`_ 
describing this implementation.

Current limitations
+++++++++++++++++++

* Only IPv4 is supported
* Neither the Nagle algorithm nor SACK are supported

Network Simulation Cradle
*************************

The `Network Simulation Cradle (NSC) <http://www.wand.net.nz/~stj2/nsc/>`_ is a
framework for wrapping real-world network code into simulators, allowing
simulation of real-world behavior at little extra cost. This work has been
validated by comparing situations using a test network with the same situations
in the simulator. To date, it has been shown that the NSC is able to produce
extremely accurate results.  NSC supports four real world stacks: FreeBSD,
OpenBSD, lwIP and Linux. Emphasis has been placed on not changing any of the
network stacks by hand. Not a single line of code has been changed in the
network protocol implementations of any of the above four stacks. However, a
custom C parser was built to programmatically change source code.

NSC has previously been ported to |ns2| and OMNeT++, and recently 
was added to |ns3|.  This section describes the |ns3| port of NSC and
how to use it.

Prerequisites
+++++++++++++

Presently, NSC has been tested and shown to work on these platforms:
Linux i386 and Linux x86-64. NSC does not support powerpc.

Building NSC requires the packages flex and bison.  

Configuring and Downloading
+++++++++++++++++++++++++++

Using the ``build.py`` script in ns-3-allinone directory, NSC will be enabled by
default unless the platform does not support it. To disable it when building
|ns3|, type:::

./waf configure --enable-examples --enable-tests --disable-nsc

Building and validating
+++++++++++++++++++++++

Building |ns3| with nsc support is the same as building it without; no
additional arguments are needed for waf. Building nsc may take some time
compared to |ns3|; it is interleaved in the |ns3| building process.

Try running the following ns-3 test suite:::  

    ./test.py -s ns3-tcp-interoperability

If NSC has been successfully built, the following test should show up 
in the results:::

    PASS TestSuite ns3-tcp-interoperability

This confirms that NSC is ready to use.

Usage
+++++

There are a few example files.  Try::

    ./waf --run tcp-nsc-zoo
    ./waf --run tcp-nsc-lfn

These examples will deposit some ``.pcap`` files in your directory,
which can be examined by tcpdump or wireshark.

Let's look at the ``examples/tcp/tcp-nsc-zoo.cc`` file for some typical
usage. How does it differ from using native |ns3| TCP? There is one main
configuration line, when using NSC and the |ns3| helper API, that needs to be
set:::

  InternetStackHelper internetStack;

  internetStack.SetNscStack ("liblinux2.6.26.so");
  // this switches nodes 0 and 1 to NSCs Linux 2.6.26 stack.
  internetStack.Install (n.Get(0));
  internetStack.Install (n.Get(1));


The key line is the ``SetNscStack``.  This tells the InternetStack
helper to aggregate instances of NSC TCP instead of native |ns3| TCP
to the remaining nodes.  It is important that this function be called
**before** calling the ``Install()`` function, as shown above.

Which stacks are available to use? Presently, the focus has been on
Linux 2.6.18 and Linux 2.6.26 stacks for |ns3|. To see which stacks
were built, one can execute the following find command at the |ns3| top level
directory:::

    ~/ns-3.10> find nsc -name "*.so" -type f 
    nsc/linux-2.6.18/liblinux2.6.18.so
    nsc/linux-2.6.26/liblinux2.6.26.so

This tells us that we may either pass the library name liblinux2.6.18.so or
liblinux2.6.26.so to the above configuration step.

Stack configuration
+++++++++++++++++++

NSC TCP shares the same configuration attributes that are common across TCP
sockets, as described above and documented in `Doxygen
<http://www.nsnam.org/doxygen/classns3_1_1_tcp_socket.html>`_

Additionally, NSC TCP exports a lot of configuration variables into the 
|ns3| :ref:`Attributes` system, via a `sysctl <http://en.wikipedia.org/wiki/Sysctl>`_-like interface. In the ``examples/tcp/tcp-nsc-zoo`` example, you
can see the following configuration:::


  // this disables TCP SACK, wscale and timestamps on node 1 (the attributes 
    represent sysctl-values).
  Config::Set ("/NodeList/1/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_sack", 
    StringValue ("0"));
  Config::Set ("/NodeList/1/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_timestamps", 
  StringValue ("0"));
  Config::Set ("/NodeList/1/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_window_scaling", 
  StringValue ("0"));

These additional configuration variables are not available to native |ns3| TCP.

NSC API
+++++++

This subsection describes the API that NSC presents to |ns3| or any other
simulator. NSC provides its API in the form of a number of classes that are
defined in ``sim/sim_interface.h`` in the nsc directory.

* **INetStack** INetStack contains the 'low level' operations for the operating
  system network stack, e.g. in and output functions from and to the network
  stack (think of this as the 'network driver interface'. There are also
  functions to create new TCP or UDP sockets.
* **ISendCallback** This is called by NSC when a packet should be sent out to
  the network. This simulator should use this callback to re-inject the packet
  into the simulator so the actual data can be delivered/routed to its
  destination, where it will eventually be handed into Receive() (and eventually
  back to the receivers NSC instance via INetStack->if_receive() ).
* **INetStreamSocket** This is the structure defining a particular connection
  endpoint (file descriptor). It contains methods to operate on this endpoint,
  e.g. connect, disconnect, accept, listen, send_data/read_data, ...
* **IInterruptCallback** This contains the wakeup callback, which is called by
  NSC whenever something of interest happens. Think of wakeup() as a replacement
  of the operating systems wakeup function: Whenever the operating system would
  wake up a process that has been waiting for an operation to complete (for
  example the TCP handshake during connect()), NSC invokes the wakeup() callback
  to allow the simulator to check for state changes in its connection endpoints. 

ns-3 implementation
+++++++++++++++++++

The |ns3| implementation makes use of the above NSC API, and is implemented as
follows.

The three main parts are:

* :cpp:class:`ns3::NscTcpL4Protocol`:  a subclass of Ipv4L4Protocol (and two nsc
  classes: ISendCallback and IInterruptCallback)
* :cpp:class:`ns3::NscTcpSocketImpl`: a subclass of TcpSocket 
* :cpp:class:`ns3::NscTcpSocketFactoryImpl`:  a factory to create new NSC
  sockets

``src/internet/model/nsc-tcp-l4-protocol`` is the main class. Upon
Initialization, it loads an nsc network stack to use (via dlopen()). Each
instance of this class may use a different stack. The stack (=shared library) to
use is set using the SetNscLibrary() method (at this time its called indirectly
via the internet stack helper). The nsc stack is then set up accordingly (timers
etc). The NscTcpL4Protocol::Receive() function hands the packet it receives
(must be a complete tcp/ip packet) to the nsc stack for further processing.  To
be able to send packets, this class implements the nsc send_callback method.
This method is called by nsc whenever the nsc stack wishes to send a packet out
to the network. Its arguments are a raw buffer, containing a complete TCP/IP
packet, and a length value. This method therefore has to convert the raw data to
a Ptr<Packet> usable by |ns3|. In order to avoid various ipv4 header issues,
the nsc ip header is not included. Instead, the tcp header and the actual
payload are put into the Ptr<Packet>, after this the Packet is passed down to
layer 3 for sending the packet out (no further special treatment is needed in
the send code path).

This class calls ``ns3::NscTcpSocketImpl`` both from the nsc wakeup() callback
and from the Receive path (to ensure that possibly queued data is scheduled for
sending).

``src/internet/model/nsc-tcp-socket-impl`` implements the nsc socket interface.
Each instance has its own nscTcpSocket. Data that is Send() will be handed to
the nsc stack via m_nscTcpSocket->send_data(). (and not to nsc-tcp-l4, this is
the major difference compared to |ns3| TCP). The class also queues up data that
is Send() before the underlying descriptor has entered an ESTABLISHED state.
This class is called from the nsc-tcp-l4 class, when the nsc-tcp-l4 wakeup()
callback is invoked by nsc. nsc-tcp-socket-impl then checks the current
connection state (SYN_SENT, ESTABLISHED, LISTEN...) and schedules appropriate
callbacks as needed, e.g. a LISTEN socket will schedule Accept to see if a new
connection must be accepted, an ESTABLISHED socket schedules any pending data
for writing, schedule a read callback, etc.

Note that ``ns3::NscTcpSocketImpl`` does not interact with nsc-tcp directly:
instead, data is redirected to nsc. nsc-tcp calls the nsc-tcp-sockets of a node
when its wakeup callback is invoked by nsc. 

Limitations
+++++++++++

* NSC only works on single-interface nodes; attempting to run it on a
  multi-interface node will cause a program error.  
* Cygwin and OS X PPC are not supported
* The non-Linux stacks of NSC are not supported in |ns3|
* Not all socket API callbacks are supported

For more information, see `this wiki page <http://www.nsnam.org/wiki/index.php/Network_Simulation_Cradle_Integration>`_.
