/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

  /**
   * Set the minimum congestion window size.
   *
   * \param minCw the minimum congestion window size
   */
  virtual void SetMinCw (uint32_t minCw) = 0;
  /**
   * Set the maximum congestion window size.
   *
   * \param maxCw the maximum congestion window size
   */
  virtual void SetMaxCw (uint32_t maxCw) = 0;
  /**
   * \param aifsn the number of slots which make up an AIFS for a specific DCF.
   *        a DIFS corresponds to an AIFSN = 2.
   *
   * Calling this method after DcfManager::Add has been called is not recommended.
   */
  virtual void SetAifsn (uint32_t aifsn) = 0;
  /**
   * Return the minimum congestion window size.
   *
   * \return the minimum congestion window size
   */
  virtual uint32_t GetMinCw (void) const = 0;
  /**
   * Return the maximum congestion window size.
   *
   * \return the maximum congestion window size
   */
  virtual uint32_t GetMaxCw (void) const = 0;
  /**
   * Return the number of slots that make up an AIFS.
   *
   * \return the number of slots that make up an AIFS
   */
  virtual uint32_t GetAifsn (void) const = 0;
};

} // namespace ns3

#endif /* DCF_H */
