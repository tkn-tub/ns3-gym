.. include:: replace.txt
.. highlight:: cpp

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

There are presently three implementations of TCP available for |ns3|.

* a natively implemented TCP for ns-3
* support for the `Network Simulation Cradle (NSC) <http://www.wand.net.nz/~stj2/nsc/>`_
* support for `Direct Code Exectution (DCE) <http://http://www.nsnam.org/~thehajime/ns-3-dce-doc/getting-started.html>`_

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

The implementation of TCP is contained in the following files:

.. sourcecode:: text

    src/internet/model/tcp-header.{cc,h}
    src/internet/model/tcp-l4-protocol.{cc,h}
    src/internet/model/tcp-socket-factory-impl.{cc,h}
    src/internet/model/tcp-socket-base.{cc,h}
    src/internet/model/tcp-tx-buffer.{cc,h}
    src/internet/model/tcp-rx-buffer.{cc,h}
    src/internet/model/tcp-rfc793.{cc,h}
    src/internet/model/tcp-tahoe.{cc,h}
    src/internet/model/tcp-reno.{cc,h}
    src/internet/model/tcp-westwood.{cc,h}
    src/internet/model/tcp-newreno.{cc,h}
    src/internet/model/rtt-estimator.{cc,h}
    src/network/model/sequence-number.{cc,h}

Different variants of TCP congestion control are supported by subclassing
the common base class :cpp:class:`TcpSocketBase`.  Several variants
are supported, including :rfc:`793` (no congestion control), Tahoe, Reno, Westwood,
Westwood+, and NewReno.  NewReno is used by default.  See the Usage section of this
document for on how to change the default TCP variant used in simulation.

Usage
+++++

In many cases, usage of TCP is set at the application layer by telling
the |ns3| application which kind of socket factory to use.

Using the helper functions defined in ``src/applications/helper`` and
``src/network/helper``, here is how one would create a TCP receiver::

  // Create a packet sink on the star "hub" to receive these packets
  uint16_t port = 50000;
  Address sinkLocalAddress(InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
  ApplicationContainer sinkApp = sinkHelper.Install (serverNode);
  sinkApp.Start (Seconds (1.0));
  sinkApp.Stop (Seconds (10.0));

Similarly, the below snippet configures OnOffApplication traffic source to use
TCP::

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
|ns3| attribute system. These are documented in the `Doxygen
<http://www.nsnam.org/doxygen/classns3_1_1_tcp_socket.html>` for class
:cpp:class:`TcpSocket`.  For example, the maximum segment size is a
settable attribute.

To set the default socket type before any internet stack-related objects are
created, one may put the following statement at the top of the simulation
program:: 

  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue ("ns3::TcpTahoe")); 

For users who wish to have a pointer to the actual socket (so that
socket operations like Bind(), setting socket options, etc. can be
done on a per-socket basis), Tcp sockets can be created by using the 
``Socket::CreateSocket()`` method.  The TypeId passed to CreateSocket()
must be of type :cpp:class:`ns3::SocketFactory`, so configuring the underlying 
socket type must be done by twiddling the attribute associated with the
underlying TcpL4Protocol object.  The easiest way to get at this would be 
through the attribute configuration system.  In the below example,
the Node container "n0n1" is accessed
to get the zeroth element, and a socket is created on this node::

  // Create and bind the socket...
  TypeId tid = TypeId::LookupByName ("ns3::TcpTahoe");
  Config::Set ("/NodeList/*/$ns3::TcpL4Protocol/SocketType", TypeIdValue (tid));
  Ptr<Socket> localSocket =
    Socket::CreateSocket (n0n1.Get (0), TcpSocketFactory::GetTypeId ());

Above, the "*" wild card for node number is passed to the attribute
configuration system, so that all future sockets on all nodes are set to 
Tahoe, not just on node 'n0n1.Get (0)'.  If one wants to limit it to just 
the specified node, one would have to do something like::

  // Create and bind the socket...
  TypeId tid = TypeId::LookupByName ("ns3::TcpTahoe");
  std::stringstream nodeId;
  nodeId << n0n1.Get (0)->GetId ();
  std::string specificNode = "/NodeList/" + nodeId.str () + "/$ns3::TcpL4Protocol/SocketType";
  Config::Set (specificNode, TypeIdValue (tid));
  Ptr<Socket> localSocket =
    Socket::CreateSocket (n0n1.Get (0), TcpSocketFactory::GetTypeId ()); 

