/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef DEFAULT_VALUE_H
#define DEFAULT_VALUE_H

#include <string>
#include <list>
#include "callback.h"

/**
 * \defgroup config Simulation configuration
 *
 */

namespace ns3 {

class DefaultValueBase
{
public:
  virtual ~DefaultValueBase ();
  std::string GetName (void) const;
  std::string GetHelp (void) const;
  // parse a matching parameter
  // return true in case of success, false otherwise.
  bool ParseValue (const std::string &value);
  std::string GetType (void) const;
  std::string GetDefaultValue (void) const;
protected:
  DefaultValueBase (const std::string &name, 
		    const std::string &help);
private:
  virtual bool DoParseValue (const std::string &value) = 0;
  virtual std::string DoGetType (void) const = 0;
  virtual std::string DoGetDefaultValue (void) const = 0;
  std::string m_name;
  std::string m_help;
};

class DefaultValueList
{
 public:
  typedef std::list<DefaultValueBase *>::iterator Iterator;

  static Iterator Begin (void);
  static Iterator End (void);
  static void Remove (const std::string &name);
  static void Add (DefaultValueBase *defaultValue);
 private:
  typedef std::list<DefaultValueBase *> List;
  static List *GetList (void);
};

/**
 * \ingroup config
 * \param name name of variable to bind
 * \param value value to bind to the specified variable
 *
 * If the variable name does not match any existing
 * variable or if the value is not compatible with
 * the variable type, this function will abort
 * at runtime and print an error message detailing
 * which variable or value triggered the problem.
 */
void Bind (std::string name, std::string value);

/**
 * \brief A Boolean variable for ns3::Bind
 * \ingroup config
 *
 * Every instance of this type is automatically 
 * registered in the variable pool which is used
 * by ns3::Bind. 
 */
class BooleanDefaultValue : public DefaultValueBase
{
public:
  /**
   * \param name name of variable
   * \param help help text which explains the purpose
   *        and the semantics of this variable
   * \param defaultValue the default value to assign
   *        to this variable.
   *
   * Unless the user invokes ns3::Bind with the right arguments,
   * the GetValue method will return the default value. Otherwise,
   * it will return the user-specified value.
   */
  BooleanDefaultValue (std::string name,
		       std::string help,
		       bool defaultValue);
  /**
   * \returns the default value for this variable or a
   *          user-provided overriden variable.
   */
  bool GetValue (void) const;
private:
  virtual bool DoParseValue (const std::string &value);
  virtual std::string DoGetType (void) const;
  virtual std::string DoGetDefaultValue (void) const;
  bool m_defaultValue;
  bool m_value;
};

/**
 * \brief An Integer variable for ns3::Bind
 * \ingroup config
 *
 * Every instance of this type is automatically 
 * registered in the variable pool which is used
 * by ns3::Bind. 
 */
template <typename T>
class IntegerDefaultValue : public DefaultValueBase
{
public:
  /**
   * \param name the name of the variable
   * \param help help text which explains the purpose
   *        and the semantics of this variable
   * \param defaultValue the default value assigned
   *        to this variable
   *
   * By default, the set of allowed values is the entire range
   * of values which can be stored and retrieved from the underlying
   * type.
   */
  IntegerDefaultValue (std::string name,
		       std::string help,
		       T defaultValue);
  /**
   * \param name the name of the variable
   * \param help help text which explains the purpose
   *        and the semantics of this variable
   * \param defaultValue the default value assigned to this
   *        variable
   * \param minValue the minimum value which can be set
   *        in this variable
   * \param maxValue the maximum value which can be set in this
   *        variable.
   */
  IntegerDefaultValue (std::string name,
		       std::string help,
		       T defaultValue,
		       T minValue,
		       T maxValue);


