/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright 2013. Lawrence Livermore National Security, LLC.
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
 * Author: Steven Smith <smith84@llnl.gov>
 *
 */

#include "null-message-mpi-interface.h"

#include "null-message-simulator-impl.h"
#include "remote-channel-bundle-manager.h"
#include "remote-channel-bundle.h"

#include "ns3/mpi-receiver.h"
#include "ns3/node.h"
#include "ns3/node-list.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

#ifdef NS3_MPI
#include <mpi.h>
#endif

#include <iostream>
#include <iomanip>
#include <list>

NS_LOG_COMPONENT_DEFINE ("NullMessageMpiInterface");

namespace ns3 {

/**
 * maximum MPI message size for easy
 * buffer creation
 */
const uint32_t NULL_MESSAGE_MAX_MPI_MSG_SIZE = 2000;


NullMessageSentBuffer::NullMessageSentBuffer ()
{
  m_buffer = 0;
  m_request = 0;
}

NullMessageSentBuffer::~NullMessageSentBuffer ()
{
  delete [] m_buffer;
}

uint8_t*
NullMessageSentBuffer::GetBuffer ()
{
  return m_buffer;
}

void
NullMessageSentBuffer::SetBuffer (uint8_t* buffer)
{
  m_buffer = buffer;
}

MPI_Request*
NullMessageSentBuffer::GetRequest ()
{
  return &m_request;
}

uint32_t              NullMessageMpiInterface::g_sid = 0;
uint32_t              NullMessageMpiInterface::g_size = 1;
uint32_t              NullMessageMpiInterface::g_numNeighbors = 0;
bool                  NullMessageMpiInterface::g_initialized = false;
bool                  NullMessageMpiInterface::g_enabled = false;
std::list<NullMessageSentBuffer> NullMessageMpiInterface::g_pendingTx;

MPI_Request* NullMessageMpiInterface::g_requests;
char**       NullMessageMpiInterface::g_pRxBuffers;

NullMessageMpiInterface::NullMessageMpiInterface ()
{
  NS_LOG_FUNCTION (this);

#ifndef NS3_MPI
  /*
   * This class can only be constructed if MPI is available.  Fail if an 
   * attempt is made to instantiate this class without MPI.
   */
  NS_FATAL_ERROR ("Must compile with MPI if Null Message simulator is used, see --enable-mpi option for waf");
#endif
}

NullMessageMpiInterface::~NullMessageMpiInterface ()
{
  NS_LOG_FUNCTION (this);
}

void
NullMessageMpiInterface::Destroy ()
{
  NS_LOG_FUNCTION (this);
}

uint32_t
NullMessageMpiInterface::GetSystemId ()
{
  NS_ASSERT (g_enabled);
  return g_sid;
}

uint32_t
NullMessageMpiInterface::GetSize ()
{
  NS_ASSERT (g_enabled);
  return g_size;
}

bool
NullMessageMpiInterface::IsEnabled ()
{
  if (!g_initialized)
    {
      Simulator::GetImplementation ();
      g_initialized = true;
    }
  return g_enabled;
}

void
NullMessageMpiInterface::Enable (int* pargc, char*** pargv)
{
  NS_LOG_FUNCTION (this << *pargc);
#ifdef NS3_MPI

  // Initialize the MPI interface
  MPI_Init (pargc, pargv);
  MPI_Barrier (MPI_COMM_WORLD);

  // SystemId and Size are unit32_t in interface but MPI uses int so convert.
  int mpiSystemId;
  int mpiSize;
  MPI_Comm_rank (MPI_COMM_WORLD, &mpiSystemId);
  MPI_Comm_size (MPI_COMM_WORLD, &mpiSize);

  g_sid = mpiSystemId;
  g_size = mpiSize;

  g_enabled = true;
  g_initialized = true;

#endif
}

void 
NullMessageMpiInterface::InitializeSendReceiveBuffers(void)
{
  NS_LOG_FUNCTION_NOARGS ();
#ifdef NS3_MPI
  NS_ASSERT (g_enabled);

  g_numNeighbors = RemoteChannelBundleManager::Size();

  // Post a non-blocking receive for all peers
  g_requests = new MPI_Request[g_numNeighbors];
  g_pRxBuffers = new char*[g_numNeighbors];
  int index = 0;
  for (uint32_t rank = 0; rank < g_size; ++rank)
    {
      Ptr<RemoteChannelBundle> bundle = RemoteChannelBundleManager::Find(rank);
      if (bundle) 
        {
          g_pRxBuffers[index] = new char[NULL_MESSAGE_MAX_MPI_MSG_SIZE];
          MPI_Irecv (g_pRxBuffers[index], NULL_MESSAGE_MAX_MPI_MSG_SIZE, MPI_CHAR, rank, 0,
                     MPI_COMM_WORLD, &g_requests[index]);
          ++index;
        }
    }
#endif
}

void
NullMessageMpiInterface::SendPacket (Ptr<Packet> p, const Time& rxTime, uint32_t node, uint32_t dev)
{
  NS_LOG_FUNCTION (this << p << rxTime.GetTimeStep () << node << dev);

  NS_ASSERT (g_enabled);

#ifdef NS3_MPI

  // Find the system id for the destination node
  Ptr<Node> destNode = NodeList::GetNode (node);
  uint32_t nodeSysId = destNode->GetSystemId ();

  NullMessageSentBuffer sendBuf;
  g_pendingTx.push_back (sendBuf);
  std::list<NullMessageSentBuffer>::reverse_iterator iter = g_pendingTx.rbegin (); // Points to the last element

  uint32_t serializedSize = p->GetSerializedSize ();
  uint32_t bufferSize = serializedSize + ( 2 * sizeof (uint64_t) ) + ( 2 * sizeof (uint32_t) );
  uint8_t* buffer =  new uint8_t[bufferSize];
  iter->SetBuffer (buffer);
  // Add the time, dest node and dest device
  uint64_t t = rxTime.GetInteger ();
  uint64_t* pTime = reinterpret_cast <uint64_t *> (buffer);
  *pTime++ = t;

  Time guarantee_update = NullMessageSimulatorImpl::GetInstance ()->CalculateGuaranteeTime (nodeSysId);
  *pTime++ = guarantee_update.GetTimeStep ();

  uint32_t* pData = reinterpret_cast<uint32_t *> (pTime);
  *pData++ = node;
  *pData++ = dev;
  // Serialize the packet
  p->Serialize (reinterpret_cast<uint8_t *> (pData), serializedSize);

  MPI_Isend (reinterpret_cast<void *> (iter->GetBuffer ()), bufferSize, MPI_CHAR, nodeSysId,
             0, MPI_COMM_WORLD, (iter->GetRequest ()));

  NullMessageSimulatorImpl::GetInstance ()->RescheduleNullMessageEvent (nodeSysId);

#endif
}

void
NullMessageMpiInterface::SendNullMessage (const Time& guarantee_update, Ptr<RemoteChannelBundle> bundle)
{
  NS_LOG_FUNCTION (guarantee_update.GetTimeStep () << bundle);

  NS_ASSERT (g_enabled);

#ifdef NS3_MPI

  NullMessageSentBuffer sendBuf;
  g_pendingTx.push_back (sendBuf);
  std::list<NullMessageSentBuffer>::reverse_iterator iter = g_pendingTx.rbegin (); // Points to the last element

  uint32_t bufferSize = 2 * sizeof (uint64_t) + 2 * sizeof (uint32_t);
  uint8_t* buffer =  new uint8_t[bufferSize];
  iter->SetBuffer (buffer);
  // Add the time, dest node and dest device
  uint64_t* pTime = reinterpret_cast <uint64_t *> (buffer);
  *pTime++ = 0;
  *pTime++ = guarantee_update.GetInteger ();
  uint32_t* pData = reinterpret_cast<uint32_t *> (pTime);
  *pData++ = 0;
  *pData++ = 0;

  // Find the system id for the destination MPI rank
  uint32_t nodeSysId = bundle->GetSystemId ();

  MPI_Isend (reinterpret_cast<void *> (iter->GetBuffer ()), bufferSize, MPI_CHAR, nodeSysId,
             0, MPI_COMM_WORLD, (iter->GetRequest ()));
#endif
}

void
NullMessageMpiInterface::ReceiveMessagesBlocking ()
{
  NS_LOG_FUNCTION_NOARGS ();

  ReceiveMessages(true);
}


void
NullMessageMpiInterface::ReceiveMessagesNonBlocking ()
{
  NS_LOG_FUNCTION_NOARGS ();

  ReceiveMessages(false);
}


void
NullMessageMpiInterface::ReceiveMessages (bool blocking)
{
  NS_LOG_FUNCTION (blocking);

  NS_ASSERT (g_enabled);

#ifdef NS3_MPI

  // stop flag set to true when no more messages are found to
  // process.
  bool stop = false;


  if (!g_numNeighbors) {
    // Not communicating with anyone.
    return;
  }

  do
    {
      int messageReceived = 0;
      int index = 0;
      MPI_Status status;

      if (blocking)
        {
          MPI_Waitany (g_numNeighbors, g_requests, &index, &status);
          messageReceived = 1; /* Wait always implies message was received */
          stop = true;
        }
      else
        {
          MPI_Testany (g_numNeighbors, g_requests, &index, &messageReceived, &status);
        }

      if (messageReceived)
        {
          int count;
          MPI_Get_count (&status, MPI_CHAR, &count);

          // Get the meta data first
          uint64_t* pTime = reinterpret_cast<uint64_t *> (g_pRxBuffers[index]);
          uint64_t time = *pTime++;
          uint64_t guaranteeUpdate = *pTime++;

          uint32_t* pData = reinterpret_cast<uint32_t *> (pTime);
          uint32_t node = *pData++;
          uint32_t dev  = *pData++;

          Time rxTime (time);

          // rxtime == 0 means this is a Null Message
          if (rxTime > 0)
            {
              count -= sizeof (time) + sizeof (guaranteeUpdate) + sizeof (node) + sizeof (dev);

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

            }

          // Update guarantee time for both packet receives and Null Messages.
          Ptr<RemoteChannelBundle> bundle = RemoteChannelBundleManager::Find (status.MPI_SOURCE);
          NS_ASSERT (bundle);

          bundle->SetGuaranteeTime (Time (guaranteeUpdate));

          // Re-queue the next read
          MPI_Irecv (g_pRxBuffers[index], NULL_MESSAGE_MAX_MPI_MSG_SIZE, MPI_CHAR, status.MPI_SOURCE, 0,
                     MPI_COMM_WORLD, &g_requests[index]);

        }
      else
        {
          // if non-blocking and no message received in testany then stop message loop
          stop = true;
        }
    }
  while (!stop);
#endif
}

void
NullMessageMpiInterface::TestSendComplete ()
{
  NS_LOG_FUNCTION_NOARGS ();

  NS_ASSERT (g_enabled);

#ifdef NS3_MPI
  std::list<NullMessageSentBuffer>::iterator iter = g_pendingTx.begin ();
  while (iter != g_pendingTx.end ())
    {
      MPI_Status status;
      int flag = 0;
      MPI_Test (iter->GetRequest (), &flag, &status);
      std::list<NullMessageSentBuffer>::iterator current = iter; // Save current for erasing
      ++iter; // Advance to next
      if (flag)
        { // This message is complete
          g_pendingTx.erase (current);
        }
    }
#endif
}

void
NullMessageMpiInterface::Disable ()
{
  NS_LOG_FUNCTION (this);

#ifdef NS3_MPI
  int flag = 0;
  MPI_Initialized (&flag);
  if (flag)
    {

      for (std::list<NullMessageSentBuffer>::iterator iter = g_pendingTx.begin ();
           iter != g_pendingTx.end ();
           ++iter)
        {
          MPI_Cancel (iter->GetRequest ());
          MPI_Request_free (iter->GetRequest ());
        }

      for (uint32_t i = 0; i < g_numNeighbors; ++i)
        {
          MPI_Cancel (&g_requests[i]);
          MPI_Request_free (&g_requests[i]);
        }

      MPI_Finalize ();

      for (uint32_t i = 0; i < g_numNeighbors; ++i)
        {
          delete [] g_pRxBuffers[i];
        }
      delete [] g_pRxBuffers;
      delete [] g_requests;

      g_pendingTx.clear ();

      g_enabled = false;
      g_initialized = false;

    }
  else
    {
      NS_FATAL_ERROR ("Cannot disable MPI environment without Initializing it first");
    }
#endif
}

} // namespace ns3
