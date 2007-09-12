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
#ifndef LOG_H
#define LOG_H

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
#define NS_LOG_COMPONENT_DEFINE(name)                                \
  static ns3::LogComponent g_log = ns3::LogComponent (name)

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
#define NS_LOG(level,msg)                       \
  do                                            \
    {                                           \
      if (g_log.IsEnabled (level))              \
        {                                       \
          std::clog << msg << std::endl;        \
        }                                       \
    }                                           \
  while (false)

#define NS_LOG_DEBUG(msg) \
  NS_LOG (ns3::LOG_LEVEL_DEBUG,msg)

#define NS_LOG_FUNCTION \
  NS_LOG (ns3::LOG_LEVEL_FUNCTION, __PRETTY_PRINT__)

#define NS_LOG_PARAM(msg) \
  NS_LOG (ns3::LOG_LEVEL_PARAM,msg)

#define NS_LOG_WARN(msg) \
  NS_LOG (ns3::LOG_LEVEL_WARN,msg)

#define NS_LOG_ERROR(msg) \
  NS_LOG (ns3::LOG_LEVEL_ERROR,msg)

#define NS_LOG_UNCOND(msg) \
  do                                            \
    {                                           \
      std::clog << msg << std::endl;            \
    }                                           \
  while (false)



namespace ns3 {

enum LogLevel {
  LOG_LEVEL_DEBUG    = 1<<0,
  LOG_LEVEL_FUNCTION = 1<<1,
  LOG_LEVEL_PARAM    = 1<<2,
  LOG_LEVEL_WARN     = 1<<3,
  LOG_LEVEL_ERROR    = 1<<4,
  LOG_LEVEL_LAST     = 1<<31
};

/**
 * \param name a log component name
 * \ingroup logging
 *
 * Enable the logging output associated with that log component.
 * The logging output can be later disabled with a call
 * to ns3::LogComponentDisable.
 */
void LogComponentEnable (char const *name, enum LogLevel level);

/**
 * \param name a log component name
 * \ingroup logging
 *
 * Disable the logging output associated with that log component.
 * The logging output can be later re-enabled with a call
 * to ns3::LogComponentEnable.
 */
void LogComponentDisable (char const *name, enum LogLevel level);

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
void LogComponentPrintList (void);

class LogComponent {
public:
  LogComponent (char const *name);
  bool IsEnabled (enum LogLevel level) const;
  bool IsNoneEnabled (void) const;
  void Enable (enum LogLevel level);
  void Disable (enum LogLevel level);
private:
  uint32_t m_levels;
};

} // namespace ns3

#endif /* LOG_H */
