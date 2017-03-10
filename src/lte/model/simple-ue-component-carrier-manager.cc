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

#include "simple-ue-component-carrier-manager.h"
#include <ns3/log.h>
#include <ns3/lte-ue-mac.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SimpleUeComponentCarrierManager");

NS_OBJECT_ENSURE_REGISTERED (SimpleUeComponentCarrierManager);

///////////////////////////////////////////////////////////
// SAP forwarders
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// MAC SAP PROVIDER SAP forwarders
///////////////////////////////////////////////////////////

/// SimpleUeCcmMacSapProvider class
class SimpleUeCcmMacSapProvider : public LteMacSapProvider
{ 
public:
  /**
   * Constructor
   *
   * \param mac the component carrier manager
   */
  SimpleUeCcmMacSapProvider (SimpleUeComponentCarrierManager* mac);

  // inherited from LteMacSapProvider
  virtual void TransmitPdu (LteMacSapProvider::TransmitPduParameters params);
  virtual void ReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters params);

private:
  SimpleUeComponentCarrierManager* m_mac; ///< the component carrier manager
};

SimpleUeCcmMacSapProvider::SimpleUeCcmMacSapProvider (SimpleUeComponentCarrierManager* mac)
  : m_mac (mac)
{
}

void
SimpleUeCcmMacSapProvider::TransmitPdu (TransmitPduParameters params)
{
  m_mac->DoTransmitPdu (params);
}


void
SimpleUeCcmMacSapProvider::ReportBufferStatus (ReportBufferStatusParameters params)
{
  m_mac->DoReportBufferStatus (params);
}

///////////////////////////////////////////////////////////
// MAC SAP USER SAP forwarders
/////////////// ////////////////////////////////////////////

/// SimpleUeCcmMacSapUser class
class SimpleUeCcmMacSapUser : public LteMacSapUser
{ 
public:
  /**
   * Constructor
   *
   * \param mac the component carrier manager
   */
  SimpleUeCcmMacSapUser  (SimpleUeComponentCarrierManager* mac);

  // inherited from LteMacSapUser
  virtual void NotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId, uint8_t componentCarrierId, uint16_t rnti, uint8_t lcid);
  virtual void ReceivePdu (Ptr<Packet> p, uint16_t rnti, uint8_t lcid);
  virtual void NotifyHarqDeliveryFailure ();


private:
  SimpleUeComponentCarrierManager* m_mac; ///< the component carrier manager
};

SimpleUeCcmMacSapUser::SimpleUeCcmMacSapUser (SimpleUeComponentCarrierManager* mac)
  : m_mac (mac)
{
}

void
SimpleUeCcmMacSapUser::NotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId, uint8_t componentCarrierId, uint16_t rnti, uint8_t lcid)
{
  NS_LOG_INFO ("SimpleUeCcmMacSapUser::NotifyTxOpportunity for ccId:"<<(uint32_t)componentCarrierId);
  m_mac->DoNotifyTxOpportunity (bytes, layer, harqId, componentCarrierId, rnti, lcid);
}


void
SimpleUeCcmMacSapUser::ReceivePdu (Ptr<Packet> p, uint16_t rnti, uint8_t lcid)
{
  m_mac->DoReceivePdu (p, rnti, lcid);
}

void
SimpleUeCcmMacSapUser::NotifyHarqDeliveryFailure ()
{
  m_mac->DoNotifyHarqDeliveryFailure ();
}

//////////////////////////////////////////////////////////
// SimpleUeComponentCarrierManager methods
///////////////////////////////////////////////////////////

SimpleUeComponentCarrierManager::SimpleUeComponentCarrierManager ()
: m_ccmRrcSapUser (0)
{
  NS_LOG_FUNCTION (this);
  m_ccmRrcSapProvider = new MemberLteUeCcmRrcSapProvider<SimpleUeComponentCarrierManager> (this);
  m_noOfComponentCarriersEnabled = 1;
  m_ccmMacSapUser = new SimpleUeCcmMacSapUser (this);
  m_ccmMacSapProvider = new SimpleUeCcmMacSapProvider (this);
}


SimpleUeComponentCarrierManager::~SimpleUeComponentCarrierManager ()
{
  NS_LOG_FUNCTION (this);

}


void
SimpleUeComponentCarrierManager::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  delete m_ccmMacSapUser;
  delete m_ccmMacSapProvider;
  delete m_ccmRrcSapProvider;
}


