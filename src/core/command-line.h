/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <list>
#include <string>
#include "default-value.h"

namespace ns3 {

/**
 * \brief Command line argument processing class
 * \ingroup config
 *
 * Often times in simulations, the user will want the ability to change and
 * tweak simulation parameters without having to recompile the entire 
 * simulation.  This can be done by passing arguments in from the command
 * line invocation of a simulation.
 * This class is a collection of static functions to aid in this facility.
 * By using it, users get the automatic things like support for --help command
 * line arguments.  See samples/main-default-value.cc for more info.
 */
class CommandLine
{
public:
  /**
   * \brief Add a command line argument to the system
   * \param name Name of the string to expect on the command line
   * \param help A help string for this param, displayed --help is used.
   * \param value The desination to store the value read from the command line
   */
  template <typename T>
  static void AddArgValue (const std::string &name,
			   const std::string &help, 
			   T &value);
  /**
   * \brief Add a command line triggered function call to the system
   * \param name Name of the string to expect on the command line
   * \param help A help string for this param, displayed --help is used.
   * \param value An ns3::Callback that gets called if name is present as a
   * commandline argument.
   */
  static void AddArgCommand (const std::string &name,
			     const std::string &help,
			     Callback<void> cb);
  /**
   * \brief Parse the command line for arguments thus far added
   * \param argc Number of strings on the command line; pass this directly from
   * the first argument of your main(int,char**) function 
   * \param argv Array of strings passed in as arguments; pass this directly from
   * the second argument of your main(int,char**) function
   */
  static void Parse (int argc, char *argv[]);
 private:
  template <typename T>
  class UserDefaultValue : public DefaultValueBase 
  {
  public:
    UserDefaultValue (const std::string &name,
                      const std::string &help,
		      T &value);
  private:
    virtual bool DoParseValue (const std::string &value);
    virtual std::string DoGetType (void) const;
    virtual std::string DoGetDefaultValue (void) const;
    T *m_valuePtr;
  };
  static void PrintHelp (void);
  typedef std::list<DefaultValueBase *> List;
  static List *GetUserList (void);
  static CommandDefaultValue g_help;
};

}//namespace ns3

namespace ns3 {

template <typename T>
void 
CommandLine::AddArgValue (const std::string &name,
			  const std::string &help, 
			  T &value)
{
  DefaultValueBase *base = 
    new UserDefaultValue<T> (name, help, value);
  GetUserList ()->push_back (base);
}


template <typename T>
CommandLine::UserDefaultValue<T>::UserDefaultValue (const std::string &name,
						    const std::string &help,
						    T &value)
  : DefaultValueBase (name, help),
    m_valuePtr (&value)
{
  // we do not register in the DefaultValueList class on purpose.
}
template <typename T>
bool
CommandLine::UserDefaultValue<T>::DoParseValue (const std::string &value)
{
  std::istringstream iss;
  iss.str (value);
  T v;
  iss >> v;
  *m_valuePtr = v;
  return !iss.bad () && !iss.fail ();
}
template <typename T>
std::string
CommandLine::UserDefaultValue<T>::DoGetType (void) const
{
  return "";
}
template <typename T>
std::string
CommandLine::UserDefaultValue<T>::DoGetDefaultValue (void) const
{
  return "";
}

}//namespace ns3

#endif /* COMMAND_LINE_H */
