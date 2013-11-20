/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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

#include "ns3/simulator.h"
#include "ns3/object-vector.h"
#include "ns3/config.h"
#include "ns3/log.h"
#include "ns3/assert.h"
#include "channel-list.h"
#include "channel.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ChannelList")
  ;

/**
 * \brief private implementation detail of the ChannelList API.
 */
class ChannelListPriv : public Object
{
public:
  static TypeId GetTypeId (void);
  ChannelListPriv ();
  ~ChannelListPriv ();

  uint32_t Add (Ptr<Channel> channel);

  ChannelList::Iterator Begin (void) const;
  ChannelList::Iterator End (void) const;

  Ptr<Channel> GetChannel (uint32_t n);
  uint32_t GetNChannels (void);

  static Ptr<ChannelListPriv> Get (void);

private:
  static Ptr<ChannelListPriv> *DoGet (void);
  static void Delete (void);
  virtual void DoDispose (void);
  std::vector<Ptr<Channel> > m_channels;
};

NS_OBJECT_ENSURE_REGISTERED (ChannelListPriv)
  ;

TypeId 
ChannelListPriv::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ChannelListPriv")
    .SetParent<Object> ()
    .AddAttribute ("ChannelList", "The list of all channels created during the simulation.",
                   ObjectVectorValue (),
                   MakeObjectVectorAccessor (&ChannelListPriv::m_channels),
                   MakeObjectVectorChecker<Channel> ())
  ;
  return tid;
}

Ptr<ChannelListPriv> 
ChannelListPriv::Get (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return *DoGet ();
}

Ptr<ChannelListPriv> *
ChannelListPriv::DoGet (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  static Ptr<ChannelListPriv> ptr = 0;
  if (ptr == 0)
    {
      ptr = CreateObject<ChannelListPriv> ();
      Config::RegisterRootNamespaceObject (ptr);
      Simulator::ScheduleDestroy (&ChannelListPriv::Delete);
    }
  return &ptr;
}

void 
ChannelListPriv::Delete (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Config::UnregisterRootNamespaceObject (Get ());
  (*DoGet ()) = 0;
}

ChannelListPriv::ChannelListPriv ()
{
  NS_LOG_FUNCTION (this);
}

ChannelListPriv::~ChannelListPriv ()
{
  NS_LOG_FUNCTION (this);
}
void
ChannelListPriv::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  for (std::vector<Ptr<Channel> >::iterator i = m_channels.begin ();
       i != m_channels.end (); i++)
    {
      Ptr<Channel> channel = *i;
      channel->Dispose ();
      *i = 0;
    }
  m_channels.erase (m_channels.begin (), m_channels.end ());
  Object::DoDispose ();
}

uint32_t
ChannelListPriv::Add (Ptr<Channel> channel)
{
  NS_LOG_FUNCTION (this << channel);
  uint32_t index = m_channels.size ();
  m_channels.push_back (channel);
  return index;

}

ChannelList::Iterator 
ChannelListPriv::Begin (void) const
{
  NS_LOG_FUNCTION (this);
  return m_channels.begin ();
}

ChannelList::Iterator 
ChannelListPriv::End (void) const
{
  NS_LOG_FUNCTION (this);
  return m_channels.end ();
}

uint32_t 
ChannelListPriv::GetNChannels (void)
{
  NS_LOG_FUNCTION (this);
  return m_channels.size ();
}

Ptr<Channel>
ChannelListPriv::GetChannel (uint32_t n)
{
  NS_LOG_FUNCTION (this << n);
  NS_ASSERT_MSG (n < m_channels.size (), "Channel index " << n <<
                 " is out of range (only have " << m_channels.size () << " channels).");
  return m_channels[n];
}

uint32_t
ChannelList::Add (Ptr<Channel> channel)
{
  NS_LOG_FUNCTION_NOARGS ();
  return ChannelListPriv::Get ()->Add (channel);
}

ChannelList::Iterator 
ChannelList::Begin (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return ChannelListPriv::Get ()->Begin ();
}

ChannelList::Iterator 
ChannelList::End (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return ChannelListPriv::Get ()->End ();
}

Ptr<Channel>
ChannelList::GetChannel (uint32_t n)
{
  NS_LOG_FUNCTION (n);
  return ChannelListPriv::Get ()->GetChannel (n);
}

uint32_t
ChannelList::GetNChannels (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return ChannelListPriv::Get ()->GetNChannels ();
}

} // namespace ns3
