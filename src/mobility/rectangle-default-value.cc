/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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

#include "rectangle-default-value.h"

namespace ns3 {

RectangleDefaultValue::RectangleDefaultValue (std::string name,
					      std::string help,
					      double xMin, double xMax,
					      double yMin, double yMax)
  : DefaultValueBase (name, help),
    m_default (xMin, xMax, yMin, yMax),
    m_rectangle (xMin, xMax, yMin, yMax)
{
  DefaultValueList::Add (this);
}

Rectangle
RectangleDefaultValue::GetValue (void) const
{
  return m_rectangle;
}
double
RectangleDefaultValue::ReadDouble (std::string str, bool &ok)
{
  double value;
  std::istringstream iss;
  iss.str (str);
  iss >> value;
  if (iss.bad () || iss.fail ())
    {
      ok = false;
    }
  return value;
}
bool 
RectangleDefaultValue::DoParseValue (const std::string &value)
{
  std::string::size_type xMinStart = 0;
  std::string::size_type xMinEnd = value.find_first_of(":", xMinStart);
  std::string::size_type xMaxStart = xMinEnd + 1;
  std::string::size_type xMaxEnd = value.find_first_of(":", xMaxStart);
  std::string::size_type yMinStart = xMaxEnd + 1;
  std::string::size_type yMinEnd = value.find_first_of(":", yMinStart);
  std::string::size_type yMaxStart = yMinEnd + 1;
  std::string::size_type yMaxEnd = std::string::npos;

  std::string xMinString = value.substr (xMinStart, xMinEnd);
  std::string xMaxString = value.substr (xMaxStart, xMaxEnd);
  std::string yMinString = value.substr (yMinStart, yMinEnd);
  std::string yMaxString = value.substr (yMaxStart, yMaxEnd);

  bool ok = true;
  m_rectangle.xMin = ReadDouble (xMinString, ok);
  m_rectangle.yMin = ReadDouble (yMinString, ok);
  m_rectangle.xMax = ReadDouble (xMaxString, ok);
  m_rectangle.yMax = ReadDouble (yMaxString, ok);
  return ok;
}
std::string 
RectangleDefaultValue::DoGetType (void) const
{
  return "(xMin:xMax:yMin:yMax)";
}
std::string 
RectangleDefaultValue::DoGetDefaultValue (void) const
{
  std::ostringstream oss;
  oss << m_default.xMin << ":" << m_default.xMax << ":" << m_default.yMin << ":" << m_default.yMax;
  return oss.str ();
}


} // namespace ns3