TypeId
SimpleUeComponentCarrierManager::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::SimpleUeComponentCarrierManager")
    .SetParent<LteUeComponentCarrierManager> ()
    .SetGroupName("Lte")
    .AddConstructor<SimpleUeComponentCarrierManager> ()
    ;
  return tid;
}

LteMacSapProvider*
SimpleUeComponentCarrierManager::GetLteMacSapProvider ()
{
  NS_LOG_FUNCTION (this);
  return m_ccmMacSapProvider;
}

void
SimpleUeComponentCarrierManager::SetLteCcmRrcSapUser (LteUeCcmRrcSapUser* s)
{
  NS_LOG_FUNCTION (this << s);
  m_ccmRrcSapUser = s;
}


LteUeCcmRrcSapProvider*
SimpleUeComponentCarrierManager::GetLteCcmRrcSapProvider ()
{
  NS_LOG_FUNCTION (this);
  return m_ccmRrcSapProvider;
}


void
SimpleUeComponentCarrierManager::DoInitialize ()
{
  NS_LOG_FUNCTION (this);
  LteUeComponentCarrierManager::DoInitialize ();
}


void
SimpleUeComponentCarrierManager::DoReportUeMeas (uint16_t rnti,
                                                 LteRrcSap::MeasResults measResults)
{
  NS_LOG_FUNCTION (this << rnti << (uint16_t) measResults.measId);
}


void
SimpleUeComponentCarrierManager::DoTransmitPdu (LteMacSapProvider::TransmitPduParameters params)
{
  NS_LOG_FUNCTION (this);
   std::map <uint8_t, LteMacSapProvider*>::iterator it =  m_macSapProvidersMap.find (params.componentCarrierId);
  NS_ASSERT_MSG (it != m_macSapProvidersMap.end (), "could not find Sap for ComponentCarrier " << (uint16_t) params.componentCarrierId);
  // with this algorithm all traffic is on Primary Carrier
  it->second->TransmitPdu (params);
}

void
SimpleUeComponentCarrierManager::DoReportBufferStatus (LteMacSapProvider::ReportBufferStatusParameters params)
{
  NS_LOG_FUNCTION (this);
  std::map <uint8_t, LteMacSapProvider*>::iterator it =  m_macSapProvidersMap.find (0);
  NS_ASSERT_MSG (it != m_macSapProvidersMap.end (), "could not find Sap for ComponentCarrier ");
  it->second->ReportBufferStatus (params);
}

void 
SimpleUeComponentCarrierManager::DoNotifyHarqDeliveryFailure ()
{
 NS_LOG_FUNCTION (this);
}


void 
SimpleUeComponentCarrierManager::DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer, uint8_t harqId, uint8_t componentCarrierId, uint16_t rnti, uint8_t lcid)
{
  NS_LOG_FUNCTION (this);
  std::map<uint8_t, LteMacSapUser*>::iterator lcidIt = m_lcAttached.find (lcid);
  NS_ASSERT_MSG (lcidIt != m_lcAttached.end (), "could not find LCID" << lcid);
  NS_LOG_DEBUG (this << " lcid= " << (uint32_t) lcid << " layer= " << (uint16_t) layer << " componentCarierId " << (uint16_t) componentCarrierId << " rnti " << rnti);
  (*lcidIt).second->NotifyTxOpportunity (bytes, layer, harqId, componentCarrierId, rnti, lcid);
}
void
SimpleUeComponentCarrierManager::DoReceivePdu (Ptr<Packet> p, uint16_t rnti, uint8_t lcid)
{
  NS_LOG_FUNCTION (this);
  std::map<uint8_t, LteMacSapUser*>::iterator lcidIt = m_lcAttached.find (lcid);
  if (lcidIt != m_lcAttached.end ())
    {
      (*lcidIt).second->ReceivePdu (p, rnti, lcid);
    }
}

///////////////////////////////////////////////////////////
// Ue CCM RRC SAP PROVIDER SAP forwarders
///////////////////////////////////////////////////////////
std::vector<uint16_t>
SimpleUeComponentCarrierManager::DoRemoveLc (uint8_t lcid)
{
  NS_LOG_FUNCTION (this << " lcId" << lcid);
  std::vector<uint16_t> res;
  NS_ASSERT_MSG (m_lcAttached.find (lcid) != m_lcAttached.end (), "could not find LCID " << lcid);
  m_lcAttached.erase (lcid);
  // send back all the configuration to the componentCarrier where we want to remove the Lc
  std::map<uint8_t, std::map<uint8_t, LteMacSapProvider*> >::iterator it =  m_componentCarrierLcMap.begin ();
  while (it != m_componentCarrierLcMap.end ())
    {
      std::map<uint8_t, LteMacSapProvider*>::iterator lcToRemove = it->second.find (lcid);
      if (lcToRemove != it->second.end ())
        {
          res.insert (res.end (), it->first);
        }
      it++;
    }
  NS_ASSERT_MSG (res.size () != 0, "Not found in the ComponentCarrierManager maps the LCID " << lcid);

  return res; 

}

