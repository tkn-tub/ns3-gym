/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef DCF_H
#define DCF_H

#include "ns3/object.h"

namespace ns3 {

/**
 * \ingroup wifi
 *
 */
class Dcf : public Object
{
public:
  static TypeId GetTypeId (void);

  virtual void SetMinCw (uint32_t minCw) = 0;
  virtual void SetMaxCw (uint32_t maxCw) = 0;
  virtual void SetAifsn (uint32_t aifsn) = 0;
  virtual uint32_t GetMinCw (void) const = 0;
  virtual uint32_t GetMaxCw (void) const = 0;
  virtual uint32_t GetAifsn (void) const = 0;
};

} // namespace ns3

#endif /* DCF_H */
