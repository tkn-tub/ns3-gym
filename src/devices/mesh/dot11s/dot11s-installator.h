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
 */


#ifndef MESH_INSTALLATOR_H
#define MESH_INSTALLATOR_H
#include "ns3/mesh-point-device.h"
namespace ns3 {
namespace dot11s {
class Dot11sStackInstallator
{
  public:
    ///\brief Installs 802.11s stack. needed by helper only
    static bool InstallDot11sStack (Ptr<MeshPointDevice> mp, bool root);
    static void Report (const ns3::Ptr<ns3::NetDevice>&, std::ostream&);
    static void ResetStats (const ns3::Ptr<ns3::NetDevice>&);
};

}
}
#endif

