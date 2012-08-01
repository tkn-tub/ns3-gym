/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */

#include "ns3/simulator.h"
#include "ns3/log.h"

#include "ns3/lte-rlc-am-header.h"
#include "ns3/lte-rlc-am.h"
#include "ns3/lte-rlc-sdu-status-tag.h"
#include "ns3/lte-rlc-tag.h"

NS_LOG_COMPONENT_DEFINE ("LteRlcAm");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LteRlcAm);

LteRlcAm::LteRlcAm ()
{
  NS_LOG_FUNCTION (this);

  // Buffers
  m_txonBufferSize = 0;
  m_retxBuffer.resize (1024);
  m_retxBufferSize = 0;
  m_txedBuffer.resize (1024);
  m_txedBufferSize = 0;

  m_statusPduRequested = false;
  m_statusPduBufferSize = 0;

  // State variables: transmitting side
  m_windowSize = 512;
  m_vtA  = 0;
  m_vtMs = m_vtA + m_windowSize;
  m_vtS  = 0;
  m_pollSn = 0;

  // State variables: receiving side
  m_vrR  = 0;
  m_vrMr = m_vrR + m_windowSize;
  m_vrX  = 0;
  m_vrMs = 0;
  m_vrH  = 0;

  // Counters
  m_pduWithoutPoll  = 0;
  m_byteWithoutPoll = 0;

  // Configurable parameters
  m_maxRetxThreshold = 5;
  m_pollPdu = 1;
  m_pollByte = 50;

  // SDU reassembling process
  m_reassemblingState = WAITING_S0_FULL;
  m_expectedSeqNumber = 0;

  Simulator::ScheduleNow (&LteRlcAm::Start, this);
}

LteRlcAm::~LteRlcAm ()
{
}

TypeId
LteRlcAm::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LteRlcAm")
    .SetParent<LteRlc> ()
    .AddConstructor<LteRlcAm> ()
    ;
  return tid;
}


/**
 * RLC SAP
 */

void
LteRlcAm::DoTransmitPdcpPdu (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << m_rnti << (uint32_t) m_lcid << p->GetSize ());

  /** Store arrival time */
  Time now = Simulator::Now ();
  RlcTag timeTag (now);
  p->AddPacketTag (timeTag);

  /** Store PDCP PDU */

  LteRlcSduStatusTag tag;
  tag.SetStatus (LteRlcSduStatusTag::FULL_SDU);
  p->AddPacketTag (tag);

  NS_LOG_LOGIC ("Txon Buffer: New packet added");
  m_txonBuffer.push_back (p);
  m_txonBufferSize += p->GetSize ();
  NS_LOG_LOGIC ("NumOfBuffers = " << m_txonBuffer.size() );
  NS_LOG_LOGIC ("txonBufferSize = " << m_txonBufferSize);

  /** Report Buffer Status */

  // Transmission Queue HOL time
  RlcTag txonQueueHolTimeTag;
  m_txonBuffer.front ()->PeekPacketTag (txonQueueHolTimeTag);
  Time txonQueueHolDelay = now - txonQueueHolTimeTag.GetSenderTimestamp ();

  // Retransmission Queue HOL time
  RlcTag retxQueueHolTimeTag;
  Time retxQueueHolDelay (0);
  if ( m_retxBufferSize )
    {
//MRE      m_retxBuffer.front ().m_pdu->PeekPacketTag (retxQueueHolTimeTag);
      retxQueueHolDelay = now - retxQueueHolTimeTag.GetSenderTimestamp ();
    }

  LteMacSapProvider::ReportBufferStatusParameters r;
  r.rnti = m_rnti;
  r.lcid = m_lcid;
  r.txQueueSize = m_txonBufferSize;
  r.txQueueHolDelay = txonQueueHolDelay.GetMilliSeconds ();
  r.retxQueueSize = m_retxBufferSize;
  r.retxQueueHolDelay = retxQueueHolDelay.GetMilliSeconds ();

  if ( m_statusPduRequested && ! m_statusProhibitTimer.IsRunning () )
    {
      r.statusPduSize = m_statusPduBufferSize;
    }
  else
    {
      r.statusPduSize = 0;
    }

  NS_LOG_INFO ("Send ReportBufferStatus: " << r.txQueueSize << ", " << r.txQueueHolDelay << ", " 
                                           << r.retxQueueSize << ", " << r.retxQueueHolDelay << ", " 
                                           << r.statusPduSize);
  m_macSapProvider->ReportBufferStatus (r);
}


/**
 * MAC SAP
 */

