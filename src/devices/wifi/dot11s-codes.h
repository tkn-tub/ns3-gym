/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008,2009 IITP RAS
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
 * Authors: Aleksey Kovalenko <kovalenko@iitp.ru>
 *          Kirill Andreev <andreev@iitp.ru>
 */


#ifndef DOT11S_CODES_H
#define DOT11S_CODES_H

namespace ns3 {

enum dot11sElementID {
	MESH_CONFIGURATION = 18,
	MESH_ID,
	LINK_METRIC_REPORT,
	CONGESTION_NOTIFICATION,
	PEER_LINK_MANAGEMENT,
	MESH_CHANNEL_SWITCH_ANNOUNCEMENT,
	MESH_TIM,
	AWAKE_WINDOW,
	SYNCHRONIZATION_PROTOCOL,
	BEACON_TIMING,
	MDAOP_SETUP_REQUEST,
	MDAOP_SETUP_REPLY,
	MDAOP_ADVERTISEMENT,
	MDAOP_SET_TEARDOWN,
	CONNECTIVITY_REPORT,
	PORTAL_ANNOUNCEMENT,
	ROOT_ANNOUCEMENT,
	PATH_REQUEST,
	PATH_REPLY,
	PATH_ERROR,
	PROXY_UPDATE,
	PROXY_UPDATE_CONFIRMATION,
	MSCIE,
	MSAIE,
};

enum dot11sReasonCode {
	PEER_LINK_CANCELLED,
	MESH_MAX_PEERS,
	MESH_CAPABILITY_POLICY_VIOLATION,
	MESH_CLOSE_RCVD,
	MESH_MAX_RETRIES,
	MESH_CONFIRM_TIMEOUT,
	MESH_SECURITY_ROLE_NEGOTIATION_DIFFERS,
	MESH_SECURITY_AUTHENTICATION_IMPOSSIBLE,
	MESH_SECURITY_FAILED_VERIFICATION,
	MESH_INVALID_GTK,
	MESH_MISMATCH_GTK,
	MESH_INCONSISTENT_PARAMETERS,
	MESH_CONFIGURATION_POLICY_VIOLATION,
	DOT11S_REASON_RESERVED,
};

enum dot11sStatusCode {
	PEAR_LINK_ESTABLISHED,
	PEAR_LINK_CLOSED,
	NO_LISTED_KEY_HOLDER,
	MESH_KEY_HANDSHAKE_MALFORMED,
	PEAR_LINK_MAX_RETRIES,
	PEAR_LINK_NO_PMK,
	PEAR_LINK_ALT_PMK,
	PEAR_LINK_NO_AKM,
	PEAR_LINK_ALT_AKM,
	PEAR_LINK_NO_KDF,
	PEAR_LINK_SA_ESTABLISHED,
	AUTHENTICATION_REJECTED_CLOGGING,
	DOT11S_STATUS_RESERVED,
};

}
#endif
