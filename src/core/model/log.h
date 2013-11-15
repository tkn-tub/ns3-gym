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

namespace ns3 {

enum LogLevel {
  LOG_NONE           = 0x00000000, // no logging

  LOG_ERROR          = 0x00000001, // serious error messages only
  LOG_LEVEL_ERROR    = 0x00000001,

  LOG_WARN           = 0x00000002, // warning messages
  LOG_LEVEL_WARN     = 0x00000003,

  LOG_DEBUG          = 0x00000004, // rare ad-hoc debug messages
  LOG_LEVEL_DEBUG    = 0x00000007,

  LOG_INFO           = 0x00000008, // informational messages (e.g., banners)
  LOG_LEVEL_INFO     = 0x0000000f,

  LOG_FUNCTION       = 0x00000010, // function tracing
  LOG_LEVEL_FUNCTION = 0x0000001f, 

  LOG_LOGIC          = 0x00000020, // control flow tracing within functions
  LOG_LEVEL_LOGIC    = 0x0000003f,

  LOG_ALL            = 0x0fffffff, // print everything
  LOG_LEVEL_ALL      = LOG_ALL,

  LOG_PREFIX_FUNC    = 0x80000000, // prefix all trace prints with function
  LOG_PREFIX_TIME    = 0x40000000, // prefix all trace prints with simulation time
  LOG_PREFIX_NODE    = 0x20000000, // prefix all trace prints with simulation node
  LOG_PREFIX_LEVEL   = 0x10000000, // prefix all trace prints with log level (severity)
  LOG_PREFIX_ALL     = 0xf0000000  // all prefixes
};

/**
 * \param name a log component name
 * \param level a logging level
 * \ingroup logging
 *
 * Enable the logging output associated with that log component.
 * The logging output can be later disabled with a call
 * to ns3::LogComponentDisable.
 *
 * Same as running your program with the NS_LOG environment
 * variable set as NS_LOG='name=level'
 */
void LogComponentEnable (char const *name, enum LogLevel level);

/**
 * \param level a logging level
 * \ingroup logging
 *
 * Enable the logging output for all registered log components.
 *
 * Same as running your program with the NS_LOG environment
 * variable set as NS_LOG='*=level'
 */
void LogComponentEnableAll (enum LogLevel level);


/**
 * \param name a log component name
 * \param level a logging level
 * \ingroup logging
 *
 * Disable the logging output associated with that log component.
 * The logging output can be later re-enabled with a call
 * to ns3::LogComponentEnable.
 */
void LogComponentDisable (char const *name, enum LogLevel level);

/**
 * \param level a logging level
 * \ingroup logging
 *
 * Disable all logging for all components.
 */
void LogComponentDisableAll (enum LogLevel level);


} // namespace ns3

/**
 * \ingroup logging
 * \param name a string
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
 * To create a log component Foo, at the top of foo.cc:
 *
 * \code
 *   NS_LOG_COMPONENT_DEFINE ("Foo")
 *     ;
 * \endcode
 *
 * Note the closing ';' is not on the same line;  this prevents 
 * Doxygen from spuriously warning that the macro invocation is undocumented.
 
 */
#define NS_LOG_COMPONENT_DEFINE(name)                           \
  static ns3::LogComponent g_log = ns3::LogComponent (name)

#define NS_LOG_APPEND_TIME_PREFIX                               \
  if (g_log.IsEnabled (ns3::LOG_PREFIX_TIME))                   \
    {                                                           \
      ns3::LogTimePrinter printer = ns3::LogGetTimePrinter ();  \
      if (printer != 0)                                         \
        {                                                       \
          (*printer)(std::clog);                               \
          std::clog << " ";                                     \
        }                                                       \
    }

#define NS_LOG_APPEND_NODE_PREFIX                               \
  if (g_log.IsEnabled (ns3::LOG_PREFIX_NODE))                   \
    {                                                           \
      ns3::LogNodePrinter printer = ns3::LogGetNodePrinter ();  \
      if (printer != 0)                                         \
        {                                                       \
          (*printer)(std::clog);                               \
          std::clog << " ";                                     \
        }                                                       \
    }

#define NS_LOG_APPEND_FUNC_PREFIX                               \
  if (g_log.IsEnabled (ns3::LOG_PREFIX_FUNC))                   \
    {                                                           \
      std::clog << g_log.Name () << ":" <<                      \
      __FUNCTION__ << "(): ";                                 \
    }                                                           \

#define NS_LOG_APPEND_LEVEL_PREFIX(level)                       \
  if (g_log.IsEnabled (ns3::LOG_PREFIX_LEVEL))                  \
    {                                                           \
      std::clog << "[" << g_log.GetLevelLabel (level) << "] ";  \
    }                                                           \
  

#ifndef NS_LOG_APPEND_CONTEXT
#define NS_LOG_APPEND_CONTEXT
#endif /* NS_LOG_APPEND_CONTEXT */



#ifdef NS3_LOG_ENABLE


/**
 * \ingroup debugging
 * \defgroup logging Logging
 * \brief Logging functions and macros
 *
 * LOG functionality: macros which allow developers to
 * send information out on screen. All logging messages 
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
 * \ingroup logging
 * \param level the log level
 * \param msg the message to log
 *
 * This macro allows you to log an arbitrary message at a specific
 * log level. The log message is expected to be a C++ ostream
 * message such as "my string" << aNumber << "my oth stream".
 *
 * Typical usage looks like:
 * \code
 * NS_LOG (LOG_DEBUG, "a number="<<aNumber<<", anotherNumber="<<anotherNumber);
 * \endcode
 */