void
LteRlcAm::DoNotifyTxOpportunity (uint32_t bytes, uint8_t layer)
{
  NS_LOG_FUNCTION (this << m_rnti << (uint32_t) m_lcid << bytes);
  
  if (bytes <= 2)
    {
      // Stingy MAC: Header fix part is 2 bytes, we need more bytes for the data
      NS_LOG_LOGIC ("TX opportunity too small = " << bytes);
      return;
    }

  if ( m_statusPduRequested && ! m_statusProhibitTimer.IsRunning () )
    {
      NS_LOG_LOGIC ("Sending STATUS PDU");

      Ptr<Packet> packet = Create<Packet> ();
      LteRlcAmHeader rlcAmHeader;
      rlcAmHeader.SetControlPdu (LteRlcAmHeader::STATUS_PDU);
      rlcAmHeader.SetAckSn (m_vrR);

      NS_LOG_LOGIC ("RLC header: " << rlcAmHeader);
      packet->AddHeader (rlcAmHeader);

      // Send RLC PDU to MAC layer
      LteMacSapProvider::TransmitPduParameters params;
      params.pdu = packet;
      params.rnti = m_rnti;
      params.lcid = m_lcid;

      m_macSapProvider->TransmitPdu (params);
      return;
    }
  else if ( m_retxBufferSize > 0 )
    {
      NS_LOG_LOGIC ("Sending data from Retransmission Buffer");
      
      Ptr<Packet> packet = m_retxBuffer.at (m_vtA.GetValue ()).m_pdu->Copy ();

      if ( packet->GetSize () <= bytes )
        {
          LteRlcAmHeader rlcAmHeader;
          packet->PeekHeader (rlcAmHeader);
          NS_LOG_LOGIC ("RLC header: " << rlcAmHeader);

          // Send RLC PDU to MAC layer
          LteMacSapProvider::TransmitPduParameters params;
          params.pdu = packet;
          params.rnti = m_rnti;
          params.lcid = m_lcid;

          m_macSapProvider->TransmitPdu (params);
          return;
        }
      else
        {
          NS_LOG_LOGIC ("Tx opportunity too small for retransmission of the packet (" << packet->GetSize () << " bytes)");
          NS_LOG_LOGIC ("Waiting for bigger tx opportunity");
          return;
        }
    }
  else if ( m_txonBufferSize > 0 )
    {
      NS_LOG_LOGIC ("Sending data from Transmission Buffer");
    }
  else
    {
      NS_LOG_LOGIC ("No data pending");
      return;
    }

  //
  //
  // Build new PDU
  //
  //

  Ptr<Packet> packet = Create<Packet> ();
  LteRlcAmHeader rlcAmHeader;
  rlcAmHeader.SetDataPdu ();

  // Build Data field
  uint32_t nextSegmentSize = bytes - 4;
  uint32_t nextSegmentId = 1;
  uint32_t dataFieldTotalSize = 0;
  uint32_t dataFieldAddedSize = 0;
  std::vector < Ptr<Packet> > dataField;

  // Remove the first packet from the transmission buffer.
  // If only a segment of the packet is taken, then the remaining is given back later
  if ( m_txonBuffer.size () == 0 )
    {
      NS_LOG_LOGIC ("No data pending");
      return;
    }

  NS_LOG_LOGIC ("SDUs in TxonBuffer  = " << m_txonBuffer.size ());
  NS_LOG_LOGIC ("First SDU buffer  = " << *(m_txonBuffer.begin()));
  NS_LOG_LOGIC ("First SDU size    = " << (*(m_txonBuffer.begin()))->GetSize ());
  NS_LOG_LOGIC ("Next segment size = " << nextSegmentSize);
  NS_LOG_LOGIC ("Remove SDU from TxBuffer");
  Ptr<Packet> firstSegment = (*(m_txonBuffer.begin ()))->Copy ();
  m_txonBufferSize -= (*(m_txonBuffer.begin()))->GetSize ();
  NS_LOG_LOGIC ("txBufferSize      = " << m_txonBufferSize );
  m_txonBuffer.erase (m_txonBuffer.begin ());

  while ( firstSegment && (firstSegment->GetSize () > 0) && (nextSegmentSize > 0) )
    {
      NS_LOG_LOGIC ("WHILE ( firstSegment && firstSegment->GetSize > 0 && nextSegmentSize > 0 )");
      NS_LOG_LOGIC ("    firstSegment size = " << firstSegment->GetSize ());
      NS_LOG_LOGIC ("    nextSegmentSize   = " << nextSegmentSize);
      if ( (firstSegment->GetSize () > nextSegmentSize) ||
           // Segment larger than 2047 octets can only be mapped to the end of the Data field
           (firstSegment->GetSize () > 2047)
         )
        {
          // Take the minimum size, due to the 2047-bytes 3GPP exception
          // This exception is due to the length of the LI field (just 11 bits)
          uint32_t currSegmentSize = std::min (firstSegment->GetSize (), nextSegmentSize);

          NS_LOG_LOGIC ("    IF ( firstSegment > nextSegmentSize ||");
          NS_LOG_LOGIC ("         firstSegment > 2047 )");
          
          // Segment txBuffer.FirstBuffer and
          // Give back the remaining segment to the transmission buffer
          Ptr<Packet> newSegment = firstSegment->CreateFragment (0, currSegmentSize);
          NS_LOG_LOGIC ("    newSegment size   = " << newSegment->GetSize ());

          // Status tag of the new and remaining segments
          // Note: This is the only place where a PDU is segmented and
          // therefore its status can change
          LteRlcSduStatusTag oldTag, newTag;
          firstSegment->RemovePacketTag (oldTag);
          newSegment->RemovePacketTag (newTag);
          if (oldTag.GetStatus () == LteRlcSduStatusTag::FULL_SDU)
            {
              newTag.SetStatus (LteRlcSduStatusTag::FIRST_SEGMENT);
              oldTag.SetStatus (LteRlcSduStatusTag::LAST_SEGMENT);
            }
          else if (oldTag.GetStatus () == LteRlcSduStatusTag::LAST_SEGMENT)
            {
              newTag.SetStatus (LteRlcSduStatusTag::MIDDLE_SEGMENT);
              //oldTag.SetStatus (LteRlcSduStatusTag::LAST_SEGMENT);
            }

          // Give back the remaining segment to the transmission buffer
          firstSegment->RemoveAtStart (currSegmentSize);
          NS_LOG_LOGIC ("    firstSegment size (after RemoveAtStart) = " << firstSegment->GetSize ());
          if (firstSegment->GetSize () > 0)
            {
              firstSegment->AddPacketTag (oldTag);

              m_txonBuffer.insert (m_txonBuffer.begin (), firstSegment);
              m_txonBufferSize += (*(m_txonBuffer.begin()))->GetSize ();

              NS_LOG_LOGIC ("    Txon buffer: Give back the remaining segment");
              NS_LOG_LOGIC ("    Txon buffers = " << m_txonBuffer.size ());
              NS_LOG_LOGIC ("    Front buffer size = " << (*(m_txonBuffer.begin()))->GetSize ());
              NS_LOG_LOGIC ("    txonBufferSize = " << m_txonBufferSize );
            }
          else
            {
              // Whole segment was taken, so adjust tag
              if (newTag.GetStatus () == LteRlcSduStatusTag::FIRST_SEGMENT)
                {
                  newTag.SetStatus (LteRlcSduStatusTag::FULL_SDU);
                }
              else if (newTag.GetStatus () == LteRlcSduStatusTag::MIDDLE_SEGMENT)
                {
                  newTag.SetStatus (LteRlcSduStatusTag::LAST_SEGMENT);
                }
            }
          // Segment is completely taken or
          // the remaining segment is given back to the transmission buffer
          firstSegment = 0;

          // Put status tag once it has been adjusted
          newSegment->AddPacketTag (newTag);

          // Add Segment to Data field
          dataFieldAddedSize = newSegment->GetSize ();
          dataFieldTotalSize += dataFieldAddedSize;
          dataField.push_back (newSegment);
          newSegment = 0;

          // ExtensionBit (Next_Segment - 1) = 0
          rlcAmHeader.PushExtensionBit (LteRlcAmHeader::DATA_FIELD_FOLLOWS);

          // no LengthIndicator for the last one

          nextSegmentSize -= dataFieldAddedSize;
          nextSegmentId++;

          // nextSegmentSize MUST be zero (only if segment is smaller or equal to 2047)

          // (NO more segments) → exit
          // break;
        }
      else if ( (nextSegmentSize - firstSegment->GetSize () <= 2) || (m_txonBuffer.size () == 0) )
        {
          NS_LOG_LOGIC ("    IF nextSegmentSize - firstSegment->GetSize () <= 2 || txonBuffer.size == 0");

          // Add txBuffer.FirstBuffer to DataField
          dataFieldAddedSize = firstSegment->GetSize ();
          dataFieldTotalSize += dataFieldAddedSize;
          dataField.push_back (firstSegment);
          firstSegment = 0;

          // ExtensionBit (Next_Segment - 1) = 0
          rlcAmHeader.PushExtensionBit (LteRlcAmHeader::DATA_FIELD_FOLLOWS);

          // no LengthIndicator for the last one

          nextSegmentSize -= dataFieldAddedSize;
          nextSegmentId++;

          NS_LOG_LOGIC ("        SDUs in TxBuffer  = " << m_txonBuffer.size ());
          if (m_txonBuffer.size () > 0)
            {
              NS_LOG_LOGIC ("        First SDU buffer  = " << *(m_txonBuffer.begin()));
              NS_LOG_LOGIC ("        First SDU size    = " << (*(m_txonBuffer.begin()))->GetSize ());
            }
          NS_LOG_LOGIC ("        Next segment size = " << nextSegmentSize);

          // nextSegmentSize <= 2 (only if txBuffer is not empty)

          // (NO more segments) → exit
          // break;
        }
      else // (firstSegment->GetSize () < m_nextSegmentSize) && (m_txBuffer.size () > 0)
        {
          NS_LOG_LOGIC ("    IF firstSegment < NextSegmentSize && txonBuffer.size > 0");
          // Add txBuffer.FirstBuffer to DataField
          dataFieldAddedSize = firstSegment->GetSize ();
          dataFieldTotalSize += dataFieldAddedSize;
          dataField.push_back (firstSegment);

          // ExtensionBit (Next_Segment - 1) = 1
          rlcAmHeader.PushExtensionBit (LteRlcAmHeader::E_LI_FIELDS_FOLLOWS);

          // LengthIndicator (Next_Segment)  = txBuffer.FirstBuffer.length()
          rlcAmHeader.PushLengthIndicator (firstSegment->GetSize ());

          nextSegmentSize -= ((nextSegmentId % 2) ? (2) : (1)) + dataFieldAddedSize;
          nextSegmentId++;

          NS_LOG_LOGIC ("        SDUs in TxBuffer  = " << m_txonBuffer.size ());
          if (m_txonBuffer.size () > 0)
            {
              NS_LOG_LOGIC ("        First SDU buffer  = " << *(m_txonBuffer.begin()));
              NS_LOG_LOGIC ("        First SDU size    = " << (*(m_txonBuffer.begin()))->GetSize ());
            }
          NS_LOG_LOGIC ("        Next segment size = " << nextSegmentSize);
          NS_LOG_LOGIC ("        Remove SDU from TxBuffer");

          // (more segments)
          firstSegment = (*(m_txonBuffer.begin ()))->Copy ();
          m_txonBufferSize -= (*(m_txonBuffer.begin()))->GetSize ();
          m_txonBuffer.erase (m_txonBuffer.begin ());
          NS_LOG_LOGIC ("        txBufferSize = " << m_txonBufferSize );
        }

    }

  //
  // Build RLC header
  //

  rlcAmHeader.SetSequenceNumber ( m_vtS++ );
  rlcAmHeader.SetResegmentationFlag (LteRlcAmHeader::PDU);
  rlcAmHeader.SetLastSegmentFlag (LteRlcAmHeader::LAST_PDU_SEGMENT);
  rlcAmHeader.SetSegmentOffset (0);

  // Calculate FramingInfo flag according the status of the SDUs in the DataField
  uint8_t framingInfo = 0;
  std::vector< Ptr<Packet> >::iterator it;
  it = dataField.begin ();

  // FIRST SEGMENT
  LteRlcSduStatusTag tag;
  (*it)->RemovePacketTag (tag);
  if ( (tag.GetStatus () == LteRlcSduStatusTag::FULL_SDU) ||
       (tag.GetStatus () == LteRlcSduStatusTag::FIRST_SEGMENT)
     )
    {
      framingInfo |= LteRlcAmHeader::FIRST_BYTE;
    }
  else
    {
      framingInfo |= LteRlcAmHeader::NO_FIRST_BYTE;
    }
  (*it)->AddPacketTag (tag);

  // Add all SDUs (in DataField) to the Packet
  while (it < dataField.end ())
    {
      NS_LOG_LOGIC ("Adding SDU/segment to packet, length = " << (*it)->GetSize ());

      packet->AddAtEnd (*it);
      it++;
    }

  // LAST SEGMENT (Note: There could be only one and be the first one)
  it--;
  (*it)->RemovePacketTag (tag);
  if ( (tag.GetStatus () == LteRlcSduStatusTag::FULL_SDU) ||
        (tag.GetStatus () == LteRlcSduStatusTag::LAST_SEGMENT) )
    {
      framingInfo |= LteRlcAmHeader::LAST_BYTE;
    }
  else
    {
      framingInfo |= LteRlcAmHeader::NO_LAST_BYTE;
    }
  (*it)->AddPacketTag (tag);

  // Set the FramingInfo flag after the calculation
  rlcAmHeader.SetFramingInfo (framingInfo);


  // Calculate the Polling Bit (5.2.2.1)
  rlcAmHeader.SetPollingBit (LteRlcAmHeader::STATUS_REPORT_NOT_REQUESTED);

  m_pduWithoutPoll++;
  NS_LOG_LOGIC ("PDU_WITHOUT_POLL = " << m_pduWithoutPoll);
  m_byteWithoutPoll += packet->GetSize ();
  NS_LOG_LOGIC ("BYTE_WITHOUT_POLL = " << m_byteWithoutPoll);

  if ( (m_pduWithoutPoll >= m_pollPdu) || (m_byteWithoutPoll >= m_pollByte) ||
       ( (m_txonBuffer.empty ()) && (m_retxBuffer.empty ()) ) ||
       (m_vtS >= m_vtMs)
     )
    {
      rlcAmHeader.SetPollingBit (LteRlcAmHeader::STATUS_REPORT_IS_REQUESTED);
      m_pduWithoutPoll = 0;
      m_byteWithoutPoll = 0;

      m_pollSn = m_vtS - 1;
      NS_LOG_LOGIC ("New POLL_SN = " << m_pollSn);

      if (! m_pollRetransmitTimer.IsRunning () )
        {
          NS_LOG_LOGIC ("Start PollRetransmit timer");

          m_pollRetransmitTimer = Simulator::Schedule (Time ("0.1s"),
                                                       &LteRlcAm::ExpirePollRetransmitTimer, this);
        }
      else
        {
          NS_LOG_LOGIC ("Restart PollRetransmit timer");

          m_pollRetransmitTimer.Cancel ();
          m_pollRetransmitTimer = Simulator::Schedule (Time ("0.1s"),
                                                       &LteRlcAm::ExpirePollRetransmitTimer, this);
        }
    }


  // Build RLC PDU with DataField and Header
  NS_LOG_LOGIC ("AM RLC header: " << rlcAmHeader);
  packet->AddHeader (rlcAmHeader);

  // Store new PDU into the Transmitted PDU Buffer
  NS_LOG_LOGIC ("Put transmitted PDU in the txedBuffer");
  m_txedBufferSize += packet->GetSize ();
  m_txedBuffer.at ( rlcAmHeader.GetSequenceNumber ().GetValue () ) = packet->Copy ();
  
  // Sender timestamp
  RlcTag rlcTag (Simulator::Now ());
  packet->AddByteTag (rlcTag);
  m_txPdu (m_rnti, m_lcid, packet->GetSize ());

  // Send RLC PDU to MAC layer
  LteMacSapProvider::TransmitPduParameters params;
  params.pdu = packet;
  params.rnti = m_rnti;
  params.lcid = m_lcid;
  params.layer = layer;

  m_macSapProvider->TransmitPdu (params);
}

