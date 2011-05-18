/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */


#ifndef LTE_MAC_HEADER_H
#define LTE_MAC_HEADER_H

#include <ns3/header.h>
#include <ns3/mac48-address.h>
#include <ns3/address-utils.h>

namespace ns3 {
/**
 * \ingroup lte
 *
 * This class implements the LTE MAC header
 */
class LteMacHeader : public Header
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

  /**
   * \brief set the source MAC address
   * \param source the source mac address
   */
  void SetSource (Mac48Address source);
  /**
   * \brief set the the destination MAC address
   * \param destination the destination MAC address
   */
  void SetDestination (Mac48Address destination);

  /**
   * \brief get the the source MAC address
   * \return the source MAC address
   */
  Mac48Address GetSource () const;
  /**
   * \brief get the the destination MAC address
   * \return the destination MAC address
   */
  Mac48Address GetDestination () const;

private:
  Mac48Address m_source;
  Mac48Address m_destination;
};



} // namespace ns3

#endif /* LTE_MAC_HEADER_H */
