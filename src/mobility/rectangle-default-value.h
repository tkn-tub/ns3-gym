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
#ifndef RECTANGLE_DEFAULT_VALUE_H
#define RECTANGLE_DEFAULT_VALUE_H

#include <string>
#include "ns3/default-value.h"
#include "rectangle.h"

namespace ns3 {

class RectangleDefaultValue : public DefaultValueBase
{
 public:
  RectangleDefaultValue (std::string name,
			 std::string help,
			 double xMin, double xMax,
			 double yMin, double yMax);

  Rectangle GetValue (void) const;
 private:
  double ReadDouble (std::string str, bool &ok);
  virtual bool DoParseValue (const std::string &value);
  virtual std::string DoGetType (void) const;
  virtual std::string DoGetDefaultValue (void) const;

  Rectangle m_default;
  Rectangle m_rectangle;
};

} // namespace ns3

#endif /* RECTANGLE_DEFAULT_VALUE_H */
