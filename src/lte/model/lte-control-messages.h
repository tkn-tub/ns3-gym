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
 * Author: Marco Miozzo <marco.miozzo@cttc.es>
 */

#ifndef LTE_CONTROL_MESSAGES_H
#define LTE_CONTROL_MESSAGES_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include <list>

namespace ns3 {

class LteNetDevice;


/**
 * \ingroup lte
 *
 * The LteControlMessage provides a basic implementations for
 * control messages (such as PDCCH allocation map, CQI feedbacks)
 * that are exchanged among eNodeB and UEs.
 */
class LteControlMessage : public SimpleRefCount<LteControlMessage>
{
public:
  /**
   * The type of the message
   */
  enum MessageType
  {
    DL_DCI, UL_DCI, // Downlink/Uplink Data Control Indicator
    DL_CQI, UL_CQI, // Downlink/Uplink Channel Quality Indicator
    BSR // Buffer Status Report
  };

  LteControlMessage (void);
  virtual ~LteControlMessage (void);

  /**
   * \brief Set the type of the message
   * \param type the type of the message
   */
  void SetMessageType (MessageType type);
  /**
   * \brief Get the type of the message
   * \return the type of the message
   */
  MessageType GetMessageType (void);

private:
  Ptr<LteNetDevice> m_source;
  Ptr<LteNetDevice> m_destination;
  MessageType m_type;
};
} // namespace ns3

#endif /* LTE_CONTROL_MESSAGES_H */




// ----------------------------------------------------------------------------------------------------------


#ifndef DL_DCI_LTE_CONTROL_MESSAGES_H
#define DL_DCI_LTE_CONTROL_MESSAGES_H

#include <ns3/object.h>
#include <ns3/ff-mac-common.h>

namespace ns3 {

/**
 * \ingroup lte
 * The Downlink Data Control Indicator messages defines the RB allocation for the
 * users in the downlink
 */
class DlDciLteControlMessage : public LteControlMessage
{
public:
  DlDciLteControlMessage (void);
  virtual ~DlDciLteControlMessage (void);

  /**
  * \brief add a DCI into the message
  * \param dci the dci
  */
  void SetDci (DlDciListElement_s dci);

  /**
  * \brief Get dic informations
  * \return dci messages
  */
  DlDciListElement_s GetDci (void);


private:
  DlDciListElement_s m_dci;
};
} // namespace ns3

#endif /* DL_DCI_LTE_CONTROL_MESSAGES_H */


// ----------------------------------------------------------------------------------------------------------


#ifndef UL_DCI_LTE_CONTROL_MESSAGES_H
#define UL_DCI_LTE_CONTROL_MESSAGES_H

#include <ns3/object.h>
#include <ns3/ff-mac-common.h>

namespace ns3 {

/**
 * \ingroup lte
 * The Uplink Data Control Indicator messages defines the RB allocation for the
 * users in the uplink
 */
class UlDciLteControlMessage : public LteControlMessage
{
public:
  UlDciLteControlMessage (void);
  virtual ~UlDciLteControlMessage (void);

  /**
  * \brief add a DCI into the message
  * \param dci the dci
  */
  void SetDci (UlDciListElement_s dci);

  /**
  * \brief Get dic informations
  * \return dci messages
  */
  UlDciListElement_s GetDci (void);


private:
  UlDciListElement_s m_dci;
};
} // namespace ns3

#endif /* UL_DCI_LTE_CONTROL_MESSAGES_H */



// ----------------------------------------------------------------------------------------------------------



#ifndef DLCQI_LTE_CONTROL_MESSAGES_H
#define DLCQI_LTE_CONTROL_MESSAGES_H

#include <ns3/object.h>
#include <ns3/ff-mac-common.h>

namespace ns3 {

class LteNetDevice;

/**
 * \ingroup lte
 * The downlink CqiLteControlMessage defines an ideal list of
 * feedback about the channel quality sent by the UE to the eNodeB.
 */
class DlCqiLteControlMessage : public LteControlMessage
{
public:
  DlCqiLteControlMessage (void);
  virtual ~DlCqiLteControlMessage (void);

  /**
  * \brief add a DL-CQI feedback record into the message.
  * \param dlcqi the DL cqi feedback
  */
  void SetDlCqi (CqiListElement_s dlcqi);

  /**
  * \brief Get DL cqi informations
  * \return dlcqi messages
  */
  CqiListElement_s GetDlCqi (void);


private:
  CqiListElement_s m_dlCqi;
};
} // namespace ns3

#endif /* DLCQI_LTE_CONTROL_MESSAGES_H */


// ----------------------------------------------------------------------------------------------------------

#ifndef BSR_LTE_CONTROL_MESSAGES_H
#define BSR_LTE_CONTROL_MESSAGES_H

#include <ns3/object.h>
#include <ns3/ff-mac-common.h>

namespace ns3 {

class LteNetDevice;

/**
 * \ingroup lte
 * The uplink BsrLteControlMessage defines the specific
 * extension of the CE element for reporting the buffer status report
 */
class BsrLteControlMessage : public LteControlMessage
{
public:
  BsrLteControlMessage (void);
  virtual ~BsrLteControlMessage (void);

  /**
  * \brief add a BSR feedback record into the message.
  * \param bsr the BSR feedback
  */
  void SetBsr (MacCeListElement_s ulcqi);

  /**
  * \brief Get BSR informations
  * \return BSR message
  */
  MacCeListElement_s GetBsr (void);


private:
  MacCeListElement_s m_bsr;


};
} // namespace ns3

#endif /* LTE_CONTROL_MESSAGES_H */