  T GetValue (void) const;
private:
  virtual bool DoParseValue (const std::string &value);
  virtual std::string DoGetType (void) const;
  virtual std::string DoGetDefaultValue (void) const;
  T m_defaultValue;
  T m_minValue;
  T m_maxValue;
  T m_value;
};

/**
 * \brief Named enumeration defaults
 * \ingroup config
 *
 * Every instance of this type is automatically 
 * registered in the variable pool which is used
 * by ns3::Bind. 
 */
class StringEnumDefaultValue : public DefaultValueBase
{
public:
  StringEnumDefaultValue (const std::string &name,
                          const std::string &help);
  void AddDefaultValue (const std::string &value);
  void AddPossibleValue (const std::string &value);
  std::string GetValue (void) const;
private:
  virtual bool DoParseValue (const std::string &value);
  virtual std::string DoGetType (void) const;
  virtual std::string DoGetDefaultValue (void) const;

  bool m_oneDefault;
  std::list<std::string> m_possibleValues;
  std::string m_defaultValue;
  std::string m_value;
};

/**
 * \brief An enum variable for ns3::Bind
 * \ingroup config
 *
 * Every instance of this type is automatically 
 * registered in the variable pool which is used
 * by ns3::Bind. 
 */
template <typename T>
class EnumDefaultValue : public DefaultValueBase
{
public:
  /**
   * \param name the name of this variable
   * \param help help text which explains the purpose
   *        and the semantics of this variable
   * \param defaultValue the default value assigned to this
   *        variable unless it is overriden with ns3::Bind
   * \param defaultValueString the string which represents
   *        the default value which should be used by ns3::Bind
   *
   * This method takes a variable number of arguments. The list of
   * arguments is terminated by the pair of values 0 and (void *)0.
   * Each pair of extra argument is assumed to be of the form 
   * (enum value, string representing enum value). If ns3::Bind
   * is invoked on this variable, it will check that the user-provided
   * values are within the set of values specified in this constructor.
   *
   * Typical useage of this method will look like this:
   * \code
   * enum MyEnum {
   *   MY_ENUM_A,
   *   MY_ENUM_B,
   *   MY_ENUM_C,
   * };
   * // set default value to be "B".
   * static EnumDefaultValue<enum MyEnum> 
   *  g_myDefaultValue ("my", "my help",
   *                    MY_ENUM_B, "B",
   *                    MY_ENUM_A, "A",
   *                    MY_ENUM_C, "C",);
   *                    0, (void*)0);
   * \endcode
   * Note that to ensure portability to 64 bit systems, make sure that
   * the last element in the variable list of arguments is (void *)0.
   */
  EnumDefaultValue (const std::string &name, const std::string &help,
		    T defaultValue, const char *defaultValueString, 
		    ...);
  void AddPossibleValue (T value, const std::string &valueString);
  /**
   * \returns the default value or any other value specified by the 
   *          user with ns3::Bind
   */
  T GetValue (void);
 private:
  virtual bool DoParseValue (const std::string &value);
  virtual std::string DoGetType (void) const;
  virtual std::string DoGetDefaultValue (void) const;

  typedef std::list<std::pair<T,std::string> > PossibleValues;

  T m_defaultValue;
  PossibleValues m_possibleValues;
  T m_value;
};

/**
 * \brief Class used to call a certain function during the configuration of the
 * simulation
 * \ingroup config
 */
class CommandDefaultValue : public DefaultValueBase
{
public:
  CommandDefaultValue (const std::string &name,
		       const std::string &help,
		       Callback<void> cb);
private:
  virtual bool DoParseValue (const std::string &value);
  virtual std::string DoGetType (void) const;
  virtual std::string DoGetDefaultValue (void) const;
  Callback<void> m_cb;
};

}//namespace ns3

#include "type-name.h"
#include "assert.h"
#include <sstream>
#include <stdarg.h>
#include <limits>

