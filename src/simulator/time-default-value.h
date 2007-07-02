/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 * All rights reserved.
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
#ifndef TIME_DEFAULT_VALUE_H
#define TIME_DEFAULT_VALUE_H

#include "ns3/default-value.h"
#include "ns3/nstime.h"

namespace ns3 {

/**
 * \ingroup config
 * \brief a ns3::Time variable for ns3::Bind
 *
 * Every instance of this type is automatically 
 * registered in the variable pool which is used
 * by ns3::Bind. 
 */
class TimeDefaultValue : public DefaultValueBase
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
  TimeDefaultValue (const std::string name,
		    const std::string help,
		    Time defaultValue);
  /**
   * \returns the default value for this variable or a
   *          user-provided overriden variable.
   */
  Time GetValue (void) const;
private:
  virtual bool DoParseValue (const std::string &value);
  virtual std::string DoGetType (void) const;
  virtual std::string DoGetDefaultValue (void) const;

  Time m_defaultValue;
  Time m_value;
};

} // namespace ns3

#endif /* TIME_DEFAULT_VALUE_H */
