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
#ifndef GLOBAL_VALUE_H
#define GLOBAL_VALUE_H

#include <string>
#include <vector>
#include "ptr.h"
#include "attribute.h"


namespace ns3 {

/**
 * \ingroup Core
 *
 * \brief hold a so-called 'global value'.
 *
 * Instances of this class are expected to be allocated as static 
 * global variables and should be used to store configurable global state.
 * GlobalValues can be set directly by calling ns3::GlobalValue::SetValue
 * but they can also be set through the NS_GLOBAL_VALUE environment variable.
 * For example, NS_GLOBAL_VALUE='Name=Value;OtherName=OtherValue;' would set
 * global values Name and OtherName to Value and OtherValue respectively. 
 *
 * Users of the ns3::CommandLine class also get the ability to set global 
 * values through commandline arguments to their program: --Name=Value will
 * set global value Name to Value.
 */
class GlobalValue
{
  typedef std::vector<GlobalValue *> Vector;
public:
  typedef Vector::const_iterator Iterator;

  /**
   * \param name the name of this global value.
   * \param help some help text which describes the purpose of this
   *        global value.
   * \param initialValue the value to assign to this global value
   *        during construction.
   * \param checker a pointer to an AttributeChecker which can verify
   *        that any user-supplied value to override the initial
   *        value matches the requested type constraints.
   */
  GlobalValue (std::string name, std::string help,
               const AttributeValue &initialValue,
               Ptr<const AttributeChecker> checker);

  /**
   * \returns the name of this GlobalValue.
   */
  std::string GetName (void) const;
  /**
   * \returns the help text of this GlobalValue.
   */
  std::string GetHelp (void) const;
  /**
   * \returns the current value of this GlobalValue.
   */
  void GetValue (AttributeValue &value) const;
  /**
   * \returns the checker associated to this GlobalValue.
   */
  Ptr<const AttributeChecker> GetChecker (void) const;
  /**
   * \param value the new value to set in this GlobalValue.
   */
  bool SetValue (const AttributeValue &value);

  /**
   * \param name the name of the global value
   * \param value the value to set in the requested global value.
   *
   * Iterate over the set of GlobalValues until a matching name is found
   * and then set its value with GlobalValue::SetValue.
   *
   * This method cannot fail. It will crash if the input is not valid.
   */
  static void Bind (std::string name, const AttributeValue &value);

  /**
   * \param name the name of the global value
   * \param value the value to set in the requested global value.
   * \returns true if the value could be set successfully, false otherwise.
   *
   * Iterate over the set of GlobalValues until a matching name is found
   * and then set its value with GlobalValue::SetValue.
   */
  static bool BindFailSafe (std::string name, const AttributeValue &value);

  /**
   * \returns an iterator which represents a pointer to the first GlobalValue registered.
   */
  static Iterator Begin (void);
  /**
   * \returns an iterator which represents a pointer to the last GlobalValue registered.
   */
  static Iterator End (void);


  /** 
   * finds the GlobalValue with the given name and returns its value
   * 
   * @param name the name of the GlobalValue to be found
   * @param value where to store the value of the found GlobalValue
   * 
   * @return true if the GlobalValue was found, false otherwise
   */
  static bool GetValueByNameFailSafe (std::string name, AttributeValue &value);

  /** 
   * finds the GlobalValue with the given name and returns its
   * value. This method cannot fail, i.e., it will trigger a
   * NS_FATAL_ERROR if the requested GlobalValue is not found.
   * 
   * @param name the name of the GlobalValue to be found
   * @param value where to store the value of the found GlobalValue
   * 
   */
  static void GetValueByName (std::string name, AttributeValue &value);


private:
  friend class GlobalValueTestCase;

  static Vector *GetVector (void);
  void InitializeFromEnv (void);
  std::string m_name;
  std::string m_help;
  Ptr<AttributeValue> m_initialValue;
  Ptr<const AttributeChecker> m_checker;
};

} // namespace ns3

#endif /* GLOBAL_VALUE_H */
