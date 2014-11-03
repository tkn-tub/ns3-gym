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

#ifndef NS3_LOG_H
#define NS3_LOG_H

#include <string>
#include <iostream>
#include <stdint.h>
#include <map>

#include "log-macros-enabled.h"
#include "log-macros-disabled.h"

namespace ns3 {

/**
 * \ingroup debugging
 * \defgroup logging Logging
 *
 * \brief Logging functions and macros
 *
 * LOG functionality: macros which allow developers to
 * send information to the std::clog output stream. All logging messages 
 * are disabled by default. To enable selected logging 
 * messages, use the ns3::LogComponentEnable
 * function or use the NS_LOG environment variable 
 *
 * Use the environment variable NS_LOG to define a ':'-separated list of
 * logging components to enable. For example (using bash syntax), 
 * NS_LOG="OlsrAgent" would enable one component at all log levels. 
 * NS_LOG="OlsrAgent:Ipv4L3Protocol" would enable two components, 
 * at all log levels, etc.
 * NS_LOG="*" will enable all available log components at all levels.
 *
 * To control more selectively the log levels for each component, use
 * this syntax: NS_LOG='Component1=func|warn:Component2=error|debug'
 * This example would enable the 'func', and 'warn' log
 * levels for 'Component1' and the 'error' and 'debug' log levels
 * for 'Component2'.  The wildcard can be used here as well.  For example
 * NS_LOG='*=level_all|prefix' would enable all log levels and prefix all
 * prints with the component and function names.
 *
 * A note on NS_LOG_FUNCTION() and NS_LOG_FUNCTION_NOARGS():
 * generally, use of (at least) NS_LOG_FUNCTION(this) is preferred.
 * Use NS_LOG_FUNCTION_NOARGS() only in static functions.
 */

/**
 *  \ingroup logging
 *
 *  Logging severity classes and levels.
 */
enum LogLevel {
  LOG_NONE           = 0x00000000, //!< no logging

  LOG_ERROR          = 0x00000001, //!< serious error messages only
  LOG_LEVEL_ERROR    = 0x00000001,

  LOG_WARN           = 0x00000002, //!< warning messages
  LOG_LEVEL_WARN     = 0x00000003,

  LOG_DEBUG          = 0x00000004, //!< rare ad-hoc debug messages
  LOG_LEVEL_DEBUG    = 0x00000007,

  LOG_INFO           = 0x00000008, //!< informational messages (e.g., banners)
  LOG_LEVEL_INFO     = 0x0000000f,

  LOG_FUNCTION       = 0x00000010, //!< function tracing
  LOG_LEVEL_FUNCTION = 0x0000001f, 

  LOG_LOGIC          = 0x00000020, //!< control flow tracing within functions
  LOG_LEVEL_LOGIC    = 0x0000003f,

  LOG_ALL            = 0x0fffffff, //!< print everything
  LOG_LEVEL_ALL      = LOG_ALL,

  LOG_PREFIX_FUNC    = 0x80000000, //!< prefix all trace prints with function
  LOG_PREFIX_TIME    = 0x40000000, //!< prefix all trace prints with simulation time
  LOG_PREFIX_NODE    = 0x20000000, //!< prefix all trace prints with simulation node
  LOG_PREFIX_LEVEL   = 0x10000000, //!< prefix all trace prints with log level (severity)
  LOG_PREFIX_ALL     = 0xf0000000  //!< all prefixes
};

/**
 * \ingroup logging
 *
 * Enable the logging output associated with that log component.
 *
 * The logging output can be later disabled with a call
 * to ns3::LogComponentDisable.
 *
 * Same as running your program with the NS_LOG environment
 * variable set as NS_LOG='name=level'
 *
 * \param name a log component name
 * \param level a logging level
 */
void LogComponentEnable (char const *name, enum LogLevel level);

/**
 * \ingroup logging
 *
 * Enable the logging output for all registered log components.
 *
 * Same as running your program with the NS_LOG environment
 * variable set as NS_LOG='*=level'
 *
 * \param level a logging level
 */
void LogComponentEnableAll (enum LogLevel level);


/**
 * \ingroup logging
 *
 * Disable the logging output associated with that log component.
 *
 * The logging output can be later re-enabled with a call
 * to ns3::LogComponentEnable.
 *
 * \param name a log component name
 * \param level a logging level
 */
void LogComponentDisable (char const *name, enum LogLevel level);

/**
 * \ingroup logging
 *
 * Disable all logging for all components.
 *
 * \param level a logging level
 */
void LogComponentDisableAll (enum LogLevel level);


} // namespace ns3

/**
 * \ingroup logging
 *
 * Define a Log component with a specific name.
 *
 * This macro should be used at the top of every file in which you want 
 * to use the NS_LOG macro. This macro defines a new
 * "log component" which can be later selectively enabled
 * or disabled with the ns3::LogComponentEnable and 
 * ns3::LogComponentDisable functions or with the NS_LOG
 * environment variable.
 *
 * LogComponent names should be simple string tokens, _i.e._,
 * "ArfWifiManager", not "ns3::ArfWifiManager".
 *
 * This macro should be placed within namespace ns3.  If functions
 * outside of namespace ns3 require access to logging, the preferred
 * solution is to add the following 'using' directive at file scope,
 * outside of namespace ns3, and after the inclusion of
 * NS_LOG_COMPONENT_DEFINE, such as follows:
 * \code
 * namespace ns3 {
 * NS_LOG_COMPONENT_DEFINE ("...");
 *
 * \\ definitions within the ns3 namespace
 *
 * } // namespace ns3
 *
 * using ns3::g_log;
 *
 * // further definitions outside of the ns3 namespace
 *\endcode
 *
 * \param name a string
 */
