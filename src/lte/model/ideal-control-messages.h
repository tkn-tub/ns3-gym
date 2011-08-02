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

#ifndef IDEAL_CONTROL_MESSAGES_H
#define IDEAL_CONTROL_MESSAGES_H

#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"
#include <list>

namespace ns3 {

class LteNetDevice;


/**
 * \ingroup lte
 *
 * The IdealControlMessage provides a basic implementations for
 * control messages (such as PDCCH allocation map, CQI feedbacks)
 * that are exchanged among eNodeB and UEs.
 */
class IdealControlMessage : public SimpleRefCount<IdealControlMessage>
{
public:

  /**
   * The type of the message
   */
  enum MessageType
  {
    CQI_FEEDBACKS, ALLOCATION_MAP
  };

  IdealControlMessage (void);
  virtual ~IdealControlMessage (void);

  /**
   * \brief Set the source  device of the message
   * \param src the device that sends the message
   */
  void SetSourceDevice (Ptr<LteNetDevice> src);
  /**
   * \brief Set the destination  device of the message
   * \param dst the device that receives the message
   */
  void SetDestinationDevice (Ptr<LteNetDevice> dst);

  /**
   * \brief Get the source  device of the message
   * \return the pointer to the device that sends the message
   */
  Ptr<LteNetDevice> GetSourceDevice (void);
  /**
   * \brief Get the destination device of the message
   * \return the pointer to the device that receives the message
   */
  Ptr<LteNetDevice> GetDestinationDevice (void);

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

#endif /* IDEAL_CONTROL_MESSAGES_H */



// ----------------------------------------------------------------------------------------------------------



#ifndef PDCCH_MAP_IDEAL_CONTROL_MESSAGES_H
#define PDCCH_MAP_IDEAL_CONTROL_MESSAGES_H

#include "ns3/object.h"
#include <list>

namespace ns3 {

class LteNetDevice;

/**
 * \ingroup lte
 *
 * \brief The PdcchMapIdealControlMessage defines an ideal allocation map
 * for both UL and DL sends by the eNodeB to all UE,
 * using an ideal PDCCH control channel.
 * IdealPdcchMessage is composed by a list of IdealPdcchRecord
 * where is indicated the UE that can use a particular sub channel
 * with a proper MCS scheme.
 * This records are the same for both UL and DL, and are created by the
 * packet scheduler at the beginning of each sub frame.
 * When the IdealPdcchMessage is sent under an ideal control channel,
 * all UE stores into a proper variables the informations about
 * the resource mapping.
 */
class PdcchMapIdealControlMessage : public IdealControlMessage
{
public:

  PdcchMapIdealControlMessage (void);
  virtual ~PdcchMapIdealControlMessage (void);

  /**
   * Direction for which the message is created 
   */
  enum Direction
  {
    DOWNLINK, UPLINK
  };

  /**
   * The PDCCH ideal record
   */
  struct IdealPdcchRecord
  {
    /** the direction */
    Direction m_direction;
    /** the sub channel */ 
    int m_idSubChannel;
    /** the ue that receive the mapping */
    Ptr<LteNetDevice> m_ue;
    /** the selected msc */
    double m_mcsIndex;
  };

  /**
   * The PDCCH ideal message
   */
  typedef std::list<struct IdealPdcchRecord>  IdealPdcchMessage;

  /**
   * \brief add a PDCCH record into the message.
   * \param direction the direction of the map
   * \param subChannel the scheduled sub channel
   * \param ue the ue the can use the sub channel for transmission
   * \param mcs the selected MCS scheme
   */
  void AddNewRecord (Direction direction,
                     int subChannel, Ptr<LteNetDevice> ue, double mcs);

  /**
   * \brief Get the message
   * \return the pointer to the message
   */
  IdealPdcchMessage* GetMessage (void);

private:
  IdealPdcchMessage *m_idealPdcchMessage;
};

} // namespace ns3

#endif /* PDCCH_MAP_IDEAL_CONTROL_MESSAGES_H */



// ----------------------------------------------------------------------------------------------------------



#ifndef CQI_IDEAL_CONTROL_MESSAGES_H
#define CQI_IDEAL_CONTROL_MESSAGES_H

#include "ns3/object.h"
#include <list>

namespace ns3 {

class LteNetDevice;

/**
 * \ingroup lte
 *
 * The CqiIdealControlMessage defines an ideal list of feedback about
 * the channel quality sent by the UE to the eNodeB.
 */
class CqiIdealControlMessage : public IdealControlMessage
{
public:

  CqiIdealControlMessage (void);
  virtual ~CqiIdealControlMessage (void);

  /**
   * The CQI feedback ideal record
   */
  struct CqiFeedback
  {
    /** the sub channel */
    int m_idSubChannel; 
    /** the cqi feedback */
    double m_cqi;
  };

  /**
   * The ideal CQI feedback message
   */
  typedef std::list<struct CqiFeedback>  CqiFeedbacks;

  /**
   * \brief add a CQI feedback record into the message.
   * \param subChannel the scheduled sub channel
   * \param cqi the cqi feedback
   */
  void AddNewRecord (int subChannel, double cqi);

  /**
   * \brief Get cqi informations
   * \return cqi messages
   */
  CqiFeedbacks* GetMessage (void);


private:
  CqiFeedbacks *m_cqiFeedbacks;
};
} // namespace ns3

#endif /* CQI_IDEAL_CONTROL_MESSAGES_H */