Once a TCP socket is created, one will want to follow conventional socket logic
and either connect() and send() (for a TCP client) or bind(), listen(), and
accept() (for a TCP server). See :ref:`Sockets-APIs` for a review of
how sockets are used in |ns3|.

Validation
++++++++++

Several TCP validation test results can be found in the
`wiki page <http://www.nsnam.org/wiki/New_TCP_Socket_Architecture>`_ 
describing this implementation.

Current limitations
+++++++++++++++++++

* SACK is not supported

Testing TCP
+++++++++++

In GSoC 2015, the TCP subsystem has been improved to support automated test
cases on both socket functionalities and congestion control algorithms. To show
how to write tests for TCP, here we will explain the process of creating a test
case which reproduces a bug, firstly reported as bug 1571 in 2013, not fixed
in years, and then put in spotlight again by the user Mahdi on 29 August, 2015.

The bug regards the zero window situation, which happens when the receiver can
not handle more data. In this case, it advertises a zero window, which causes
the sender to pause the transmission and wait for the receiver to be ready again.
The sender has a timer, to periodically check the receiver's window: however,
in modern TCP implementation, when the receiver has freed a "significant" amount
of data, the receiver itself sends an "active" window update, meaning that
the transmission could be resumed. Nevertheless, the sender timer is still
necessary because window updates can be lost.

.. note::
   During the text, we will assume some knowledge about the general design
   of the TCP test infrastructure, which is explained in details into the
   Doxygen documentation. As a brief recap, our strategy is to have a class
   that setup a TCP connection, and which calls protected member of itself.
   In this way, subclasses can implement the necessary members, which will
   be called by the main TcpGeneralTest class when events occour. For example,
   after processing an ACK, the method ProcessedAck will be invoked. Subclasses
   interested in checking some particular things which must have happened during
   an ACK processing, should implement the ProcessedAck method and check
   the interesting values inside the method. To get a list of available methods,
   please check the Doxygen documentation.

We will work to write two test case, covering both situations, as said the
sender's zero-window probing and the receiver "active" window update. Our focus
will be on dealing with the reported problems, which are:

* an ns-3 receiver does not send "active" window update when its receive buffer
  is being freed;
* even if the window update is artificially crafted, the transmission does not
  resume.

However, other things should be checked in the test:

* Persistent timer setup
* Persistent timer teardown if rWnd increases

At first, we should define the general parameters for the TCP connection, which
will be one-sided (one node is acting as SENDER, while the other is acting as
RECEIVER). They are coded in the constructor of the test case, as subclass of
TcpGeneralTest (see the doxygen documentation to discover all methods you have
at your disposal):

* Application packet size set to 500, and 20 packet in total (it means a stream
  of 10k bytes)
* Segment size for both SENDER and RECEIVER set to 500 bytes
* Initial slow start threshold set to UINT32_MAX
* Initial congestion window for the SENDER set to 10 segments (5000 bytes)
* Congestion control: NewReno

We have also to define the link properties, because the above definition does
not work for every combination of propagation delay and sender application behavior.
We can specify the following parameters through the same test constructor:

* Link one-way propagation delay: 50 ms
* Application packet generation interval: 10 ms
* Application starting time: 20 s after the starting point

The code is the following:

.. code-block:: c++

   TcpZeroWindowTest::TcpZeroWindowTest (const std::string &desc)
      : TcpGeneralTest (desc, 500, 20, Seconds (0.01), Seconds (0.05), Seconds (2.0),
                        0xffffffff, 10, 500)
   {
   }

To define a zero-window situation, we choose (by design) to initiate the connection
with a 0-byte rx buffer. This implies that the RECEIVER, in its first SYN-ACK,
advertizes a zero window. To this aim, we implement the method
CreateReceiverSocket, setting an Rx buffer value of 0 bytes (at line 6 of the
following code):

.. code-block:: c++
   :linenos:
   :emphasize-lines: 6,7,8

   Ptr<TcpSocketMsgBase>
   TcpZeroWindowTest::CreateReceiverSocket (Ptr<Node> node)
   {
     Ptr<TcpSocketMsgBase> socket = TcpGeneralTest::CreateReceiverSocket (node);

     socket->SetAttribute("RcvBufSize", UintegerValue (0));
     Simulator::Schedule (Seconds (10.0),
                          &TcpZeroWindowTest::IncreaseBufSize, this);

     return socket;
   }

Even so, to check the active window update, we should schedule an increase
of the buffer size. We do this at line 7 and 8, scheduling the function
IncreaseBufSize.

