/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 The Boeing Company
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
 *  Author: kwong yin <kwong-sang.yin@boeing.com>
 */

/**
 * the following classes implements the 802.15.4 Mac Header
 * There are 4 types of 802.15.4 Mac Headers Frames, and they have these fields
 *
 *    Headers Frames  : Fields
 *    -------------------------------------------------------------------------------------------
 *    Beacon          : Frame Control, Sequence Number, Address Fields+, Auxiliary Security Header++.
 *    Data            : Frame Control, Sequence Number, Address Fields++, Auxiliary Security Header++.
 *    Acknowledgment  : Frame Control, Sequence Number.
 *    Command         : Frame Control, Sequence Number, Address Fields++, Auxiliary Security Header++.
 *
 *    + - The Address fields in Beacon frame is made up of the Source PAN Id and address only and size
 *        is  4 or 8 octets whereas the other frames may contain the Destination PAN Id and address as
 *        well. (see specs).
 *    ++ - These fields are optional and of variable size
 */

#ifndef LR_WPAN_MAC_HEADER_H
#define LR_WPAN_MAC_HEADER_H

#include <ns3/header.h>
#include <ns3/mac16-address.h>
#include <ns3/mac64-address.h>


namespace ns3 {


/* 
 * \ingroup lr-wpan
 * Represent the Mac Header with the Frame Control and Sequence Number fields 
 */
class LrWpanMacHeader : public Header
{

public:
  enum LrWpanMacType
  {
    LRWPAN_MAC_BEACON = 0,
    LRWPAN_MAC_DATA = 1,
    LRWPAN_MAC_ACKNOWLEDGMENT = 2,
    LRWPAN_MAC_COMMAND = 3,
    LRWPAN_MAC_RESERVED
  };

  enum AddrModeType
  {
    NOADDR = 0,
    RESADDR = 1,
    SHORTADDR = 2,
    EXTADDR = 3
  };

  enum KeyIdModeType
  {
    IMPLICIT = 0,
    NOKEYSOURCE = 1,
    SHORTKEYSOURCE = 2,
    LONGKEYSOURCE = 3
  };

  LrWpanMacHeader (void);

  LrWpanMacHeader (enum LrWpanMacType wpanMacType,      // Data, ACK, Control MAC Header must have
                   uint8_t seqNum);                     // frame control and sequence number.
                                                        // Beacon MAC Header must have frame control,
                                                        // sequence number, source PAN Id, source address.

  ~LrWpanMacHeader (void);


  enum LrWpanMacType GetType (void) const;
  uint16_t GetFrameControl (void) const;
  bool IsSecEnable (void) const;
  bool IsSecDisable (void) const;
  bool IsFrmPend (void) const;
  bool IsNoFrmPend (void) const;
  bool IsAckReq (void) const;
  bool IsNoAckReq (void) const;
  bool IsPanIdComp (void) const;
  bool IsNoPanIdComp (void) const;
  uint8_t GetFrmCtrlRes (void) const;
  uint8_t GetDstAddrMode (void) const;
  uint8_t GetFrameVer (void) const;
  uint8_t GetSrcAddrMode (void) const;

  uint8_t GetSeqNum (void) const;

  uint16_t GetDstPanId (void) const;
  Mac16Address GetShortDstAddr (void) const;
  Mac64Address GetExtDstAddr (void) const;
  uint16_t GetSrcPanId (void) const;
  Mac16Address GetShortSrcAddr (void) const;
  Mac64Address GetExtSrcAddr (void) const;

  uint8_t GetSecControl (void) const;
  uint32_t GetFrmCounter (void) const;

  uint8_t GetSecLevel (void) const;
  uint8_t GetKeyIdMode (void) const;
  uint8_t GetsecctrlReserved (void) const;

  uint32_t GetKeyIdSrc32 (void) const;
  uint64_t GetKeyIdSrc64 (void) const;
  uint8_t GetKeyIdIndex (void) const;


  bool IsBeacon (void) const;
  bool IsData (void) const;
  bool IsAcknowledgment (void) const;
  bool IsCommand (void) const;


  void SetType (enum LrWpanMacType wpanMacType);
  void SetFrameControl (uint16_t frameControl);
  void SetSecEnable (void);
  void SetSecDisable (void);
  void SetFrmPend (void);
  void SetNoFrmPend (void);
  void SetAckReq (void);
  void SetNoAckReq (void);
  void SetPanIdComp (void);
  void SetNoPanIdComp (void);
  void SetFrmCtrlRes (uint8_t res);
  void SetDstAddrMode (uint8_t addrMode);
  void SetFrameVer (uint8_t ver);
  void SetSrcAddrMode (uint8_t addrMode);

