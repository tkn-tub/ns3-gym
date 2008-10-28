/*!
  \page application How to create a traffic generator ?
  \anchor howtos-application

  <b>Question:</b> How do I create a new traffic generator ?

  <b>Answer:</b> It is possible to instanciate \ref ns3::Packet "Packet"
  objects, schedule events, and call functions from any piece of code
  in ns-3 so, technically, there is no single answer to the question of
  how we can write a new traffic generator. However, the 
  \ref ns3::Socket "Socket" API was really designed to be the single 
  point of entry for traffic generators or traffic analysers and the
  \ref ns3::Application "Application" class was designed to hold
  together any number of sockets.

  Implementing a new traffic generator thus boils down to:
- implementing a new subclass of the \ref ns3::Application "Application"
  base class
- instanciate one or many sockets within that application
- start scheduling events when \ref ns3::Application::StartApplication "StartApplication"
  is called
- stop scheduling events when \ref ns3::Application::StopApplication "StopApplication" 
  is called
- create packets and send them over one or many sockets in each event

The following "random" generator generates packets separated by a random
delay and with a random size.

\code
class RandomGenerator : public Application
{
public:
  RandomGenerator ();
  void SetDelay (RandomVariable delay);
  void SetSize (RandomVariable size);
  void SetRemote (std::string socketType, 
                  Address remote);
private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);
  void DoGenerate (void);

  RandomVariable m_delay;
  RandomVariable m_size;
  Ptr<Socket> m_socket;
};
\endcode

The socket is created in the SetRemote method:
\code
void 
RandomGenerator::SetRemote (std::string socketType, 
                            Address remote)
{
  TypeId tid = TypeId::LookupByName (socketType);
  m_socket = Socket::CreateSocket (GetNode (), tid);
  m_socket->Bind ();
  m_socket->ShutdownRecv ();
  m_socket->Connect (remote);
}
\endcode
While the the crux of the logic is located in the DoGenerate method
which is called from within StartApplication:
\code
void
RandomGenerator::DoGenerate (void)
{
  m_next = Simulator::Schedule (Seconds (m_delay.GetValue ()), 
				&RandomGenerator::DoGenerate, this);
  Ptr<Packet> p = Create<Packet> (m_size.GetIntValue ());
  m_socket->Send (p);
}
\endcode

To make that application more integrated in ns-3, it needs an associated 
helper class:
\code
class RandomAppHelper
{
public:
  RandomAppHelper (std::string protocol, Address remote);
  void SetPacketSize (RandomVariable packetSize);
  void SetDelay (RandomVariable delay);
  ApplicationContainer Install (NodeContainer nodes);
private:
  std::string m_protocol;
  Address m_remote;
  RandomVariable m_packetSize;
  RandomVariable m_delay;
};
\endcode

which could be trivially implemented as:
\code
ApplicationContainer 
RandomAppHelper::Install (NodeContainer nodes)
{
  ApplicationContainer applications;
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i)
    {
      Ptr<RandomAppHelper> app = CreateObject<RandomAppHelper> ();
      app->SetSize (m_packetSize);
      app->SetDelay (m_delay);
      app->SetRemote (m_protocol, m_remote);
      (*i)->AddApplication (app);
      applications.Add (app);
    }
  return applications;
}
\endcode

Despite being functional, this API is not very consistant with the style of 
the other helper classes, all of which allow you to control the parameters
of the underlying class through attributes and not explicit setters. The
following API thus replaces the pair SetPacketSize/SetDelay with a single
method SetAttribute:
\code
class RandomAppHelper
{
public:
  RandomAppHelper (std::string protocol, Address remote);
  void SetAttribute (std::string name, const AttributeValue &value);
  ApplicationContainer Install (NodeContainer c);
private:
  std::string m_protocol;
  Address m_remote;
  ObjectFactory m_factory;
};
\endcode

And which can be used as follows:
\code
RandomAppHelper app = RandomAppHelper ("ns3::TcpSocketFactory", 
				       InetSocketAddress (Ipv4Address ("192.168.1.10"), 10));
app.SetAttribute ("Delay", StringValue ("Constant:2.5"));
app.SetAttribute ("Size", StringValue ("Constant:2100"));
app.Install (nodes);
\endcode

The implementation, in this case, is not necessarily longer but its 
simplicity hides a lot of behind-the-scenes complexity:

\code
void 
RandomAppHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}
ApplicationContainer 
RandomAppHelper::Install (NodeContainer nodes)
{
  ApplicationContainer applications;
  for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i)
    {
      Ptr<RandomAppHelper> app = m_factory.Create<RandomAppHelper> ();
      app->SetRemote (m_socketType, m_remote);
      (*i)->AddApplication (app);
      applications.Add (app);
    }
  return applications;
}
\endcode

The key difference between this implementation and the previous one
is that this helper does not handle explicitely the attributes
delay and packet size. Instead, it stores them in an 
\ref ns3::ObjectFactory "ObjectFactory" object. This, of course,
does not work magically, and requires extra support from the
underlying RandomGenerator class. Specifically, it requires
that the underlying RandomGenerator defines its attributes
in its \ref ns3::TypeId "TypeId" in a new public static method:

\code
class RandomGenerator
{
public:
  static TypeId GetTypeId (void);
};
\endcode

The corresponding implementation is shown below:

\code
TypeId
RandomGenerator::GetTypeId (void)
{
  static TypeId tid = TypeId ("RandomGenerator")
    .SetParent<Application> ()
    .AddConstructor<RandomGenerator> ()
    .AddAttribute ("Delay", "The delay between two packets (s)",
		   RandomVariableValue (ConstantVariable (1.0)),
		   MakeRandomVariableAccessor (&RandomGenerator::m_delay),
		   MakeRandomVariableChecker ())
    .AddAttribute ("PacketSize", "The size of each packet (bytes)",
		   RandomVariableValue (ConstantVariable (2000)),
		   MakeRandomVariableAccessor (&RandomGenerator::m_size),
		   MakeRandomVariableChecker ())
    ;
  return tid;
}   
\endcode

*/