void
LteRlcAm::DoNotifyHarqDeliveryFailure ()
{
  NS_LOG_FUNCTION (this);
}

void
LteRlcAm::DoReceivePdu (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this << m_rnti << (uint32_t) m_lcid << p->GetSize ());

  // Receiver timestamp
  RlcTag rlcTag;
  Time delay;
  if (p->FindFirstMatchingByteTag (rlcTag))
    {
      delay = Simulator::Now() - rlcTag.GetSenderTimestamp ();
    }
  m_rxPdu (m_rnti, m_lcid, p->GetSize (), delay.GetNanoSeconds ());

  // Get RLC header parameters
  LteRlcAmHeader rlcAmHeader;
  p->PeekHeader (rlcAmHeader);
  NS_LOG_LOGIC ("RLC header: " << rlcAmHeader);

  if ( rlcAmHeader.IsDataPdu () )
    {

      // 5.1.3.1   Transmit operations

      // 5.1.3.1.1       General
      //
      // The transmitting side of an AM RLC entity shall prioritize transmission of RLC control PDUs
      // over RLC data PDUs. The transmitting side of an AM RLC entity shall prioritize retransmission
      // of RLC data PDUs over transmission of new AMD PDUs.
      //
      // The transmitting side of an AM RLC entity shall maintain a transmitting window according to
      // state variables VT(A) and VT(MS) as follows:
      // - a SN falls within the transmitting window if VT(A) <= SN < VT(MS);
      // - a SN falls outside of the transmitting window otherwise.
      //
      // The transmitting side of an AM RLC entity shall not deliver to lower layer any RLC data PDU
      // whose SN falls outside of the transmitting window.
      //
      // When delivering a new AMD PDU to lower layer, the transmitting side of an AM RLC entity shall:
      // - set the SN of the AMD PDU to VT(S), and then increment VT(S) by one.
      //
      // The transmitting side of an AM RLC entity can receive a positive acknowledgement (confirmation
      // of successful reception by its peer AM RLC entity) for a RLC data PDU by the following:
      // - STATUS PDU from its peer AM RLC entity.
      //
      // When receiving a positive acknowledgement for an AMD PDU with SN = VT(A), the transmitting
      // side of an AM RLC entity shall:
      // - set VT(A) equal to the SN of the AMD PDU with the smallest SN, whose SN falls within the
      //   range VT(A) <= SN <= VT(S) and for which a positive acknowledgment has not been received yet.
      // - if positive acknowledgements have been received for all AMD PDUs associated with
      //   a transmitted RLC SDU:
      // - send an indication to the upper layers of successful delivery of the RLC SDU.


      // 5.1.3.2 Receive operations
      //
      // 5.1.3.2.1 General
      //
      // The receiving side of an AM RLC entity shall maintain a receiving window according to state
      // variables VR(R) and VR(MR) as follows:
      // - a SN falls within the receiving window if VR(R) <= SN < VR(MR);
      // - a SN falls outside of the receiving window otherwise.
      //
      // When receiving a RLC data PDU from lower layer, the receiving side of an AM RLC entity shall:
      // - either discard the received RLC data PDU or place it in the reception buffer (see sub clause 5.1.3.2.2);
      // - if the received RLC data PDU was placed in the reception buffer:
      // - update state variables, reassemble and deliver RLC SDUs to upper layer and start/stop t-Reordering as needed (see sub clause 5.1.3.2.3).
      //
      // When t-Reordering expires, the receiving side of an AM RLC entity shall:
      // - update state variables and start t-Reordering as needed (see sub clause 5.1.3.2.4).


      SequenceNumber10 seqNumber = rlcAmHeader.GetSequenceNumber ();

      if ( rlcAmHeader.GetResegmentationFlag () == LteRlcAmHeader::SEGMENT )
        {
          NS_LOG_LOGIC ("PDU segment received ( SN = " << seqNumber << " )");
        }
      else if ( rlcAmHeader.GetResegmentationFlag () == LteRlcAmHeader::PDU )
        {
          NS_LOG_LOGIC ("PDU received ( SN = " << seqNumber << " )");
        }
      else
        {
          NS_ASSERT_MSG (false, "Neither a PDU segment nor a PDU received");
          return ;
        }

      // STATUS PDU is requested
      if ( rlcAmHeader.GetPollingBit () == LteRlcAmHeader::STATUS_REPORT_IS_REQUESTED )
        {
          m_statusPduRequested = true;
          m_statusPduBufferSize = 4;

          if (! m_statusProhibitTimer.IsRunning ())
            {
              Time now = Simulator::Now ();

              // Transmission Queue HOL time
              RlcTag txonQueueHolTimeTag;
              Time txonQueueHolDelay (0);
              if ( ! m_txonBuffer.empty () )
                {
                  m_txonBuffer.front ()->PeekPacketTag (txonQueueHolTimeTag);
                  txonQueueHolDelay = now - txonQueueHolTimeTag.GetSenderTimestamp ();
                }

              // Retransmission Queue HOL time
              RlcTag retxQueueHolTimeTag;
              Time retxQueueHolDelay (0);
              if ( m_retxBufferSize )
                {
                  m_retxBuffer.front ().m_pdu->PeekPacketTag (retxQueueHolTimeTag);
                  retxQueueHolDelay = now - retxQueueHolTimeTag.GetSenderTimestamp ();
                }

              LteMacSapProvider::ReportBufferStatusParameters r;
              r.rnti = m_rnti;
              r.lcid = m_lcid;
              r.txQueueSize = m_txonBufferSize;
              r.txQueueHolDelay = txonQueueHolDelay.GetMilliSeconds ();
              r.retxQueueSize = m_retxBufferSize;
              r.retxQueueHolDelay = retxQueueHolDelay.GetMilliSeconds ();

              if ( m_statusPduRequested && ! m_statusProhibitTimer.IsRunning () )
                {
                  r.statusPduSize = m_statusPduBufferSize;
                }
              else
                {
                  r.statusPduSize = 0;
                }

              NS_LOG_INFO ("Send ReportBufferStatus: " << r.txQueueSize << ", " << r.txQueueHolDelay << ", "
                                                      << r.retxQueueSize << ", " << r.retxQueueHolDelay << ", "
                                                      << r.statusPduSize );
              m_macSapProvider->ReportBufferStatus (r);
            }
        }

      // 5.1.3.2.2 Actions when a RLC data PDU is received from lower layer
      //
      // When a RLC data PDU is received from lower layer, where the RLC data PDU contains
      // byte segment numbers y to z of an AMD PDU with SN = x, the receiving side of an AM RLC entity shall:
      // - if x falls outside of the receiving window; or
      // - if byte segment numbers y to z of the AMD PDU with SN = x have been received before:
      //     - discard the received RLC data PDU;
      // - else:
      //     - place the received RLC data PDU in the reception buffer;
      //     - if some byte segments of the AMD PDU contained in the RLC data PDU have been received before:
      //         - discard the duplicate byte segments.

      NS_LOG_LOGIC ("VR(R)  = " << m_vrR);
      NS_LOG_LOGIC ("VR(MR) = " << m_vrMr);
      NS_LOG_LOGIC ("VR(X)  = " << m_vrX);
      NS_LOG_LOGIC ("VR(MS) = " << m_vrMs);
      NS_LOG_LOGIC ("VR(H)  = " << m_vrH);

      // - if x falls outside of the receiving window; or
      // - if byte segment numbers y to z of the AMD PDU with SN = x have been received before:
      if ( ! IsInsideReceivingWindow (seqNumber) )
        {
          NS_LOG_LOGIC ("PDU discarded");
          return;
        }
      else
        {
          NS_LOG_LOGIC ("Place PDU in the reception buffer ( SN = " << seqNumber << " )");
          m_rxonBuffer[ seqNumber.GetValue () ].m_byteSegments.push_back (p);
          m_rxonBuffer[ seqNumber.GetValue () ].m_pduComplete = true;

          // - if some byte segments of the AMD PDU contained in the RLC data PDU have been received before:
          //         - discard the duplicate byte segments.
        }

      // 5.1.3.2.3 Actions when a RLC data PDU is placed in the reception buffer
      // When a RLC data PDU with SN = x is placed in the reception buffer,
      // the receiving side of an AM RLC entity shall:

      // - if x >= VR(H)
      //     - update VR(H) to x+ 1;

      if ( seqNumber >= m_vrH )
        {
          m_vrH = seqNumber + 1;
          NS_LOG_LOGIC ("New VR(H)  = " << m_vrH);
        }

      // - if all byte segments of the AMD PDU with SN = VR(MS) are received:
      //     - update VR(MS) to the SN of the first AMD PDU with SN > current VR(MS) for
      //       which not all byte segments have been received;

      std::map <uint16_t, PduBuffer>::iterator it = m_rxonBuffer.find (m_vrMs.GetValue ());
      if ( it != m_rxonBuffer.end () &&
           m_rxonBuffer[ m_vrMs.GetValue () ].m_pduComplete )
        {
          int firstVrMs = m_vrMs.GetValue ();
          while ( it != m_rxonBuffer.end () &&
                  m_rxonBuffer[ m_vrMs.GetValue () ].m_pduComplete )
            {
              m_vrMs++;
              it = m_rxonBuffer.find (m_vrMs.GetValue ());
              NS_LOG_LOGIC ("Incr VR(MS) = " << m_vrMs);

              NS_ASSERT_MSG (firstVrMs != m_vrMs.GetValue (), "Infinite loop in RxonBuffer");
            }
          NS_LOG_LOGIC ("New VR(MS) = " << m_vrMs);
        }

      // - if x = VR(R):
      //     - if all byte segments of the AMD PDU with SN = VR(R) are received:
      //         - update VR(R) to the SN of the first AMD PDU with SN > current VR(R) for which not all byte segments have been received;
      //         - update VR(MR) to the updated VR(R) + AM_Window_Size;
      //     - reassemble RLC SDUs from any byte segments of AMD PDUs with SN that falls outside of the receiving window and in-sequence byte segments of the AMD PDU with SN = VR(R), remove RLC headers when doing so and deliver the reassembled RLC SDUs to upper layer in sequence if not delivered before;

      if ( seqNumber == m_vrR )
        {
          std::map <uint16_t, PduBuffer>::iterator it = m_rxonBuffer.find (seqNumber.GetValue ());
          if ( it != m_rxonBuffer.end () &&
               m_rxonBuffer[ seqNumber.GetValue () ].m_pduComplete )
            {
              it = m_rxonBuffer.find (m_vrR.GetValue ());
              int firstVrR = m_vrR.GetValue ();
              while ( it != m_rxonBuffer.end () &&
                      m_rxonBuffer[ m_vrR.GetValue () ].m_pduComplete )
                {
                  NS_LOG_LOGIC ("Reassemble and Deliver ( SN = " << m_vrR << " )");
                  NS_ASSERT_MSG (m_rxonBuffer[ m_vrR.GetValue () ].m_byteSegments.size () == 1,
                                "Too many segments. PDU Reassembly process didn't work");
                  ReassembleAndDeliver (m_rxonBuffer[ m_vrR.GetValue () ].m_byteSegments.front ());
                  m_rxonBuffer.erase (m_vrR.GetValue ());

                  m_vrR++;
                  it = m_rxonBuffer.find (m_vrR.GetValue ());

                  NS_ASSERT_MSG (firstVrR != m_vrR.GetValue (), "Infinite loop in RxonBuffer");
                }
              NS_LOG_LOGIC ("New VR(R)  = " << m_vrR);
              m_vrMr = m_vrR + m_windowSize;
              NS_LOG_LOGIC ("New VR(MR) = " << m_vrMr);
            }

//           NS_LOG_LOGIC ("Reassemble and Deliver ( SN = " << seqNumber << " )");
//           NS_ASSERT_MSG (m_rxonBuffer[ seqNumber.GetValue () ].m_byteSegments.size () == 1,
//                          "Too many segments. PDU Reassembly process didn't work");
//           ReassembleAndDeliver (m_rxonBuffer[ seqNumber.GetValue () ].m_byteSegments.front ());
//           m_rxonBuffer.erase (seqNumber.GetValue ());
        }

      // - if t-Reordering is running:
      //     - if VR(X) = VR(R); or
      //     - if VR(X) falls outside of the receiving window and VR(X) is not equal to VR(MR):
      //         - stop and reset t-Reordering;

      if ( m_reorderingTimer.IsRunning () )
        {
          NS_LOG_LOGIC ("Reordering timer is running");
          if ( (m_vrX == m_vrR) ||
               ( (! IsInsideReceivingWindow (m_vrX)) && (m_vrX != m_vrMr) )
             )
            {
              // TODO stop and reset the t-Reordering
              NS_LOG_LOGIC ("Stop reordering timer");
              m_reorderingTimer.Cancel ();
            }
        }

      // - if t-Reordering is not running (includes the case t-Reordering is stopped due to actions above):
      //     - if VR (H) > VR(R):
      //         - start t-Reordering;
      //         - set VR(X) to VR(H).

      if ( ! m_reorderingTimer.IsRunning () )
        {
          NS_LOG_LOGIC ("Reordering timer is not running");
          if ( m_vrH > m_vrR )
            {
              NS_LOG_LOGIC ("Start reordering timer");
              m_reorderingTimer = Simulator::Schedule (Time ("0.1s"),
                                                       &LteRlcAm::ExpireReorderingTimer ,this);
              m_vrX = m_vrH;
              NS_LOG_LOGIC ("New VR(X) = " << m_vrX);
            }
        }



      // TODO To remove

      // 5.1.2.2.3 Actions when an UMD PDU is placed in the reception buffer
      // When an UMD PDU with SN = x is placed in the reception buffer, the receiving UM RLC entity shall:

      // - if x falls outside of the reordering window:
      //    - update VR(UH) to x + 1;
      //    - reassemble RLC SDUs from any UMD PDUs with SN that falls outside of the reordering window, remove
      //      RLC headers when doing so and deliver the reassembled RLC SDUs to upper layer in ascending order of the
      //      RLC SN if not delivered before;
      //    - if VR(UR) falls outside of the reordering window:
      //        - set VR(UR) to (VR(UH) - UM_Window_Size);

//       if ( ! IsInsideReorderingWindow (seqNumber))
//         {
//           NS_LOG_LOGIC ("SN outside the reordering window");
// 
//           m_vrUh = seqNumber + 1;
//           NS_LOG_LOGIC ("New VR(UH) = " << m_vrUh);
// 
//           ReassembleOutsideWindow ();
// 
//           if ( ! IsInsideReorderingWindow (m_vrUr) )
//             {
//               m_vrUr = m_vrUh - m_windowSize;
//               NS_LOG_LOGIC ("VR(UR) outside the reordering window");
//               NS_LOG_LOGIC ("New VR(UR) = " << m_vrUr);
//             }
//         }

      // - if the reception buffer contains an UMD PDU with SN = VR(UR):
      //    - update VR(UR) to the SN of the first UMD PDU with SN > current VR(UR) that has not been received;
      //    - reassemble RLC SDUs from any UMD PDUs with SN < updated VR(UR), remove RLC headers when doing
      //      so and deliver the reassembled RLC SDUs to upper layer in ascending order of the RLC SN if not delivered
      //      before;

//       if ( m_rxBuffer.count (m_vrUr) > 0 )
//         {
//           NS_LOG_LOGIC ("Reception buffer contains SN = " << m_vrUr);
// 
//           std::map <uint16_t, Ptr<Packet> >::iterator it;
//           uint16_t newVrUr;
// 
//           it = m_rxBuffer.find (m_vrUr);
//           newVrUr = (it->first) + 1;
//           while ( m_rxBuffer.count (newVrUr) > 0 )
//             {
//               newVrUr++;
//             }
//           m_vrUr = newVrUr;
//           NS_LOG_LOGIC ("New VR(UR) = " << m_vrUr);
// 
//           ReassembleSnLessThan (m_vrUr);
//         }

      // - if t-Reordering is running:
      //    - if VR(UX) <= VR(UR); or
      //    - if VR(UX) falls outside of the reordering window and VR(UX) is not equal to VR(UH)::
      //        - stop and reset t-Reordering;
//       if ( m_reorderingTimer.IsRunning () )
//         {
//           NS_LOG_LOGIC ("Reordering timer is running");
// 
//           if ( (m_vrUx <= m_vrUr) ||
//               ((! IsInsideReorderingWindow (m_vrUx)) && (m_vrUx != m_vrUh)) )
//             {
//               NS_LOG_LOGIC ("Stop reordering timer");
//               m_reorderingTimer.Cancel ();
//             }
//         }

      // - if t-Reordering is not running (includes the case when t-Reordering is stopped due to actions above):
      //    - if VR(UH) > VR(UR):
      //        - start t-Reordering;
      //        - set VR(UX) to VR(UH).
//       if ( ! m_reorderingTimer.IsRunning () )
//         {
//           NS_LOG_LOGIC ("Reordering timer is not running");
// 
//           if ( m_vrUx > m_vrUr )
//             {
//               NS_LOG_LOGIC ("VR(UX) > VR(UR). " << m_vrUx << " > " << m_vrUr);
//               NS_LOG_LOGIC ("Start reordering timer");
//               m_reorderingTimer = Simulator::Schedule (Time ("1.0s"),
//                                                       &LteRlcAm::ExpireReorderingTimer ,this);
//               m_vrUx = m_vrUh;
//               NS_LOG_LOGIC ("New VR(UX) = " << m_vrUx);
//             }
//         }

    }
  else if ( rlcAmHeader.IsControlPdu () )
    {
      NS_LOG_INFO ("Control AM RLC PDU");

      SequenceNumber10 ackSn = rlcAmHeader.GetAckSn ();
//       SequenceNumber10 seqNumber = m_vtA;

      NS_LOG_INFO ("ackSn     = " << ackSn);
      NS_LOG_INFO ("VT(A)     = " << m_vtA);
      NS_LOG_INFO ("VT(S)     = " << m_vtS);
      while (m_vtA < ackSn && m_vtA < m_vtS)
        {
//           NS_LOG_INFO ("seqNumber = " << seqNumber);
//           NS_LOG_INFO ("m_txedBuffer( VT(A) ).size = " << m_txedBuffer.size ());

          uint16_t seqNumberValue = m_vtA.GetValue ();
          if (m_txedBuffer.at (seqNumberValue))
            {
              NS_LOG_INFO ("ACKed SN = " << seqNumberValue << " from txedBuffer");
//               NS_LOG_INFO ("m_txedBuffer( " << m_vtA << " )->GetSize = " << m_txedBuffer.at (m_vtA.GetValue ())->GetSize ());
              m_txedBufferSize -= m_txedBuffer.at (seqNumberValue)->GetSize ();
              m_txedBuffer.at (seqNumberValue) = 0;
            }

          if (m_retxBuffer.at (seqNumberValue).m_pdu)
            {
              NS_LOG_INFO ("ACKed SN = " << seqNumberValue << " from retxBuffer");
              m_retxBufferSize -= m_retxBuffer.at (seqNumberValue).m_pdu->GetSize ();
              m_retxBuffer.at (seqNumberValue).m_pdu = 0;
              m_retxBuffer.at (seqNumberValue).m_retxCount = 0;
            }

          m_vtA++;
        }

      NS_LOG_INFO ("New VT(A) = " << m_vtA);

      SequenceNumber10 seqNumber = m_vtA;
      uint16_t seqNumberValue;
      while (seqNumber < m_vtS)
        {
          seqNumberValue = seqNumber.GetValue ();
          if (m_txedBuffer.at (seqNumberValue))
            {
              NS_LOG_INFO ("Move SN = " << seqNumberValue << " to retxBuffer");
              m_retxBuffer.at (seqNumberValue).m_pdu = m_txedBuffer.at (seqNumberValue)->Copy ();
              m_retxBuffer.at (seqNumberValue).m_retxCount = 0;
              m_retxBufferSize += m_retxBuffer.at (seqNumberValue).m_pdu->GetSize ();

              m_txedBufferSize -= m_txedBuffer.at (seqNumberValue)->GetSize ();
              m_txedBuffer.at (seqNumberValue) = 0;
            }
          else if (m_retxBuffer.at (seqNumberValue).m_pdu)
            {
              m_retxBuffer.at (seqNumberValue).m_retxCount++;
              NS_LOG_INFO ("Incr RETX_COUNT for SN = " << seqNumberValue);
              if (m_retxBuffer.at (seqNumberValue).m_retxCount >= m_maxRetxThreshold)
                {
                  NS_LOG_INFO ("Max RETX_COUNT for SN = " << seqNumberValue);
                }
            }

          seqNumber++;
        }

      return;
    }
  else
    {
      NS_LOG_WARN ("Wrong AM RLC PDU type");
      return;
    }

}


