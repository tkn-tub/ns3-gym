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
 * Authors: Kirill Andreev <andreev@iitp.ru>
 *          Evgeny Khorov <horov@frtk.ru>
 */


#ifndef _MESHWIFIHELPER_H
#define	_MESHWIFIHELPER_H

#include "ns3/wifi-net-device.h"
#include "ns3/l2-routing-net-device.h"
#include "ns3/mesh-wifi-mac.h"
#include "ns3/wifi-channel.h"
#include "ns3/mesh-wifi-peer-manager.h"
#include "wifi-helper.h"

namespace ns3
{
	class WifiChannel;

	class MeshWifiHelper
	{
		public:
			MeshWifiHelper ();
			virtual ~MeshWifiHelper ();

			/**
			 * \param type the type of peer manager to use.
			 * \param n0 the name of the attribute to set in the peer manager.
			 * \param v0 the value of the attribute to set in the peer manager.
			 * etc.
			 *
			 */
			void SetRemoteStationManager (std::string type,
					std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
					std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
					std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
					std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
					std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
					std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
					std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
					std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

			/**
			 * \param type the type of ns3::WifiMac to create.
			 * \param n0 the name of the attribute to set
			 * \param v0 the value of the attribute to set
			 *
			 * All the attributes specified in this method should exist
			 * in the requested mac.
			 */
			void SetMac (std::string type,
					std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
					std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
					std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
					std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
					std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
					std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
					std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
					std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue ());

			void SetPeerLinkManager (std::string type,
					std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
					std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
					std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
					std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
					std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
					std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
					std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
					std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue (),
					std::string n8 = "", const AttributeValue &v8 = EmptyAttributeValue (),
					std::string n9 = "", const AttributeValue &v9 = EmptyAttributeValue ());
			void SetL2RoutingProtocol(std::string type,
					std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
					std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
					std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
					std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
					std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
					std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
					std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
					std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue (),
					std::string n8 = "", const AttributeValue &v8 = EmptyAttributeValue (),
					std::string n9 = "", const AttributeValue &v9 = EmptyAttributeValue ());

			void SetL2RoutingNetDevice(std::string type,
					std::string n0 = "", const AttributeValue &v0 = EmptyAttributeValue (),
					std::string n1 = "", const AttributeValue &v1 = EmptyAttributeValue (),
					std::string n2 = "", const AttributeValue &v2 = EmptyAttributeValue (),
					std::string n3 = "", const AttributeValue &v3 = EmptyAttributeValue (),
					std::string n4 = "", const AttributeValue &v4 = EmptyAttributeValue (),
					std::string n5 = "", const AttributeValue &v5 = EmptyAttributeValue (),
					std::string n6 = "", const AttributeValue &v6 = EmptyAttributeValue (),
					std::string n7 = "", const AttributeValue &v7 = EmptyAttributeValue (),
					std::string n8 = "", const AttributeValue &v8 = EmptyAttributeValue (),
					std::string n9 = "", const AttributeValue &v9 = EmptyAttributeValue ());
			NetDeviceContainer Install (const WifiPhyHelper &phyHelper, NodeContainer c, uint8_t numOfPorts) const;
			NetDeviceContainer Install (const WifiPhyHelper &phy, Ptr<Node> node, uint8_t numOfPorts) const;
		private:
			ObjectFactory m_stationManager;
			ObjectFactory m_meshMac;
			ObjectFactory m_peerManager;
			//L2routing:
			ObjectFactory m_deviceFactory;
			ObjectFactory m_routingProtocol;
	};
}//namespace ns3
#endif	/* _MESHWIFIHELPER_H */

