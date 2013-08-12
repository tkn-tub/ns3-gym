/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 University of Jyvaskyla
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
 * Author: Budiarto Herman <buherman@student.jyu.fi>
 *
 */

#ifndef LTE_HANDOVER_ALGORITHM_H
#define LTE_HANDOVER_ALGORITHM_H

#include <ns3/object.h>
#include <ns3/lte-rrc-sap.h>

namespace ns3 {


class LteHandoverManagementSapUser;
class LteHandoverManagementSapProvider;


/**
 * \brief This abstract base class identifies the interface by means of which
 *        the helper object can plug on the eNodeB RRC a handover algorithm
 *        implementation.
 */
class LteHandoverAlgorithm : public Object
{
public:
  LteHandoverAlgorithm ();
  virtual ~LteHandoverAlgorithm ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  /**
   * \brief Set the user part of the Handover Management SAP that this handover
   *        algorithm will interact with. Normally this part of the SAP is
   *        exported by the eNodeB RRC.
   * \param s
   */
  virtual void SetLteHandoverManagementSapUser (LteHandoverManagementSapUser* s) = 0;

  /**
   *
   * \return the Provider part of the Handover Management SAP provided by the
   *         handover algorithm
   */
  virtual LteHandoverManagementSapProvider* GetLteHandoverManagementSapProvider () = 0;

}; // end of class LteHandoverAlgorithm


} // end of namespace ns3


#endif /* LTE_HANDOVER_ALGORITHM_H */
