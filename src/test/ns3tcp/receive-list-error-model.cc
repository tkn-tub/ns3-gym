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
 * This code should be moved to src/common/error-model.h during ns-3.9 
 * release cycle with some minor modifications, such as adding GetTypeId 
 * method and logging to all methods. This can be done by uncommenting 
 * relavent code below.
 */

#include "ns3/packet.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/random-variable.h"
#include "ns3/boolean.h"
#include "ns3/enum.h"
#include "ns3/double.h"

#include "receive-list-error-model.h"

namespace ns3 {

//
// ReceiveListErrorModel
//

//NS_OBJECT_ENSURE_REGISTERED (ReceiveListErrorModel);


/*
TypeId ReceiveListErrorModel::GetTypeId (void)
{ 
  static TypeId tid = TypeId ("ns3::ReceiveListErrorModel")
    .SetParent<ErrorModel> ()
    .AddConstructor<ReceiveListErrorModel> ()
    ;
  return tid;
}
*/

ReceiveListErrorModel::ReceiveListErrorModel () :
  m_timesInvoked (0)
{
  //NS_LOG_FUNCTION_NOARGS ();
}

ReceiveListErrorModel::~ReceiveListErrorModel () 
{
  //NS_LOG_FUNCTION_NOARGS ();
}

std::list<uint32_t> 
ReceiveListErrorModel::GetList (void) const 
{ 
  //NS_LOG_FUNCTION_NOARGS ();
  return m_packetList; 
}

void 
ReceiveListErrorModel::SetList (const std::list<uint32_t> &packetlist)
{ 
  //NS_LOG_FUNCTION_NOARGS ();
  m_packetList = packetlist;
}

bool 
ReceiveListErrorModel::DoCorrupt (Ptr<Packet> p) 
{ 
  //NS_LOG_FUNCTION_NOARGS ();
  if (!IsEnabled ())
    {
      return false;  
    }
  m_timesInvoked += 1;
  for (PacketListCI i = m_packetList.begin (); 
    i != m_packetList.end (); i++) 
    {
      if (m_timesInvoked - 1 == *i)
      {
        return true;
      }
    }
  return false;
}

void 
ReceiveListErrorModel::DoReset (void) 
{ 
  //NS_LOG_FUNCTION_NOARGS ();
  m_packetList.clear();
}


} //namespace ns3