.. code-block:: c++

   void
   TcpZeroWindowTest::IncreaseBufSize ()
   {
     SetRcvBufSize (RECEIVER, 2500);
   }

Which utilizes the SetRcvBufSize method to edit the RxBuffer object of the
RECEIVER. As said before, check the Doxygen documentation for class TcpGeneralTest
to be aware of the various possibility that it offers.

.. note::
   By design, we choose to mantain a close relationship between TcpSocketBase
   and TcpGeneralClass: they are connected by a friendship relation. Since
   friendship is not passed through inheritance, if you discover that you
   need to access or to modify a private (or protected) member of TcpSocketBase,
   you can do it by adding a method in the class TcpGeneralSocket. An example
   of such method is SetRcvBufSize, which allows TcpGeneralSocket subclasses
   to forcefully set the RxBuffer size.

   .. code-block:: c++

      void
      TcpGeneralTest::SetRcvBufSize (SocketWho who, uint32_t size)
      {
        if (who == SENDER)
          {
            m_senderSocket->SetRcvBufSize (size);
          }
        else if (who == RECEIVER)
          {
            m_receiverSocket->SetRcvBufSize (size);
          }
        else
          {
            NS_FATAL_ERROR ("Not defined");
          }
      }

After saying that, we can start to follow the TCP connection:

#. At time 0.0 s the connection is opened sender side, with a SYN packet sent from
   SENDER to RECEIVER
#. At time 0.05 s the RECEIVER gets the SYN and replies with a SYN-ACK
#. At time 0.10 s the SENDER gets the SYN-ACK and replies with a SYN.

While the general structure is defined, and the connection is started,
we need to define a way to check the rWnd field on the segments. To this aim,
we can implement the methods Rx and Tx in the TcpGeneralTest subclass,
checking each time the actions of the RECEIVER and the SENDER. These methods are
defined in TcpGeneralTest, and they are attached to the Rx and Tx traces in the
TcpSocketBase. We should code each single thing we want to ensure during the
connection (it will prevent the test from changing over the time, and it ensures
that the behavior will stay consistent through releases). We start by ensuring that
the first SYN-ACK has 0 as advertized window size:

.. code-block:: c++

   void
   TcpZeroWindowTest::Tx(const Ptr<const Packet> p, const TcpHeader &h, SocketWho who)
   {
     ...
     else if (who == RECEIVER)
       {
         NS_LOG_INFO ("\tRECEIVER TX " << h << " size " << p->GetSize());

         if (h.GetFlags () & TcpHeader::SYN)
           {
             NS_TEST_ASSERT_MSG_EQ (h.GetWindowSize(), 0,
                                    "RECEIVER window size is not 0 in the SYN-ACK");
           }
       }
       ....
    }

Pratically, we are checking that every SYN packet sent by the RECEIVER has the
advertised window set to 0. The same thing is done also by checking, in the Rx
method, that each SYN received by SENDER has the advertised window set to 0.
Thanks to the log subsystem, we can print what is happening through messages.
If we run the experiment, enabling the logging, we can see the following:

.. code-block:: bash

   ./waf shell
   gdb --args ./build/utils/ns3-dev-test-runner-debug --test-name=tcp-zero-window-test --stop-on-failure --fullness=QUICK --assert-on-failure --verbose
   (gdb) run

   0.00s TcpZeroWindowTestSuite:Tx(): 0.00	SENDER TX 49153 > 4477 [SYN] Seq=0 Ack=0 Win=32768 ns3::TcpOptionWinScale(2) ns3::TcpOptionTS(0;0) size 36
   0.05s TcpZeroWindowTestSuite:Rx(): 0.05	RECEIVER RX 49153 > 4477 [SYN] Seq=0 Ack=0 Win=32768 ns3::TcpOptionWinScale(2) ns3::TcpOptionTS(0;0) ns3::TcpOptionEnd(EOL) size 0
   0.05s TcpZeroWindowTestSuite:Tx(): 0.05	RECEIVER TX 4477 > 49153 [SYN|ACK] Seq=0 Ack=1 Win=0 ns3::TcpOptionWinScale(0) ns3::TcpOptionTS(50;0) size 36
   0.10s TcpZeroWindowTestSuite:Rx(): 0.10	SENDER RX 4477 > 49153 [SYN|ACK] Seq=0 Ack=1 Win=0 ns3::TcpOptionWinScale(0) ns3::TcpOptionTS(50;0) ns3::TcpOptionEnd(EOL) size 0
   0.10s TcpZeroWindowTestSuite:Tx(): 0.10	SENDER TX 49153 > 4477 [ACK] Seq=1 Ack=1 Win=32768 ns3::TcpOptionTS(100;50) size 32
   0.15s TcpZeroWindowTestSuite:Rx(): 0.15	RECEIVER RX 49153 > 4477 [ACK] Seq=1 Ack=1 Win=32768 ns3::TcpOptionTS(100;50) ns3::TcpOptionEnd(EOL) size 0
   (...)

