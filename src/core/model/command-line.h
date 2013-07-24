/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <string>
#include <sstream>
#include <list>

#include "callback.h"

namespace ns3 {

/**
 * \ingroup core
 * \defgroup commandline Command Line Parsing
 *
 * A uniform way to specify program documentation,
 * allowed command line arguments and help strings,
 * and set any attribute or global value, all from
 * the command line directly.
 *
 * The main entry point is CommandLine
 */
/**
 * \ingroup commandline
 * \brief Parse command-line arguments
 *
 * Instances of this class can be used to parse command-line 
 * arguments: programs can register new arguments with
 * CommandLine::AddValue.
 *
 * In addition, this class can be used to set the
 * 'initial value' of every attribute in the system with the
 * \code
 *   --TypeIdName::AttributeName=value
 * \endcode
 * syntax, and it can be used to set the value of every GlobalValue
 * in the system with the
 * \code
 *   --GlobalValueName=value
 * \endcode
 * syntax.
 *
 * A simple example is in \c src/core/example/command-line-example.cc
 * The heart of that example is this code:
 *
 * \code
 *  int         val1 = 1;
 *  bool        val2 = false;
 *  std::string val3 = "";
 *  
 *  CommandLine cmd;
 *  cmd.Usage ("CommandLine example program.\n"
 *             "\n"
 *             "This little program demonstrates how to use CommandLine.");
 *  cmd.AddValue ("val1", "an int argument",       val1);
 *  cmd.AddValue ("val2", "a bool argument",       val2);
 *  cmd.AddValue ("val3", "a string argument",     val3);
 *  cmd.AddValue ("val4", "a string via callback", MakeCallback (SetVal4));
 *  cmd.Parse (argc, argv);
 * \endcode
 *
 * Here is the output from a few runs of that program:
 *
 * \code
 *   $ ./waf --run="command-line-example"
 *   val1:   1
 *   val2:   0
 *   val3:   "val3 default"
 *   val4:   "val4 default"
 *
 *   $ ./waf --run="command-line-example --val1=2 --val2 --val3=Hello --val4=World"
 *   val1:   2
 *   val2:   1
 *   val3:   "Hello"
 *   val4:   "World"
 *   
 *   $ ./waf --run="command-line-example --help"
 *   ns3-dev-command-line-example-debug [Program Arguments] [General Arguments]
 *   
 *   CommandLine example program.
 *   
 *   This little program demonstrates how to use CommandLine.
 *   
 *   Program Arguments:
 *       --val1: an int argument
 *       --val2: a bool argument
 *       --val3: a string argument
 *       --val4: a string via callback
 *   
 *   General Arguments:
 *       --PrintGlobals:              Print the list of globals.
 *       --PrintGroups:               Print the list of groups.
 *       --PrintGroup=[group]:        Print all TypeIds of group.
 *       --PrintTypeIds:              Print all TypeIds.
 *       --PrintAttributes=[typeid]:  Print all attributes of typeid.
 *       --PrintHelp:                 Print this help message.
 * \endcode
 */
class CommandLine
{
public:
  /** Constructor */
  CommandLine ();
  /**
   * Copy constructor
   *
   * \param cmd the CommandLine to copy from
   */
  CommandLine (const CommandLine &cmd);
  /**
   * Assignment
   *
   * \param cmd the CommandLine to assign from
   * \return the CommandLine
   */
  CommandLine &operator = (const CommandLine &cmd);
  /** Destructor */
  ~CommandLine ();

  /**
   * Supply the program usage and documentation.
   *
   * \param usage Program usage message to write with help.
   */
  void Usage (const std::string usage);
  
  /**
   * Add a program argument, assigning to POD
   *
   * \param name the name of the program-supplied argument
   * \param help the help text used by \c \-\-PrintHelp
   * \param value a reference to the variable where the
   *        value parsed will be stored (if no value
   *        is parsed, this variable is not modified).
   */
  template <typename T>
  void AddValue (const std::string &name,
                 const std::string &help,
                 T &value);


  /**
   * Add a program argument, using a Callback to parse the value
   *
   * \param name the name of the program-supplied argument
   * \param help the help text used by \c \-\-PrintHelp
   * \param callback a Callback function that will be invoked to parse
   * and collect the value.  This is normally used by language bindings.
   */
  void AddValue (const std::string &name,
                 const std::string &help,
                 Callback<bool, std::string> callback);

  /**
   * Parse the program arguments
   *
   * \param argc the 'argc' variable: number of arguments (including the
   *        main program name as first element).
   * \param argv the 'argv' variable: a null-terminated array of strings,
   *        each of which identifies a command-line argument.
   * 
   * Obviously, this method will parse the input command-line arguments and
   * will attempt to handle them all.
   *
   * As a side effect, this method saves the program basename, which
   * can be retrieved by GetName().
   */
  void Parse (int argc, char *argv[]);

