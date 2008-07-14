/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2007 University of Washington
 * Copyright 2007 Georgia Tech Research Corporation
 * Copyright 2008 Sandia Corporation
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
 *
 * Sandia is a multiprogram laboratory operated by Sandia
 * Corporation, a Lockheed Martin Company, for the United States
 * Department of Energy's National Nuclear Security Administration
 * under Contract DE-AC04-94AL85000.
 *
 * Author: David Evensky, Sandia National Labs
 *
 * includes GPL'ed ns-3-dev code from:
 *   udp-echo,          // ??, (c) U. Wash. 2007
 *   tcp-large-transfer // ??
 *   packet-sink        // Tom Henderson, (c) U. Wash. 2007
 *   onoff              // George F. Riley, (c) GT Res Corp. 2007
 */


/* NOTE
This isn't really an example file, so don't look to closely at it as a starting
script.  It is here for validation of the TCP model.
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <map>

#include "ns3/core-module.h"
#include "ns3/helper-module.h"
#include "ns3/node-module.h"
#include "ns3/global-route-manager.h"
#include "ns3/simulator-module.h"
#include "ns3/log.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("tcp-2way");

//----------defines-------------------------------------------------------------
//define to print more debugging info.
#define VERBOSE_TESTING

/* define to die because Socket::Recv() is told to
 * only read an "undesired" number of bytes
 * fails NS_ASSERT in HandleRead because rx_addr_tag
 * tag isn't there.
 */
//#define TOO_SMALL_BUG

//define to print this pointer in NS_LOG_FUNCTION
//#define PRINT_THIS

/*define to print packet info in HandleRead
 *expected output is of the form:
 *536:P{536} *bytes from 10.1.0.2 [02-06-0a:01:00:02:20:00] <0|536|884018>
 * ^  ^      ^              ^             ^                  ^   ^ ^
 * |  |      |              |             |                  |   | |
 * |  |      |              |             |                  |   | +bytes left
 * |  |      |              |             |                  |   |  to read in
 * |  |      |              |             |                  |   |  sent buffer
 * |  |      |              |             |                  |   +bytes read in
 * |  |      |              |             |                  |    this packet if
 * |  |      |              |             |                  |    header read,
 * |  |      |              |             |                  |    prints size
 * |  |      |              |             |                  |    in ()
 * |  |      |              |             |                  +bytes cur. packet
 * |  |      |              |             |                   left to process
 * |  |      |              |             +mac addr of sender
 * |  |      |              +IP addr of sender
 * |  |      +print '*' while processing first chunk in a received packet.
 * |  |       if there are multiple small sent packets, these can appear in
 * |  |       a single packet, and are processed by a do {} while() loop.
 * |  +rle output of packet::PeekData(), in this case filled with 536 bytes
 * |   filled with the letter 'P' (brought to you by the letter 'Q' and the
 * |   number '9' :-))
 * + packet::GetSize()
 *
 * If there is a header it will look more like:
 * 536:N{317}@B.{6}HELLOP{206} *bytes from 10.1.0.2 [02-06-0a:01:00:02:20:00]\
 *                                                                   <219|317|0>
 * 536:N{317}@B.{6}HELLOP{206}  bytes from 10.1.0.2 [02-06-0a:01:00:02:20:00]\
 *                               <0|206(13)|999794> (ArchHeader=HELLO:1000000)
 * where you can see that this first packet includes 317 bytes from the
 * previous send packet (filled with N) and 13 bytes of header before the 206
 * bytes of payload (filled with P).
 */

#define RECV_PRINT

// dcl non-method functions....
std::string rle(const uint8_t *, int );
std::ofstream fileOut("tcp-2way.out");

//----------class ArchHeader dcl and definition---------------------------------
/*
 * ArchHeader: A toy header that holds a string
 * and a uint32_t. The string is an abitrary label
 * and the int is used as the length of the payload.
 */

class ArchHeader : public Header {
public:
  virtual uint32_t  GetSerializedSize (void) const;
  virtual void      Serialize (Buffer::Iterator) const;
  virtual uint32_t  Deserialize (Buffer::Iterator);
  virtual void      Print (std::ostream &) const;
  static  TypeId    GetTypeId (void);
  virtual TypeId    GetInstanceTypeId (void) const;

