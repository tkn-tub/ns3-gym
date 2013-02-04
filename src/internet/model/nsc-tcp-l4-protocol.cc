/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * based on earlier integration work by Tom Henderson and Sam Jansen.
 * 2008 Florian Westphal <fw@strlen.de>
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/nstime.h"

#include "ns3/packet.h"
#include "ns3/node.h"
#include "ns3/ipv4-route.h"

#include "ns3/object-vector.h"
#include "ns3/string.h"
#include "tcp-header.h"
#include "ipv4-end-point-demux.h"
#include "ipv4-end-point.h"
#include "ipv4-l3-protocol.h"
#include "nsc-tcp-l4-protocol.h"
#include "nsc-tcp-socket-impl.h"
#include "nsc-sysctl.h"
#include "nsc-tcp-socket-factory-impl.h"
#include "sim_interface.h"

#include <vector>
#include <sstream>
#include <dlfcn.h>
#include <iomanip>

#include <netinet/in.h>
#include <arpa/inet.h>

NS_LOG_COMPONENT_DEFINE ("NscTcpL4Protocol");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (NscTcpL4Protocol);

/* see http://www.iana.org/assignments/protocol-numbers */
const uint8_t NscTcpL4Protocol::PROT_NUMBER = 6;

class NscInterfaceImpl : public ISendCallback, public IInterruptCallback 
{
public:
  NscInterfaceImpl (Ptr<NscTcpL4Protocol> prot);
private:
  virtual void send_callback (const void *data, int datalen);
  virtual void wakeup ();
  virtual void gettime (unsigned int *, unsigned int *);
private:
  Ptr<NscTcpL4Protocol> m_prot;
};

NscInterfaceImpl::NscInterfaceImpl (Ptr<NscTcpL4Protocol> prot)
  : m_prot (prot)
{
}

void
NscInterfaceImpl::send_callback (const void *data, int datalen)
{
  m_prot->send_callback (data, datalen);
}
void
NscInterfaceImpl::wakeup ()
{
  m_prot->wakeup ();
}
void
NscInterfaceImpl::gettime (unsigned int *sec, unsigned int *usec)
{
  m_prot->gettime (sec,usec);
}


#undef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT                                   \
  if (m_node) { std::clog << Simulator::Now ().GetSeconds () << " [node " << m_node->GetId () << "] "; } 

TypeId 
NscTcpL4Protocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::NscTcpL4Protocol")
    .SetParent<IpL4Protocol> ()
    .AddConstructor<NscTcpL4Protocol>()
    .AddAttribute ("SocketList", "The list of sockets associated to this protocol.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&NscTcpL4Protocol::m_sockets),
                   MakeObjectVectorChecker<NscTcpSocketImpl> ())
    .AddAttribute ("Library",
                   "Set the linux library to be used to create the stack",
                   TypeId::ATTR_GET|TypeId::ATTR_CONSTRUCT,
                   StringValue ("liblinux2.6.26.so"),
                   MakeStringAccessor (&NscTcpL4Protocol::GetNscLibrary,&NscTcpL4Protocol::SetNscLibrary),
                   MakeStringChecker ())
  ;
  return tid;
}

int external_rand ()
{
  return 1;   // TODO
}

NscTcpL4Protocol::NscTcpL4Protocol ()
  : m_endPoints (new Ipv4EndPointDemux ()),
    m_nscStack (0),
    m_nscInterface (new NscInterfaceImpl (this)),
    m_softTimer (Timer::CANCEL_ON_DESTROY)
{
  m_dlopenHandle = NULL;
  NS_LOG_LOGIC ("Made a NscTcpL4Protocol "<<this);
}

NscTcpL4Protocol::~NscTcpL4Protocol ()
{
  NS_LOG_FUNCTION (this);
  dlclose (m_dlopenHandle);
}