The output is cut to show the threeway handshake. As we can see from the headers,
the rWnd of RECEIVER is set to 0, and thankfully our tests are not failing.
Now we need to test for the persistent timer, which sould be started by
the SENDER after it receives the SYN-ACK. Since the Rx method is called before
any computation on the received packet, we should utilize another method, namely
ProcessedAck, which is the method called after each processed ACK. In the
following, we show how to check if the persistent event is running after the
processing of the SYN-ACK:

.. code-block:: c++

   void
   TcpZeroWindowTest::ProcessedAck (const Ptr<const TcpSocketState> tcb,
                                    const TcpHeader& h, SocketWho who)
   {
     if (who == SENDER)
       {
         if (h.GetFlags () & TcpHeader::SYN)
           {
             EventId persistentEvent = GetPersistentEvent (SENDER);
             NS_TEST_ASSERT_MSG_EQ (persistentEvent.IsRunning (), true,
                                    "Persistent event not started");
           }
       }
    }

Since we programmed the increase of the buffer size after 10 simulated seconds,
we expect the persistent timer to fire before any rWnd changes. When it fires,
the SENDER should send a window probe, and the receiver should reply reporting
again a zero window situation. At first, we investigates on what the sender sends:

..  code-block:: c++
    :linenos:
    :emphasize-lines: 1,6,7,11

      if (Simulator::Now ().GetSeconds () <= 6.0)
        {
          NS_TEST_ASSERT_MSG_EQ (p->GetSize () - h.GetSerializedSize(), 0,
                                 "Data packet sent anyway");
        }
      else if (Simulator::Now ().GetSeconds () > 6.0 &&
               Simulator::Now ().GetSeconds () <= 7.0)
        {
          NS_TEST_ASSERT_MSG_EQ (m_zeroWindowProbe, false, "Sent another probe");

          if (! m_zeroWindowProbe)
            {
              NS_TEST_ASSERT_MSG_EQ (p->GetSize () - h.GetSerializedSize(), 1,
                                     "Data packet sent instead of window probe");
              NS_TEST_ASSERT_MSG_EQ (h.GetSequenceNumber(), SequenceNumber32 (1),
                                     "Data packet sent instead of window probe");
              m_zeroWindowProbe = true;
            }
        }

We divide the events by simulated time. At line 1, we check everything which
happens before the 6.0 seconds mark, for instance that no data packets are sent,
and that the state remains OPEN for both sender and receiver.

Since the persistent timeout is initialized at 6 seconds (excercise left for the
reader: edit the test, getting this value from the Attribute system), we need
to check (line 6) between 6.0 and 7.0 simulated seconds that the probe is sent.
Only one probe is allowed, and this is the reason for the check at line 11.

.. code-block:: c++
   :linenos:
   :emphasize-lines: 6,7

   if (Simulator::Now ().GetSeconds () > 6.0 &&
       Simulator::Now ().GetSeconds () <= 7.0)
     {
       NS_TEST_ASSERT_MSG_EQ (h.GetSequenceNumber(), SequenceNumber32 (1),
                              "Data packet sent instead of window probe");
       NS_TEST_ASSERT_MSG_EQ (h.GetWindowSize(), 0,
                              "No zero window advertised by RECEIVER");
     }

For what regards the RECEIVER, between the 6 and 7 seconds mark the most important
thing to check is that is sends the zero window segment.

Other checks are redundant; the most safe approach is to deny any other packet
exchange between 7 and 10 seconds mark.

.. code-block:: c++

   else if (Simulator::Now ().GetSeconds () > 7.0 &&
            Simulator::Now ().GetSeconds () < 10.0)
     {
       NS_FATAL_ERROR ("No packets should be sent before the window update");
     }

The state checks are performed at the end of the methods, since they are valid
in every condition:

