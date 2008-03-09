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

class GlobalValue
{
  typedef std::vector<GlobalValue *> Vector;
public:
  typedef Vector::const_iterator Iterator;

  GlobalValue (std::string name, std::string help,
		Attribute initialValue,
		Ptr<const AttributeChecker> checker);

  std::string GetName (void) const;
  std::string GetHelp (void) const;
  Attribute GetValue (void) const;
  Ptr<const AttributeChecker> GetChecker (void) const;
  
  bool SetValue (Attribute value);

  static void Bind (std::string name, Attribute value);
  static bool BindFailSafe (std::string name, Attribute value);

  static Iterator Begin (void);
  static Iterator End (void);
private:
  friend class GlobalValueTests;
  static Vector *GetVector (void);
  std::string m_name;
  std::string m_help;
  Attribute m_initialValue;
  Ptr<const AttributeChecker> m_checker;
};

} // namespace ns3

#endif /* GLOBAL_VALUE_H */