void
NscTcpL4Protocol::SetNscLibrary (const std::string &soname)
{
  if (soname!="")
    {
      m_nscLibrary = soname;
      NS_ASSERT (!m_dlopenHandle);
      m_dlopenHandle = dlopen (soname.c_str (), RTLD_NOW);
      if (m_dlopenHandle == NULL)
        NS_FATAL_ERROR (dlerror ());
    }
}

std::string 
NscTcpL4Protocol::GetNscLibrary () const
{
  return m_nscLibrary;
}
void 
NscTcpL4Protocol::SetNode (Ptr<Node> node)
{
  m_node = node;

  if (m_nscStack)
    { // stack has already been loaded...
      return;
    }

  NS_ASSERT (m_dlopenHandle);

  FCreateStack create = (FCreateStack)dlsym (m_dlopenHandle, "nsc_create_stack");
  NS_ASSERT (create);
  m_nscStack = create (m_nscInterface, m_nscInterface, external_rand);
  int hzval = m_nscStack->get_hz ();

  NS_ASSERT (hzval > 0);

  m_softTimer.SetFunction (&NscTcpL4Protocol::SoftInterrupt, this);
  m_softTimer.SetDelay (MilliSeconds (1000/hzval));
  m_nscStack->init (hzval);
  // This enables stack and NSC debug messages
  // m_nscStack->set_diagnostic(1000);

  Ptr<Ns3NscStack> nscStack = Create<Ns3NscStack> ();
  nscStack->SetStack (m_nscStack);
  node->AggregateObject (nscStack);

  m_softTimer.Schedule ();

  // its likely no ns-3 interface exits at this point, so
  // we dealy adding the nsc interface until the start of the simulation.
  Simulator::ScheduleNow (&NscTcpL4Protocol::AddInterface, this);
}

void
NscTcpL4Protocol::NotifyNewAggregate ()
{ 
  if (m_node == 0)
    {
      Ptr<Node>node = this->GetObject<Node> ();
      if (node != 0)
        {
          Ptr<Ipv4L3Protocol> ipv4 = this->GetObject<Ipv4L3Protocol> ();
          if (ipv4 != 0 && m_downTarget.IsNull ())
            {
              this->SetNode (node);
              ipv4->Insert (this);
              Ptr<NscTcpSocketFactoryImpl> tcpFactory = CreateObject<NscTcpSocketFactoryImpl> ();
              tcpFactory->SetTcp (this);
              node->AggregateObject (tcpFactory);
              this->SetDownTarget (MakeCallback (&Ipv4L3Protocol::Send, ipv4));
            }
        }
    }
  Object::NotifyNewAggregate ();
}

int 
NscTcpL4Protocol::GetProtocolNumber (void) const
{
  return PROT_NUMBER;
}
int 
NscTcpL4Protocol::GetVersion (void) const
{
  return 2;
}

void
NscTcpL4Protocol::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  for (std::vector<Ptr<NscTcpSocketImpl> >::iterator i = m_sockets.begin (); i != m_sockets.end (); i++)
    {
      *i = 0;
    }
  m_sockets.clear ();


  if (m_endPoints != 0)
    {
      delete m_endPoints;
      m_endPoints = 0;
    }
  m_node = 0;
  delete m_nscInterface;
  m_nscInterface = 0;
  m_downTarget.Nullify ();
  IpL4Protocol::DoDispose ();
}

Ptr<Socket>
NscTcpL4Protocol::CreateSocket (void)
{
  NS_LOG_FUNCTION (this);

  Ptr<NscTcpSocketImpl> socket = CreateObject<NscTcpSocketImpl> ();
  socket->SetNode (m_node);
  socket->SetTcp (this);
  m_sockets.push_back (socket);
  return socket;
}

Ipv4EndPoint *
NscTcpL4Protocol::Allocate (void)
{
  NS_LOG_FUNCTION (this);
  return m_endPoints->Allocate ();
}

Ipv4EndPoint *
NscTcpL4Protocol::Allocate (Ipv4Address address)
{
  NS_LOG_FUNCTION (this << address);
  return m_endPoints->Allocate (address);
}

