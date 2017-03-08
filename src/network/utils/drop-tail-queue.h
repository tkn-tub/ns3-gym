/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
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
 */

#ifndef DROPTAIL_H
#define DROPTAIL_H

#include <queue>
#include "ns3/queue.h"

namespace ns3 {

/**
 * \ingroup queue
 *
 * \brief A FIFO packet queue that drops tail-end packets on overflow
 */
template <typename Item>
class DropTailQueue : public Queue<Item>
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \brief DropTailQueue Constructor
   *
   * Creates a droptail queue with a maximum size of 100 packets by default
   */
  DropTailQueue ();

  virtual ~DropTailQueue ();

private:
  virtual bool DoEnqueue (Ptr<Item> item);
  virtual Ptr<Item> DoDequeue (void);
  virtual Ptr<Item> DoRemove (void);
  virtual Ptr<const Item> DoPeek (void) const;

  std::queue<Ptr<Item> > m_packets;         //!< the items in the queue
};


/**
 * Implementation of the templates declared above.
 */

template <typename Item>
TypeId
DropTailQueue<Item>::GetTypeId (void)
{
  static TypeId tid = TypeId (("ns3::DropTailQueue<" + GetTypeParamName<DropTailQueue<Item> > () + ">").c_str ())
    .SetParent<Queue<Item> > ()
    .SetGroupName ("Network")
    .template AddConstructor<DropTailQueue<Item> > ()
  ;
  return tid;
}

template <typename Item>
DropTailQueue<Item>::DropTailQueue () :
  Queue<Item> (),
  m_packets ()
{
  QUEUE_LOG (LOG_LOGIC, "DropTailQueue(" << this << ")");
}

template <typename Item>
DropTailQueue<Item>::~DropTailQueue ()
{
  QUEUE_LOG (LOG_LOGIC, "~DropTailQueue(" << this << ")");
}

template <typename Item>
bool
DropTailQueue<Item>::DoEnqueue (Ptr<Item> item)
{
  QUEUE_LOG (LOG_LOGIC, "DropTailQueue:DoEnqueue(" << this << ", " << item << ")");
  NS_ASSERT (m_packets.size () == this->GetNPackets ());

  m_packets.push (item);

  return true;
}

template <typename Item>
Ptr<Item>
DropTailQueue<Item>::DoDequeue (void)
{
  QUEUE_LOG (LOG_LOGIC, "DropTailQueue:DoDequeue(" << this << ")");
  NS_ASSERT (m_packets.size () == this->GetNPackets ());

  Ptr<Item> item = m_packets.front ();
  m_packets.pop ();

  QUEUE_LOG (LOG_LOGIC, "Popped " << item);

  return item;
}

template <typename Item>
Ptr<Item>
DropTailQueue<Item>::DoRemove (void)
{
  QUEUE_LOG (LOG_LOGIC, "DropTailQueue:DoRemove(" << this << ")");
  NS_ASSERT (m_packets.size () == this->GetNPackets ());

  Ptr<Item> item = m_packets.front ();
  m_packets.pop ();

  QUEUE_LOG (LOG_LOGIC, "Removed " << item);

  return item;
}

template <typename Item>
Ptr<const Item>
DropTailQueue<Item>::DoPeek (void) const
{
  QUEUE_LOG (LOG_LOGIC, "DropTailQueue:DoPeek(" << this << ")");
  NS_ASSERT (m_packets.size () == this->GetNPackets ());

  return m_packets.front ();
}

} // namespace ns3

#endif /* DROPTAIL_H */
