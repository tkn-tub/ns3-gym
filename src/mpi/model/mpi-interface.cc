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
 * Author: George Riley <riley@ece.gatech.edu>
 */

// This object contains static methods that provide an easy interface
// to the necessary MPI information.

#include <iostream>
#include <iomanip>
#include <list>

#include "mpi-interface.h"
#include "mpi-receiver.h"

#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/net-device.h"
#include "ns3/simulator.h"
#include "ns3/simulator-impl.h"
#include "ns3/nstime.h"

#ifdef NS3_MPI
#include <mpi.h>
#endif

namespace ns3 {

SentBuffer::SentBuffer ()
{
  m_buffer = 0;
  m_request = 0;
}

SentBuffer::~SentBuffer ()
{
  delete [] m_buffer;
}

uint8_t*
SentBuffer::GetBuffer ()
{
  return m_buffer;
}

void
SentBuffer::SetBuffer (uint8_t* buffer)
{
  m_buffer = buffer;
}

#ifdef NS3_MPI
MPI_Request*
SentBuffer::GetRequest ()
{
  return &m_request;
}
#endif

uint32_t              MpiInterface::m_sid = 0;
uint32_t              MpiInterface::m_size = 1;
bool                  MpiInterface::m_initialized = false;
bool                  MpiInterface::m_enabled = false;
uint32_t              MpiInterface::m_rxCount = 0;
uint32_t              MpiInterface::m_txCount = 0;
std::list<SentBuffer> MpiInterface::m_pendingTx;

#ifdef NS3_MPI
MPI_Request* MpiInterface::m_requests;
char**       MpiInterface::m_pRxBuffers;
#endif

void
MpiInterface::Destroy ()
{
#ifdef NS3_MPI
  for (uint32_t i = 0; i < GetSize (); ++i)
    {
      delete [] m_pRxBuffers[i];
    }
  delete [] m_pRxBuffers;
  delete [] m_requests;

  m_pendingTx.clear ();
#endif
}

uint32_t
MpiInterface::GetRxCount ()
{
  return m_rxCount;
}

uint32_t
MpiInterface::GetTxCount ()
{
  return m_txCount;
}

uint32_t
MpiInterface::GetSystemId ()
{
  if (!m_initialized)
    {
      Simulator::GetImplementation ();
      m_initialized = true;
    }
  return m_sid;
}

uint32_t
MpiInterface::GetSize ()
{
  if (!m_initialized)
    {
      Simulator::GetImplementation ();
      m_initialized = true;
    }
  return m_size;
}

bool
MpiInterface::IsEnabled ()
{
  if (!m_initialized)
    {
      Simulator::GetImplementation ();
      m_initialized = true;
    }
  return m_enabled;
}

void
MpiInterface::Enable (int* pargc, char*** pargv)
{
#ifdef NS3_MPI
  // Initialize the MPI interface
  MPI_Init (pargc, pargv);
  MPI_Barrier (MPI_COMM_WORLD);
  MPI_Comm_rank (MPI_COMM_WORLD, reinterpret_cast <int *> (&m_sid));
  MPI_Comm_size (MPI_COMM_WORLD, reinterpret_cast <int *> (&m_size));
  m_enabled = true;
  m_initialized = true;
  // Post a non-blocking receive for all peers
  m_pRxBuffers = new char*[m_size];
  m_requests = new MPI_Request[m_size];
  for (uint32_t i = 0; i < GetSize (); ++i)
    {
      m_pRxBuffers[i] = new char[MAX_MPI_MSG_SIZE];
      MPI_Irecv (m_pRxBuffers[i], MAX_MPI_MSG_SIZE, MPI_CHAR, MPI_ANY_SOURCE, 0,
                 MPI_COMM_WORLD, &m_requests[i]);
    }
#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif
}

void
MpiInterface::SendPacket (Ptr<Packet> p, const Time& rxTime, uint32_t node, uint32_t dev)
{
#ifdef NS3_MPI
  SentBuffer sendBuf;
  m_pendingTx.push_back (sendBuf);
  std::list<SentBuffer>::reverse_iterator i = m_pendingTx.rbegin (); // Points to the last element

  uint32_t serializedSize = p->GetSerializedSize ();
  uint8_t* buffer =  new uint8_t[serializedSize + 16];
  i->SetBuffer (buffer);
  // Add the time, dest node and dest device
  uint64_t t = rxTime.GetNanoSeconds ();
  uint64_t* pTime = reinterpret_cast <uint64_t *> (buffer);
  *pTime++ = t;
  uint32_t* pData = reinterpret_cast<uint32_t *> (pTime);
  *pData++ = node;
  *pData++ = dev;
  // Serialize the packet
  p->Serialize (reinterpret_cast<uint8_t *> (pData), serializedSize);

  // Find the system id for the destination node
  Ptr<Node> destNode = NodeList::GetNode (node);
  uint32_t nodeSysId = destNode->GetSystemId ();

  MPI_Isend (reinterpret_cast<void *> (i->GetBuffer ()), serializedSize + 16, MPI_CHAR, nodeSysId,
             0, MPI_COMM_WORLD, (i->GetRequest ()));
  m_txCount++;
#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif
}

void
MpiInterface::ReceiveMessages ()
{ // Poll the non-block reads to see if data arrived
#ifdef NS3_MPI
  while (true)
    {
      int flag = 0;
      int index = 0;
      MPI_Status status;

      MPI_Testany (GetSize (), m_requests, &index, &flag, &status);
      if (!flag)
        {
          break;        // No more messages
        }
      int count;
      MPI_Get_count (&status, MPI_CHAR, &count);
      m_rxCount++; // Count this receive

      // Get the meta data first
      uint64_t* pTime = reinterpret_cast<uint64_t *> (m_pRxBuffers[index]);
      uint64_t nanoSeconds = *pTime++;
      uint32_t* pData = reinterpret_cast<uint32_t *> (pTime);
      uint32_t node = *pData++;
      uint32_t dev  = *pData++;

      Time rxTime = NanoSeconds (nanoSeconds);

      count -= sizeof (nanoSeconds) + sizeof (node) + sizeof (dev);

      Ptr<Packet> p = Create<Packet> (reinterpret_cast<uint8_t *> (pData), count, true);

      // Find the correct node/device to schedule receive event
      Ptr<Node> pNode = NodeList::GetNode (node);
      Ptr<MpiReceiver> pMpiRec = 0;
      uint32_t nDevices = pNode->GetNDevices ();
      for (uint32_t i = 0; i < nDevices; ++i)
        {
          Ptr<NetDevice> pThisDev = pNode->GetDevice (i);
          if (pThisDev->GetIfIndex () == dev)
            {
              pMpiRec = pThisDev->GetObject<MpiReceiver> ();
              break;
            }
        }

      NS_ASSERT (pNode && pMpiRec);

      // Schedule the rx event
      Simulator::ScheduleWithContext (pNode->GetId (), rxTime - Simulator::Now (),
                                      &MpiReceiver::Receive, pMpiRec, p);

      // Re-queue the next read
      MPI_Irecv (m_pRxBuffers[index], MAX_MPI_MSG_SIZE, MPI_CHAR, MPI_ANY_SOURCE, 0,
                 MPI_COMM_WORLD, &m_requests[index]);
    }
#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif
}

void
MpiInterface::TestSendComplete ()
{
#ifdef NS3_MPI
  std::list<SentBuffer>::iterator i = m_pendingTx.begin ();
  while (i != m_pendingTx.end ())
    {
      MPI_Status status;
      int flag = 0;
      MPI_Test (i->GetRequest (), &flag, &status);
      std::list<SentBuffer>::iterator current = i; // Save current for erasing
      i++;                                    // Advance to next
      if (flag)
        { // This message is complete
          m_pendingTx.erase (current);
        }
    }
#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif
}

void 
MpiInterface::Disable ()
{
#ifdef NS3_MPI
  int flag = 0;
  MPI_Initialized (&flag);
  if (flag)
    {
      MPI_Finalize ();
      m_enabled = false;
      m_initialized = false;
    }
  else
    {
      NS_FATAL_ERROR ("Cannot disable MPI environment without Initializing it first");
    }
#else
  NS_FATAL_ERROR ("Can't use distributed simulator without MPI compiled in");
#endif
}


} // namespace ns3
