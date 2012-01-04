.. include:: replace.txt

.. _Sockets-APIs:

Sockets APIs
------------

The `sockets API <http://en.wikipedia.org/wiki/Berkeley_sockets>`_
is a long-standing API used by user-space applications to access
network services in the kernel.  A *socket* is an abstraction, like
a Unix file handle, that allows applications to connect to other
Internet hosts and exchange reliable byte streams and unreliable
datagrams, among other services.   

|ns3| provides two types of sockets APIs, and it is important to
understand the differences between them.  The first is a *native*
|ns3| API, while the second uses the services of the native API to 
provide a `POSIX-like <http://en.wikipedia.org/wiki/POSIX>`_
API as part of an overall application process.  Both APIs strive
to be close to the typical sockets API that application writers
on Unix systems are accustomed to, but the POSIX variant is much
closer to a real system's sockets API.

ns-3 sockets API
*****************

The native sockets API for ns-3 provides an interface to various
types of transport protocols (TCP, UDP) as well as to packet sockets
and, in the future, Netlink-like sockets.  However, users are cautioned
to understand that the semantics are *not* the exact same as 
one finds in a real system (for an API which is very much aligned
to real systems, see the next section).

:cpp:class:`ns3::Socket` is defined in ``src/network/model/socket.h``.
Readers will note that many public member functions are aligned
with real sockets function calls, and all other things being equal,
we have tried to align with a Posix sockets API.  However, note that:

* ns-3 applications handle a smart pointer to a Socket object, not
  a file descriptor;

* there is no notion of synchronous API or a *blocking* API;
  in fact, the model for interaction between application and socket is
  one of asynchronous I/O, which is not typically found in real systems
  (more on this below);

* the C-style socket address structures are not used;

* the API is not a complete sockets API, such as supporting
  all socket options or all function variants; 

* many calls use :cpp:class:`ns3::Packet` class to transfer data
  between application and socket.  This may seem peculiar to
  pass *Packets* across a stream socket API, but think
  of these packets as just fancy byte buffers at this level (more
  on this also below).

Basic operation and calls
=========================

.. _sockets-overview:

.. figure:: figures/sockets-overview.*

   Implementation overview of native sockets API

Creating sockets
================

An application that wants to use sockets must first create one.
On real systems using a C-based API, this is accomplished by calling :c:func:`socket()` ::

     int socket(int domain, int type, int protocol);

which creates a socket in the system and returns an integer descriptor.

In ns-3, we have no equivalent of a system call at the lower layers,
so we adopt the following model.  There are certain *factory*
objects that can create sockets.  Each factory is capable of creating
one type of socket, and if sockets of a particular type are able to
be created on a given node, then a factory that can create such sockets
must be aggregated to the Node::

  static Ptr<Socket> CreateSocket (Ptr<Node> node, TypeId tid);

Examples of TypeIds to pass to this method are :cpp:class:`ns3::TcpSocketFactory`,
:cpp:class:`ns3::PacketSocketFactory`, and :cpp:class:`ns3::UdpSocketFactory`.

This method returns a smart pointer to a Socket object.  Here is an
example::

  Ptr<Node> n0;
  // Do some stuff to build up the Node's internet stack
  Ptr<Socket> localSocket = 
     Socket::CreateSocket (n0, TcpSocketFactory::GetTypeId ());

In some ns-3 code, sockets will not be explicitly created by user's
main programs, if an ns-3 application does it.  For instance, for
:cpp:class:`ns3::OnOffApplication`, the function :cpp:func:`ns3::OnOffApplication::StartApplication`
performs the socket creation, and the application holds the socket
pointer.

Using sockets
=============

Below is a typical sequence of socket calls for a TCP client in a
real implementation:

* ``sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);``
* ``bind(sock, ...);``
* ``connect(sock, ...);``
* ``send(sock, ...);``
* ``recv(sock, ...);``
* ``close(sock);``

There are analogs to all of these calls in ns-3, but we will focus on  
two aspects here.  First, most usage of sockets in real systems 
requires a way to manage I/O between the application and kernel.  
These models include *blocking sockets*, *signal-based I/O*,
and *non-blocking sockets* with polling.  In ns-3, we make use
of the callback mechanisms to support a fourth mode, which is 
analogous to POSIX *asynchronous I/O*.

In this model, on the sending side, if the :c:func:`send()` call were to
fail because of insufficient buffers, the application suspends the
sending of more data until a function registered at the 
:cpp:func:`ns3::Socket::SetSendCallback` callback is invoked.  
An application can also ask the socket how much space is available 
by calling :cpp:func:`ns3::Socket::GetTxAvailable`.  A typical sequence 
of events for sending data (ignoring connection setup) might be:::

    * ``SetSendCallback (MakeCallback(&HandleSendCallback));``
    * ``Send ();``
    * ``Send ();``
    * ...
    * Send fails because buffer is full
    * wait until :cpp:func:`HandleSendCallback` is called
    * :cpp:func:`HandleSendCallback` is called by socket, since space now available
    * ``Send (); // Start sending again``

Similarly, on the receive side, the socket user does not block on
a call to ``recv()``.  Instead, the application sets a callback
with :cpp:func:`ns3::Socket::SetRecvCallback` in which the socket will notify the
application when (and how much) there is data to be read, and
the application then calls :cpp:func:`ns3::Socket::Recv` to read the data until
no more can be read.


Packet vs. buffer variants
**************************

There are two basic variants of ``Send()`` and ``Recv()`` supported::

  virtual int Send (Ptr<Packet> p) = 0;
  int Send (const uint8_t* buf, uint32_t size);

  Ptr<Packet> Recv (void);
  int Recv (uint8_t* buf, uint32_t size);

The non-Packet variants are provided for legacy API reasons.  When calling
the raw buffer variant of :cpp:func:`ns3::Socket::Send`, the buffer is immediately
written into a Packet and the packet variant is invoked.

Users may find it semantically odd to pass a Packet to a stream socket
such as TCP.  However, do not let the name bother you; think of 
:cpp:class:`ns3::Packet` to be a fancy byte buffer.  There are a few reasons why
the Packet variants are more likely to be preferred in ns-3:

* Users can use the Tags facility of packets to, for example, encode
  a flow ID or other helper data at the application layer.

* Users can exploit the copy-on-write implementation to avoid
  memory copies (on the receive side, the conversion back to a 
  ``uint8_t* buf`` may sometimes incur an additional copy).

* Use of Packet is more aligned with the rest of the ns-3 API

Sending dummy data
******************

Sometimes, users want the simulator to just pretend that there is an
actual data payload in the packet (e.g. to calculate transmission delay)
but do not want to actually produce or consume the data.  This is
straightforward to support in ns-3; have applications call
``Create<Packet> (size);`` instead of ``Create<Packet> (buffer, size);``.
Similarly, passing in a zero to the pointer argument in the raw buffer
variants has the same effect.  Note that, if some subsequent code tries
to read the Packet data buffer, the fake buffer will be converted to
a real (zeroed) buffer on the spot, and the efficiency will be lost there.

Socket options
**************

*to be completed*

Socket errno
************

*to be completed*

Example programs
****************

*to be completed*

POSIX-like sockets API
**********************
