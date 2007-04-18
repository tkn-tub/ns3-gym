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

/**
 * \defgroup debugging Debugging
 * \brief Debugging functions and macros
 *
 *   - DEBUG functionality: macros which allow developers to
 *     send information out on screen only in debugging builds.
 *     All debug messages are disabled by default. To enable 
 *     selected debug messages, use the ns3::DebugComponentEnable
 *     function. Alternatively, you can use the NS_DEBUG 
 *     environment variable to define a ';'-separated list of
 *     messages to enable. For example, NS_DEBUG=a;b;c;DAFD;GH
 *     would enable the components 'a', 'b', 'c', 'DAFD', and, 'GH'.
 */

namespace ns3 {

/**
 * \param name a debug component name
 * \ingroup debugging
 *
 * Enable the debugging output associated with that debug component.
 * The debugging output can be later disabled with a call
 * to ns3::DebugComponentDisable.
 */
void DebugComponentEnable (char const *name);
/**
 * \param name a debug component name
 * \ingroup debugging
 *
 * Disable the debugging output associated with that debug component.
 * The debugging output can be later re-enabled with a call
 * to ns3::DebugComponentEnable.
 */
void DebugComponentDisable (char const *name);
/**
 * \ingroup debugging
 * Print the list of debugging messages available.
 */
void DebugComponentPrintList (void);

class DebugComponent {
public:
  DebugComponent (char const *name);
  bool IsEnabled (void);
  void Enable (void);
  void Disable (void);
private:
  bool m_isEnabled;
};

}; // namespace ns3


#ifdef NS3_DEBUG_ENABLE

#include <string>
#include <iostream>


/**
 * \ingroup debugging
 * \param name a string
 *
 * Define a Debug component with a specific name. This macro
 * should be used at the top of every file in which you want 
 * to use the NS_DEBUG macro. This macro defines a new
 * "debug component" which can be later selectively enabled
 * or disabled with the ns3::DebugComponentEnable and 
 * ns3::DebugComponentDisable functions or with the NS_DEBUG
 * environment variable.
 */
#define NS_DEBUG_COMPONENT_DEFINE(name)                                \
  static ns3::DebugComponent g_debug = ns3::DebugComponent (name)

/**
 * \ingroup debugging
 * \param msg message to output
 *
 * Generate debugging output in the "debug component" of the 
 * current file. i.e., every call to NS_DEBUG from within
 * a file implicitely generates out within the component
 * defined with the NS_DEBUG_COMPONENT_DEFINE macro in the
 * same file.
 */
#define NS_DEBUG(msg)				\
  do                                            \
    {                                           \
      if (g_debug.IsEnabled ())			\
        {                                       \
          std::cout << msg << std::endl;        \
        }                                       \
    }                                           \
  while (false)

/**
 * \ingroup debugging
 * \param msg message to output
 *
 * Generate debugging output unconditionally in all
 * debug builds.
 */
#define NS_DEBUG_UNCOND(msg)         \
  do                                 \
    {                                \
      std::cout << msg << std::endl; \
    }                                \
  while (false)

#else /* NS3_DEBUG_ENABLE */

#define NS_DEBUG_COMPONENT_DEFINE(name)
#define NS_DEBUG(x)
#define NS_DEBUG_UNCOND(msg)

#endif /* NS3_DEBUG_ENABLE */

#endif /* DEBUG_H */