  // accessor functions.....
  void        SetIntValue (uint32_t);
  uint32_t    GetIntValue (void) const;
  void        SetStrValue (const std::string &s);
  std::string GetStrValue (void) const;
private:
  uint32_t          int_value;
  std::string       str_value;
};


TypeId
ArchHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ArchHeader")
    .AddConstructor<ArchHeader> () // NOTE: Required for ????
    .SetParent<Header> ()          // ditto
    ;
  return tid;
}

TypeId
ArchHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
ArchHeader::GetSerializedSize (void) const
{
  return 2*sizeof (uint32_t) + str_value.size (); // int, str len & bytes
}

void
ArchHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU32 (int_value);
  uint32_t str_len = str_value.size ();
  i.WriteU32 (str_len);
  for (uint32_t j = 0; j < str_value.size (); j++)
    i.WriteU8 (str_value[j]);
}
  
uint32_t
ArchHeader::Deserialize (Buffer::Iterator i)
{
  int_value = i.ReadU32 ();
  uint32_t str_len = i.ReadU32 ();
  str_value.reserve (str_len);
  for (uint32_t j = 0; j < str_len; j++) {
    char v = i.ReadU8 ();
    str_value.push_back (v);
  }
  return 2*sizeof (uint32_t) + str_value.size ();
}


void
ArchHeader::SetIntValue (uint32_t v_)
{
  int_value = v_;
}

uint32_t
ArchHeader::GetIntValue (void) const
{
  return int_value;
}

void
ArchHeader::SetStrValue (const std::string &s)
{
  str_value = s;
}

std::string
ArchHeader::GetStrValue (void) const
{
  return str_value;
}
   
void
ArchHeader::Print (std::ostream &os) const
{
  os << "(ArchHeader=" << str_value << ":" << int_value << ")";
}

//----------class TalkerApp dcl and definition----------------------------------
/*
 * TalkerApp:
 * An an Application that can serve as both Rx and Tx side.
 * and is configured by ConfRecv/ConfSend and driven by
 * ScheduleSendPacket. Becareful not to try to send a packet
 * from the Rx side to the Tx side (back channel) before the
 * connection is setup, which has to be from the Tx to the Rx
 * (the forward channel).
 */

class TalkerApp : public Application {
public:
  TalkerApp ();
  virtual ~TalkerApp ();
  void ConfRecv (Ptr<Node>,const Address &);
  void ConfSend (Ptr<Node>,const Address &);
  void ScheduleSendPacket(const Time ,const char , const uint32_t size );

protected:
  virtual void DoDispose (void);
private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void StartApplicationRecv (void);
  void StartApplicationSend (void);

  void HandleRead (Ptr<Socket>);
  void SendPacket (std::string,char,uint32_t);

  void CloseConnection (Ptr<Socket> socket);
  void ConnectionSucceeded (Ptr<Socket>);
  void ConnectionFailed (Ptr<Socket>);
  bool ConnectionRequested (Ptr<Socket>, const Address &);
  void ConnectionCreated (Ptr<Socket>, const Address &);


  bool            verbose;
  Ptr<Socket>     m_socket;
  Ptr<Socket>     m_servsocket;
  Address         m_local;
  Address         m_remote;
  int             i_am_listener;
  typedef std::map<Ptr<Socket>, uint32_t>::iterator left_to_read_iterator_type;
  std::map<Ptr<Socket>, uint32_t> m_left_to_read;
};

class TalkerApp_sub : public TalkerApp {
public:
  int foo(void);
};

int
TalkerApp_sub::foo(void) {return 0;}

TalkerApp::TalkerApp () :
  verbose (false),
  m_socket (0),
  m_servsocket (0),
  i_am_listener (0)
{}

TalkerApp::~TalkerApp ()
{}

void
TalkerApp::DoDispose (void)
{
  m_socket = 0;
  m_servsocket = 0;
  Application::DoDispose ();
}


void
TalkerApp::CloseConnection (Ptr<Socket> sock)
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  << sock
                  );
  sock->Close ();
}

void
TalkerApp::ConnectionSucceeded (Ptr<Socket> sock)
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  << sock
	
		   );
}

void
TalkerApp::ConnectionFailed (Ptr<Socket> sock)
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  << sock
                  );
}

