/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Josh Pelkey <jpelkey@gatech.edu>
 */

#ifndef POINT_TO_POINT_GRID_HELPER_H
#define POINT_TO_POINT_GRID_HELPER_H

#include <vector>

#include "internet-stack-helper.h"
#include "point-to-point-helper.h"
#include "ipv4-address-helper.h"
#include "ipv4-interface-container.h"
#include "net-device-container.h"

namespace ns3 {
  
class PointToPointGridHelper 
{
  public: 
    PointToPointGridHelper (uint32_t nRows, uint32_t nCols, PointToPointHelper pointToPoint);
    ~PointToPointGridHelper ();

    Ptr<Node> GetNode (uint32_t row, uint32_t col);
    Ipv4Address GetIpv4Address (uint32_t row, uint32_t col);

    void InstallStack (InternetStackHelper stack);
    void AssignIpv4Addresses (Ipv4AddressHelper rowIp, Ipv4AddressHelper colIp);
    void BoundingBox (double ulx, double uly, double lrx, double lry);

  private:
    uint32_t m_xSize;
    uint32_t m_ySize;
    std::vector<NetDeviceContainer> m_rowDevices;
    std::vector<NetDeviceContainer> m_colDevices;
    std::vector<Ipv4InterfaceContainer> m_rowInterfaces;
    std::vector<Ipv4InterfaceContainer> m_colInterfaces;
    std::vector<NodeContainer> m_nodes;

};

} // namespace ns3
      
#endif /* POINT_TO_POINT_GRID_HELPER_H */
