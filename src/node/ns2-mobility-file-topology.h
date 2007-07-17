/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef NS2_MOBILE_FILE_TOPOLOGY_H
#define NS2_MOBILE_FILE_TOPOLOGY_H

#include <string>
#include <stdint.h>
#include "ns3/ptr.h"
#include "ns3/object.h"
#include "static-speed-mobility-model.h"

namespace ns3 {

class Ns2MobileFileTopology
{
public:
  Ns2MobileFileTopology (std::string filename);

  void Layout (void) const;
  template <typename T>
  void Layout (T begin, T end) const;
private:
  class ObjectStore
  {
  public:
    virtual ~ObjectStore () {}
    virtual Ptr<Object> Get (uint32_t i) const = 0;
  };
  void LayoutObjectStore (const ObjectStore &store) const;
  Ptr<StaticSpeedMobilityModel> GetMobilityModel (std::string idString, const ObjectStore &store) const;
  double ReadDouble (std::string valueString) const;
  std::string m_filename;
};

} // namespace ns3

namespace ns3 {

template <typename T>
void 
Ns2MobileFileTopology::Layout (T begin, T end) const
{
  class MyObjectStore : public ObjectStore
  {
  public:
    MyObjectStore (T begin, T end)
      : m_begin (begin),
      m_end (end)
	{}
    virtual Ptr<Object> Get (uint32_t i) const {
      T iterator = m_begin;
      iterator += i;
      if (iterator >= m_end)
	{
	  return 0;
	}
      return *iterator;
    }
  private:
    T m_begin;
    T m_end;
  };
  LayoutObjectStore (MyObjectStore (begin, end));
}


} // namespace ns3

#endif /* NS2_MOBILE_FILE_TOPOLOGY_H */
