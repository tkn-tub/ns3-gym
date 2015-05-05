/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Dean Armstrong
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
 * Author: Dean Armstrong <deanarm@gmail.com>
 */

#ifndef MESH_INFORMATION_ELEMENT_H
#define MESH_INFORMATION_ELEMENT_H

#include "ns3/wifi-information-element.h"

namespace ns3 {

#define IE11S_LINK_METRIC_REPORT               ((WifiInformationElementId)115)
#define IE11S_CONGESTION_NOTIFICATION          ((WifiInformationElementId)116)
#define IE11S_SUPP_MBSS_REG_CLASSES_CHANNELS   ((WifiInformationElementId)23)
#define IE11S_MESH_CHANNEL_SWITCH_ANNOUNCEMENT ((WifiInformationElementId)24)
#define IE11S_MESH_TIM                         ((WifiInformationElementId)25)
#define IE11S_AWAKE_WINDOW                     ((WifiInformationElementId)119)
#define IE11S_BEACON_TIMING                    ((WifiInformationElementId)120)
#define IE11S_MCCAOP_SETUP_REQUEST             ((WifiInformationElementId)121)
#define IE11S_MCCAOP_SETUP_REPLY               ((WifiInformationElementId)122)
#define IE11S_MCCAOP_ADVERTISEMENT             ((WifiInformationElementId)123)
#define IE11S_MCCAOP_RESERVATION_TEARDOWN      ((WifiInformationElementId)31)
#define IE11S_PORTAL_ANNOUNCEMENT              ((WifiInformationElementId)32)
#define IE11S_PROXY_UPDATE                     ((WifiInformationElementId)137)
#define IE11S_PROXY_UPDATE_CONFIRMATION        ((WifiInformationElementId)138)
#define IE11S_ABBREVIATED_HANDSHAKE            ((WifiInformationElementId)39)
/* begin of open80211s-compatible IDs */
#define IE11S_MESH_CONFIGURATION               ((WifiInformationElementId)113)
#define IE11S_MESH_ID                          ((WifiInformationElementId)114)
#define IE11S_PEERING_MANAGEMENT               ((WifiInformationElementId)117)
/* end of open80211s-compatible IDs */
#define IE11S_RANN                             ((WifiInformationElementId)126)
/* begin of open80211s-compatible IDs */
#define IE11S_PREQ                             ((WifiInformationElementId)130)
#define IE11S_PREP                             ((WifiInformationElementId)131)
#define IE11S_PERR                             ((WifiInformationElementId)132)
/* end of open80211s-compatible IDs */
#define IE11S_MESH_PEERING_PROTOCOL_VERSION    ((WifiInformationElementId)74)

}

#endif /* MESH_INFORMATION_ELEMENT_H */