namespace ns3 {

/**************************************************************
 **************************************************************/


template <typename T>
IntegerDefaultValue<T>::IntegerDefaultValue (std::string name,
					     std::string help,
					     T defaultValue)
  : DefaultValueBase (name, help),
    m_defaultValue (defaultValue),
    m_minValue (std::numeric_limits<T>::min ()),
    m_maxValue (std::numeric_limits<T>::max ()),
    m_value (defaultValue)
{
  DefaultValueList::Add (this);
}
template <typename T>
IntegerDefaultValue<T>::IntegerDefaultValue (std::string name,
					     std::string help,
					     T defaultValue,
					     T minValue,
					     T maxValue)
  : DefaultValueBase (name, help),
    m_defaultValue (defaultValue),
    m_minValue (minValue),
    m_maxValue (maxValue),
    m_value (defaultValue)
{
  DefaultValueList::Add (this);
  NS_ASSERT (m_defaultValue <= m_maxValue &&
	     m_defaultValue >= m_minValue);
}

template <typename T>
T
IntegerDefaultValue<T>::GetValue (void) const
{
  return m_value;
}

template <typename T>
bool
IntegerDefaultValue<T>::DoParseValue (const std::string &value)
{
  std::istringstream iss;
  iss.str (value);
  iss >> m_value;
  if (m_value > m_maxValue ||
      m_value < m_minValue)
    {
      return false;
    }
  return !iss.bad () && !iss.fail ();
}

template <typename T>
std::string
IntegerDefaultValue<T>::DoGetType (void) const
{
  std::ostringstream oss;
  oss << TypeNameGet<T> () << "("
      << m_minValue << ":" 
      << m_maxValue << ")";
  return oss.str ();
}

template <typename T>
std::string
IntegerDefaultValue<T>::DoGetDefaultValue (void) const
{
  std::ostringstream oss;
  oss << m_defaultValue;
  return oss.str ();
}

/**************************************************************
 **************************************************************/

template <typename T>
EnumDefaultValue<T>::EnumDefaultValue (const std::string &name, const std::string &help,
				       T defaultValue, const char *defaultValueString, 
				       ...)
  : DefaultValueBase (name, help),
    m_defaultValue (defaultValue),
    m_value (defaultValue)
{
  AddPossibleValue (defaultValue, defaultValueString);
  va_list list;
  va_start (list, defaultValueString);
  while (true)
    {
      T v = (T) va_arg (list, int);
      const char *str = va_arg (list, const char *);
      if (v == 0 && str == 0)
	{
	  break;
	}
      AddPossibleValue (v, str);
    }
  DefaultValueList::Add (this);
}
template <typename T>
void 
EnumDefaultValue<T>::AddPossibleValue (T value, const std::string &valueString)
{
  m_possibleValues.push_back (std::make_pair (value, valueString));
}
template <typename T>
T 
EnumDefaultValue<T>::GetValue (void)
{
  return m_value;
}
template <typename T>
bool 
EnumDefaultValue<T>::DoParseValue (const std::string &value)
{
  for (typename PossibleValues::iterator i = m_possibleValues.begin ();
       i != m_possibleValues.end (); i++)
    {
      if (value == i->second)
	{
	  m_value = i->first;
	  return true;
	}
    }
  return false;
}
template <typename T>
std::string 
EnumDefaultValue<T>::DoGetType (void) const
{
  std::string retval;
  retval += "(";
  for (typename PossibleValues::const_iterator i = m_possibleValues.begin ();
       i != m_possibleValues.end (); i++)
    {
      if (i != m_possibleValues.begin ())
	{
	  retval += "|";
	}
      retval += i->second;
    }
  retval += ")";
  return retval;
}
template <typename T>
std::string 
EnumDefaultValue<T>::DoGetDefaultValue (void) const
{
  for (typename PossibleValues::const_iterator i = m_possibleValues.begin ();
       i != m_possibleValues.end (); i++)
    {
      if (i->first == m_defaultValue)
	{
	  return i->second;
	}
    }
  // cannot happen theoretically.
  NS_ASSERT (false);
  return ""; // quiet compiler
}

}//namespace ns3

#endif /* DEFAULT_VALUE_H */