Ipv4EndPoint *
NscTcpL4Protocol::Allocate (uint16_t port)
{
  NS_LOG_FUNCTION (this << port);
  return m_endPoints->Allocate (port);
}

Ipv4EndPoint *
NscTcpL4Protocol::Allocate (Ipv4Address address, uint16_t port)
{
  NS_LOG_FUNCTION (this << address << port);
  return m_endPoints->Allocate (address, port);
}

Ipv4EndPoint *
NscTcpL4Protocol::Allocate (Ipv4Address localAddress, uint16_t localPort,
                            Ipv4Address peerAddress, uint16_t peerPort)
{
  NS_LOG_FUNCTION (this << localAddress << localPort << peerAddress << peerPort);
  return m_endPoints->Allocate (localAddress, localPort,
                                peerAddress, peerPort);
}

void 
NscTcpL4Protocol::DeAllocate (Ipv4EndPoint *endPoint)
{
  NS_LOG_FUNCTION (this << endPoint);
  // NSC m_endPoints->DeAllocate (endPoint);
}

IpL4Protocol::RxStatus
NscTcpL4Protocol::Receive (Ptr<Packet> packet,
                           Ipv4Header const &header,
                           Ptr<Ipv4Interface> incomingInterface)
{
  NS_LOG_FUNCTION (this << packet << header << incomingInterface);
  Ipv4Header ipHeader;
  uint32_t packetSize = packet->GetSize ();

  // The way things work at the moment, the IP header has been removed
  // by the ns-3 IPv4 processing code. However, the NSC stack expects
  // a complete IP packet, so we add the IP header back.
  // Since the original header is already gone, we create a new one
  // based on the information we have.
  ipHeader.SetSource (header.GetSource ());
  ipHeader.SetDestination (header.GetDestination ());
  ipHeader.SetProtocol (PROT_NUMBER);
  ipHeader.SetPayloadSize (packetSize);
  ipHeader.SetTtl (1);
  // all NSC stacks check the IP checksum
  ipHeader.EnableChecksum ();

  packet->AddHeader (ipHeader);
  packetSize = packet->GetSize ();

  uint8_t *buf = new uint8_t[packetSize];
  packet->CopyData (buf, packetSize);
  const uint8_t *data = const_cast<uint8_t *>(buf);

  // deliver complete packet to the NSC network stack
  m_nscStack->if_receive_packet (0, data, packetSize);
  delete[] buf;

  wakeup ();
  return IpL4Protocol::RX_OK;
}

IpL4Protocol::RxStatus
NscTcpL4Protocol::Receive(Ptr<Packet>, Ipv6Header const &, Ptr<Ipv6Interface>)
{
  return IpL4Protocol::RX_ENDPOINT_UNREACH;
}

void NscTcpL4Protocol::SoftInterrupt (void)
{
  m_nscStack->timer_interrupt ();
  m_nscStack->increment_ticks ();
  m_softTimer.Schedule ();
}

void NscTcpL4Protocol::send_callback (const void* data, int datalen)
{
  Ptr<Packet> p;
  uint32_t ipv4Saddr, ipv4Daddr;

  NS_ASSERT (datalen > 20);


  // create packet, without IP header. The TCP header is not touched.
  // Not using the IP header makes integration easier, but it destroys
  // eg. ECN.
  const uint8_t *rawdata = reinterpret_cast<const uint8_t *>(data);
  rawdata += 20; // skip IP header. IP options aren't supported at this time.
  datalen -= 20;
  p = Create<Packet> (rawdata, datalen);

  // we need the real source/destination ipv4 addresses for Send ().
  const uint32_t *ipheader = reinterpret_cast<const uint32_t *>(data);
  ipv4Saddr = *(ipheader+3);
  ipv4Daddr = *(ipheader+4);

  Ipv4Address saddr (ntohl (ipv4Saddr));
  Ipv4Address daddr (ntohl (ipv4Daddr));

  Ptr<Ipv4L3Protocol> ipv4 = m_node->GetObject<Ipv4L3Protocol> ();
  NS_ASSERT_MSG (ipv4, "nsc callback invoked, but node has no ipv4 object");

  m_downTarget (p, saddr, daddr, PROT_NUMBER, 0);
  m_nscStack->if_send_finish (0);
}

