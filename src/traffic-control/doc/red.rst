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
as the queue disc and having a size equal to the RED QueueLimit attribute
is created. If the user provides an internal queue, such a queue must
operate in the same mode as the queue disc and have a size not less than
the RED QueueLimit attribute.

Adaptive Random Early Detection (ARED)
======================================
ARED is a variant of RED with two main features: (i) automatically sets Queue
weight, MinTh and MaxTh and (ii) adapts maximum drop probability. The model
in ns-3 contains implementation of both the features, and is a port of Sally
Floyd's ns-2 ARED model. Note that the user is allowed to choose and explicitly
configure the simulation by selecting feature (i) or feature (ii), or both.
 

References
==========

The RED queue aims to be close to the results cited in:
S.Floyd, K.Fall http://icir.org/floyd/papers/redsims.ps

ARED queue implementation is based on the algorithm provided in:
S. Floyd et al, http://www.icir.org/floyd/papers/adaptiveRed.pdf

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

In addition to RED attributes, ARED queue requires following attributes:

* ARED (Boolean attribute. Default: false)
* AdaptMaxP (Boolean attribute to adapt m_curMaxP. Default: false)
* Target Delay (time)
* Interval (time)
* LastSet (time)
* Top (upper limit of m_curMaxP)
* Bottom (lower limit of m_curMaxP)
* Alpha (increment parameter for m_curMaxP)
* Beta (decrement parameter for m_curMaxP)
* RTT

Consult the ns-3 documentation for explanation of these attributes.

Simulating ARED
===============

To switch on ARED algorithm, the attribute ARED must be set to true,
as done in ``src/traffic-control/examples/adaptive-red-tests.cc``:

.. sourcecode:: cpp

  Config::SetDefault ("ns3::RedQueueDisc::ARED", BooleanValue (true));

Setting ARED to true implicitly configures both: (i) automatic setting
of Queue weight, MinTh and MaxTh and (ii) adapting m_curMaxP.

NOTE: To explicitly configure (i) or (ii), set ARED attribute to false
and follow the procedure described next:

To configure (i); Queue weight, MinTh and MaxTh, all must be set to 0,
as done in ``src/traffic-control/examples/adaptive-red-tests.cc``:

.. sourcecode:: cpp

  Config::SetDefault ("ns3::RedQueueDisc::QW", DoubleValue (0.0));
  Config::SetDefault ("ns3::RedQueueDisc::MinTh", DoubleValue (0));
  Config::SetDefault ("ns3::RedQueueDisc::MaxTh", DoubleValue (0));

To configure (ii); AdaptMaxP must be set to true, as done in
``src/traffic-control/examples/adaptive-red-tests.cc``:

.. sourcecode:: cpp

  Config::SetDefault ("ns3::RedQueueDisc::AdaptMaxP", BooleanValue (true));

Examples
========

The RED queue example is found at ``src/traffic-control/examples/red-tests.cc``.

ARED queue examples can be found at:
``src/traffic-control/examples/adaptive-red-tests.cc`` and 
``src/traffic-control/examples/red-vs-ared.cc``

Validation
**********

The RED model has been validated and the report is currently stored
at: https://github.com/downloads/talau/ns-3-tcp-red/report-red-ns3.pdf 