std::vector<LteUeCcmRrcSapProvider::LcsConfig>
SimpleUeComponentCarrierManager::DoAddLc (uint8_t lcId,  LteUeCmacSapProvider::LogicalChannelConfig lcConfig, LteMacSapUser* msu)
{
  NS_LOG_FUNCTION (this);
  std::vector<LteUeCcmRrcSapProvider::LcsConfig> res;
  std::map<uint8_t, LteMacSapUser*>::iterator it = m_lcAttached.find (lcId);
  NS_ASSERT_MSG (it == m_lcAttached.end (), "Warning, LCID " << lcId << " already exist");
  m_lcAttached.insert (std::pair<uint8_t, LteMacSapUser*> (lcId, msu));
  LteUeCcmRrcSapProvider::LcsConfig elem;
  std::map <uint8_t, std::map<uint8_t, LteMacSapProvider*> >::iterator ccLcMapIt;
  for (uint8_t ncc = 0; ncc < m_noOfComponentCarriersEnabled; ncc++)
    {
      elem.componentCarrierId = ncc;
      elem.lcConfig = &lcConfig;
      elem.msu = m_ccmMacSapUser;
      res.insert (res.end (), elem);
      
      ccLcMapIt = m_componentCarrierLcMap.find (ncc);
      if (ccLcMapIt != m_componentCarrierLcMap.end ())
        {
          ccLcMapIt->second.insert (std::pair <uint8_t, LteMacSapProvider*> (lcId, m_macSapProvidersMap.at (ncc)));
        }
      else
        {
          std::map<uint8_t, LteMacSapProvider*> empty;
          std::pair <std::map <uint8_t, std::map<uint8_t, LteMacSapProvider*> >::iterator, bool>
            ret = m_componentCarrierLcMap.insert (std::pair <uint8_t,  std::map<uint8_t, LteMacSapProvider*> >
                                                  (ncc, empty));
          NS_ASSERT_MSG (ret.second, "element already present, ComponentCarrierId already existed");
          ccLcMapIt = m_componentCarrierLcMap.find (ncc);
          ccLcMapIt->second.insert (std::pair <uint8_t, LteMacSapProvider*> (lcId, m_macSapProvidersMap.at (ncc)));
        }
    }
  
  return res;  
}

void
SimpleUeComponentCarrierManager::DoNotifyConnectionReconfigurationMsg ()
{
  NS_LOG_FUNCTION (this);
  // this method need to be extended, now support only up to 2 ComponentCarrier Simulations

  if (m_noOfComponentCarriersEnabled < m_noOfComponentCarriers)
   {
     // new ComponentCarrierConfiguration Requested
     m_noOfComponentCarriersEnabled++;
     std::vector<uint8_t> res;
     res.insert (res.end (), m_noOfComponentCarriersEnabled);
     //here the code to update all the Lc, since now  those should be mapped on all ComponentCarriers
     m_ccmRrcSapUser->ComponentCarrierEnabling (res);
   }
  
}
LteMacSapUser*
SimpleUeComponentCarrierManager::DoConfigureSignalBearer (uint8_t lcid,  LteUeCmacSapProvider::LogicalChannelConfig lcConfig, LteMacSapUser* msu)
{
  NS_LOG_FUNCTION (this);
  std::map<uint8_t, LteMacSapUser*>::iterator it = m_lcAttached.find (lcid);
  //NS_ASSERT_MSG (it == m_lcAttached.end (), "Warning, LCID " << (uint8_t) lcid << " already exist");
  if (it != m_lcAttached.end ())
    {
      // This line will remove the former SignalBearer. It is needed in case of handover
      // since an update of the signal bearer performed.
      // Now it points on the right LteMacSapUser
      m_lcAttached.erase (it); 
    }
  m_lcAttached.insert (std::pair<uint8_t, LteMacSapUser*> (lcid, msu));

  return m_ccmMacSapUser;
 } 

} // end of namespace ns3
