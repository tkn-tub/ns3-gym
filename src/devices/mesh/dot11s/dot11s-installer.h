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


#ifndef DOT11S_STACK_INSTALLER_H
#define DOT11S_STACK_INSTALLER_H
#include "ns3/mesh-stack-installer.h"
namespace ns3 {
class Dot11sStack : public MeshStack
{
  public:
    static TypeId GetTypeId ();
    Dot11sStack ();
    ~Dot11sStack ();
    void DoDispose ();

    ///\brief Installs 802.11s stack. needed by helper only
    bool InstallStack (Ptr<MeshPointDevice> mp);
    void Report (const Ptr<MeshPointDevice> mp, std::ostream&);
    void ResetStats (const Ptr<MeshPointDevice> mp);
  private:
    Mac48Address m_root;
};
} //namespace ns3
#endif

