/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include <string>

#include "ns3/attribute.h"
#include "ns3/object.h"

struct INetStack;

namespace ns3 {

// This object represents the underlying nsc stack,
// which is aggregated to a Node object, and which provides access to the
// sysctls of the nsc stack through attributes.
class Ns3NscStack : public Object
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void SetStack (INetStack *stack) { m_stack = stack; }

private:
  friend class NscStackStringAccessor;
  void Set (std::string name, std::string value);
  std::string Get (std::string name) const;
  INetStack *m_stack;
};
} // namespace ns3