void
LteRlcAm::Start ()
{
  NS_LOG_FUNCTION (this);

  LteMacSapProvider::ReportBufferStatusParameters p;
  p.rnti = m_rnti;
  p.lcid = m_lcid;
  p.txQueueSize = 0;
  p.txQueueHolDelay = 0;
  p.retxQueueSize = 0;
  p.retxQueueHolDelay = 0;
  p.statusPduSize = 0;

  m_macSapProvider->ReportBufferStatus (p);
}


bool
LteRlcAm::IsInsideReceivingWindow (SequenceNumber10 seqNumber)
{
  NS_LOG_FUNCTION (this << seqNumber);
  NS_LOG_LOGIC ("Receiving Window: " <<
                m_vrR << " <= " << seqNumber << " <= " << m_vrMr);

  m_vrR.SetModulusBase (m_vrR);
  m_vrMr.SetModulusBase (m_vrR);
  seqNumber.SetModulusBase (m_vrR);

  if ( (m_vrR <= seqNumber) && (seqNumber < m_vrMr ) )
    {
      NS_LOG_LOGIC (seqNumber << " is INSIDE the receiving window");
      return true;
    }
  else
    {
      NS_LOG_LOGIC (seqNumber << " is OUTSIDE the receiving window");
      return false;
    }
}


