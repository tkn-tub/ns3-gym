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
#include "random-variable-default-value.h"
#include "debug.h"

NS_DEBUG_COMPONENT_DEFINE ("RandomVariableDefaultValue");

namespace ns3 {

RandomVariableDefaultValue::RandomVariableDefaultValue (std::string name,
							std::string help,
							std::string defaultValue)
  : DefaultValueBase (name, help),
    m_defaultValue (defaultValue),
    m_value (defaultValue)
{
  if (!Parse (defaultValue, false, 0))
    {
      NS_FATAL_ERROR ("Invalid Random Variable specification: " << defaultValue);
    }
  DefaultValueList::Add (this);
}

RandomVariable *
RandomVariableDefaultValue::GetCopy (void)
{
  RandomVariable *variable;
  bool ok;
  ok = Parse (m_value, true, &variable);
  NS_ASSERT (ok);
  return variable;
}
double
RandomVariableDefaultValue::ReadAsDouble (std::string value, bool &ok)
{
  double v;
  std::istringstream iss;
  iss.str (value);
  iss >> v;
  ok = !iss.bad () && !iss.fail ();
  return v;
}
bool
RandomVariableDefaultValue::Parse (const std::string &value, 
				   bool mustCreate, RandomVariable **pVariable)
{
  std::string::size_type pos = value.find_first_of(":");
  if (pos == std::string::npos)
    {
      return false;
    }
  bool ok;
  std::string type = value.substr (0, pos);
  std::string v = value.substr (pos+1, std::string::npos);
  if (type == "Constant")
    {
      double constant = ReadAsDouble (v, ok);
      if (mustCreate)
	{
          NS_DEBUG ("create Constant constant=" << constant);
	  *pVariable = new ConstantVariable (constant);
	}
      else
        {
          NS_DEBUG ("parse  Constant constant=" << constant);
        }
      return ok;
    }
  else if (type == "Uniform")
    {
      std::string::size_type maxPos = v.find_first_of(":");
      if (maxPos == std::string::npos)
	{
	  return false;
	}
      std::string min = v.substr (0, maxPos);
      std::string max = v.substr (maxPos + 1, std::string::npos);
      double minVal;
      double maxVal;
      minVal = ReadAsDouble (min, ok);
      maxVal = ReadAsDouble (max, ok);
      if (mustCreate)
	{
          NS_DEBUG ("create Uniform min=" << min << ", max=" << max);
	  *pVariable = new UniformVariable (minVal, maxVal);
	}
      else
        {
          NS_DEBUG ("parse  Uniform min=" << min << ", max=" << max);
        }
      return ok;
    }
  else
    {
      // XXX parse other types of distributions.
      return false;
    }
}
bool 
RandomVariableDefaultValue::DoParseValue (const std::string &value)
{
  bool ok = Parse (value, false, 0);
  if (ok)
    {
      m_value = value;
    }
  return ok;
}
std::string 
RandomVariableDefaultValue::DoGetType (void) const
{
  return "(Uniform:min:max|Constant:cst)";
}
std::string 
RandomVariableDefaultValue::DoGetDefaultValue (void) const
{
  return m_defaultValue;
}

} // namespace ns3
