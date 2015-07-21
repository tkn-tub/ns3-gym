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

/**
 * \file
 * \ingroup core
 * ns3::GlobalValue declaration.
 */

class GlobalValueTestCase;

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
  /** Container type for holding all the GlobalValues. */
  typedef std::vector<GlobalValue *> Vector;
  
public:
  /** Iterator type for the list of all global values. */
  typedef Vector::const_iterator Iterator;

  /**
   * Constructor.
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
   * Get the name.
   * \returns the name of this GlobalValue.
   */
  std::string GetName (void) const;
  /**
   * Get the help string.
   * \returns the help text of this GlobalValue.
   */
  std::string GetHelp (void) const;
  /**
   * Get the value.
   * \param [out] value The AttributeValue to set to the value
   *                    of this GlobalValue
   * \returns the current value of this GlobalValue.
   */
  void GetValue (AttributeValue &value) const;
  /**
   * Get the AttributeChecker.
   * \returns the checker associated to this GlobalValue.
   */
  Ptr<const AttributeChecker> GetChecker (void) const;
  /**
   * Set the value of this GlobalValue.
   * \param value the new value to set in this GlobalValue.
   * \returns \c true if the Global Value was set successfully.
   */
  bool SetValue (const AttributeValue &value);

  /** Reset to the initial value. */
  void ResetInitialValue (void);

  /**
   * Iterate over the set of GlobalValues until a matching name is found
   * and then set its value with GlobalValue::SetValue.
   *
   * \param name the name of the global value
   * \param value the value to set in the requested global value.
   *
   * This method cannot fail. It will crash if the input is not valid.
   */
  static void Bind (std::string name, const AttributeValue &value);

  /**
   * Iterate over the set of GlobalValues until a matching name is found
   * and then set its value with GlobalValue::SetValue.
   *
   * \param name the name of the global value
   * \param value the value to set in the requested global value.
   * \returns true if the value could be set successfully, false otherwise.
   */
  static bool BindFailSafe (std::string name, const AttributeValue &value);

  /**
   * The Begin iterator.
   * \returns an iterator which represents a pointer to the first GlobalValue registered.
   */
  static Iterator Begin (void);
  /**
   * The End iterator.
   * \returns an iterator which represents a pointer to the last GlobalValue registered.
   */
  static Iterator End (void);


  /** 
   * Finds the GlobalValue with the given name and returns its value
   * 
   * \param name the name of the GlobalValue to be found
   * \param value where to store the value of the found GlobalValue
   * 
   * \return true if the GlobalValue was found, false otherwise
   */
  static bool GetValueByNameFailSafe (std::string name, AttributeValue &value);

  /** 
   * Finds the GlobalValue with the given name and returns its
   * value.
   *
   * This method cannot fail, i.e., it will trigger a
   * NS_FATAL_ERROR if the requested GlobalValue is not found.
   * 
   * \param name the name of the GlobalValue to be found
   * \param value where to store the value of the found GlobalValue
   */
  static void GetValueByName (std::string name, AttributeValue &value);


private:
  friend class ::GlobalValueTestCase;

  /**
   * Get the static vector of all GlobalValues.
   *
   * \returns The vector.
   */
  static Vector *GetVector (void);
  /** Initialize from the \c NS_GLOBAL_VALUE environment variable. */
  void InitializeFromEnv (void);

  /** The name of this GlobalValue. */
  std::string m_name;
  /** The help string. */
  std::string m_help;
  /** The initial value. */
  Ptr<AttributeValue> m_initialValue;
  /** The current value. */
  Ptr<AttributeValue> m_currentValue;
  /** The AttributeChecker for this GlobalValue. */
  Ptr<const AttributeChecker> m_checker;
};

} // namespace ns3

#endif /* GLOBAL_VALUE_H */
