/**
 * \defgroup tracing Tracing
 *
 * The flexibility of the ns-3 tracing system comes at the cost of quite
 * a bit of complexity so, before trying to use the low-level aspects
 * of the tracing API, it is important to focus on some basic definitions:
 *
 * - A trace source is an object instance which can report trace events
 *   to a set of listening trace sinks.
 *
 * - A trace sink is a user-provided callback (a function) which can
 *   be connected to a set of trace sources to receive the events generated
 *   by each trace source.
 *
 * - A trace resolver is an object which allows users to establish 
 *   connections between a set of trace sources and a set of trace sinks.
 *
 * So, what does it look like in practice ? First, let's look at trace
 * sources. We have two types of trace sources: numeric, and, normal 
 * trace sources. Numeric trace sources behave as normal c++ integers 
 * or c++ floating point numbers except that they report as trace events 
 * each change of their value. For example:
 * \code
 * class MyModel 
 * {
 * public:
 *   void DoSomething (void) 
 *   {
 *     // use the "int" trace source just 
 *     // like any other "int" variable.
 *     m_cwnd *= 2;
 *     m_cwnd += 4;
 *     if (m_cwnd > 100)
 *       {
 *         // do something.
 *       }
 *   }
 * private:
 *   // declare an instance of a "int" trace source
 *   SVTraceSource<int> m_cwnd;
 * };
 * \endcode
 * Normal trace sources, on the other hand, allow you to trace the
 * call of arbitrary functions and methods, as shown below. They are
 * typically used to track "rx", "tx", or "drop" events but could
 * also be used to track route change events, or position change
 * events:
 * \code
 * class MyModel 
 * {
 * public:
 *   void DoSomething (Packet packet, double value) 
 *   {
 *     m_doSomething (packet, value);
 *     // do something
 *   }
 * private:
 *   // report every "something" function call.
 *   CallbackTraceSource<Packet,double> m_doSomething;
 * };
 * \endcode
 * Every type of trace source derives from the ns3::TraceSource base class.
 * As of today, the set of concrete subclasses is relatively short:
 * ns3::CallbackTraceSource, ns3::SvTraceSource, ns3::UvTraceSource, and,
 * ns3::FvTraceSource.
 *
 * To receive these trace events, a user should specify a set of trace sinks.
 * For example, to receive the "int" and the "something" events shown in the
 * examples above, a user would declare the following functions:
 * \code
 * // oldValue and newValue contain the previous and new values of 
 * // the connected SVTraceSource<int> trace source.
 * void 
 * CwndTraceSink (const TraceContext &context, int64_t oldValue, int64_t newValue)
 * {
 *   // for example, print the new value:
 *   std::cout << "cwnd=" << newValue << std::endl;
 * }
 * void 
 * DoSomethingTraceSink (const TraceContext &context, Packet packet, double value)
 * {
 *   // for example, print the arguments
 *   std::cout << "value=" << value << ", packet " << packet << std::endl;
 * }
 * \endcode
 * Each of these sink function takes, as a first argument, a reference to a 
 * const TraceContext object. This context object contains information which
 * describes the instance of the connected trace source: that information is
 * setup during the connection process and does not change afterwards
 * The type and the number of the other arguments to each trace sink depends
 * on the type of the connected trace source: it conveys per-event information
 * from the trace source to the trace sink. For example, UVTraceSource and 
 * SVTraceSource trace sources require two extra arguments. The former requires
 * two unsigned 64 bit integers while the latter requires two signed 64 bit 
 * integers. More generally, users can consult the \ref trace-source-list
 * to figure out the arguments which a trace sink is required to receive
 * for each trace source.
 *
 * The hard part of this tracing framework is the "connection" step: there is a point
 * in the simulation scenario where the user is expected to specify which trace sources
 * should be connected to which trace sinks. There are many ways to do this: the
 * users who want to could implement the "quick and dirty" way, that is, they could
 * write adhoc code to connect their trace sinks to the trace sources using the 
 * TraceSource::AddCallback method. For example, they could patch their models to
 * the following:
 * \code
 * class MyModel 
 * {
 * public:
 *   void DoSomething (void) 
 *   {
 *     // ...
 *   }
 *   SVTraceSource<int> *GetCwndTraceSource (void) const
 *   {
 *     return &m_cwnd;
 *   }
 * private:
 *   // declare an instance of a "int" trace source
 *   SVTraceSource<int> m_cwnd;
 * };
 * \endcode
 * And, then, call directly the GetCwndTraceSource method:
 * \code
 * CwndTraceSink (const TraceContext &context, int64_t oldValue, int64_t newValue)
 * {
 *   // for example, print the new value:
 *   std::cout << "cwnd=" << newValue << std::endl;
 * }
 * // create a model instance
 * MyModel *model = ...;
 * SVTraceSource<int> *cwnd = model->GetCwndTraceSource ();
 * // connect the trace sink to the cwnd trace source of
 * // this model instance.
 * cwnd->AddCallback (MakeCallback (&CwndTraceSink), 
 *                    TraceContext ());
 * \endcode
 * 
 * The solution described above is simple to implement for a model
 * author but it is hard to extend and becomes quickly cumbersome
 * to use with complex models made of composite objects. TraceResolvers
 * deal with these problems and offer a simple API to connect large
 * sets of trace sources to a single sink (as is typical for simulations
 * where users want to receive the "ipv4 rx" events from all nodes).
 *
 * The user-visible API to connect and disconnect trace sources to
 * and from trace sinks is quite small: ns3::Object::Connect
 * and ns3::Object::Disconnect both take a "namespace" regexp string
 * and a callback. The input callback is connected to each trace source
 * which matches the namespace regexp string. The format of the namespace
 * string depends on the set of models implemented by the simulator. 
 * The following diagram shows a small part of the namespace exported
 * by the current version of the simulator:
 *
 * \image html namespace-2.png ns-3 namespace
 * 
 * In this namespace, the "rx" trace source of the PointToPointNetdevice 
 * index 0 within node index 3 is uniquely identified by the namespace
 * string "/nodes/3/devices/0/rx". It is also possible to match all
 * such "rx" trace sources with a single namespace string using 
 * a limited form of regular expressions: "/nodes/3/devices/* /rx" 
 * identifies the "rx" trace source within all NetDevices within node
 * index 3. Similarly, "/nodes/* /devices/* /rx" identifies the "rx"
 * trace source within all NetDevices within all nodes. It is thus
 * possible to connect a single trace sink to a set of matching trace
 * sources in a single operation:
 * \code
 * void DeviceRxSink (const TraceContext &context, const Packet &packet)
 * {
 *   // context contains enough information to uniquely identify
 *   // the trace source instance.
 *   std::cout << "context: \"" << context << "\"";
 *   // packet is the per-event information conveyed from the
 *   // trace source to the trace sink.
 *   std:: cout << " packet: " << packet << std::endl;
 * }
 * NodeList::Connect ("/nodes/* /devices/* /rx", MakeCallback (&DeviceRxSink));
 * \endcode
 * Which, at runtime, is going to generate output looking like this:
 * \code
 * context: "nodeid=2 device=0 dev-rx" packet: IPV4(tos 0x0 ttl 64 id 0 offset ...
 * context: "nodeid=1 device=0 dev-rx" packet: IPV4(tos 0x0 ttl 64 id 0 offset ...
 * ...
 * \endcode
 * In the example above, we see that the ns3::TraceContext contains three 
 * ns3::TraceContextElement which were printed using a space separator:
 *  - nodeid=i
 *  - device=j
 *  - dev-rx
 *
 * Of course, the user could also extract each of these elements from
 * the context to generate a different output:
 * \code
 * void DeviceRxSink (const TraceContext &context, const Packet &packet)
 * {
 *   NodeListIndex nodeIndex;
 *   NodeNetDeviceIndex deviceIndex;
 *   context.Get (nodeIndex);
 *   context.Get (deviceIndex);
 *   std::cout << "node-index=" << nodeIndex.Get ();
 *   std::cout << ", device-index=" << deviceIndex.Get ();
 *   std::cout << ", packet: " << packet;
 *   std::cout << std::endl;
 * }
 * NodeList::Connect ("/nodes/* /devices/* /rx", MakeCallback (&DeviceRxSink));
 * \endcode
 * Extracting TraceContextElement objects from a TraceContext in this manner
 * raises a few obvious questions:
 *   - how do I know which trace context elements are present in a given
 *     TraceContext ?
 *   - how are these elements added to the TraceContext ?
 *
 * 
 *
 * 
 * Connecting trace sinks to a set of existing trace sources is nice but
 * model developers also often need to be able to create new trace sources
 * and hook them into the namespace resolution system. Creating new trace
 * sources is pretty easy: it is a matter of instantiating a proper
 * subclass of the ns3::TraceSource base class. However, hooking each
 * new trace source in the overall namespace resolution system requires
 * some new effort. The core requirement is that the user needs to
 * subclass from the ns3::Object base class which provides the most
 * basic ns3::Object::Connect, and, ns3::Object::Disconnect methods.
 * These two methods are simple forwarding methods to the virtual 
 * ns3::Object::GetTraceResolver method which does the bulk of the work
 * required to setup properly trace sources.
 *
 * Every subclass of the ns3::Object base class which wishes to export
 * a set of trace sources and make them available through the Connect/Disconnect
 * functions needs to override the ns3::Object::GetTraceResolver method.
 * This method needs to return a subclass of the ns3::TraceResolver
 * base class which implements the ns3::TraceResolver::Connect and
 * ns3::TraceResolver::Disconnect methods. Providing an implementation
 * of these two methods is a bit complicated so, a default implementation
 * named CompositeTraceResolver is provided:
 * \code
 * class MyModel : public Object
 * {
 * public:
 *   void DoSomething (void) 
 *   {
 *     // change value of m_cwnd
 *   }
 * protected:
 *   virtual Ptr<TraceResolver> GetTraceResolver (void)
 *   {
 *     // create the composite resolver
 *     Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
 *     resolver->AddSource ("cwnd", m_cwnd);
 *     resolver->AddSource ("rx", m_rx);
 *     return resolver;
 *   }
 * private:
 *   SVTraceSource<int> m_cwnd;
 *   CallbackTraceSource<Packet> m_rx;
 * };
 * void MyTraceSink (const TraceContext &context, Packet packet)
 * {
 *   std::cout << context << " packet: " << packet << std::endl;
 * }
 * object->Connect ("/.../rx", MakeCallback (&MyTraceSink));
 * \endcode
 * 
 * The above example is enough to export a trace source as a member of the
 * tracing namespace so, it would be enough to allow a user to perform a
 * pair of Connect/Disconnect operations but it would not be enough to allow
 * a TraceContext to contain information about these trace sources. Specifically, 
 * printing the content of the TraceContext as shown above would give no 
 * information whatsoever about the type of trace source which was connected.
 *
 * but it is not enough to allow the TraceContext
 * stored in each TraceSource to store information about these trace sources.
 *
 * - A trace source: a trace source is an object instance which is a 
 *   subclass of the ns3::TraceSource base class. Each instance
 *   of a trace source should be used to report a single type of
 *   event. For example, if you want to report ipv4 rx and tx events, 
 *   you should use two different trace source instances.
 *
 *   - Trace sinks: a trace sink is a callback, that is, a function,
 *     which is provided by the user of a model to receive the events
 *     reported by a set of trace sources within that model. A trace 
 *     sink is said to be "connected" once it has been associated
 *     to a set of trace sources.
 *
 *   - Trace context: each trace source instance is associated with a single
 *     instance of an immutable trace context. Each ns3::TraceContext stores 
 *     a set of trace context element instances, each of which is a subclass 
 *     of the ns3::TraceContextElement base class. Whenever a trace sink
 *     provided by a user is called because a trace event was reported on
 *     a connected trace source, the trace context associated to the
 *     relevant trace source is passed as an extra argument to the user's
 *     trace sink.
 *
 *   - instrumentation of models is done through a set of trace source 
 *     instances, each of which is a subclass of the ns3::TraceSource
 *     base class.
 *
 *   
 */
