.. include:: replace.txt
.. highlight:: cpp

RED queue disc
---------------------

Model Description
*****************

Random Early Detection (RED) is a queue discipline that aims to provide
early signals to transport protocol congestion control (e.g. TCP) that
congestion is imminent, so that they back off their rate gracefully
rather than with a bunch of tail-drop losses (possibly incurring
TCP timeout).  The model in ns-3 is a port of Sally Floyd's ns-2
RED model.

Note that, starting from ns-3.25, RED is no longer a queue variant and
cannot be installed as a NetDevice queue. Instead, RED is a queue disc
and must be installed in the context of the traffic control (see the
examples mentioned below).

The RED queue disc does not require packet filters, does not admit
child queue discs and uses a single internal queue. If not provided by
the user, a DropTail queue operating in the same mode (packet or byte)
as the queue disc and having a size equal to the RED queue limit attribute
is created. If the user provides an internal queue, such a queue must
operate in the same mode as the queue disc and have a size not less than
the RED queue limit attribute.

The RED model just supports default RED.  Adaptive RED is not supported.

References
==========

The RED queue aims to be close to the results cited in:
S.Floyd, K.Fall http://icir.org/floyd/papers/redsims.ps

Attributes
==========

The RED queue contains a number of attributes that control the RED
policies:

* Mode (bytes or packets)
* MeanPktSize
* IdlePktSize
* Wait (time)
* Gentle mode
* MinTh, MaxTh
* QueueLimit
* Queue weight
* LInterm
* LinkBandwidth
* LinkDelay

Consult the ns-3 documentation for explanation of these attributes.

Examples
========

The RED queue example is found at ``src/traffic-control/examples/red-tests.cc``.

Validation
**********

The RED model has been validated and the report is currently stored
at: https://github.com/downloads/talau/ns-3-tcp-red/report-red-ns3.pdf 
