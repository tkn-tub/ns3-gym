/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <list>
#include <string>
#include "default-value.h"

namespace ns3 {

class CommandLine
{
public:
  template <typename T>
  static void AddArgValue (const std::string &name,
			   const std::string &help, 
			   T &value);
  static void AddArgCommand (const std::string &name,
			     const std::string &help,
			     Callback<void> cb);
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
  std::ostringstream iss;
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