  void SetSeqNum (uint8_t seqNum);

  /* The Source/Destination Addressing fields are only set if SrcAddrMode/DstAddrMode are set */
  void SetSrcAddrFields (uint16_t panId,
                         Mac16Address addr);
  void SetSrcAddrFields (uint16_t panId,
                         Mac64Address addr);
  void SetDstAddrFields (uint16_t panId,
                         Mac16Address addr);
  void SetDstAddrFields (uint16_t panId,
                         Mac64Address addr);

  /* Auxiliary Security Header is only set if Sec Enable (SecU) field is set to 1 */
  void SetSecControl (uint8_t secLevel);
  void SetFrmCounter (uint32_t frmCntr);

  void SetSecLevel (uint8_t secLevel);
  void SetKeyIdMode (uint8_t keyIdMode);
  void SetsecctrlReserved (uint8_t res);

  /* Variable length will be dependent on Key Id Mode*/
  void SetKeyId (uint8_t keyIndex);
  void SetKeyId (uint32_t keySrc,
                 uint8_t keyIndex);
  void SetKeyId (uint64_t keySrc,
                 uint8_t keyIndex);


  std::string GetName (void) const;
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  void PrintFrameControl (std::ostream &os) const;
  void Print (std::ostream &os) const;
  uint32_t GetSerializedSize (void) const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);


private:
  /* Frame Control 2 Octets */
  /* Frame Control fields */
  uint8_t m_fctrlFrmType;               // Bit 0-2    = 0 - Beacon, 1 - Data, 2 - Ack, 3 - Command
  uint8_t m_fctrlSecU;                  // Bit 3      = 0 - no AuxSecHdr ,  1 - security enabled AuxSecHdr present
  uint8_t m_fctrlFrmPending;            // Bit 4
  uint8_t m_fctrlAckReq;                // Bit 5
  uint8_t m_fctrlPanIdComp;             // Bit 6      = 0 - no compression, 1 - using only DstPanId for both Src and DstPanId
  uint8_t m_fctrlReserved;              // Bit 7-9
  uint8_t m_fctrlDstAddrMode;           // Bit 10-11  = 0 - No DstAddr, 2 - ShtDstAddr, 3 - ExtDstAddr
  uint8_t m_fctrlFrmVer;                // Bit 12-13
  uint8_t m_fctrlSrcAddrMode;           // Bit 14-15  = 0 - No DstAddr, 2 - ShtDstAddr, 3 - ExtDstAddr

  /* Sequence Number */
  uint8_t m_SeqNum;                     // 1 Octet

  /* Addressing fields */
  uint16_t m_addrDstPanId;              // 0 or 2 Octet
  Mac16Address m_addrShortDstAddr;      // 0 or 8 Octet
  Mac64Address m_addrExtDstAddr;        // 0 or 8 Octet
  uint16_t m_addrSrcPanId;              // 0 or 2 Octet
  Mac16Address m_addrShortSrcAddr;      // 0 or 8 Octet
  Mac64Address m_addrExtSrcAddr;        // 0 or 8 Octet

  /* Auxiliary Security Header          // 0, 5, 6, 10 or 14 Octets */
  // uint8_t m_auxSecCtrl;                 // 1 Octet see below
  uint32_t m_auxFrmCntr;                // 4 Octet

  /* Security Control fields */
  uint8_t m_secctrlSecLevel;            // Bit 0-2
  uint8_t m_secctrlKeyIdMode;           // Bit 3-4 will indicate size of Key Id
                                        // = 0 - Key is determined implicitly
                                        //       from originator and receipient
                                        // = 1 - 1 Octet Key Index
                                        // = 2 - 1 octet Key Index and 4 oct Key src
                                        // = 3 - 1 octet Key Index and 8 oct Key src

  uint8_t m_secctrlReserved;            // Bit 5-7

  union
  {
    uint32_t m_auxKeyIdKeySrc32;        // 4 octets
    uint64_t m_auxKeyIdKeySrc64;        // 8 octets
  };

  uint8_t m_auxKeyIdKeyIndex;           // 1 octet

}; //LrWpanMacHeader

}; // namespace ns-3

#endif /* LR_WPAN_MAC_HEADER_H */
