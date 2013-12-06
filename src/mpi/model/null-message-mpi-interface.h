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

#ifndef NS3_NULLMESSAGE_MPI_INTERFACE_H
#define NS3_NULLMESSAGE_MPI_INTERFACE_H

#include "parallel-communication-interface.h"

#include <ns3/nstime.h>
#include <ns3/buffer.h>

#ifdef NS3_MPI
#include "mpi.h"
#else
typedef void* MPI_Request;
#endif

#include <list>

namespace ns3 {

class RemoteChannelBundle;
class Packet;

/**
 * \ingroup mpi
 *
 * \brief Non-blocking send buffers for Null Message implementation.
 * 
 * One buffer is allocated for each non-blocking send.
 */
class NullMessageSentBuffer
{
public:
  NullMessageSentBuffer ();
  ~NullMessageSentBuffer ();

  /**
   * \return pointer to sent buffer
   */
  uint8_t* GetBuffer ();
  /**
   * \param buffer pointer to sent buffer
   */
  void SetBuffer (uint8_t* buffer);
  /**
   * \return MPI request
   */
  MPI_Request* GetRequest ();

private:

  /**
   * Buffer for send.
   */
  uint8_t* m_buffer;

  /**
   * MPI request posted for the send.
   */
  MPI_Request m_request;
};

/**
 * \ingroup mpi
 *
 * \brief Interface between ns-3 and MPI for the Null Message
 * distributed simulation implementation.
 */
class NullMessageMpiInterface : public ParallelCommunicationInterface
{
public:

  NullMessageMpiInterface ();
  ~NullMessageMpiInterface ();

  /**
   * Delete all buffers
   */
  virtual void Destroy ();
  /**
   * \return system id (MPI rank)
   */
  virtual uint32_t GetSystemId ();
  /**
   * \return number of systems (MPI size)
   */
  virtual uint32_t GetSize ();
  /**
   * \return true if interface is enabled
   */
  virtual bool IsEnabled ();
  /**
   * \param pargc number of command line arguments
   * \param pargv command line arguments
   *
   * Sets up interface.   Calls MPI Init and 
   * posts receives.
   */
  virtual void Enable (int* pargc, char*** pargv);
  /**
   * Terminates the MPI environment by calling MPI_Finalize This
   * function must be called after Destroy ().  Resets m_initialized
   * and m_enabled.
   */
  virtual void Disable ();
  /**
   * \param p packet to send
   * \param rxTime received time at destination node
   * \param node destination node
   * \param dev destination device
   *
   * Serialize and send a packet to the specified node and net device.
   *
   * \internal
   * The MPI buffer format packs a delivery information and the serialized packet.
   *
   * uint64_t time the packed should be delivered
   * uint64_t guarantee time for the Null Message algorithm.
   * uint32_t node id of destination
   * unit32_t dev id on destination
   * uint8_t[] serialized packet
   * \endinternal
   */
  virtual void SendPacket (Ptr<Packet> p, const Time &rxTime, uint32_t node, uint32_t dev);
  /**
   * \param guaranteeUpdate guarantee update time for the Null Message
   * \bundle the destination bundle for the Null Message.
   *
   * \brief Send a Null Message to across the specified bundle.  
   *
   * Guarantee update time is the lower bound time on the next
   * possible event from this MPI task to the remote MPI task across
   * the bundle.  Remote task may execute events up to time.
   *
   * Null Messages are sent when a packet has not been sent across
   * this bundle in order to allow time advancement on the remote
   * MPI task.
   *
   * \internal
   * The Null Message MPI buffer format is based on the format for sending a packet with
   * several fields set to 0 to signal that it is a Null Message.  Overloading the normal packet
   * format simplifies receive logic.
   *
   * uint64_t 0 must be zero for Null Message
   * uint64_t guarantee time
   * uint32_t 0 must be zero for Null Message
   * uint32_t 0 must be zero for Null Message
   * \endinternal
   */
  static void SendNullMessage (const Time& guaranteeUpdate, Ptr<RemoteChannelBundle> bundle);
  /**
   * Non-blocking check for received messages complete.  Will
   * receive all messages that are queued up locally.
   */
  static void ReceiveMessagesNonBlocking ();
  /**
   * Blocking message receive.  Will block until at least one message
   * has been received.
   */
  static void ReceiveMessagesBlocking ();
  /**
   * Check for completed sends
   */
  static void TestSendComplete ();

  /**
   * \brief Initialize send and receive buffers.
   *
   * This method should be called after all links have been added to the RemoteChannelBundle
   * manager to setup any required send and receive buffers.
   */
  static void InitializeSendReceiveBuffers (void);

private:

  /**
   * Check for received messages complete.  Will block until message
   * has been received if blocking flag is true.  When blocking will
   * return after the first message is received.   Non-blocking mode will
   * Non-blocking check for received messages complete.  Will
   * receive all messages that are queued up locally.
   */
  static void ReceiveMessages (bool blocking = false);

  // System ID (rank) for this task
  static uint32_t g_sid;

  // Size of the MPI COM_WORLD group.
  static uint32_t g_size;

  // Number of neighbor tasks, tasks that this task shares a link with.
  static uint32_t g_numNeighbors;

  static bool     g_initialized;
  static bool     g_enabled;

  // Pending non-blocking receives
  static MPI_Request* g_requests;

  // Data buffers for non-blocking receives
  static char**   g_pRxBuffers;

  // List of pending non-blocking sends
  static std::list<NullMessageSentBuffer> g_pendingTx;
};

} // namespace ns3

#endif /* NS3_NULL_MESSAGE_MPI_INTERFACE_H */
