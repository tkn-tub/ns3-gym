/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Danilo Abrignani
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
 * Author: Danilo Abrignani <danilo.abrignani@unibo.it>
 *
 */

#ifndef LTE_CCM_MAC_SAP_H
#define LTE_CCM_MAC_SAP_H

#include <ns3/lte-rrc-sap.h>
#include <ns3/eps-bearer.h>
#include <ns3/lte-enb-cmac-sap.h>
#include <ns3/lte-mac-sap.h>
#include <ns3/ff-mac-common.h>


namespace ns3 {
/**
 * \ingroup lte
 *
 * \brief Service Access Point (SAP) offered by the component carrier manager (CCM) 
 *  by MAC to CCM.
 *
 * This is the *Component Carrier Manager SAP Provider*, i.e., the part of the SAP
 * that contains the MAC methods called by the eNodeB CCM
 * instance.
 */
class LteCcmMacSapProvider
{
 
public:
  virtual ~LteCcmMacSapProvider ();

  /**
   * \brief Add the Buffer Status Report to the list.
   * \param bsr LteEnbComponentCarrierManager used this function to
   *  send back an uplink BSR to some of the MAC instances
   */
  virtual void ReportMacCeToScheduler (MacCeListElement_s bsr) = 0;

}; // end of class LteCcmMacSapProvider


/**
 * \ingroup lte
 *
 * \brief Service Access Point (SAP) offered by MAC to the 
 *        component carrier manager (CCM).
 *  
 *
 * This is the *CCM MAC SAP User*, i.e., the part of the SAP
 * that contains the component carrier manager methods called 
 * by the eNodeB MAC instance.
 */
class LteCcmMacSapUser : public LteMacSapUser
{
public:
  virtual ~LteCcmMacSapUser ();
  /**
   * \brief When the Primary Component carrier receive a buffer status report 
   *  it is sent to the CCM.
   * \param bsr Buffer Status Report received from a Ue
   * \param componentCarrierId
   */
  virtual void UlReceiveMacCe (MacCeListElement_s bsr, uint8_t componentCarrierId) = 0;

  /**
   * \brief Notifies component carrier manager about physical resource block occupancy
   * \param prbOccupancy The physical resource block occupancy
   * \param componentCarrierId The component carrier id
   */
  virtual void NotifyPrbOccupancy (double prbOccupancy, uint8_t componentCarrierId) = 0;

}; // end of class LteCcmMacSapUser

/// MemberLteCcmMacSapProvider class
template <class C>
class MemberLteCcmMacSapProvider : public LteCcmMacSapProvider
{
public:
  /**
   * Constructor
   *
   * \param owner the owner class
   */
  MemberLteCcmMacSapProvider (C* owner);
  // inherited from LteCcmRrcSapProvider
  virtual void ReportMacCeToScheduler (MacCeListElement_s bsr);

private:
  C* m_owner; ///< the owner class
};

template <class C>
MemberLteCcmMacSapProvider<C>::MemberLteCcmMacSapProvider (C* owner)
  : m_owner (owner)
{
}
 
template <class C>
void MemberLteCcmMacSapProvider<C>::ReportMacCeToScheduler (MacCeListElement_s bsr)
{
  m_owner->DoReportMacCeToScheduler (bsr);
}


/// MemberLteCcmMacSapUser class
template <class C>
class MemberLteCcmMacSapUser : public LteCcmMacSapUser
{
public:
  /**
   * Constructor
   *
   * \param owner the owner class
   */
  MemberLteCcmMacSapUser (C* owner);
  // inherited from LteCcmRrcSapUser
  virtual void UlReceiveMacCe (MacCeListElement_s bsr, uint8_t componentCarrierId);
  virtual void NotifyPrbOccupancy (double prbOccupancy, uint8_t componentCarrierId);
  // inherited from LteMacSapUser
  virtual void NotifyTxOpportunity (LteMacSapUser::TxOpportunityParameters txOpParams);
  virtual void ReceivePdu (LteMacSapUser::ReceivePduParameters rxPduParams);
  virtual void NotifyHarqDeliveryFailure ();


private:
  C* m_owner; ///< the owner class
};

template <class C>
MemberLteCcmMacSapUser<C>::MemberLteCcmMacSapUser (C* owner)
  : m_owner (owner)
{
}

template <class C>
void MemberLteCcmMacSapUser<C>::UlReceiveMacCe (MacCeListElement_s bsr, uint8_t componentCarrierId)
{
  m_owner->DoUlReceiveMacCe (bsr, componentCarrierId);
}

template <class C>
void MemberLteCcmMacSapUser<C>::NotifyPrbOccupancy (double prbOccupancy, uint8_t componentCarrierId)
{
  m_owner->DoNotifyPrbOccupancy (prbOccupancy, componentCarrierId);
}

template <class C>
void MemberLteCcmMacSapUser<C>::NotifyTxOpportunity (LteMacSapUser::TxOpportunityParameters txOpParams)
{
  m_owner->DoNotifyTxOpportunity (txOpParams);
}

template <class C>
void MemberLteCcmMacSapUser<C>::ReceivePdu (LteMacSapUser::ReceivePduParameters rxPduParams)
{
  m_owner->DoReceivePdu (rxPduParams);
}

template <class C>
void MemberLteCcmMacSapUser<C>::NotifyHarqDeliveryFailure ()
{
  m_owner->DoNotifyHarqDeliveryFailure ();
}

  
} // end of namespace ns3


#endif /* LTE_CCM_MAC_SAP_H */

