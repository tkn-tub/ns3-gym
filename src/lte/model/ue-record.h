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

#ifndef UE_RECORD_H_
#define UE_RECORD_H_

#include "ns3/net-device.h"
#include "ns3/object.h"
#include "ns3/log.h"
#include <vector>
#include <stdint.h>

namespace ns3 {

/**
 * \ingroup lte
 *
 * \brief The UeRecord class is developerd in order to store at the eNodeB
 * all information (such as feedback cqi, mac address etc...) of a UE registered
 * into that eNodeB. All UE records are managed by the UeManager class
 */
class UeRecord : public Object
{
public:
  UeRecord ();
  ~UeRecord ();

  /**
   * \brief CqiFeedbacks represents a list of CQI feedbacks
   * sent by the UE. The downlink packet scheduler of
   * the eNB uses these values to assign accordingly
   * radio resources.
   */
  struct CqiFeedback
  {
    /** the sub channel */
    int m_subChannelId; 
    /** the cqi feedback */
    int m_cqi; 
  };

  /**
   * \brief a list of CQI feedbacks
   */
  typedef std::vector<struct CqiFeedback> CqiFeedbacks;



  /**
   * \brief Creates a ue record of the UE registered into the eNB
   * \param ue the pointer of the ue device
   * \param enb the pointer of the enb device
   */
  UeRecord (Ptr<NetDevice> ue, Ptr<NetDevice> enb);

  /**
   * \brief Set the UE of the record
   * \param ue the pointer of the ue device
   */
  void SetUe (Ptr<NetDevice> ue);

  /**
   * \brief Get the UE of the record
   * \returns the pointer of the UE
   */
  Ptr<NetDevice> GetUe (void);

  /**
   * \brief Set the eNB of the record
   * \param enb the pointer of the enb device
   */
  void SetEnb (Ptr<NetDevice> enb);

  /**
   * \brief Get the eNB of the record
   * \returns the pointer of the eNB
   */
  Ptr<NetDevice> GetEnb (void);


  /**
   * \brief Set CQI feedbacks of the registered UE
   * \param cqiFeedbacks a list of CQI feedback
   */
  void SetCqiFeedbacks (CqiFeedbacks cqiFeedbacks);

  /**
   * \brief Get CQI feedbacks of the registered UE
   * \returns a list of CQI feedback
   */
  CqiFeedbacks GetCqiFeedbacks (void);


private:
  Ptr<NetDevice> m_ue;
  Ptr<NetDevice> m_enb;

  CqiFeedbacks m_cqiFeedbacks;
};

}
; // namespace ns3

#endif /* UE_RECORD_H_ */