void NscTcpL4Protocol::wakeup ()
{
  // TODO
  // this should schedule a timer to read from all tcp sockets now... this is
  // an indication that data might be waiting on the socket

  Ipv4EndPointDemux::EndPoints endPoints = m_endPoints->GetAllEndPoints ();
  for (Ipv4EndPointDemux::EndPointsI endPoint = endPoints.begin ();
       endPoint != endPoints.end (); endPoint++) {
      // NSC HACK: (ab)use TcpSocket::ForwardUp for signalling
      (*endPoint)->ForwardUp (NULL, Ipv4Header (), 0, 0);
    }
}

void NscTcpL4Protocol::gettime (unsigned int* sec, unsigned int* usec)
{
  // Only used by the Linux network stack, e.g. during ISN generation
  // and in the kernel rng initialization routine. Also used in Linux
  // printk output.
  Time t = Simulator::Now ();
  int64_t us = t.GetMicroSeconds ();
  *sec = us / (1000*1000);
  *usec = us - *sec * (1000*1000);
}


void NscTcpL4Protocol::AddInterface (void)
{
  Ptr<Ipv4> ip = m_node->GetObject<Ipv4> ();
  const uint32_t nInterfaces = ip->GetNInterfaces ();

  NS_ASSERT_MSG (nInterfaces <= 2, "nsc does not support multiple interfaces per node");

  // start from 1, ignore the loopback interface (HACK)
  // we really don't need the loop, but its here to illustrate
  // how things _should_ be (once nsc can deal with multiple interfaces...)
  for (uint32_t i = 1; i < nInterfaces; i++)
    {
      Ipv4InterfaceAddress ifAddr = ip->GetAddress (i, 0);
      Ipv4Address addr = ifAddr.GetLocal ();
      Ipv4Mask mask = ifAddr.GetMask ();
      uint16_t mtu = ip->GetMtu (i);

      std::ostringstream addrOss, maskOss;

      addr.Print (addrOss);
      mask.Print (maskOss);

      NS_LOG_LOGIC ("if_attach " << addrOss.str ().c_str () << " " << maskOss.str ().c_str () << " " << mtu);

      std::string addrStr = addrOss.str ();
      std::string maskStr = maskOss.str ();
      const char* addrCStr = addrStr.c_str ();
      const char* maskCStr = maskStr.c_str ();
      m_nscStack->if_attach (addrCStr, maskCStr, mtu);

      if (i == 1)
        {
          // The NSC stack requires a default gateway and only supports
          // single-interface nodes.  The below is a hack, but
          // it turns out that we can pass the interface address to nsc as
          // a default gateway.  Bug 1398 has been opened to track this
          // issue (NSC's limitation to single-interface nodes)
          //
          // Previous versions of this code tried to assign the "next"
          // IP address of the subnet but this was found to fail for
          // some use cases in /30 subnets.

          // XXX
          m_nscStack->add_default_gateway (addrOss.str ().c_str ());
        }
    }
}

void
NscTcpL4Protocol::SetDownTarget (IpL4Protocol::DownTargetCallback callback)
{
  m_downTarget = callback;
}

void
NscTcpL4Protocol::SetDownTarget6 (IpL4Protocol::DownTargetCallback6 callback)
{
}

IpL4Protocol::DownTargetCallback
NscTcpL4Protocol::GetDownTarget (void) const
{
  return m_downTarget;
}

IpL4Protocol::DownTargetCallback6
NscTcpL4Protocol::GetDownTarget6 (void) const
{
  return (IpL4Protocol::DownTargetCallback6)0;
}

} // namespace ns3

