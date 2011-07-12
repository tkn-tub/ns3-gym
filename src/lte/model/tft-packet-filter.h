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
 * Author: Nicola Baldo  <nbaldo@cttc.es>
 */


#ifndef TFT_PACKET_FILTER_H
#define TFT_PACKET_FILTER_H


/**
 * Implement the data structure representing a TrafficFlowTemplate
 * Packet Filter.
 * See 3GPP TS 24.008 version 8.7.0 Release 8, Table 10.5.162/3GPP TS
 * 24.008: Traffic flow template information element  
 * 
 * With respect to the Packet Filter specification in the above doc,
 * the following features are NOT supported:
 *  - IPv6 filtering (including flow labels)
 *  - IPSec filtering
 */
struct TftPacketFilter : public SimpleRefCount<TftPacketFilter>
{
  
  Ipv4Address remoteAddress;     /**< IPv4 address of the remote host  */
  Ipv4Address remoteAddressMask; /**< IPv4 address mask of the remote host */
  Ipv4Address localAddress;      /**< IPv4 address of the UE */
  Ipv4Address localAddressMask;  /**< IPv4 address mask of the UE */
  
  uint16_t remotePortStart;  /**< start of the port number range of the remote host */
  uint16_t remotePortEnd;    /**< end of the port number range of the remote host */
  uint16_t localPortStart;   /**< start of the port number range of the UE */
  uint16_t localPortEnd;     /**< end of the port number range of the UE */
  
  uint8_t typeOfService;     /**< type of service field */
  uint8_t typeOfServiceMask; /**< type of service field mask */
}




#endif /* TFT_PACKET_FILTER_H */ 


