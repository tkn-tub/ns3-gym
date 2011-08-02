/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef BEARER_QOS_PARAMETERS_H
#define BEARER_QOS_PARAMETERS_H

#include "ns3/object.h"

namespace ns3 {


/**
 * \ingroup lte
 *
 * \brief the BearerQosParameters class implements a set of Qos
 * parameters associated to the bearer.
 * Bearer Level Qos (TS 23.401, Clause 4.7.3)
 * A bearer uniquely identifies traffic flows that receive a
 * common Qos treatment.
 */
class BearerQosParameters : public Object
{
public:
  BearerQosParameters (void);

  /**
   * \brief Create a Qos Parameters Set
   *
   * \param qci the Qos Class Identifier
   * \param gbr the maximum bit rate to guarantee
   * \param mbr the minimum bit rate to guarantee
   */
  BearerQosParameters (int qci,
                       double gbr,
                       double mbr);

  /**
   * \brief Create a Qos Parameters Set
   *
   * \param qci the Qos Class Identifier
   * \param apec the Allocation and Retention Priority of pre-emption capability
   * \param apev the Allocation and Retention Priority of pre-emption vulnerability
   * \param gbr the maximum bit rate to guarantee
   * \param mbr the minimum bit rate to guarantee
   */
  BearerQosParameters (int qci,
                       bool apec,
                       bool apev,
                       double gbr,
                       double mbr);

  virtual ~BearerQosParameters (void);

  /**
   * Type og bearer (GBR or non GBR)
   */
  enum BearerQosType
  {
    BEARER_TYPE_GBR, BEARER_TYPE_NGBR
  };

  /**
   * \brief Set the Qos type of the bearer
   * \param QosType the Qos type
   */
  void SetBearerQosType (BearerQosType QosType);

  /**
   * \brief Get the Qos type of the bearer
   * \return the Qos tyope of the bearer
   */
  BearerQosType GetBearerQosType (void) const;


  /**
   * \param qci the Qos Class Identifier
   */
  void SetQci (int qci);
  /**
   * \return the QCI value
   */
  int GetQci (void) const;

  /**
   * \param apec the Allocation and Retention Priority of pre-emption capability
   */
  void SetArpPreEmptionCapability (bool apec);
  /**
   * \return the ArpPreEmptionCapability value
   */
  bool GetArpPreEmptionCapability (void) const;

  /**
   * \param apev the Allocation and Retention Priority of pre-emption vulnerability
   */
  void SetArpPreEmptionVulnerability (bool apev);
  /**
   * \return the ArpPreEmptionVulnerability value
   */
  bool GetArpPreEmptionVulnerability (void) const;

  /**
   * \param gbr the maximum bit rate to guarantee
   */
  void SetGbr (double gbr);
  /**
   * \return the maximum bit rate
   */
  double GetGbr (void) const;

  /**
   * \param mbr the minimum bit rate to guarantee
   */
  void SetMbr (double mbr);
  /**
   * \return the minimum bit rate
   */
  double GetMbr (void) const;

  /**
   * \param targetDelay the target delay
   */
  void SetMaxDelay (double targetDelay);
  /**
   * \return the targetDelay value
   */
  double GetMaxDelay (void) const;

private:
  BearerQosType m_bearerQosType;

  int m_qci;
  bool m_arpPreEmptionCapability;
  bool m_arpPreEmptionVulnerability;
  double m_gbr;
  double m_mbr;
  double m_maxDelay;
};


}
#endif /* BEARER_QOS_PARAMETERS_H */
