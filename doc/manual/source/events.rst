.. include:: replace.txt
.. highlight:: cpp


.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

Events and Simulator
--------------------

|ns3| is a discrete-event network simulator.  Conceptually, the simulator
keeps track of a number of events that are scheduled to execute at a
specified simulation time.  The job of the simulator is to execute the
events in sequential time order.  Once the completion of an event occurs,
the simulator will move to the next event (or will exit if there are no
more events in the event queue).  If, for example, an event scheduled
for simulation time "100 seconds" is executed, and the next event is not
scheduled until "200 seconds", the simulator will immediately jump from 
100 seconds to 200 seconds (of simulation time) to execute the next event.
This is what is meant by "discrete-event" simulator.

To make this all happen, the simulator needs a few things:

1) a simulator object that can access an event queue where events are 
   stored and that can manage the execution of events
2) a scheduler responsible for inserting and removing events from the queue
3) a way to represent simulation time
4) the events themselves

This chapter of the manual describes these fundamental objects 
(simulator, scheduler, time, event) and how they are used.

Event
*****

*To be completed*

Simulator
*********

The Simulator class is the public entry point to access event scheduling
facilities. Once a couple of events have been scheduled to start the
simulation, the user can start to execute them by entering the simulator
main loop (call ``Simulator::Run``). Once the main loop starts running, it
will sequentially execute all scheduled events in order from oldest to
most recent until there are either no more events left in the event
queue or Simulator::Stop has been called.

To schedule events for execution by the simulator main loop, the
Simulator class provides the Simulator::Schedule* family of functions.

1) Handling event handlers with different signatures

These functions are declared and implemented as C++ templates to handle
automatically the wide variety of C++ event handler signatures used in
the wild. For example, to schedule an event to execute 10 seconds in the
future, and invoke a C++ method or function with specific arguments, you
might write this:

::

   void handler (int arg0, int arg1)
   {
     std::cout << "handler called with argument arg0=" << arg0 << " and
        arg1=" << arg1 << std::endl;
   }

   Simulator::Schedule(Seconds(10), &handler, 10, 5);

Which will output:

.. sourcecode:: text

   handler called with argument arg0=10 and arg1=5

Of course, these C++ templates can also handle transparently member
methods on C++ objects:

*To be completed:  member method example*

Notes:

* the ns-3 Schedule methods recognize automatically functions and
  methods only if they take less than 5 arguments. If you need them to
  support more arguments, please, file a bug report.
* Readers familiar with the term 'fully-bound functors' will recognize
  the Simulator::Schedule methods as a way to automatically construct such
  objects. 

2) Common scheduling operations

The Simulator API was designed to make it really simple to schedule most
events. It provides three variants to do so (ordered from most commonly
used to least commonly used):

* Schedule methods which allow you to schedule an event in the future
  by providing the delay between the current simulation time and the
  expiration date of the target event.
* ScheduleNow methods which allow you to schedule an event for the
  current simulation time: they will execute _after_ the current event is
  finished executing but _before_ the simulation time is changed for the
  next event.
* ScheduleDestroy methods which allow you to hook in the shutdown
  process of the Simulator to cleanup simulation resources: every
  'destroy' event is executed when the user calls the Simulator::Destroy
  method.

3) Maintaining the simulation context

There are two basic ways to schedule events, with and without *context*.
What does this mean?

::

  Simulator::Schedule (Time const &time, MEM mem_ptr, OBJ obj);

vs.

::

  Simulator::ScheduleWithContext (uint32_t context, Time const &time, MEM mem_ptr, OBJ obj);

Readers who invest time and effort in developing or using a non-trivial
simulation model will know the value of the ns-3 logging framework to
debug simple and complex simulations alike. One of the important
features that is provided by this logging framework is the automatic
display of the network node id associated with the 'currently' running
event.

The node id of the currently executing network node is in fact tracked
by the Simulator class. It can be accessed with the
Simulator::GetContext method which returns the 'context' (a 32-bit
integer) associated and stored in the currently-executing event. In some
rare cases, when an event is not associated with a specific network
node, its 'context' is set to 0xffffffff.

To associate a context to each event, the Schedule, and ScheduleNow
methods automatically reuse the context of the currently-executing event
as the context of the event scheduled for execution later. 

In some cases, most notably when simulating the transmission of a packet
from a node to another, this behavior is undesirable since the expected
context of the reception event is that of the receiving node, not the
sending node. To avoid this problem, the Simulator class provides a
specific schedule method: ScheduleWithContext which allows one to
provide explicitly the node id of the receiving node associated with
the receive event.

*XXX: code example*

In some very rare cases, developers might need to modify or understand
how the context (node id) of the first event is set to that of its
associated node. This is accomplished by the NodeList class: whenever a
new node is created, the NodeList class uses ScheduleWithContext to
schedule a 'initialize' event for this node. The 'initialize' event thus executes
with a context set to that of the node id and can use the normal variety
of Schedule methods. It invokes the Node::Initialize method which propagates
the 'initialize' event by calling the DoInitialize method for each object
associated with the node. The DoInitialize method overridden in some of these
objects (most notably in the Application base class) will schedule some
events (most notably Application::StartApplication) which will in turn
scheduling traffic generation events which will in turn schedule
network-level events.

Notes:

* Users need to be careful to propagate DoInitialize methods across objects
  by calling Initialize explicitly on their member objects
* The context id associated with each ScheduleWithContext method has
  other uses beyond logging: it is used by an experimental branch of ns-3
  to perform parallel simulation on multicore systems using
  multithreading.

The Simulator::* functions do not know what the context is: they
merely make sure that whatever context you specify with
ScheduleWithContext is available when the corresponding event executes
with ::GetContext.

It is up to the models implemented on top of Simulator::* to interpret
the context value. In ns-3, the network models interpret the context
as the node id of the node which generated an event. This is why it is
important to call ScheduleWithContext in ns3::Channel subclasses
because we are generating an event from node i to node j and we want
to make sure that the event which will run on node j has the right
context.

Time
****

*To be completed*


Scheduler
*********

*To be completed*


