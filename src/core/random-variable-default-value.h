/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#ifndef RANDOM_VARIABLE_DEFAULT_VALUE_H
#define RANDOM_VARIABLE_DEFAULT_VALUE_H

#include "random-variable.h"
#include "default-value.h"

namespace ns3 {

class RandomVariableDefaultValue : public DefaultValueBase
{
 public:
  RandomVariableDefaultValue (std::string name,
			      std::string help,
			      std::string defaultValue);

  RandomVariable Get (void) const;
private:
  bool Parse (const std::string &value, bool mustCreate, RandomVariable *pVariable) const;
  double ReadAsDouble (const std::string value, bool &ok) const;
  virtual bool DoParseValue (const std::string &value);
  virtual std::string DoGetType (void) const;
  virtual std::string DoGetDefaultValue (void) const;

  std::string m_defaultValue;
  std::string m_value;
};

} // namespace ns3

#endif /* RANDOM_VARIABLE_DEFAULT_VALUE_H */