#define NS_LOG(level, msg)                                      \
  do                                                            \
    {                                                           \
      if (g_log.IsEnabled (level))                              \
        {                                                       \
          NS_LOG_APPEND_TIME_PREFIX;                            \
          NS_LOG_APPEND_NODE_PREFIX;                            \
          NS_LOG_APPEND_CONTEXT;                                \
          NS_LOG_APPEND_FUNC_PREFIX;                            \
          NS_LOG_APPEND_LEVEL_PREFIX (level);                   \
          std::clog << msg << std::endl;                        \
        }                                                       \
    }                                                           \
  while (false)

/**
 * \ingroup logging
 * \param msg the message to log
 *
 * Use \ref NS_LOG to output a message of level LOG_ERROR.
 */
#define NS_LOG_ERROR(msg) \
  NS_LOG (ns3::LOG_ERROR, msg)

/**
 * \ingroup logging
 * \param msg the message to log
 *
 * Use \ref NS_LOG to output a message of level LOG_WARN.
 */
#define NS_LOG_WARN(msg) \
  NS_LOG (ns3::LOG_WARN, msg)

/**
 * \ingroup logging
 * \param msg the message to log
 *
 * Use \ref NS_LOG to output a message of level LOG_DEBUG.
 */
#define NS_LOG_DEBUG(msg) \
  NS_LOG (ns3::LOG_DEBUG, msg)

/**
 * \ingroup logging
 * \param msg the message to log
 *
 * Use \ref NS_LOG to output a message of level LOG_INFO.
 */
#define NS_LOG_INFO(msg) \
  NS_LOG (ns3::LOG_INFO, msg)

/**
 * \ingroup logging
 *
 * Output the name of the function.
 *
 * This should be used only in static functions; most member functions
 * should instead use NS_LOG_FUNCTION().
 */
#define NS_LOG_FUNCTION_NOARGS()                                \
  do                                                            \
    {                                                           \
      if (g_log.IsEnabled (ns3::LOG_FUNCTION))                  \
        {                                                       \
          NS_LOG_APPEND_TIME_PREFIX;                            \
          NS_LOG_APPEND_NODE_PREFIX;                            \
          NS_LOG_APPEND_CONTEXT;                                \
          std::clog << g_log.Name () << ":"                     \
                    << __FUNCTION__ << "()" << std::endl;       \
        }                                                       \
    }                                                           \
  while (false)


/**
 * \ingroup logging
 * \param parameters the parameters to output.
 *
 * If log level LOG_FUNCTION is enabled, this macro will output
 * all input parameters separated by ", ".
 *
 * Typical usage looks like:
 * \code
 * NS_LOG_FUNCTION (aNumber<<anotherNumber);
 * \endcode
 * And the output will look like:
 * \code
 * Component:Function (aNumber, anotherNumber)
 * \endcode
 *
 * To facilitate function tracing, most functions should begin with
 * (at least) NS_LOG_FUNCTION(this).  Static functions should use
 * NS_LOG_FUNCTION_NOARGS() instead.
 *
 */
#define NS_LOG_FUNCTION(parameters)                             \
  do                                                            \
    {                                                           \
      if (g_log.IsEnabled (ns3::LOG_FUNCTION))                  \
        {                                                       \
          NS_LOG_APPEND_TIME_PREFIX;                            \
          NS_LOG_APPEND_NODE_PREFIX;                            \
          NS_LOG_APPEND_CONTEXT;                                \
          std::clog << g_log.Name () << ":"                     \
                    << __FUNCTION__ << "(";                     \
          ns3::ParameterLogger (std::clog) << parameters;      \
          std::clog << ")" << std::endl;                        \
        }                                                       \
    }                                                           \
  while (false)


/**
 * \ingroup logging
 * \param msg the message to log
 *
 * Use \ref NS_LOG to output a message of level LOG_LOGIC
 */
#define NS_LOG_LOGIC(msg) \
  NS_LOG (ns3::LOG_LOGIC, msg)

/**
 * \ingroup logging
 * \param msg the message to log
 *
 * Output the requested message unconditionaly.
 */
#define NS_LOG_UNCOND(msg)              \
  do                                    \
    {                                   \
      std::clog << msg << std::endl;    \
    }                                   \
  while (false)

#else /* LOG_ENABLE */

#define NS_LOG(level, msg)
#define NS_LOG_ERROR(msg)
#define NS_LOG_WARN(msg)
#define NS_LOG_DEBUG(msg)
#define NS_LOG_INFO(msg)
#define NS_LOG_FUNCTION_NOARGS()
#define NS_LOG_FUNCTION(msg)
#define NS_LOG_LOGIC(msg)
#define NS_LOG_UNCOND(msg)

#endif /* LOG_ENABLE */

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


class LogComponent {
public:
  LogComponent (const std::string & name);
  void EnvVarCheck (const std::string & name);
  bool IsEnabled (enum LogLevel level) const;
  bool IsNoneEnabled (void) const;
  void Enable (enum LogLevel level);
  void Disable (enum LogLevel level);
  char const *Name (void) const;
  std::string GetLevelLabel(const enum LogLevel level) const;
private:
  int32_t     m_levels;
  std::string m_name;
};

class ParameterLogger : public std::ostream
{
  int m_itemNumber;
  std::ostream &m_os;
public:
  ParameterLogger (std::ostream &os);

  template<typename T>
  ParameterLogger& operator<< (T param)
  {
    switch (m_itemNumber)
      {
      case 0: // first parameter
        m_os << param;
        break;
      default: // parameter following a previous parameter
        m_os << ", " << param;
        break;
      }
    m_itemNumber++;
    return *this;
  }
};

} // namespace ns3


#endif /* NS3_LOG_H */