void
LteRlcAm::ReassembleAndDeliver (Ptr<Packet> packet)
{
  LteRlcAmHeader rlcAmHeader;
  packet->RemoveHeader (rlcAmHeader);
  uint8_t framingInfo = rlcAmHeader.GetFramingInfo ();
  SequenceNumber10 currSeqNumber = rlcAmHeader.GetSequenceNumber ();
  bool expectedSnLost;

  if ( currSeqNumber != m_expectedSeqNumber )
    {
      expectedSnLost = true;
      NS_LOG_LOGIC ("There are losses. Expected SN = " << m_expectedSeqNumber << ". Current SN = " << currSeqNumber);
      m_expectedSeqNumber = currSeqNumber + 1;
    }
  else
    {
      expectedSnLost = false;
      NS_LOG_LOGIC ("No losses. Expected SN = " << m_expectedSeqNumber << ". Current SN = " << currSeqNumber);
      m_expectedSeqNumber = m_expectedSeqNumber + 1;
    }

  // Build list of SDUs
  uint8_t extensionBit;
  uint16_t lengthIndicator;
  do
    {
      extensionBit = rlcAmHeader.PopExtensionBit ();
      NS_LOG_LOGIC ("E = " << (uint16_t)extensionBit);

      if ( extensionBit == 0 )
        {
          m_sdusBuffer.push_back (packet);
        }
      else // extensionBit == 1
        {
          lengthIndicator = rlcAmHeader.PopLengthIndicator ();
          NS_LOG_LOGIC ("LI = " << lengthIndicator);

          // Check if there is enough data in the packet
          if ( lengthIndicator >= packet->GetSize () )
            {
              NS_LOG_LOGIC ("INTERNAL ERROR: Not enough data in the packet (" << packet->GetSize () << "). Needed LI=" << lengthIndicator);
              // TODO What to do in this case? Discard packet and continue? Or Assert?
            }

          // Split packet in two fragments
          Ptr<Packet> data_field = packet->CreateFragment (0, lengthIndicator);
          packet->RemoveAtStart (lengthIndicator);

          m_sdusBuffer.push_back (data_field);
        }
    }
  while ( extensionBit == 1 );

  std::list < Ptr<Packet> >::iterator it;

  // Current reassembling state
  if      (m_reassemblingState == WAITING_S0_FULL)  NS_LOG_LOGIC ("Reassembling State = 'WAITING_S0_FULL'");
  else if (m_reassemblingState == WAITING_SI_SF)    NS_LOG_LOGIC ("Reassembling State = 'WAITING_SI_SF'");
  else                                              NS_LOG_LOGIC ("Reassembling State = Unknown state");

  // Received framing Info
  NS_LOG_LOGIC ("Framing Info = " << (uint16_t)framingInfo);
  NS_LOG_LOGIC ("m_sdusBuffer = " << m_sdusBuffer.size ());

  // Reassemble the list of SDUs (when there is no losses)
  if (!expectedSnLost)
    {
      switch (m_reassemblingState)
        {
          case WAITING_S0_FULL:
                  switch (framingInfo)
                    {
                      case (LteRlcAmHeader::FIRST_BYTE | LteRlcAmHeader::LAST_BYTE):
                              m_reassemblingState = WAITING_S0_FULL;

                              /**
                              * Deliver one or multiple PDUs
                              */
                              for ( it = m_sdusBuffer.begin () ; it != m_sdusBuffer.end () ; it++ )
                                {
                                  m_rlcSapUser->ReceivePdcpPdu (*it);
                                }
                              m_sdusBuffer.clear ();
                      break;

                      case (LteRlcAmHeader::FIRST_BYTE | LteRlcAmHeader::NO_LAST_BYTE):
                              m_reassemblingState = WAITING_SI_SF;

                              /**
                              * Deliver full PDUs
                              */
                              while ( m_sdusBuffer.size () > 1 )
                                {
                                  m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                  m_sdusBuffer.pop_front ();
                                }

                              /**
                              * Keep S0
                              */
                              m_keepS0 = m_sdusBuffer.front ();
                              m_sdusBuffer.pop_front ();
                      break;

                      case (LteRlcAmHeader::NO_FIRST_BYTE | LteRlcAmHeader::LAST_BYTE):
                      case (LteRlcAmHeader::NO_FIRST_BYTE | LteRlcAmHeader::NO_LAST_BYTE):
                      default:
                              /**
                              * ERROR: Transition not possible
                              */
                              NS_LOG_LOGIC ("INTERNAL ERROR: Transition not possible. FI = " << (uint32_t) framingInfo);
                      break;
                    }
          break;

          case WAITING_SI_SF:
                  switch (framingInfo)
                    {
                      case (LteRlcAmHeader::NO_FIRST_BYTE | LteRlcAmHeader::LAST_BYTE):
                              m_reassemblingState = WAITING_S0_FULL;

                              /**
                              * Deliver (Kept)S0 + SN
                              */
                              m_keepS0->AddAtEnd (m_sdusBuffer.front ());
                              m_sdusBuffer.pop_front ();
                              m_rlcSapUser->ReceivePdcpPdu (m_keepS0);

                              /**
                                * Deliver zero, one or multiple PDUs
                                */
                              while ( ! m_sdusBuffer.empty () )
                                {
                                  m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                  m_sdusBuffer.pop_front ();
                                }
                      break;

                      case (LteRlcAmHeader::NO_FIRST_BYTE | LteRlcAmHeader::NO_LAST_BYTE):
                              m_reassemblingState = WAITING_SI_SF;

                              /**
                              * Keep SI
                              */
                              if ( m_sdusBuffer.size () == 1 )
                                {
                                  m_keepS0->AddAtEnd (m_sdusBuffer.front ());
                                  m_sdusBuffer.pop_front ();
                                }
                              else // m_sdusBuffer.size () > 1
                                {
                                  /**
                                  * Deliver (Kept)S0 + SN
                                  */
                                  m_keepS0->AddAtEnd (m_sdusBuffer.front ());
                                  m_sdusBuffer.pop_front ();
                                  m_rlcSapUser->ReceivePdcpPdu (m_keepS0);

                                  /**
                                  * Deliver zero, one or multiple PDUs
                                  */
                                  while ( m_sdusBuffer.size () > 1 )
                                    {
                                      m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                      m_sdusBuffer.pop_front ();
                                    }

                                  /**
                                  * Keep S0
                                  */
                                  m_keepS0 = m_sdusBuffer.front ();
                                  m_sdusBuffer.pop_front ();
                                }
                      break;

                      case (LteRlcAmHeader::FIRST_BYTE | LteRlcAmHeader::LAST_BYTE):
                      case (LteRlcAmHeader::FIRST_BYTE | LteRlcAmHeader::NO_LAST_BYTE):
                      default:
                              /**
                                * ERROR: Transition not possible
                                */
                              NS_LOG_LOGIC ("INTERNAL ERROR: Transition not possible. FI = " << (uint32_t) framingInfo);
                      break;
                    }
          break;

          default:
                NS_LOG_LOGIC ("INTERNAL ERROR: Wrong reassembling state = " << (uint32_t) m_reassemblingState);
          break;
        }
    }
  else // Reassemble the list of SDUs (when there are losses, i.e. the received SN is not the expected one)
    {
      switch (m_reassemblingState)
        {
          case WAITING_S0_FULL:
                  switch (framingInfo)
                    {
                      case (LteRlcAmHeader::FIRST_BYTE | LteRlcAmHeader::LAST_BYTE):
                              m_reassemblingState = WAITING_S0_FULL;

                              /**
                               * Deliver one or multiple PDUs
                               */
                              for ( it = m_sdusBuffer.begin () ; it != m_sdusBuffer.end () ; it++ )
                                {
                                  m_rlcSapUser->ReceivePdcpPdu (*it);
                                }
                              m_sdusBuffer.clear ();
                      break;

                      case (LteRlcAmHeader::FIRST_BYTE | LteRlcAmHeader::NO_LAST_BYTE):
                              m_reassemblingState = WAITING_SI_SF;

                              /**
                               * Deliver full PDUs
                               */
                              while ( m_sdusBuffer.size () > 1 )
                                {
                                  m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                  m_sdusBuffer.pop_front ();
                                }

                              /**
                               * Keep S0
                               */
                              m_keepS0 = m_sdusBuffer.front ();
                              m_sdusBuffer.pop_front ();
                      break;

                      case (LteRlcAmHeader::NO_FIRST_BYTE | LteRlcAmHeader::LAST_BYTE):
                              m_reassemblingState = WAITING_S0_FULL;

                              /**
                               * Discard SN
                               */
                              m_sdusBuffer.pop_front ();

                              /**
                               * Deliver zero, one or multiple PDUs
                               */
                              while ( ! m_sdusBuffer.empty () )
                                {
                                  m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                  m_sdusBuffer.pop_front ();
                                }
                      break;

                      case (LteRlcAmHeader::NO_FIRST_BYTE | LteRlcAmHeader::NO_LAST_BYTE):
                              if ( m_sdusBuffer.size () == 1 )
                                {
                                  m_reassemblingState = WAITING_S0_FULL;
                                }
                              else
                                {
                                  m_reassemblingState = WAITING_SI_SF;
                                }

                              /**
                               * Discard SI or SN
                               */
                              m_sdusBuffer.pop_front ();

                              if ( m_sdusBuffer.size () > 0 )
                                {
                                  /**
                                  * Deliver zero, one or multiple PDUs
                                  */
                                  while ( m_sdusBuffer.size () > 1 )
                                    {
                                      m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                      m_sdusBuffer.pop_front ();
                                    }

                                  /**
                                  * Keep S0
                                  */
                                  m_keepS0 = m_sdusBuffer.front ();
                                  m_sdusBuffer.pop_front ();
                                }
                      break;

                      default:
                              /**
                               * ERROR: Transition not possible
                               */
                              NS_LOG_LOGIC ("INTERNAL ERROR: Transition not possible. FI = " << (uint32_t) framingInfo);
                      break;
                    }
          break;

          case WAITING_SI_SF:
                  switch (framingInfo)
                    {
                      case (LteRlcAmHeader::FIRST_BYTE | LteRlcAmHeader::LAST_BYTE):
                              m_reassemblingState = WAITING_S0_FULL;

                              /**
                               * Discard S0
                               */
                              m_keepS0 = 0;

                              /**
                               * Deliver one or multiple PDUs
                               */
                              while ( ! m_sdusBuffer.empty () )
                                {
                                  m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                  m_sdusBuffer.pop_front ();
                                }
                      break;

                      case (LteRlcAmHeader::FIRST_BYTE | LteRlcAmHeader::NO_LAST_BYTE):
                              m_reassemblingState = WAITING_SI_SF;

                              /**
                               * Discard S0
                               */
                              m_keepS0 = 0;

                              /**
                               * Deliver zero, one or multiple PDUs
                               */
                              while ( m_sdusBuffer.size () > 1 )
                                {
                                  m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                  m_sdusBuffer.pop_front ();
                                }

                              /**
                               * Keep S0
                               */
                              m_keepS0 = m_sdusBuffer.front ();
                              m_sdusBuffer.pop_front ();

                      break;

                      case (LteRlcAmHeader::NO_FIRST_BYTE | LteRlcAmHeader::LAST_BYTE):
                              m_reassemblingState = WAITING_S0_FULL;

                              /**
                               * Discard S0
                               */
                              m_keepS0 = 0;

                              /**
                               * Discard SI or SN
                               */
                              m_sdusBuffer.pop_front ();

                              /**
                               * Deliver zero, one or multiple PDUs
                               */
                              while ( ! m_sdusBuffer.empty () )
                                {
                                  m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                  m_sdusBuffer.pop_front ();
                                }
                      break;

                      case (LteRlcAmHeader::NO_FIRST_BYTE | LteRlcAmHeader::NO_LAST_BYTE):
                              if ( m_sdusBuffer.size () == 1 )
                                {
                                  m_reassemblingState = WAITING_S0_FULL;
                                }
                              else
                                {
                                  m_reassemblingState = WAITING_SI_SF;
                                }

                              /**
                               * Discard S0
                               */
                              m_keepS0 = 0;

                              /**
                               * Discard SI or SN
                               */
                              m_sdusBuffer.pop_front ();

                              if ( m_sdusBuffer.size () > 0 )
                                {
                                  /**
                                   * Deliver zero, one or multiple PDUs
                                   */
                                  while ( m_sdusBuffer.size () > 1 )
                                    {
                                      m_rlcSapUser->ReceivePdcpPdu (m_sdusBuffer.front ());
                                      m_sdusBuffer.pop_front ();
                                    }

                                  /**
                                   * Keep S0
                                   */
                                  m_keepS0 = m_sdusBuffer.front ();
                                  m_sdusBuffer.pop_front ();
                                }
                      break;

                      default:
                              /**
                                * ERROR: Transition not possible
                                */
                              NS_LOG_LOGIC ("INTERNAL ERROR: Transition not possible. FI = " << (uint32_t) framingInfo);
                      break;
                    }
          break;

          default:
                NS_LOG_LOGIC ("INTERNAL ERROR: Wrong reassembling state = " << (uint32_t) m_reassemblingState);
          break;
        }
    }

}


