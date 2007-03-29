/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef TRACE_ROOT_H
#define TRACE_ROOT_H

#include <string>
#include "ns3/callback.h"

/**
 * \defgroup lowleveltracing Low-level tracing
 *
 * This low-level API is built around a few concepts:
 *   - There can be any number of trace source objects. Each trace source
 *     object can generate any number of trace events. The current
 *     trace source objects are: ns3::CallbackTraceSourceSource, ns3::UVTraceSource,
 *     ns3::SVTraceSource, and, ns3::FVTraceSource.
 *   - Each trace source can be connected to any number of trace sinks.
 *     A trace sink is a ns3::Callback with a very special signature. Its
 *     first argument is always a ns3::TraceContext.
 *   - Every trace source is uniquely identified by a ns3::TraceContext. Every
 *     trace sink can query a ns3::TraceContext for information. This allows
 *     a trace sink which is connected to multiple trace sources to identify
 *     from which source each event is coming from.
 *
 * To define new trace sources, a model author needs to instante one trace source
 * object for each kind of tracing event he wants to export. The trace source objects
 * currently defined are:
 *  - ns3::CallbackTraceSourceSource: this trace source can be used to convey any kind of 
 *    trace event to the user. It is a functor, that is, it is a variable
 *    which behaves like a function which will forward every event to every
 *    connected trace sink (i.e., ns3::Callback). This trace source takes
 *    up to four arguments and forwards these 4 arguments together with the
 *    ns3::TraceContext which identifies this trace source to the connected
 *    trace sinks.
 *  - ns3::UVTraceSource: this trace source is used to convey key state variable
 *    changes to the user. It behaves like a normal integer unsigned variable:
 *    you can apply every normal arithmetic operator to it. It will forward
 *    every change in the value of the variable back to every connected trace 
 *    sink by providing a TraceContext, the old value and the new value.
 *  - ns3::SVTraceSource: this is the signed integer equivalent of 
 *    ns3::UVTraceSource.
 *  - ns3::FVTraceSource: this is the floating point equivalent of 
 *    ns3::UVTraceSource and ns3::SVTraceSource.
 *
 * For example, to define a trace source which notifies you of a new packet
 * being transmitted, you would have to:
 * \code
 * class MyModel
 * {
 *  public:
 *   void Tx (Packet const &p);
 *  private:
 *   CallbackTraceSource<Packet const &> m_txTrace;
 * };
 *
 * void
 * MyModel::Tx (Packet const &p)
 * {
 *   // trace packet tx event.
 *   m_txTrace (p);
 *   // ... send the packet for real.
 * }
 * \endcode
 *
 * Once the model author has instantiated these objects and has wired them 
 * in his simulation code (that is, he calls them wherever he wants to trigger 
 * a trace event), he needs to make these trace sources available to users
 * to allow them to connect any number of trace sources to any number
 * of user trace sinks. While it would be possible to make each model
 * export directly each of his trace source instances and request users to
 * invoke a source->Connect (callback) method to perform the connection
 * explicitely, it was felt that this was a bit cumbersome to do.
 *
 * As such, the ``connection'' between a set of sources and a sink is 
 * performed through a third-party class, the TraceResolver, which
 * can be used to automate the connection of multiple matching trace sources
 * to a single sink. This TraceResolver works by defining a hierarchical 
 * tracing namespace: the root of this namespace is accessed through the 
 * ns3::TraceRoot class. The namespace is represented as a string made of 
 * multiple elements, each of which is separated from the other elements 
 * by the '/' character. A namespace string always starts with a '/'.
 * 
 * By default, the current simulation models provide a '/nodes' tracing root. 
 * This '/nodes' namespace is structured as follows:
 * \code
 *  /nodes/n/arp
 *  /nodes/n/udp
 *  /nodes/n/ipv4
 *               /tx
 *               /rx
 *               /drop
 *               /interfaces/n/netdevice
 *                                      /queue/
 *                                            /enque
 *                                            /deque
 *                                            /drop
 * \endcode
 *
 * The 'n' element which follows the /nodes and /interfaces namespace elements
 * identify a specific node and interface through their index within the 
 * ns3::NodeList and ns3::Ipv4 objects respectively.
 *
 * To connect a trace sink to a trace source identified by a namespace string,
 * a user can call the ns3::TraceRoot::Connect method (the ns3::TraceRoot::Disconnect
 * method does the symmetric operation). This connection method can accept
 * fully-detailed namespace strings but it can also perform pattern matching
 * on the user-provided namespace strings to connect multiple trace sources
 * to a single trace sink in a single connection operation.
 *
 * The syntax of the pattern matching rules are loosely based on regular 
 * expressions:
 *   - the '*' character matches every element
 *   - the (a|b) construct matches element 'a' or 'b'
 *   - the [ss-ee] construct matches all numerical values which belong
 *     to the interval which includes ss and ee
 *
 * For example, the user could use the following to connect a single sink
 * to the ipv4 tx, rx, and drop trace events:
 *
 * \code
 * void MyTraceSink (TraceContext const &context, Packet &packet);
 * TraceRoot::Connect ("/nodes/ * /ipv4/ *", MakeCallback (&MyTraceSink));
 * \endcode
 *
 * Of course, this code would work only if the signature of the trace sink
 * is exactly equal to the signature of all the trace sources which match
 * the namespace string (if one of the matching trace source does not match
 * exactly, a fatal error will be triggered at runtime during the connection
 * process). The ns3::TraceContext extra argument contains
 * information on where the trace source is located in the namespace tree.
 * In that example, if there are multiple nodes in this scenario, each
 * call to the MyTraceSink function would receive a different TraceContext,
 * each of which would contain a different NodeList::Index object.
 *
 * It is important to understand exactly what an ns3::TraceContext
 * is. It is a container for a number of type instances. Each instance of
 * a ns3::TraceContext contains one and only one instance of a given type.
 * ns3::TraceContext::Add can be called to add a type instance into a 
 * TraceContext instance and ns3::TraceContext::Get can be called to get
 * a copy of a type instance stored into the ns3::TraceContext. If ::Get
 * cannot retrieve the requested type, a fatal error is triggered at
 * runtime. The values stored into an ns3::TraceContext attached to a 
 * trace source are automatically determined during the namespace
 * resolution process. To retrieve a value from a ns3::TraceContext, the
 * code can be as simple as this:
 * \code
 * void MyTraceSink (TraceContext const &context, Packet &packet)
 * {
 *   NodeList::Index index;
 *   context.Get (index);
 *   std::cout << "node id=" << NodeList::GetNode (index)->GetId () << std::endl;
 * }
 * \endcode
 *
 * The hierarchical global namespace described here is not implemented
 * in a single central location: it was felt that doing this would make
 * it too hard to introduce user-specific models which could hook
 * automatically into the overal tracing system. If the tracing
 * namespace was implemented in a single central location, every model
 * author would have had to modify this central component to make
 * his own model available to trace users.
 *
 * Instead, the handling of the namespace is distributed across every relevant
 * model: every model implements only the part of the namespace it is
 * really responsible for. To do this, every model is expected
 * to provide an instance of a TraceResolver whose
 * responsability is to recursively provide access to the trace sources
 * defined in its model. Each TraceResolver instance should be a subclass
 * of the TraceResolver base class which implements either the DoLookup
 * or the DoConnect and DoDisconnect methods. Because implementing these
 * methods can be a bit tedious, our tracing framework provides a number 
 * of helper template classes which should save the model author from 
 * having to implement his own in most cases:
 *    - ns3::CompositeTraceResolver: this subclass of ns3::TraceResolver can 
 *      be used to aggregate together multiple trace sources and multiple other 
 *      ns3::TraceResolver instances.
 *    - ns3::ArrayTraceResolver: this subclass of ns3::TraceResolver can be 
 *      used to match any number of elements within an array where every element 
 *      is identified by its index.
 *
 * Once you can instantiate your own ns3::TraceResolver object instance, you 
 * have to hook it up into the global namespace. There are two ways to do this:
 *   - you can hook your ns3::TraceResolver creation method as a new trace 
 *     root by using the ns3::TraceRoot::Register method
 *   - you can hook your new ns3::TraceResolver creation method into the 
 *     container of your model. This step will obvsiouly depend on which model
 *     contains your own model but, if you wrote a new l3 protocol, all you
 *     would have to do to hook into your container L3Demux class is to implement 
 *     the pure virtual method inherited from the L3Protocol class whose name is 
 *     ns3::L3protocol::CreateTraceResolver.
 *
 * So, in most cases, exporting a model's trace sources is a matter of 
 * implementing a method CreateTraceResolver as shown below:
 * \code
 * class MyModel
 * {
 * public:
 *   enum TraceType {
 *    TX,
 *    RX,
 *    ...
 *   };
 *   TraceResolver *CreateTraceResolver (TraceContext const &context);
 *   void Tx (Packet const &p);
 * private:
 *   CallbackTraceSource<Packet const &> m_txTrace;
 * };
 *
 * TraceResolver *
 * MyModel::CreateTraceResolver (TraceContext const &context)
 * {
 *   CompositeTraceResolver *resolver = new CompositeTraceResolver (context);
 *   resolver->Add ("tx", m_txTrace, MyModel::TX);
 *   return resolver;
 * }
 * void 
 * MyModel::Tx (Packet const &p)
 * {
 *   m_txTrace (p);
 * }
 * \endcode
 *
 * If you really want to have fun and implement your own ns3::TraceResolver 
 * subclass, you need to understand the basic Connection and Disconnection
 * algorithm. The code of that algorithm is wholy contained in the
 * ns3::TraceResolver::Connect and ns3::TraceResolver::Disconnect methods.
 * The idea is that we recursively parse the input namespace string by removing
 * the first namespace element. This element is 'resolved' is calling
 * the ns3::TraceResolver::DoLookup method which returns a list of
 * TraceResolver instances. Each of the returned TraceResolver instance is
 * then given what is left of the namespace by calling ns3::TraceResolver::Connect
 * until the last namespace element is processed. At this point, we invoke
 * the ns3::TraceResolver::DoConnect or ns3::TraceResolver::DoDisconnect 
 * methods to break the recursion. A good way to understand this algorithm
 * is to trace its behavior. Let's say that you want to connect to
 * '/nodes/ * /ipv4/interfaces/ * /netdevice/queue/ *'. It would generate
 * the following call traces:
 *
 * \code
 * TraceRoot::Connect ("/nodes/ * /ipv4/interfaces/ * /netdevice/queue/ *", callback);
 * traceContext = TraceContext ();
 * rootResolver = CompositeTraceResolver (traceContext);
 * rootResolver->Connect ("/nodes/ * /ipv4/interfaces/ * /netdevice/queue/ *", callback);
 *   resolver = CompositeTraceResolver::DoLookup ("nodes");
 *     return NodeList::CreateTraceResolver (GetContext ());
 *       return ArrayTraceResolver (context);
 *   resolver->Connect ("/ * /ipv4/interfaces/ * /netdevice/queue/ *", callback);
 *     ArrayTraceResolver::DoLookup ("*");
 *       for (i = 0; i < n_nodes; i++)
 *         resolver = nodes[i]->CreateTraceResolver (GetContext ());
 *           return CompositeTraceResolver (context);
 *         resolvers.add (resolver);
 *       return resolvers;
 *     for resolver in (resolvers)
 *       resolver->Connect ("/ipv4/interfaces/ * /netdevice/queue/ *", callback);
 *         CompositeTraceResolver::DoLookup ("ipv4");
 *           resolver = ipv4->CreateTraceResolver (GetContext ());
 *             return CompositeTraceResolver (context);
 *           return resolver;
 *         resolver->Connect ("/interfaces/ * /netdevice/queue/ *", callback);
 *           CompositeTraceResolver::DoLookup ("interfaces");
 *             resolver = ArrayTraceResolver (GetContext ());
 *           resolver->Connect ("/ * /netdevice/queue/ *", callback);
 *             ArrayTraceResolver::DoLookup ("*");
 *               for (i = 0; i < n_interfaces; i++)
 *                  resolver = interfaces[i]->CreateTraceResolver (GetContext ());
 *                    return CompositeTraceResolver ()
 *                  resolvers.add (resolver);
 *               return resolvers;
 *             resolver->Connect ("/netdevice/queue/ *", callback);
 *               CompositeTraceResolver::DoLookup ("netdevice");
 *                 resolver = NetDevice::CreateTraceResolver (GetContext ());
 *                   return CompositeTraceResolver ();
 *                 return resolver;
 *               resolver->Connect ("/queue/ *", callback);
 *                 CompositeTraceResolver::DoLookup ("queue");
 *                   resolver = Queue::CreateTraceResolver (GetContext ());
 *                     return CompositeTraceResolver ();
 *                   return resolver
 *                 resolver->Connect ("*", callback);
 *                   CompositeTraceResolver::DoLookup ("*");
 *                     for match in (matches)
 *                       resolver = TerminalTraceResolver ("match");
 *                       resolvers.add (resolver)
 *                     return resolvers;
 *                   for resolver in (resolvers)
 *                     TerminalTraceResolver->DoConnect (callback);
 * \endcode
 */

namespace ns3 {

class CompositeTraceResolver;
class TraceResolver;
class TraceContext;
class CallbackBase;

/**
 * \brief The main class used to access tracing functionality for
 * a user.
 *
 * \ingroup lowleveltracing
 */
class TraceRoot
{
public:
  static void Connect (std::string path, CallbackBase const &cb);
  static void Disconnect (std::string path, CallbackBase const &cb);
  static void Register (std::string name, 
                        Callback<TraceResolver *,TraceContext const &> createResolver);
private:
  static CompositeTraceResolver *GetComposite (void);
  enum TraceType {
    NOTHING,
  };
};

}// namespace ns3

#endif /* TRACE_ROOT_H */
