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

#ifndef HANDOVER_ALGORITHM_H
#define HANDOVER_ALGORITHM_H

#include <ns3/object.h>
#include <ns3/lte-rrc-sap.h>

namespace ns3 {

class HandoverManagementSapProvider;
class HandoverManagementSapUser;


/**
 * \brief This abstract base class identifies the interface by means of which
 *        the helper object can plug on the eNodeB RRC a handover algorithm
 *        implementation.
 */
class HandoverAlgorithm : public Object
{
public:
  HandoverAlgorithm ();
  virtual ~HandoverAlgorithm ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  /**
   * \brief Set the user part of the HandoverManagementSap that this handover
   *        algorithm will interact with. Normally this part of the SAP is
   *        exported by the eNodeB RRC.
   * \param s
   */
  virtual void SetHandoverManagementSapUser (HandoverManagementSapUser* s) = 0;

  /**
   *
   * \return the Provider part of the HandoverManagementSap provided by the
   *         handover algorithm
   */
  virtual HandoverManagementSapProvider* GetHandoverManagementSapProvider () = 0;

}; // end of class HandoverAlgorithm



/**
 * \brief A sample implementation of the Handover Management SAP which simply
 *        does nothing.
 */
class BareHandoverAlgorithm : public HandoverAlgorithm
{
public:
  BareHandoverAlgorithm ();
  virtual ~BareHandoverAlgorithm ();

  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  // inherited from HandoverAlgorithm
  virtual void SetHandoverManagementSapUser (HandoverManagementSapUser* s);
  virtual HandoverManagementSapProvider* GetHandoverManagementSapProvider ();

  friend class BareMemberHandoverManagementSapProvider;

protected:
  // inherited from Object
  virtual void DoInitialize ();

private:

  // Handover Management SAP implementation
  void DoReportUeMeas (uint16_t rnti, LteRrcSap::MeasResults measResults);

  // Handover Management SAPs
  HandoverManagementSapUser* m_handoverManagementSapUser;
  HandoverManagementSapProvider* m_handoverManagementSapProvider;

}; // end of class BareHandoverAlgorithm



} // end of namespace ns3


#endif /* HANDOVER_ALGORITHM_H */