.. code-block:: c++

   NS_TEST_ASSERT_MSG_EQ (GetCongStateFrom (GetTcb(SENDER)), TcpSocketState::CA_OPEN,
                          "Sender State is not OPEN");
   NS_TEST_ASSERT_MSG_EQ (GetCongStateFrom (GetTcb(RECEIVER)), TcpSocketState::CA_OPEN,
                          "Receiver State is not OPEN");

Now, the interesting part in the Tx method is to check that after the 10.0
seconds mark (when the RECEIVER sends the active window update) the value of
the window should be greater than zero (and precisely, set to 2500):

.. code-block:: c++

   else if (Simulator::Now().GetSeconds() >= 10.0)
     {
       NS_TEST_ASSERT_MSG_EQ (h.GetWindowSize(), 2500,
                              "Receiver window not updated");
     }

To be sure that the sender receives the window update, we can use the Rx
method:

.. code-block:: c++
   :linenos:
   :emphasize-lines: 5

   if (Simulator::Now().GetSeconds() >= 10.0)
     {
       NS_TEST_ASSERT_MSG_EQ (h.GetWindowSize(), 2500,
                              "Receiver window not updated");
       m_windowUpdated = true;
     }

And check every packets, after the 10 seconds mark, that should have the
window updated. At line 5, we also set to true a boolean variable, to check
that we effectively reach this test.

Last but not least, we implement also the NormalClose() method, to check that
the connection ends with a success:

.. code-block:: c++

   void
   TcpZeroWindowTest::NormalClose (SocketWho who)
   {
     if (who == SENDER)
       {
         m_senderFinished = true;
       }
     else if (who == RECEIVER)
       {
         m_receiverFinished = true;
       }
   }

The method is called only if all bytes are transmitted successfully. Then, in
the method FinalChecks(), we check all variables, which should be true (which
indicates that we have perfectly closed the connection).

.. code-block:: c++

   void
   TcpZeroWindowTest::FinalChecks ()
   {
     NS_TEST_ASSERT_MSG_EQ (m_zeroWindowProbe, true,
                            "Zero window probe not sent");
     NS_TEST_ASSERT_MSG_EQ (m_windowUpdated, true,
                            "Window has not updated during the connection");
     NS_TEST_ASSERT_MSG_EQ (m_senderFinished, true,
                            "Connection not closed successfully (SENDER)");
     NS_TEST_ASSERT_MSG_EQ (m_receiverFinished, true,
                            "Connection not closed successfully (RECEIVER)");
   }

To run the test, the usual way is

.. code-block:: bash

   ./test.py -s tcp-zero-window-test

   PASS: TestSuite tcp-zero-window-test
   1 of 1 tests passed (1 passed, 0 skipped, 0 failed, 0 crashed, 0 valgrind errors)

To see INFO messages, use a combination of ./waf shell and gdb (really useful):

.. code-block:: bash


    ./waf shell && gdb --args ./build/utils/ns3-dev-test-runner-debug --test-name=tcp-zero-window-test --stop-on-failure --fullness=QUICK --assert-on-failure --verbose

and then, hit "Run".

.. note::
   This code magically runs without any reported errors; however, in real cases,
   when you discover a bug you should expect the being-written test to fail (this
   could indicate a well-written test and a bad-writted model, or a bad-written
   test: we hope you are in the first situation). Correcting bugs is an iterative
   process. For instance, commits created to make this test case running without
   errors are 11633:6b74df04cf44, (others to be merged).

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

NSC has previously been ported to |ns2| and OMNeT++, and was 
was added to |ns3| in September 2008 (ns-3.2 release).  This section 
describes the |ns3| port of NSC and how to use it.

To some extent, NSC has been superseded by the Linux kernel support within 
`Direct Code Execution (DCE) <http://www.nsnam.org/docs/dce/manual/singlehtml/index.html>`_.  However, NSC is still available through the bake build
system.  NSC supports Linux kernels 2.6.18 and 2.6.26, but newer
versions of the kernel have not been ported.  

Prerequisites
+++++++++++++

Presently, NSC has been tested and shown to work on these platforms:
Linux i386 and Linux x86-64.  NSC does not support powerpc.  Use on
FreeBSD or OS X is unsupported (although it may be able to work).

Building NSC requires the packages flex and bison.  

Configuring and Downloading
+++++++++++++++++++++++++++

As of ns-3.17 or later, NSC must either be downloaded separately from
its own repository, or downloading when using the 
`bake build system <http://www.nsnam.org/docs/tutorial/html/getting-started.html#downloading-ns3-using-bake>`_ of 
|ns3|.  