#define NS_LOG_COMPONENT_DEFINE(name)                           \
  static ns3::LogComponent g_log = ns3::LogComponent (name, __FILE__)

/**
 * \ingroup logging
 *
 * Define a logging component with a mask.
 *
 * See LogComponent().
 *
 * \param name a string
 * \param mask the default mask
 */
#define NS_LOG_COMPONENT_DEFINE_MASK(name, mask)                \
  static ns3::LogComponent g_log = ns3::LogComponent (name, __FILE__, mask)

/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_ERROR.
 *
 * \param msg the message to log
 */
#define NS_LOG_ERROR(msg) \
  NS_LOG (ns3::LOG_ERROR, msg)

/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_WARN.
 *
 * \param msg the message to log
 */
#define NS_LOG_WARN(msg) \
  NS_LOG (ns3::LOG_WARN, msg)

/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_DEBUG.
 *
 * \param msg the message to log
 */
#define NS_LOG_DEBUG(msg) \
  NS_LOG (ns3::LOG_DEBUG, msg)

/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_INFO.
 *
 * \param msg the message to log
 */
#define NS_LOG_INFO(msg) \
  NS_LOG (ns3::LOG_INFO, msg)

/**
 * \ingroup logging
 *
 * Use \ref NS_LOG to output a message of level LOG_LOGIC
 *
 * \param msg the message to log
 */
#define NS_LOG_LOGIC(msg) \
  NS_LOG (ns3::LOG_LOGIC, msg)


namespace ns3 {

/**
 * \ingroup logging
 *
 * Print the list of logging messages available.
 * Same as running your program with the NS_LOG environment
 * variable set as NS_LOG=print-list
 */
void LogComponentPrintList (void);

typedef void (*LogTimePrinter)(std::ostream &os);
typedef void (*LogNodePrinter)(std::ostream &os);

void LogSetTimePrinter (LogTimePrinter);
LogTimePrinter LogGetTimePrinter (void);

void LogSetNodePrinter (LogNodePrinter);
LogNodePrinter LogGetNodePrinter (void);


/**
 * \ingroup logging
 *
 * A single log component configuration.
 */
class LogComponent
{
public:
  /**
   * Constructor
   *
   * \param [in] name the user-visible name for this component.
   * \param [in] mask LogLevels blocked for this LogComponent.  Blocking
   *                  a log level helps prevent recursion by logging in
   *                  functions which help implement the logging facility.
   */
  LogComponent (const std::string & name,
                const std::string & file,
                const enum LogLevel mask = LOG_NONE);
  /**
   * Check if this LogComponent is enabled for \pname{level}
   *
   * \param [in] level the level to check for.
   * \return true if \pname{level} is enabled.
   */
  bool IsEnabled (const enum LogLevel level) const;
  /**
   * Check if all levels are disabled.
   *
   * \return true if all levels are disabled.
   */
  bool IsNoneEnabled (void) const;
  /**
   * Enable this LogComponent at \pname{level}
   *
   * \param [in] level the LogLevel to enable.
   */
  void Enable (const enum LogLevel level);
  /**
   * Disable logging at \pname{level} for this LogComponent.
   *
   * \param [in] level the LogLevel to disable.
   */
  void Disable (const enum LogLevel level);
  /**
   * Get the name of this LogComponent.
   *
   * \return the name of this LogComponent.
   */
  char const *Name (void) const;
  /**
   * Get the compilation unit defining this LogComponent.
   */
  std::string File (void) const;
  /**
   * Get the string label for the given LogLevel.
   *
   * \param [in] level the LogLevel to get the label for.
   * \return the string label for \pname{level}
   */
  static std::string GetLevelLabel(const enum LogLevel level);
  /**
   * Prevent the enabling of a specific LogLevel.
   *
   * \param level the LogLevel to block
   */
  void SetMask (const enum LogLevel level);

  /**
   * LogComponent name map.
   *
   * \internal
   * This should really be considered an internal API.
   * It is exposed here to allow print-introspected-doxygen.cc
   * to generate a list of all LogComponents.
   */
  typedef std::map<std::string, LogComponent *> ComponentList;

  /**
   * Get the list of LogComponnents.
   *
   * \internal
   * This should really be considered an internal API.
   * It is exposed here to allow print-introspected-doxygen.cc
   * to generate a list of all LogComponents.
   *
   * \returns The list of LogComponents.
   */
  static ComponentList *GetComponentList (void);

  
private:
  /**
   * Parse the `NS_LOG` environment variable for options relating to this
   * LogComponent.
   */
  void EnvVarCheck (void);
  
  int32_t     m_levels;  //!< Enabled LogLevels
  int32_t     m_mask;    //!< Blocked LogLevels
  std::string m_name;    //!< LogComponent name
  std::string m_file;    //!< File defining this LogComponent

};  // class LogComponent

/**
 * \ingroup logging
 *
 * Insert `, ` when streaming function arguments.
 */
class ParameterLogger
{
  bool m_first;        //!< First argument flag, doesn't get `, `.
  std::ostream &m_os;  //!< Underlying output stream.
public:
  /**
   * Constructor.
   *
   * \param [in] os Underlying output stream.
   */
  ParameterLogger (std::ostream &os);

  /**
   * Write a function parameter on the output stream,
   * separating paramters after the first by `,` strings.
   *
   * \param [in] param the function parameter
   */
  template<typename T>
  ParameterLogger& operator<< (T param)
  {
    if (m_first)
      {
        m_os << param;
        m_first = false;
      }
    else
      {
        m_os << ", " << param;
      }
    return *this;
  }
};

} // namespace ns3


#endif /* NS3_LOG_H */