bool
TalkerApp::ConnectionRequested (Ptr<Socket> sock, const Address &addr)
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  << sock
                  << addr
                  );
  return true;
}

void
TalkerApp::ConnectionCreated (Ptr<Socket> sock, const Address &addr)
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  << sock
                  << addr
                  );
  m_socket = sock;
  m_socket->SetRecvCallback(MakeCallback (&TalkerApp::HandleRead, this));
}

void 
TalkerApp::ScheduleSendPacket (const Time dt,const char fill,
                               const uint32_t size)
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                   << dt
                   << fill
                   << size
                   );
  Simulator::Schedule(dt,&TalkerApp::SendPacket, this,
                      std::string("HELLO"),fill,size);
}


void
TalkerApp::StartApplication ()
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  );
  if (i_am_listener)
    StartApplicationRecv ();
  else
    StartApplicationSend ();
}

void
TalkerApp::StartApplicationSend ()
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  );

  if (!m_socket) {
    m_socket = Socket::CreateSocket (GetNode (),
				    TcpSocketFactory::GetTypeId ());
    m_socket->Bind ();
  }

  m_socket->Connect (m_remote);
  m_socket->SetConnectCallback
    (
     MakeCallback (&TalkerApp::ConnectionSucceeded,this),
     MakeCallback (&TalkerApp::ConnectionFailed,this)
     );
  m_socket->SetRecvCallback (MakeCallback (&TalkerApp::HandleRead, this));
}

void
TalkerApp::StartApplicationRecv ()
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  );
  if (!m_servsocket) {
    m_servsocket = Socket::CreateSocket (GetNode (),
					TcpSocketFactory::GetTypeId ());
    m_servsocket->Bind (m_local);
    m_servsocket->Listen (0);
  }

  m_servsocket->SetAcceptCallback
    (
     MakeCallback (&TalkerApp::ConnectionRequested,this),
     MakeCallback (&TalkerApp::ConnectionCreated,this)
     );
}
 
void
TalkerApp::ConfRecv (Ptr<Node> node, const Address &addr)
{
  i_am_listener = 1;
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  << node
                  << addr
                  );
  m_local = addr;
  node->AddApplication (this);
}

void
TalkerApp::ConfSend (Ptr<Node> node, const Address &addr)
{
  i_am_listener = 0;
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  << node
                  << addr
                  );
  m_remote = addr;
  node->AddApplication (this);
}


void
TalkerApp::SendPacket (std::string label,char fill,uint32_t payloadsize)
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  << label
                  << fill
                  << payloadsize
                  );

#if 0
  Ptr<Packet> p = Create<Packet> (payloadsize); // zero byte filled packet.
#else
  uint8_t *buf = (uint8_t *) malloc(payloadsize*sizeof(uint8_t));
  memset(buf,fill,payloadsize);
  Ptr<Packet> p = Create<Packet> (buf,payloadsize); // filled packet.
  free(buf);
#endif
  if (verbose) {
    p->Print (fileOut << "Before Marking|");
    fileOut << std::endl;
  }

  ArchHeader hdr;
  hdr.SetStrValue (label);
  hdr.SetIntValue (p->GetSize());
  p->AddHeader (hdr);
  if (verbose) {
    p->Print(fileOut << "After Header Marking|");
    fileOut << std::endl;
  }

  m_socket->Send (p);
}

void
TalkerApp::StopApplication ()
{
  NS_LOG_FUNCTION ((i_am_listener ? "Server|" : "Client|")
#ifdef PRINT_THIS                  
                  << this
#endif
                  );
  if (m_socket)
    m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
}