For ns-3.17 or later releases, when using bake, one must configure NSC as 
part of an "allinone" configuration, such as:

.. sourcecode:: bash

  $ cd bake
  $ python bake.py configure -e ns-allinone-3.19
  $ python bake.py download
  $ python bake.py build

Instead of a released version, one may use the ns-3 development version
by specifying "ns-3-allinone" to the configure step above.

NSC may also be downloaded from 
`its download site <http://research.wand.net.nz/software/nsc.php>`_ 
using Mercurial:

.. sourcecode:: bash

  $ hg clone https://secure.wand.net.nz/mercurial/nsc

Prior to the ns-3.17 release, NSC was included in the allinone tarball and
the released version did not need to be separately downloaded.

Building and validating
+++++++++++++++++++++++

NSC may be built as part of the bake build process; alternatively, one
may build NSC by itself using its build system; e.g.:

.. sourcecode:: bash

  $ cd nsc-dev
  $ python scons.py

Once NSC has been built either manually or through the bake system, change
into the |ns3| source directory and try running the following configuration:

.. sourcecode:: bash

  $ ./waf configure

If NSC has been previously built and found by waf, then you will see:

.. sourcecode:: bash

  Network Simulation Cradle     : enabled

If NSC has not been found, you will see:

.. sourcecode:: bash

  Network Simulation Cradle     : not enabled (NSC not found (see option --with-nsc))

In this case, you must pass the relative or absolute path to the NSC libraries
with the "--with-nsc" configure option; e.g.

.. sourcecode:: bash

  $ ./waf configure --with-nsc=/path/to/my/nsc/directory

For |ns3| releases prior to the ns-3.17 release, using the ``build.py`` 
script in ns-3-allinone directory, NSC will be built by default unless the 
platform does not support it. To explicitly disable it when building |ns3|, 
type:

.. sourcecode:: bash

  $ ./waf configure --enable-examples --enable-tests --disable-nsc

If waf detects NSC, then building |ns3| with NSC is performed the same way
with waf as without it.  Once |ns3| is built, try running the following 
test suite:

.. sourcecode:: bash

    $ ./test.py -s ns3-tcp-interoperability

If NSC has been successfully built, the following test should show up 
in the results:

.. sourcecode:: text

    PASS TestSuite ns3-tcp-interoperability

This confirms that NSC is ready to use.

Usage
+++++

There are a few example files.  Try:

.. sourcecode:: bash

    $ ./waf --run tcp-nsc-zoo
    $ ./waf --run tcp-nsc-lfn

These examples will deposit some ``.pcap`` files in your directory,
which can be examined by tcpdump or wireshark.

Let's look at the ``examples/tcp/tcp-nsc-zoo.cc`` file for some typical
usage. How does it differ from using native |ns3| TCP? There is one main
configuration line, when using NSC and the |ns3| helper API, that needs to be
set::

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
directory:

.. sourcecode:: bash

    $ find nsc -name "*.so" -type f 
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
|ns3| attributes system, via a `sysctl <http://en.wikipedia.org/wiki/Sysctl>`_-like interface. In the ``examples/tcp/tcp-nsc-zoo`` example, you
can see the following configuration::


  // this disables TCP SACK, wscale and timestamps on node 1 (the attributes 
    represent sysctl-values).
  Config::Set ("/NodeList/1/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_sack", 
    StringValue ("0"));
  Config::Set ("/NodeList/1/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_timestamps", 
  StringValue ("0"));
  Config::Set ("/NodeList/1/$ns3::Ns3NscStack<linux2.6.26>/net.ipv4.tcp_window_scaling", 
  StringValue ("0"));

These additional configuration variables are not available to native |ns3| TCP.

Also note that default values for TCP attributes in |ns3| TCP may differ from the nsc TCP implementation.  Specifically in |ns3|:

  1) TCP default MSS is 536
  2) TCP Delayed Ack count is 2 
		
Therefore when making comparisons between results obtained using nsc and |ns3| TCP, care must be taken to ensure these values are set appropriately.  See /examples/tcp/tcp-nsc-comparision.cc for an example.

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
* Cygwin and OS X PPC are not supported; OS X Intel is not supported but may work
* The non-Linux stacks of NSC are not supported in |ns3|
* Not all socket API callbacks are supported

For more information, see `this wiki page <http://www.nsnam.org/wiki/Network_Simulation_Cradle_Integration>`_.
