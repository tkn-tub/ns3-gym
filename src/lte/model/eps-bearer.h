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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */


#ifndef EPS_BEARER
#define EPS_BEARER

#include <ns3/uinteger.h>

namespace ns3 {

/**
 * 3GPP TS 36.413 9.2.1.18 GBR QoS Information
 *
 */
struct GbrQosInformation
{
  /** 
   * Default constructor, initializes member variables to zero or equivalent
   */
  GbrQosInformation ();

  uint64_t gbrDl;  /**< Guaranteed Bit Rate (bit/s) in downlink */
  uint64_t gbrUl;  /**< Guaranteed Bit Rate (bit/s) in uplink */
  uint64_t mbrDl;  /**< Maximum Bit Rate (bit/s) in downlink */
  uint64_t mbrUl;  /**< Maximum Bit Rate (bit/s) in uplink */
};


/**
 * 3GPP 23.203 Section 6.1.7.3 Allocation and Retention Priority characteristics
 *
 */
struct AllocationRetentionPriority
{
  /** 
   * Default constructor, initializes member variables to zero or equivalent
   */
  AllocationRetentionPriority ();
  uint8_t priorityLevel;     ///< 1-15; 1 = highest
  bool preemptionCapability; ///< true if bearer can preempt others
  bool preemptionVulnerability; ///< true if bearer can be preempted by others
};

/**
 * This class contains the specification of EPS Bearers.
 *
 * See the following references:
 * 3GPP TS 23.203, Section 4.7.2 The EPS bearer
 * 3GPP TS 23.203, Section 4.7.3 Bearer level QoS parameters
 * 3GPP TS 36.413 Section 9.2.1.15 E-RAB Level QoS Parameters
 *
 */
struct EpsBearer
{

  /**
   * QoS Class Indicator. See 3GPP 23.203 Section 6.1.7.2 for standard values.
   */
  enum Qci
  {
    GBR_CONV_VOICE          = 1,
    GBR_CONV_VIDEO          = 2,
    GBR_GAMING              = 3,
    GBR_NON_CONV_VIDEO      = 4,
    NGBR_IMS                = 5,
    NGBR_VIDEO_TCP_OPERATOR = 6,
    NGBR_VOICE_VIDEO_GAMING = 7,
    NGBR_VIDEO_TCP_PREMIUM  = 8,
    NGBR_VIDEO_TCP_DEFAULT  = 9,
  } qci; ///< Qos class indicator

  GbrQosInformation gbrQosInfo; ///< GBR QOS information
  AllocationRetentionPriority arp; ///< allocation retention priority

  /**
   * Default constructor. QCI will be initialized to NGBR_VIDEO_TCP_DEFAULT
   * 
   */
  EpsBearer ();

  /**
   *
   * @param x the QoS Class Indicator
   *
   */
  EpsBearer (Qci x);

  /**
   *
   * @param x the QoS Class Indicator
   * @param y the GbrQosInformation
   *
   */
  EpsBearer (Qci x, GbrQosInformation y);

  /**
   *
   * @return true if the EPS Bearer is a Guaranteed Bit Rate bearer, false otherwise
   */
  bool IsGbr () const;

  /**
   *
   * @return the priority associated with the QCI of this bearer as per 3GPP 23.203 Section 6.1.7.2
   */
  uint8_t GetPriority () const;

  /**
   *
   *
   *
   * @return the packet delay budget associated with the QCI of this bearer as per 3GPP 23.203 Section 6.1.7.2
   */
  uint16_t GetPacketDelayBudgetMs () const;

  /**
   *
   *
   *
   * @return the packet error loss rate associated with the QCI of this bearer as per 3GPP 23.203 Section 6.1.7.2
   */
  double  GetPacketErrorLossRate () const;

};

} // namespace ns3


#endif // EPS_BEARER
