/**
 * \ingroup core
 * \defgroup TraceSourceList List of trace sources
 */

/**
 * \ingroup core
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
 * \section TraceSource Generating Trace Events
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
 *   void DoSomething (Ptr<Packet> packet) 
 *   {
 *     // report this event on packet
 *     m_doSomething (packet);
 *     // do something
 *   }
 * private:
 *   // report every "something" function call.
 *   CallbackTraceSource<Ptr<Packet> > m_doSomething;
 * };
 * \endcode
 * Every type of trace source derives from the ns3::TraceSource base class.
 * As of today, the set of concrete subclasses is relatively short:
 * ns3::CallbackTraceSource, ns3::SvTraceSource, ns3::UvTraceSource, and,
 * ns3::FvTraceSource.
 *
 * \section TraceSink Receiving Trace Events
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
 * DoSomethingTraceSink (const TraceContext &context, Ptr<Packet> packet)
 * {
 *   // for example, print the packet
 *   std::cout << "packet " << packet->Print () << std::endl;
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
 * integers. More generally, users can consult the \ref TraceSourceList
 * to figure out the arguments which a trace sink is required to receive
 * for each trace source: a signature of the user trace sink must match 
 * _exactly_ the signature documented in the \ref TraceSourceList.
 *
 *
 * \section TraceSourceSimpleExport A simple way to connect Trace Sources with Trace Sinks
 *
 * The crux of the complexity of the ns-3 tracing system comes from its 
 * flexible system used to connect trace sources to trace sinks but what is really
 * nice about it is that it is not necessary to use it to setup simple traces.
 * 
 * The simplest way to export a set of trace sources to a user, for example, 
 * during the early prototyping phases of a system, is to add a set of public methods
 * to give to your users access to the trace source object instances you use to generate
 * trace events:
 * \code
 * class MyModel 
 * {
 * public:
 *   void DoSomething (Ptr<Packet> packet) 
 *   {
 *     // report this event on packet
 *     m_doSomething (packet);
 *     // do something
 *   }
 *   CallbackTraceSource<Ptr<Packet>> *PeekSomethingTraceSource (void) const 
 *   {
 *     return &m_doSomething
 *   }
 * private:
 *   // report every "something" function call.
 *   CallbackTraceSource<Ptr<Packet>> m_doSomething;
 * };
 * \endcode
 * If your users hold a pointer to an instance of MyModel, and if they want to connect
 * a MySomethingSink, they can simply do the following which invokes the 
 * TraceSource::AddCallback method and creates a Callback object from the user's
 * sink with the MakeCallback function.
 * \code
 * void 
 * MySomethingSink (const TraceContext &context, Ptr<Packet> packet)
 * {
 *   // do whatever you want.
 * }
 * MyModel *model = ...;
 * CallbackTraceSource<Ptr<Packet>> *source = model->PeekSomethingTraceSource ();
 * source->AddCallback (MakeCallback (&MySomethingSink));
 * \endcode
 *
 * The full power of the tracing system comes however from its ns3::NodeList::Connect
 * method which is described in the following sections.
 *
 * \section TraceConnection Connecting Trace Sources to Trace Sinks
 * 
 * If a trace source is integrated in the ns-3 trace connection facility, a user 
 * should call the ns3::NodeList::ConnectWithoutContext method to establish a connection between
 * a trace sink and a set of matching trace sources. The second argument to that
 * method is a callback to the user's trace sink.
 * That callback is easy to construct: call ns3::MakeCallback and you are done. The
 * first argument is a string whose format is similar to a unix path and which is 
 * used to uniquely identify the set of trace sources you want to connect to.
 * The set of acceptable path strings is also documented in the \ref TraceSourceList.
 *
 * So, what does this look like from the perspective of a user ? If we wanted to 
 * connect to a trace source defined somewhere deep into the a set of NetDevice objects
 * located in some nodes of the system, we could write the following:
 * \code
 * void 
 * DoSomethingTraceSink (const TraceContext &context, Ptr<Packet> packet)
 * {
 *   // for example, print the packet
 *   std::cout << "packet: " << packet->Print () << std::endl;
 * }
 * // connect the above sink to a matching trace source
 * NodeList::ConnectWithoutContext ("/nodes/* /devices/* /rx", MakeCallback (&DoSomethingTraceSink));
 * \endcode
 *
 * The connection path string "/nodes/* /devices/* /rx" matches the "rx" trace source
 * located in every netdevice located in every node. The syntax of that path string
 * is loosely based on regular expressions so, a user could conceivably connect
 * to the trace sources present in only one node identified by node index:
 * "/nodex/3/devices/* /rx".
 *
 * The matching algorithm used here is very useful since it allows you to connect
 * at once a large set of trace sources to a single sink but it introduces another 
 * problem: it becomes impossible when you receive an event in your trace sink to
 * know from _which_ trace source the event is coming from. In our example, the
 * trace source might be coming from the NetDevice number 2 of Node 10 or Netdevice
 * number 0 of Node 5. In both cases, you might need to know which of these NetDevice
 * is generating this event, if only to generate some ascii trace dump. Another 
 * similar use-case is that you might have connected the same trace sink to
 * multiple types of events which have the same signature: it is quite common
 * to receive all tx, rx, and drop events in the same trace sink and that would be
 * quite trivial to achieve with a string such as: "/nodes/* /devices/* /*"
 *
 * The source of a trace event can be retrieved from a trace sink using 
 * different means: the simplest
 * way to get this information is to use the builtin printing facility of
 * the TraceContext object:
 * \code
 * void 
 * DoSomethingTraceSink (const TraceContext &context, Ptr<Packet> packet)
 * {
 *   // for example, print the packet
 *   std::cout << "context=\"" << context << "\" packet: " << packet->Print () << std::endl;
 * }
 * \endcode
 * The above code is going to generate output which looks like the following:
 * \code
 * context="nodeid=2 device=0 dev-rx" packet: IPV4(tos 0x0 ttl 64 id 0 offset ...
 * context="nodeid=1 device=0 dev-rx" packet: IPV4(tos 0x0 ttl 64 id 0 offset ...
 * ...
 * \endcode
 *
 * Another more advanced way to get information out of a TraceContext is to call its
 * ns3::TraceContext::GetElement method. This method takes as its first and only
 * argument an instance of the object we want to read and the list of available
 * object instances we can read from a TraceContext is documented, once again,
 * in the \ref TraceSourceList. For example, we could write the following to
 * generate adhoc trace output:
 * \code
 * void DeviceRxSink (const TraceContext &context, Ptr<const Packet> packet)
 * {
 *   NodeListIndex nodeIndex;
 *   NodeNetDeviceIndex deviceIndex;
 *   context.GetElement (nodeIndex);
 *   context.GetElement (deviceIndex);
 *   std::cout << "node-index=" << nodeIndex.Get ();
 *   std::cout << ", device-index=" << deviceIndex.Get ();
 *   std::cout << ", packet: " << packet->Print ();
 *   std::cout << std::endl;
 * }
 * \endcode
 *
 * \section ExportingTraceSources Exporting new Trace Sources
 *
 * Using existing trace sources to connect them to a set of adhoc trace sinks
 * is not really complicated but, setting up new trace sources which can hook
 * in this automatic connection system is a bit more complicated.
 *
 * So far, we know that a model author can generate trace events really easily:
 * \code
 * class MyModel 
 * {
 * public:
 *   void DoSomething (Ptr<Packet> packet) 
 *   {
 *     // report this event on packet with value
 *     m_doSomething (packet);
 *     // do something
 *   }
 * private:
 *   // report every "something" function call.
 *   CallbackTraceSource<Ptr<Packet>> m_doSomething;
 * };
 * \endcode
 *
 * To make these new trace sources available to the rest of the connection system,
 * the first step is to make sure that your model object derives from the ns3::Object
 * base class either directly (as shown below) or indirectly through another base class:
 * \code
 * class MyModel : public Object {...};
 * // or:
 * class SomeOtherObject : public Object {...};
 * class MyModel : public SomeOtherObject {...};
 * \endcode
 *
 * This is pretty trivial and lays the ground for the second step: overriding the
 * ns3::Object::GetTraceResolver method:
 * \code
 * class MyModel : public MyParent
 * {
 * public:
 *   // declare overriden method
 *   virtual Ptr<TraceResolver> GetTraceResolver (void) const;
 * private:
 *   // the new trace source to export.
 *   CallbackTraceSource<Ptr<Packet>> m_rxSource;
 * };
 * \endcode
 *
 * To implement this method, you could attempt to implement a new subclass of
 * the ns3::TraceResolver base class and return an instance from this method but
 * this would be very hard. Instead, you should use the helper class
 * ns3::CompositeTraceResolver to register your trace sources and chain up to
 * your parent:
 * \code
 * Ptr<TraceResolver>
 * MyModel::GetTraceResolver (void) const
 * {
 *   // create an empty trace resolver
 *   Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
 *   // register m_rxSource
 *   resolver->AddSource ("rx", // the name of the trace source in the path string
 *                        TraceDoc ("some help text to explain the purpose of this trace source",
 *                                  "Packet", // the type of the first argument to the trace source
 *                                  "the purpose of the first argument",
 *                                  "type-of-second-argument", "purpose-of-second-argument"),
 *                        m_rxSource // the trace source itself is registered
 *                       );
 *   // make sure we include the trace sources implemented in the parent.
 *   resolver->SetParentResolver (MyParent::GetTraceResolver ());
 *   return resolver;
 * }
 * \endcode
 *
 * Once you have written that code, you must make sure that this new method GetTraceResolver
 * is going to be called at some point by the tracing system. If your model is located somewhere
 * deep in MAC or PHY layer, that is, it is part of a NetDevice implementation, all you
 * have to do is to make sure that your model is registered as a "composite" of your NetDevice
 * subclass:
 * \code
 * class MyNetDevice : public NetDevice
 * {
 * public:
 *   Ptr<TraceResolver> GetTraceResolver (void) const;
 * private:
 *   Ptr<MyModel> m_model;
 * };
 * 
 * Ptr<TraceResolver>
 * MyNetDevice::GetTraceResolver (void) const
 * {
 *   Ptr<CompositeTraceResolver> resolver = ...;
 *   // register other trace source
 *   ...
 *   // register now your model as a "composite"
 *   resolver->AddComposite ("my-model", m_model);
 *   // chain up to parent.
 *   resolver->SetParentResolver (NetDevice::GetTraceResolver ());
 *   return resolver;
 * }
 * \endcode
 * 
 * The code above will make your "rx" trace source appear under the
 * /nodes/xx/devices/xx/my-model/rx namespace path.
 *
 * If you have implemented a new layer 3 or 4 protocol object, the process to
 * export your trace sources is quite similar. You need to subclass from
 * ns3::Object, override the ns3::Object::GetTraceResolver method, make
 * sure you chain up to your parent's GetTraceResolver method, and, finally,
 * make sure that someone calls your new GetTraceResolver method. How to accomplish
 * the latter should be documented in the node's API documentation which describes
 * how to implement a new layer 3 or 4 protocol object.
 *
 * \section AdvancedTraceContext Creating new Trace Context Elements
 *
 * The last important feature which model developers need to understand
 * is how to provide extra context information to trace sinks. For example,
 * if your model exports both rx and tx trace sources which share the same 
 * signature, it is quite natural for a user to connect to a single trace sink
 * to both of them with a trace path string such as "/nodes/* /devices/* /(rx|tx)".
 * In this case, it becomes necessary to be able, from the trace sink function,
 * to tell which event triggered the call to the trace sink: a rx or a tx event.
 *
 * That example is detailed below with a TX, a RX, and a DROP source:
 * \code
 * class MyModel
 * {
 * private:
 *   CallbackTraceSource<Ptr<Packet>> m_rxSource;
 *   CallbackTraceSource<Ptr<Packet>> m_txSource;
 *   CallbackTraceSource<Ptr<Packet>> m_dropSource;
 * };
 * \endcode
 * When a single sink is connected to all 3 sources here, one might want
 * to write code like the following:
 * \code
 * void DeviceRxSink (const TraceContext &context, Ptr<const Packet> &packet)
 * {
 *   switch (type) {
 *     case RX:
 *       std::cout << "rx" << std::endl;
 *       break;
 *     case TX:
 *       std::cout << "tx" << std::endl;
 *       break;
 *     case DROP:
 *       std::cout << "drop" << std::endl;
 *       break;
 *   }
 * \endcode
 *
 * \subsection AdvancedTraceContextSimpleSolution The simple solution
 *
 * The simplest way to do achieve the result shown above is to include
 * in the trace source an extra explicit argument which describes the source event:
 *   - define a small enum with 3 values
 *   - change the signature of m_rxSource, m_txSource, and m_dropSource to include
 *     the enum
 *   - pass the enum value in each event
 *
 * The resulting code is shown below:
 * \code
 * class MyModel
 * {
 * public:
 *   // define the trace type enum.
 *   enum TraceType {
 *     RX,
 *     TX,
 *     DROP
 *   };
 * private:
 *   // generate events
 *   void NotifyRxPacket (Ptr<Packet> p) {
 *     m_rxSource (p, MyModel::RX);
 *   }
 *   void NotifyTxPacket (Ptr<Packet> p) {
 *     m_rxSource (p, MyModel::TX);
 *   }
 *   void NotifyDropPacket (Ptr<Packet> p) {
 *     m_rxSource (p, MyModel::DROP);
 *   }
 *   CallbackTraceSource<Ptr<Packet>,enum TraceType> m_rxSource;
 *   CallbackTraceSource<Ptr<Packet>,enum TraceType> m_txSource;
 *   CallbackTraceSource<Ptr<Packet>,enum TraceType> m_dropSource;
 * };
 * \endcode
 * These 3 new sources can be connected easily to a new trace sink:
 * \code
 * void ASimpleTraceSink (const TraceContext &context, Ptr<const Packet> packet, enum MyModel::TraceType type)
 * {
 *   // here, read the "type" argument
 * }
 * \endcode
 *
 * This solution works but it makes it impossible to connect a single trace sink to a set
 * of trace sources which represent "rx" events in different NetDevice objects since
 * each of them will define a different enum type with different values: since the
 * trace sink signature must match exactly the trace source signature, it is impossible
 * to connect at the same time to all "rx" events of different NetDevice.
 *
 * \subsection AdvancedTraceContextFancySolution The more complex and generic solution
 *
 * There is, hopefully, a way to get the best of both worlds, that is, to allow a
 * user to connect to a lot of trace source events of the same kind but coming from different
 * implementations and to allow the user to differentiate between these different
 * implementations.
 *
 * Rather than define an adhoc enum type with a list of trace sources, you can also
 * define a new ns3::TraceContextElement for your source sources. For example, if you
 * define a new MyModelTraceType class which contains the type of trace, your users can
 * then write trace sink code which looks like this:
 * \code
 * void AFancyTraceSink (const TraceContext &context, Ptr<const Packet> packet)
 * {
 *   MyModelTraceType type;
 *   if (context.GetElement (type))
 *     {
 *       switch (type.Get ())
 *         {
 *         case MyModelTraceType::RX:
 *           std::cout << "rx" << std::endl;
 *           break;
 *         case MyModelTraceType::TX:
 *           std::cout << "tx" << std::endl;
 *           break;
 *         case MyModelTraceType::DROP:
 *           std::cout << "drop" << std::endl;
 *           break;
 *         }
 *     }
 * }
 * \endcode
 *
 * Of course, since the type of trace is stored in the TraceContext, your users can
 * also take the shortcut which uses the printing functionality of the TraceContext:
 * \code
 * void ALessFancyTraceSink (const TraceContext &context, Ptr<const Packet> packet)
 * {
 *   std::cout << "context=\"" << context << "\" packet: " << packet->Print () << std::endl;
 * }
 * \endcode
 * which will generate something like the following when the trace source comes
 * from MyModel:
 * \code
 * context="my-model-rx" packet: ...
 * \endcode
 *
 * The first step to achieve this is to define and implement a new
 * subclass of the ns3::TraceContextElement base class. The exact list of
 * public methods which must be implemented is described in the API
 * documentation of the ns3::TraceContextElement class. 
 * \code
 * class MyModelTraceType : public TraceContextElement
 * {
 * public:
 *   enum Type {
 *     RX,
 *     TX,
 *     DROP
 *   };
 *   // called from MyModel::GetTraceResolver
 *   MyModelTraceType (enum Type type);
 *   // needed for by the tracing subsystem.
 *   MyModelTraceType ();
 *   // called from trace sink
 *   enum Type Get (void) const;
 *   // needed by the tracing subsystem
 *   static uint16_t GetUid (void);
 *   // needed by the tracing subsystem to
 *   // print the content of a TraceContext
 *   void Print (std::ostream &os) const;
 *   // needed by the tracing subsystem to
 *   // generate the doxygen documentation.
 *   std::string GetTypeName (void) const;
 * private:
 *   enum Type m_type;
 * };
 * \endcode
 * The implementation does not require much thinking:
 * \code
 * MyModelTraceType::MyModelTraceType ()
 *  : m_type (RX)
 * {// an arbitrary default value.
 * }
 * MyModelTraceType::MyModelTraceType (enum Type type)
 *  : m_type (type)
 * {}
 * enum MyModelTraceType::Type 
 * MyModelTraceType::Get (void) const
 * {
 *   return m_type;
 * }
 * uint16_t 
 * MyModelTraceType::GetUid (void)
 * {
 *   // use protected TraceContextElement::AllocateUid method
 *   // the input string is used to uniquely identify this new subclass
 *   static uint16_t uid = AllocateUid<MyModelTraceType> ("ns3::MyModelTraceType");
 *   return uid;
 * }
 * void 
 * MyModelTraceType::Print (std::ostream &os) const
 * {
 *   // this method is invoked by the print function of a TraceContext
 *   // if it contains an instance of this TraceContextElement.
 *   switch (m_type) {
 *     case RX: os << "rx"; break;
 *     // ...
 *   }
 * }
 * std::string 
 * MyModelTraceType::GetTypeName (void) const
 * {
 *   // This method should return a fully-qualified c++ typename
 *   // This method is used only for documentation purposes to
 *   // generate the content of the Trace Source List.
 *   return "ns3::MyModelTraceType";
 * }
 * \endcode
 *
 * Once this subclass is implemented, the work is almost completed: you
 * just need to pass an instance of that class as the last argument of 
 * the ns3::CompositeTraceResolver::AddSource method as shown below:
 * \code
 * Ptr<TraceResolver>
 * MyModel::GetTraceResolver (void) const
 * {
 *   // create an empty trace resolver
 *   Ptr<CompositeTraceResolver> resolver = Create<CompositeTraceResolver> ();
 *   // register m_rxSource
 *   resolver->AddSource ("rx", // the name of the trace source in the path string
 *                        TraceDoc ("some help text to explain the purpose of this trace source",
 *                                  "Packet", // the type of the first argument to the trace source
 *                                  "the purpose of the first argument",
 *                                  "type-of-second-argument", "purpose-of-second-argument"),
 *                        m_rxSource, // the trace source itself is registered
 *                        // the TraceContextElement associated to this trace source.
 *                        MyModelTraceType (MyModelTraceType::RX) 
 *                       );
 *   // make sure we include the trace sources implemented in the parent.
 *   resolver->SetParentResolver (MyParent::GetTraceResolver ());
 *   return resolver;
 * }
 * \endcode
 */