void
TalkerApp::HandleRead (Ptr<Socket> socket)
{
  left_to_read_iterator_type iter = m_left_to_read.find (socket);
  if (iter == m_left_to_read.end ())
    m_left_to_read[socket] = 0;

  std::ostringstream  pretty;
  pretty << (i_am_listener ? "Server" : "Client")
         << (iter == m_left_to_read.end () ? 
             "(new)" : "(old)")
         << "|";

  NS_LOG_FUNCTION ( pretty.str ()
#ifdef PRINT_THIS                  
                  << this
#endif
                  << socket
                  );
  /*
   * NOTE: I can crash with the # bytes == 500 rather than 536
   * which seems to be the default MTU
   */

#ifdef TOO_SMALL_BUG
  const uint32_t number_of_bytes_willing_to_read = 500;
#else
  const uint32_t number_of_bytes_willing_to_read = 536;
#endif
  const uint32_t unused_flags = 0;

  Ptr<Packet> packet;
  while (packet = socket->Recv (number_of_bytes_willing_to_read,
                                unused_flags) ){
    std::string raw_rle = rle(packet->PeekData(),
                              packet->GetSize());
    SocketAddressTag rx_addr_tag;
    bool found;
    found = packet->FindFirstMatchingTag (rx_addr_tag);
    NS_ASSERT (found);
    Address from = rx_addr_tag.GetAddress ();
    // XXX packet->RemoveTag (tag);
    if (InetSocketAddress::IsMatchingType (from)) {
      InetSocketAddress address = InetSocketAddress::ConvertFrom (from);

#ifdef VERBOSE_TESTING
      packet->Print (fileOut << "Packet Dump: ");
      fileOut << std::endl;
#endif


      bool freshly_in_the_loop = true;
      do {
#if defined(RECV_PRINT)
        fileOut
          << raw_rle // <length>:<rle of packet->PeekBuffer>
          << (freshly_in_the_loop ? " *" : "  ") /* used to see if the do while
                                                  * does anything */
          << "bytes from "
          << address.GetIpv4 () << " [" << address << "] ";
#endif
        freshly_in_the_loop = false;
        int header_bytes = 0;
        ArchHeader* hdr =  0; // sadly, can't be a Ptr<>
        if (
            (m_left_to_read[socket] == 0)
            ) {
          hdr =  new ArchHeader;
          if (packet->RemoveHeader (*hdr)) {
            m_left_to_read[socket] = hdr->GetIntValue ();
            header_bytes = hdr->GetSerializedSize ();
          } else {
            ; // never get here, ns3 dumps if no header is there.
          }
        }
        uint32_t num_bytes_processed = 0;
        if (m_left_to_read[socket] <= packet->GetSize ()) {
          packet->RemoveAtStart (m_left_to_read[socket]);
          num_bytes_processed = m_left_to_read[socket];
          m_left_to_read[socket] = 0;

        } else {
          m_left_to_read[socket] -= packet->GetSize ();
          num_bytes_processed = packet->GetSize ();
          packet->RemoveAtStart (packet->GetSize ());
        }

#if defined(RECV_PRINT)
        fileOut
          << "<"
          << packet->GetSize ()
          << "|"
          <<  num_bytes_processed
          ;
        if (header_bytes)
          fileOut
            << "("
            << header_bytes
            << ")"
            ;
        fileOut
          << "|"
          << m_left_to_read[socket]
          << ">";
#ifdef VERBOSE_TESTING
        if (hdr)
          hdr->Print(fileOut << " ");
#endif
        fileOut << std::endl;
#endif
        if (hdr)
          delete hdr; // free what we new....
      } while (
               packet->GetSize ()
               );
    } else {
      fileOut << "not a match in HandleRead" << std::endl;
    }

  }
}

//---------------Non-member functions-------------------------------------------
/*
 * returns a string containing the length of the buffer,
 * followed by an run length encoded version of the buffer
 * un-printables are represented by '.' (i.e. special_marker
 * const).
 */
std::string rle(const uint8_t *buf, int buflen) {

  int justify = 5;
  std::ostringstream o;
  const uint8_t min_rep = 3;
  uint8_t cur_char = 0;
  const uint8_t special_marker = '.';
  int cur_cnt = 0;
  if (justify) {
    o << std::setw(justify) << std::right << buflen << ":";
  } else {
    o << buflen << ":";
  }

  for (int i = 0; i < buflen; i++) {
    if (
        (!isprint(buf[i]) && cur_char == special_marker) ||
        (buf[i] == cur_char)
        )
      cur_cnt++;
    else {
      if (cur_cnt >= min_rep)
        o << (isprint(cur_char) ? static_cast<char>(cur_char) : '.') << "{"
          << cur_cnt << "}";
      else
        for (int irep=0; irep < cur_cnt; irep++)
          o << (isprint(cur_char) ? static_cast<char>(cur_char) : '.');
      if (isprint(buf[i]))
        cur_char = buf[i];
      else
        cur_char = special_marker;
      cur_cnt = 1;
    }
  }
  if (cur_cnt >= min_rep)
    o << (isprint(cur_char) ? static_cast<char>(cur_char) : '.') << "{"
      << cur_cnt << "}";
  else
    for (int irep=0; irep < cur_cnt; irep++)
      o << (isprint(cur_char) ? static_cast<char>(cur_char) : '.');
  return o.str();
}

