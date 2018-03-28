/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Sébastien Deronne
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
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

#ifndef ERP_INFORMATION_H
#define ERP_INFORMATION_H

#include "wifi-information-element.h"

namespace ns3 {

/**
 * \brief The ErpInformation Information Element
 * \ingroup wifi
 *
 * This class knows how to serialise and deserialise the ErpInformation Information Element.
 */
class ErpInformation : public WifiInformationElement
{
public:
  ErpInformation ();
  /**
   * Set the ERP supported field.
   *
   * \param erpSupported the ERP supported field in the ErpInformation information element
   */
  void SetErpSupported (uint8_t erpSupported);

  /**
   * Set the Barker_Preamble_Mode field in the ErpInformation information element.
   *
   * \param barkerPreambleMode the Barker_Preamble_Mode field in the ErpInformation information element
   */
  void SetBarkerPreambleMode (uint8_t barkerPreambleMode);
  /**
   * Set the Use_Protection field in the ErpInformation information element.
   *
   * \param useProtection the Use_Protection field in the ErpInformation information element
   */
  void SetUseProtection (uint8_t useProtection);
  /**
   * Set the Non_Erp_Present field in the ErpInformation information element.
   *
   * \param nonErpPresent the Non_Erp_Present field in the ErpInformation information element
   */
  void SetNonErpPresent (uint8_t nonErpPresent);

  /**
   * Return the Barker_Preamble_Mode field in the ErpInformation information element.
   *
   * \return the Barker_Preamble_Mode field in the ErpInformation information element
   */
  uint8_t GetBarkerPreambleMode (void) const;
  /**
   * Return the Use_Protection field in the ErpInformation information element.
   *
   * \return the Use_Protection field in the ErpInformation information element
   */
  uint8_t GetUseProtection (void) const;
  /**
   * Return the Non_Erp_Present field in the ErpInformation information element.
   *
   * \return the Non_Erp_Present field in the ErpInformation information element
   */
  uint8_t GetNonErpPresent (void) const;

  /**
   * Get the element ID field.
   *
   * \returns WifiInformationElementId
   */
  WifiInformationElementId ElementId () const;
  /**
   * Get the information field size.
   *
   * \returns the element field size
   */
  uint8_t GetInformationFieldSize () const;
  /**
   * Serialize the information field.
   *
   * \param start the information element iterator
   */
  void SerializeInformationField (Buffer::Iterator start) const;
  /**
   * Get the information field size.
   *
   * \param start the information element iterator
   * \param length the information field length
   *
   * \returns the information field
   */
  uint8_t DeserializeInformationField (Buffer::Iterator start,
                                       uint8_t length);

  /**
   * This information element is a bit special in that it is only
   * included if the STA is an ERP STA. To support this we
   * override the Serialize and GetSerializedSize methods of
   * WifiInformationElement.
   *
   * \param start
   *
   * \return an iterator
   */
  Buffer::Iterator Serialize (Buffer::Iterator start) const;
  /**
   * Return the serialized size of this ErpInformation information element.
   *
   * \return the serialized size of this ErpInformation information element
   */
  uint16_t GetSerializedSize () const;

private:
  uint8_t m_erpInformation; ///< ERP information

  /// This is used to decide whether this element should be added to the frame or not
  bool m_erpSupported;
};

/**
 * output stream output operator
 *
 * \param os output stream
 * \param erpinformation
 *
 * \returns output stream
 */
std::ostream &operator << (std::ostream &os, const ErpInformation &erpinformation);

} //namespace ns3

#endif /* ERP_INFORMATION_H */
