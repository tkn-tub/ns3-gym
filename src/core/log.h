/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006,2007 INRIA
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

#ifndef __LOG_H__
#define __LOG_H__

#include <string>
#include <iostream>

/**
 * \defgroup logging Logging
 * \brief Logging functions and macros
 *
 *   LOG functionality: macros which allow developers to
 *     send information out on screen. All logging messages 
 *     are disabled by default. To enable selected logging 
 *     messages, use the ns3::LogComponentEnable
 *     function. 
 * 
 * Alternatively, you can use the NS_LOG
 * environment variable to define a ';'-separated list of
 * logging components to enable. For example, NS_LOG=a;b;c;DAFD;GH
 * would enable the components 'a', 'b', 'c', 'DAFD', and, 'GH'.
 *
 * For each component, the "debug" log level is enabled by default
 * but more components can be enabled selectively with the following
 * syntax: NS_LOG='Component1=func|param|warn;Component2=error|debug'
 * This example would enable the 'func', 'param', and 'warn' log
 * levels for 'Component1' and the 'error' and 'debug' log levels
 * for 'Component2'.
 *
 * The list of available log components can be printed on stdout
 * with the NS_LOG=print-list syntax.
 */

/**
 * \ingroup logging
 * \param name a string
 *
 * Define a Log component with a specific name. This macro
 * should be used at the top of every file in which you want 
 * to use the NS_LOG macro. This macro defines a new
 * "log component" which can be later selectively enabled
 * or disabled with the ns3::LogComponentEnable and 
 * ns3::LogComponentDisable functions or with the NS_LOG
 * environment variable.
 */

#ifdef NS3_LOG_ENABLE

#define NS_LOG_COMPONENT_DEFINE(name)                                \
  static ns3::LogComponent g_log = ns3::LogComponent (name)

#else

#define NS_LOG_COMPONENT_DEFINE(name)

#endif

/**
 * \ingroup logging
 * \param msg message to output
 *
 * Generate logging output in the "log component" of the 
 * current file. i.e., every call to NS_LOG from within
 * a file implicitely generates out within the component
 * defined with the NS_LOG_COMPONENT_DEFINE macro in the
 * same file.
 */

#ifdef NS3_LOG_ENABLE

#define NS_LOG(level, msg)                                      \
  do                                                            \
    {                                                           \
      if (g_log.IsEnabled (level))                              \
        {                                                       \
          std::clog << __PRETTY_FUNCTION__ << " ==> " <<        \
            msg << std::endl;                                   \
        }                                                       \
    }                                                           \
  while (false)

#define NS_LOG_F(level)                                 \
  do                                                    \
    {                                                   \
      if (g_log.IsEnabled (level))                      \
        {                                               \
          std::clog << __PRETTY_FUNCTION__ << std::endl;\
        }                                               \
    }                                                   \
  while (false)

#define NS_LOG_ERROR(msg) \
  NS_LOG(ns3::LOG_ERROR, msg)

#define NS_LOG_WARN(msg) \
  NS_LOG(ns3::LOG_WARN, msg)

#define NS_LOG_DEBUG(msg) \
  NS_LOG(ns3::LOG_DEBUG, msg)

#define NS_LOG_INFO(msg) \
  NS_LOG(ns3::LOG_INFO, msg)

#define NS_LOG_FUNCTION \
  NS_LOG_F(ns3::LOG_FUNCTION)

#define NS_LOG_PARAM(msg) \
  NS_LOG(ns3::LOG_PARAM, msg)

#define NS_LOG_LOGIC(msg) \
  NS_LOG(ns3::LOG_LOGIC, msg)

#define NS_LOG_ALL(msg) \
  NS_LOG(ns3::LOG_ALL, msg)

#define NS_LOG_UNCOND(msg)              \
  do                                    \
    {                                   \
      std::clog << msg << std::endl;    \
    }                                   \
  while (false)

#else

#define NS_LOG(level, msg)
#define NS_LOG_F(level)
#define NS_LOG_ERROR(msg)
#define NS_LOG_WARN(msg)
#define NS_LOG_DEBUG(msg)
#define NS_LOG_INFO(msg)
#define NS_LOG_FUNCTION
#define NS_LOG_PARAM(msg)
#define NS_LOG_LOGIC(msg)
#define NS_LOG_ALL(msg)
#define NS_LOG_UNCOND(msg)

#endif

namespace ns3 {

#ifdef NS3_LOG_ENABLE

enum LogLevel {
  LOG_ERROR          = 0x0001, // serious error messages only
  LOG_LEVEL_ERROR    = 0x0001,

  LOG_WARN           = 0x0002, // warning messages
  LOG_LEVEL_WARN     = 0x0003,

  LOG_DEBUG          = 0x0004, // rare ad-hoc debug messages
  LOG_LEVEL_DEBUG    = 0x0007,

  LOG_INFO           = 0x0008, // informational messages (e.g., banners)
  LOG_LEVEL_INFO     = 0x000f,

  LOG_FUNCTION       = 0x0010, // function tracing
  LOG_LEVEL_FUNCTION = 0x001f, 

  LOG_PARAM          = 0x0020, // parameters to functions
  LOG_LEVEL_PARAM    = 0x003f,

  LOG_LOGIC          = 0x0040, // control flow tracing within functions
  LOG_LEVEL_LOGIC    = 0x007f,

  LOG_ALL            = 0x4000, // print everything
  LOG_LEVEL_ALL      = 0x7fff
};

#endif

/**
 * \param name a log component name
 * \ingroup logging
 *
 * Enable the logging output associated with that log component.
 * The logging output can be later disabled with a call
 * to ns3::LogComponentDisable.
 */
#ifdef NS3_LOG_ENABLE
void LogComponentEnable (char const *name, enum LogLevel level);
#else
#define LogComponentEnable(a,b)
#endif

/**
 * \param name a log component name
 * \ingroup logging
 *
 * Disable the logging output associated with that log component.
 * The logging output can be later re-enabled with a call
 * to ns3::LogComponentEnable.
 */
#ifdef NS3_LOG_ENABLE
void LogComponentDisable (char const *name, enum LogLevel level);
#else
#define LogComponentDisable(a,b)
#endif

/**
 * \ingroup logging
 *
 * Print the list of logging messages available.
 * The output of this function can be obtained by setting
 * the NS_LOG environment variable to the special value 
 * 'print-list'.
 * 
 * For example: NS_LOG=print-list
 */
#ifdef NS3_LOG_ENABLE
void LogComponentPrintList (void);
#else
#define LogComponentPrintList()
#endif

#ifdef NS3_LOG_ENABLE

class LogComponent {
public:
  LogComponent (char const *name);
  bool IsEnabled (enum LogLevel level) const;
  bool IsNoneEnabled (void) const;
  void Enable (enum LogLevel level);
  void Disable (enum LogLevel level);
private:
  int32_t m_levels;
};

#endif

} // namespace ns3

#endif // __LOG_H__
