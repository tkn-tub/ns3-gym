/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <iostream>

namespace ns3 {

void DebugComponentEnable (char const *name);
void DebugComponentDisable (char const *name);
void DebugComponentPrintList (void);

class DebugComponent {
public:
  DebugComponent (std::string name);
  bool IsEnabled (void);
  void Enable (void);
  void Disable (void);
private:
  bool m_isEnabled;
};

}; // namespace ns3


#ifdef NS3_DEBUG_ENABLE

#define NS3_DEBUG_COMPONENT_DEFINE(name)                                \
  static ns3::DebugComponent g_debug = ns3::DebugComponent (name);

#define NS3_DEBUG(x)				\
  if (g_debug.IsEnabled ())			\
    {						\
      std::cout << x << std::endl;		\
    }
#else /* NS3_DEBUG_ENABLE */

#define NS3_DEBUG_COMPONENT_DEFINE(name)
#define NS3_DEBUG(x)

#endif /* NS3_DEBUG_ENABLE */

#endif /* DEBUG_H */