  /**
   * Get the program name
   *
   * \return the program name.  Only valid after calling Parse()
   */
  std::string GetName () const;

private:

  /**
   * \ingroup commandline
   * \brief The argument base class
   */
  class Item 
  {
  public:
    std::string m_name;       /**< Argument label:  \c \-\--m_name=... */
    std::string m_help;       /**< Argument help string */
    virtual ~Item ();         /**< Destructor */
    /**
     * Parse from a string.
     *
     * \param value the string representation
     * \return true if parsing the value succeeded
     */
    virtual bool Parse (std::string value) = 0;
    /**
     * \return true if this item have a default value?
     */
    virtual bool HasDefault () const;
    /**
     * \return the default value
     */
    virtual std::string GetDefault () const;
  };

  /**
   * \ingroup commandline
   *\brief An argument Item assigning to POD
   */
  template <typename T>
  class UserItem : public Item
  {
  public:
    /**
     * Parse from a string.
     *
     * \param value the string representation
     * \return true if parsing the value succeeded
     */
    virtual bool Parse (std::string value);

    bool HasDefault () const;
    std::string GetDefault () const;
      
    T *m_valuePtr;            /**< Pointer to the POD location */
    std::string m_default;    /**< String representation of default value */
  };

  /**
   * \ingroup commandline
   * \brief An argument Item using a Callback to parse the input
   */
  class CallbackItem : public Item
  {
  public:
    /**
     * Parse from a string.
     *
     * \param value the string representation
     * \return true if parsing the value succeeded
     */
    virtual bool Parse (std::string value);
    Callback<bool, std::string> m_callback;  /**< The Callback */
  };

  /**
   * Match name against the program or general arguments,
   * and dispatch to the appropriate handler.
   *
   * \param name the argument name
   * \param value the command line value
   */
  void HandleArgument (std::string name, std::string value) const;
  /**
   * Handler for \c \-\-PrintHelp and \c \-\-help:  print Usage(), argument names, and help strings
   */
  void PrintHelp (void) const;
  /** Handler for \c \-\-PrintGlobals:  print all global variables and values */
  void PrintGlobals (void) const;
  /**
   * Handler for \c \-\-PrintAttributes:  print the attributes for a given type.
   *
   * \param type the TypeId whose Attributes should be displayed
   */
  void PrintAttributes (std::string type) const;
  /**
   * Handler for \c \-\-PrintGroup:  print all types belonging to a given group.
   *
   * \param group the name of the TypeId group to display
   */
  void PrintGroup (std::string group) const;
  /** Handler for \c \-\-PrintTypeIds:  print all TypeId names. */
  void PrintTypeIds (void) const;
  /** Handler for \c \-\-PrintGroups:  print all TypeId group names */
  void PrintGroups (void) const;
  /**
   * Copy constructor
   *
   * \param cmd CommandLine to copy
   */
  void Copy (const CommandLine &cmd);
  /** Remove all arguments, Usage(), name */
  void Clear (void);

  typedef std::list<Item *> Items;      /**< Argument list container */
  Items m_items;                        /**< The list of arguments */
  std::string m_usage;                  /**< The Usage string */
  std::string m_name;                   /**< The program name */
};  // class CommandLine

/**
 * \ingroup commandline
 * Helpers for CommandLine
 */
namespace CommandLineHelper {

  /**
   * \ingroup commandline
   * \brief Helper to specialize UserItem on bool
   *
   * \param value the argument name
   * \param val the argument location
   * \return true if parsing was successful
   * @{
   */
  template <typename T>
  bool UserItemParse (const std::string value, T & val);
  template <>
  bool UserItemParse<bool> (const std::string value, bool & val);
  /**@}*/

}  // namespace CommandLineHelper
    
  
  
} // namespace ns3

namespace ns3 {

template <typename T>
void 
CommandLine::AddValue (const std::string &name,
                       const std::string &help,
                       T &value)
{
  UserItem<T> *item = new UserItem<T> ();
  item->m_name = name;
  item->m_help = help;
  item->m_valuePtr = &value;
  
  std::stringstream ss;
  ss << value;
  ss >> item->m_default;
    
  m_items.push_back (item);
}


template <typename T>
bool
CommandLine::UserItem<T>::HasDefault () const
{
  return true;
}

template <typename T>
std::string
CommandLine::UserItem<T>::GetDefault () const
{
  std::ostringstream oss;
  oss << *m_valuePtr;
  return oss.str ();
}


template <typename T>
bool
CommandLine::UserItem<T>::Parse (std::string value)
{
  return CommandLineHelper::UserItemParse<T> (value, *m_valuePtr);
}

template <typename T>
bool
CommandLineHelper::UserItemParse (const std::string value, T & val)
{
  std::istringstream iss;
  iss.str (value);
  iss >> val;
  return !iss.bad () && !iss.fail ();
}

} // namespace ns3

#endif /* COMMAND_LINE_H */
