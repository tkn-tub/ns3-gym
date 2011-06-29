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
 * Author: George F. Riley<riley@ece.gatech.edu>
 */

// Interface between ns3 and the network animator

#include <stdio.h>
#include <sstream>

// Socket related includes
#if defined(HAVE_SYS_SOCKET_H) && defined(HAVE_NETINET_IN_H)
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <fcntl.h>
#endif

// ns3 includes
#include "ns3/animation-interface.h"
#include "ns3/channel.h"
#include "ns3/config.h"
#include "ns3/node.h"
#include "ns3/mobility-model.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

using namespace std;

NS_LOG_COMPONENT_DEFINE ("AnimationInterface");

namespace ns3 {

AnimationInterface::AnimationInterface ()
  : m_fHandle (STDOUT_FILENO), m_model (0)
{
}

AnimationInterface::~AnimationInterface ()
{
}

bool AnimationInterface::SetOutputFile (const std::string& fn)
{
  FILE* f = fopen (fn.c_str (), "w");
  if (!f)
    {
      return false; // Can't open
    }
  m_fHandle = fileno (f); // Set the file handle
  return true;
}

bool AnimationInterface::SetServerPort (uint16_t port)
{
#if defined(HAVE_SYS_SOCKET_H) && defined(HAVE_NETINET_IN_H)
  int s = socket (AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons (port);
  addr.sin_addr.s_addr = htonl (INADDR_ANY);
  if (bind (s, (struct sockaddr*)&addr, sizeof (addr)) < 0)
    {
      NS_LOG_WARN ("Can't bind to port " << port << ", exiting.");
      return false;
    }
  listen (s, 1);
  NS_LOG_INFO ("Waiting for animator connection");
  // Now wait for the animator to connect in
  m_fHandle = accept (s, 0, 0);
  NS_LOG_INFO ("Got animator connection from remote");
  // set the linger socket option
  int t = 1;
  setsockopt (s, SOL_SOCKET, SO_LINGER, &t, sizeof(t));
  return true;
#endif
  return false; // never reached unless the above is disabled
                // which is done to support a platform like MinGW
}

void AnimationInterface::StartAnimation ()
{
  // Dump the topology
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> n = *i;
      Ptr<MobilityModel> loc = n->GetObject<MobilityModel> ();
      if (loc)
        {
          // Location exists, dump it
          Vector v = loc->GetPosition ();
          ostringstream oss;
          oss << "0.0 N " << n->GetId () 
              << " " << v.x << " " << v.y << endl;
          WriteN (m_fHandle, oss.str ().c_str (), oss.str ().length ());
        }
    }
  // Now dump the p2p links
  for (NodeList::Iterator i = NodeList::Begin (); i != NodeList::End (); ++i)
    {
      Ptr<Node> n = *i;
      uint32_t n1Id = n->GetId ();
      uint32_t nDev = n->GetNDevices ();  // Number of devices
      for (uint32_t i = 0; i < nDev; ++i)
        {
          Ptr<NetDevice> dev = n->GetDevice (i);
          Ptr<Channel>   ch = dev->GetChannel ();
          if (!ch) 
            {
              continue; // No channel, can't be p2p device
            }
          string channelType = ch->GetInstanceTypeId ().GetName ();
          if (channelType == string ("ns3::PointToPointChannel"))
            { // Since these are duplex links, we only need to dump
              // if srcid < dstid
              uint32_t nChDev = ch->GetNDevices ();
              for (uint32_t j = 0; j < nChDev; ++j)
                {
                  Ptr<NetDevice> chDev = ch->GetDevice (j);
                  uint32_t n2Id = chDev->GetNode ()->GetId ();
                  if (n1Id < n2Id)
                    { // ouptut the p2p link
                      ostringstream oss;
                      oss << "0.0 L "  << n1Id << " " << n2Id << endl;
                      WriteN (m_fHandle, oss.str ().c_str (),
                              oss.str ().length ());
                    }
                }
            }
          else
            {
              NS_FATAL_ERROR ("Net animation currently only supports point-to-point links.");
            }
        }
    }

  // Connect the callback for packet tx events
  Config::Connect ("/ChannelList/*/TxRxPointToPoint",
                   MakeCallback (&AnimationInterface::DevTxTrace, this));
}

void AnimationInterface::StopAnimation ()
{
  if (m_fHandle > 0) 
    {
      close (m_fHandle);
    }
}


// Private methods
int AnimationInterface::WriteN (int h, const char* data, uint32_t count)
{ // Write count bytes to h from data
  uint32_t    nLeft   = count;
  const char* p       = data;
  uint32_t    written = 0;

  while (nLeft)
    {
      int n = write (h, p, nLeft);
      if (n <= 0) 
        {
          return written;
        }
      written += n;
      nLeft -= n;
      p += n;
    }
  return written;
}

void AnimationInterface::DevTxTrace (std::string context, Ptr<const Packet> p,
                                     Ptr<NetDevice> tx, Ptr<NetDevice> rx,
                                     Time txTime, Time rxTime)
{
  Time now = Simulator::Now ();
  ostringstream oss;
  oss << now.GetSeconds () << " P "
      << tx->GetNode ()->GetId () << " "
      << rx->GetNode ()->GetId () << " "
      << (now + txTime).GetSeconds () << " " // last bit tx time
      << (now + rxTime - txTime).GetSeconds () << " " // first bit rx time
      << (now + rxTime).GetSeconds () << endl;         // last bit rx time
  WriteN (m_fHandle, oss.str ().c_str (), oss.str ().length ());
}

} // namespace ns3