//---------------Main-----------------------------------------------------------
int
main (int argc, char *argv[]) {

  std::clog.rdbuf (fileOut.rdbuf ()); /* till we can do this in ns3::log
                                         * without this ns3 logging messages
                                         * are far away from normal output
                                         */

#ifdef VERBOSE_TESTING  
  LogComponentEnable ("tcp-2way",LOG_LEVEL_ALL);
#endif

  RandomVariable::UseGlobalSeed (1, 1, 2, 3, 5, 8);
  Packet::EnableMetadata ();

  // boilerplate:  "c0[0]" is Tx, "c0[1]" is Rx.
  NodeContainer c0;
  c0.Create (2);

  // We create the channels first without any IP addressing information
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate",DataRateValue (DataRate (10000000)));
  p2p.SetChannelAttribute ("Delay",TimeValue (MilliSeconds (10)));
  NetDeviceContainer dev0 = p2p.Install (c0);

  // add ip/tcp stack to nodes.
  InternetStackHelper internet;
  internet.Install (c0);

  // Later, we add IP addresses.  
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.0.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaces = ipv4.Assign (dev0);

  // Configure the apps.
  uint16_t servPort = 32;


  // setup Rx side using ns3::Node "c0[1]" to listen for ANY:servPort
  Ptr<TalkerApp> sink = CreateObject<TalkerApp_sub>();
  sink->ConfRecv (c0.Get (1),
		InetSocketAddress (Ipv4Address::GetAny (), servPort));
  sink->Start (Seconds (0.0)); // somehow we get by without a Stop. bad form?

  // setup Tx side using ns3::Node "c0[0]" to talk to "c0[1]":servPort
  Ptr<TalkerApp>  source = CreateObject<TalkerApp_sub>();
  source->ConfSend (c0.Get (0),
                  InetSocketAddress (ifaces.GetAddress (1),
                                     servPort));
  source->Start (Seconds (0.0));

  // 10 sec is an eternity for this case, we should be done by then.
  Simulator::Stop (Seconds (10));


  // For regression use only output ascii and pcap traces.
#ifdef REGRESSION_MODE
  std::ofstream ascii;
  ascii.open ("tcp-2way.tr");
  PointToPointHelper::EnableAsciiAll (ascii);
  PointToPointHelper::EnablePcapAll ("tcp-2way");
#endif

  /*
   * Now lets schedule some packets and go. For scheduling packets
   * we have to be careful not to do the backchannel too early or
   * we try to use a socket that doesn't yet exist since on the Rx
   * side the socket is created as part of the normal Tx->Rx connection
   * setup.
   *
   * The delay of 40ms was by trial and error and seems good enough. It
   * would be nice to know what the bounds are and why....
   */

  char tagchar = 'A'; // the fill character in case we fill in SendPacket.

  Time d2t = MilliSeconds(40); /* rate to change time offset (dt) so packets
                                * are sent in order (Tx->Rx, Rx->Tx for a given
                                * size over a range of sizes).*/

  Time dt = MilliSeconds(0); /* ns3 doesn't take absolute times, but times
                              * relative to Simulator::Now() */

  uint32_t size = 0;
  source->ScheduleSendPacket (dt, tagchar, size);
  dt += d2t;
  ++tagchar;
  sink->ScheduleSendPacket (dt, tagchar, size);
  for (size = 1; size <= 1000000; size *= 10) {
    dt += d2t;
    ++tagchar;
    source->ScheduleSendPacket (dt, tagchar, size);
    dt += d2t;
    ++tagchar;
    sink->ScheduleSendPacket (dt, tagchar, size);
  }

  // Release the Hounds!!
  Simulator::Run ();
  Simulator::Destroy ();
  fileOut.close();
}