// TODO To remove
// void
// LteRlcAm::ReassembleOutsideWindow (void)
// {
//   NS_LOG_LOGIC ("Reassemble Outside Window");
// 
//   std::map <uint16_t, Ptr<Packet> >::iterator it;
//   it = m_rxBuffer.begin ();
// 
//   while ( (it != m_rxBuffer.end ()) && ! IsInsideReorderingWindow (it->first) )
//     {
//       NS_LOG_LOGIC ("SN = " << it->first);
// 
//       // Reassemble RLC SDUs and deliver the PDCP PDU to upper layer
//       ReassembleAndDeliver (it->second);
//       m_rxBuffer.erase (it);
//       it++;
//     }
// 
//   if (it != m_rxBuffer.end ())
//     {
//       NS_LOG_LOGIC ("(SN = " << it->first << ") is inside the reordering window");
//     }
// }


// TODO To remove
// void
// LteRlcAm::ReassembleSnLessThan (uint16_t seqNumber)
// {
//   NS_LOG_LOGIC ("Reassemble SN < updated VR(UR)" );
// 
//   std::map <uint16_t, Ptr<Packet> >::iterator it;
//   it = m_rxBuffer.begin ();
// 
//   while ( (it != m_rxBuffer.end ()) && (it->first < seqNumber) )
//     {
//       NS_LOG_LOGIC ("SN = " << it->first);
// 
//       // Reassemble RLC SDUs and deliver the PDCP PDU to upper layer
//       ReassembleAndDeliver (it->second);
//       m_rxBuffer.erase (it);
//       it++;
//     }
// 
//   if (it != m_rxBuffer.end ())
//     {
//       NS_LOG_LOGIC ("(SN = " << it->first << ") >= " << m_vrUr);
//     }
// }


