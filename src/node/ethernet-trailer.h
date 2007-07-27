/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Emmanuelle Laprise
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
 * Author: Emmanuelle Laprise <emmanuelle.laprise@bluekazoo.ca>
 */

#ifndef ETHERNET_TRAILER_H
#define ETHERNET_TRAILER_H

#include "ns3/trailer.h"
#include "ns3/packet.h"

namespace ns3 {
/**
 * \brief Packet trailer for Ethernet
 *
 * This class can be used to add and verify the FCS at the end of an
 * ethernet packet. The actual FCS functionality is not yet coded and
 * so this acts more as a placeholder.
 */
class EthernetTrailer : public Trailer {
public:
  /**
   * \brief Construct a null ethernet trailer
   */
  EthernetTrailer ();
  virtual ~EthernetTrailer ();
  /**
   * \brief Enable or disabled FCS checking and calculations
   * \param enable If true, enables FCS calculations.
   */
  static void EnableFcs (bool enable);
  /**
   * \brief Updates the Fcs Field to the correct FCS
   * \param p Reference to a packet on which the FCS should be
   * calculated. The packet should not currently contain an FCS
   * trailer.
   */
  void CalcFcs (const Packet& p);
  /**
   * \brief Sets the FCS to a new value
   * \param fcs New FCS value
   */
  void SetFcs (uint32_t fcs);
  /**
   * \return the FCS contained in this trailer
   */
  uint32_t GetFcs ();

  /**
   * \param p Reference to the packet on which the FCS should be
   * calculated. The packet should not contain an FCS trailer.
   * \return Returns true if the packet fcs is correct, false otherwise.
   *
   * If FCS checking is disabled, this method will always
   * return true.
   */
  bool CheckFcs (const Packet& p) const;

  /**
   *\return Returns the size of the trailer
   */ 
  uint32_t GetTrailerSize() const;

private:
  virtual std::string DoGetName (void) const;
  virtual void PrintTo (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void SerializeTo (Buffer::Iterator end) const;
  virtual uint32_t DeserializeFrom (Buffer::Iterator end);

  /**
   * Initializes the trailer parameters during construction.
   */
  void Init (void);

  /**
   * Enabled FCS calculations. If false, fcs is set to 0 and checkFCS
   * returns true.
   */
  static bool m_calcFcs;
  uint32_t m_fcs; /// Value of the fcs contained in the trailer

};

}; // namespace ns3


#endif /* ETHERNET_TRAILER_H */
