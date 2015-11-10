Internet Applications Module Documentation
------------------------------------------

.. include:: replace.txt
.. highlight:: cpp

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

The goal of this module is to hold all the Internet-specific applications,
and most notably some very specific applications (e.g., ping) or daemons (e.g., radvd).  Other non-Internet-specific applications such as packet generators
are contained in other modules.

Model Description
*****************

The source code for the new module lives in the directory ``src/internet-apps``.

Each application has its own goals, limitations and scope, which are briefly explained
in the following.

V4Ping
======

This app mimics a "ping" (ICMP Echo) using IPv4. The application allows the 
following attributes to be set:

* Remote address
* Verbose mode
* Packet size (default 56 bytes)
* Packet interval  (default 1 second)

Moreover, the user can access the measured rtt value (as a Traced Source).

Ping6
=====

This app mimics a "ping" (ICMP Echo) using IPv6. The application allows the 
following attributes to be set:

* Remote address
* Local address (sender address)
* Packet size (default 56 bytes)
* Packet interval  (default 1 second)
* Max number of packets to send

Radvd
=====

This app mimics a "RADVD" daemon. I.e., the daemon responsible for IPv6 routers 
advertisements. All the IPv6 routers should have a RADVD daemon installed.

The configuration of the Radvd application mimics the one of the radvd Linux program.

Examples and use
****************

All the applications are extensively used in the top-level ``examples'' 
directories. The users are encouraged to check the scripts therein to have a 
clear overview of the various options and usage tricks.
