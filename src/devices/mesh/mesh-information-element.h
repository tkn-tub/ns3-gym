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

#define IE11S_LINK_METRIC_REPORT               ((WifiElementId)20)
#define IE11S_CONGESTION_NOTIFICATION          ((WifiElementId)21)
#define IE11S_SUPP_MBSS_REG_CLASSES_CHANNELS   ((WifiElementId)23)
#define IE11S_MESH_CHANNEL_SWITCH_ANNOUNCEMENT ((WifiElementId)24)
#define IE11S_MESH_TIM                         ((WifiElementId)25)
#define IE11S_AWAKE_WINDOW                     ((WifiElementId)26)
#define IE11S_BEACON_TIMING                    ((WifiElementId)27)
#define IE11S_MCCAOP_SETUP_REQUEST             ((WifiElementId)28)
#define IE11S_MCCAOP_SETUP_REPLY               ((WifiElementId)29)
#define IE11S_MCCAOP_ADVERTISEMENT             ((WifiElementId)30)
#define IE11S_MCCAOP_RESERVATION_TEARDOWN      ((WifiElementId)31)
#define IE11S_PORTAL_ANNOUNCEMENT              ((WifiElementId)32)
#define IE11S_PROXY_UPDATE                     ((WifiElementId)37)
#define IE11S_PROXY_UPDATE_CONFIRMATION        ((WifiElementId)38)
#define IE11S_ABBREVIATED_HANDSHAKE            ((WifiElementId)39)
  /* begin of open80211s-compatible IDs */
#define IE11S_MESH_CONFIGURATION               ((WifiElementId)51)
#define IE11S_MESH_ID                          ((WifiElementId)52)
#define IE11S_PEERING_MANAGEMENT               ((WifiElementId)55)
  /* end of open80211s-compatible IDs */
#define IE11S_RANN                             ((WifiElementId)67)
  /* begin of open80211s-compatible IDs */
#define IE11S_PREQ                             ((WifiElementId)68)
#define IE11S_PREP                             ((WifiElementId)69)
#define IE11S_PERR                             ((WifiElementId)70)
  /* end of open80211s-compatible IDs */
#define IE11S_MESH_PEERING_PROTOCOL_VERSION    ((WifiElementId)74)

}

#endif /* MESH_INFORMATION_ELEMENT_H */
