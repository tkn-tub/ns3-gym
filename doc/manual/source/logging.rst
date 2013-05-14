.. include:: replace.txt

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

Logging
-------

The |ns3| logging facility can be used to monitor or debug the progress
of simulation programs.  Logging output can be enabled by program statements
in your ``main()`` program or by the use of the ``NS_LOG`` environment variable.

Logging statements are not compiled into optimized builds of |ns3|.  To use
logging, one must build the (default) debug build of |ns3|.

The project makes no guarantee about whether logging output will remain 
the same over time.  Users are cautioned against building simulation output
frameworks on top of logging code, as the output and the way the output
is enabled may change over time. 

Logging overview
****************

|ns3| logging statements are typically used to log various program
execution events, such as the occurrence of simulation events or the
use of a particular function.

For example, this code snippet is from ``Ipv4L3Protocol::IsDestinationAddress()``:

::

  if (address == iaddr.GetBroadcast ())
    {
      NS_LOG_LOGIC ("For me (interface broadcast address)");
      return true;
     }

If logging has been enabled for the ``Ipv4L3Protocol`` component at a level
of ``LOGIC`` or above (see below about logging levels), the statement
will be printed out; otherwise, it will be suppressed.

Logging levels
==============

The following levels are defined; each level will enable the levels above
it, with the ``ALL`` level being most verbose:

#. ``LOG_NONE``:  the default, no logging
#. ``LOG_ERROR``:  serious error messages only
#. ``LOG_WARN``:  warning messages
#. ``LOG_DEBUG``:  for use in debugging
#. ``LOG_FUNCTION``: function tracing
#. ``LOG_LOGIC``:  control flow tracing within functions
#. ``LOG_ALL``:  print everything

A special logging level will cause logging output to unconditionally
appear on ``std::clog``, regardless of whether the user has explicitly enabled 
logging.
This macro, ``NS_LOG_UNCOND()``, can be used like a kind of ``printf()`` in
your code.  An example can be found in ``scratch/scratch-simulator.cc``:

::

   NS_LOG_UNCOND ("Scratch Simulator");

Logging prefixes
================

This section still needs documentation; bug 1496 is open on this:

::

   $ NS_LOG="*=all|prefix_all" ./waf --run scratch-simulator
   Scratch Simulator
   ScratchSimulator:main(): [ERROR] error message
   ScratchSimulator:main(): [WARN] warn message
   ScratchSimulator:main(): [DEBUG] debug message
   ScratchSimulator:main(): [INFO] info message
   ScratchSimulator:main(function)
   ScratchSimulator:main(): [LOGIC] logic message

Enabling logging output
=======================

There are two ways that users typically control logging output.  The
first is by setting an ``NS_LOG`` environment variable; e.g.:

::

   NS_LOG="*" ./waf --run first

will run the first tutorial program with all logging output.  This can
be made more granular by selecting individual components:

::

   NS_LOG="Ipv4L3Protocol" ./waf --run first
  
The second way to enable this is to use explicit statements in your
program, such as in the first tutorial program:

::
   
   int
   main (int argc, char *argv[])
   {
     LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
     LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
     ...

Some helpers have special methods to enable the logging of all components
in a module (across different compilation units, but logically grouped
together such as the |ns3| wifi code:

::

   WifiHelper wifiHelper;
   wifiHelper.EnableLogComponents (); 


How to add logging to your code
*******************************

To add logging to your code, please follow the below steps:

1) Put ``NS_LOG_COMPONENT_DEFINE`` macro outside of namespace ns3

Create a unique string identifier (usually based on the name of the file
and/or class defined within the file) and register it with a macro call
such as follows:

::

   NS_LOG_COMPONENT_DEFINE ("Ipv4L3Protocol");

   namespace ns3 {
   ...

The macro was carefully written to permit inclusion either within or
outside of namespace ``ns3``, and usage will vary across the codebase, but
the original intent was to register this *outside* of namespace ``ns3``.

2) Add logging statements to your functions and function bodies.

There are a couple of guidelines on this:

* Do *not* add function logging in operators or explicit copy constructors, 
  since these will cause infinite recursion and stack overflow.
* Use the ``NS_LOG_FUNCTION_NOARGS()`` variant for static methods *only*. When 
  a non-static member function has no arguments, it should be logged by 
  ``NS_LOG_FUNCTION (this)`` macro.
* Make sure that you test that your logging changes do not break the code; 
  running some example programs with all log components turned on (e.g. 
  ``NS_LOG="*"``) is one way to test this.