void
LteRlcAm::ExpireReorderingTimer (void)
{
  NS_LOG_LOGIC ("Reordering Timer has expired");

  // 5.1.3.2.4 Actions when t-Reordering expires
  // When t-Reordering expires, the receiving side of an AM RLC entity shall:
  // - update VR(MS) to the SN of the first AMD PDU with SN >= VR(X) for which not all byte segments
  //   have been received;
  // - if VR(H) > VR(MS):
  //    - start t-Reordering;
  //    - set VR(X) to VR(H).

  m_vrMs = m_vrX;
  int firstVrMs = m_vrMs.GetValue ();
  std::map <uint16_t, PduBuffer>::iterator it = m_rxonBuffer.find (m_vrMs.GetValue ());
  while ( it != m_rxonBuffer.end () &&
          m_rxonBuffer[ m_vrMs.GetValue () ].m_pduComplete )
    {
      m_vrMs++;
      it = m_rxonBuffer.find (m_vrMs.GetValue ());

      NS_ASSERT_MSG (firstVrMs != m_vrMs.GetValue (), "Infinite loop in ExpireReorderingTimer");
    }
  NS_LOG_LOGIC ("New VR(MS) = " << m_vrMs);

  if ( m_vrH > m_vrMs )
    {
      NS_LOG_LOGIC ("Start reordering timer");
      m_reorderingTimer = Simulator::Schedule (Time ("0.1s"),
                                              &LteRlcAm::ExpireReorderingTimer ,this);
      m_vrX = m_vrH;
      NS_LOG_LOGIC ("New VR(MS) = " << m_vrMs);
    }
}

void
LteRlcAm::ExpirePollRetransmitTimer (void)
{
  NS_LOG_LOGIC ("PollRetransmit Timer has expired");
  NS_LOG_LOGIC ("TODO To Check");
}



} // namespace ns3
