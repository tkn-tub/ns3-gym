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
#ifndef INTEGER_H
#define INTEGER_H

#include "attribute.h"
#include "attribute-helper.h"
#include <stdint.h>

namespace ns3 {

/**
 * \brief Hold a signed integer type
 *
 * \anchor int8_t
 * \anchor int16_t
 * \anchor int32_t
 * \anchor int64_t
 *
 * This class can be used to hold variables of signed integer
 * type such as int8_t, int16_t, int32_t, int64_t, or,
 * int, etc.
 */
class Integer
{
public:
  Integer (int64_t value);
  Integer ();
  void Set (int64_t value);
  int64_t Get (void) const;

  operator int64_t () const;
  ATTRIBUTE_CONVERTER_DEFINE (Integer);
private:
  int64_t m_value;
};

std::ostream &operator << (std::ostream &os, const Integer &integer);
std::istream &operator >> (std::istream &is, Integer &integer);

ATTRIBUTE_VALUE_DEFINE(Integer);
ATTRIBUTE_ACCESSOR_DEFINE(Integer);

template <typename T>
Ptr<const AttributeChecker> MakeIntegerChecker (void);

template <typename T>
Ptr<const AttributeChecker> MakeIntegerChecker (int64_t min);

template <typename T>
Ptr<const AttributeChecker> MakeIntegerChecker (int64_t min, int64_t max);

} // namespace ns3

#include "type-name.h"

namespace ns3 {

namespace internal {

Ptr<const AttributeChecker> MakeIntegerChecker (int64_t min, int64_t max, std::string name);

} // internal

template <typename T>
Ptr<const AttributeChecker>
MakeIntegerChecker (int64_t min, int64_t max)
{
  return internal::MakeIntegerChecker (min,
                                       max, TypeNameGet<T> ());
}

template <typename T>
Ptr<const AttributeChecker>
MakeIntegerChecker (int64_t min)
{
  return internal::MakeIntegerChecker (min,
                                       std::numeric_limits<T>::max (),
                                       TypeNameGet<T> ());
}

template <typename T>
Ptr<const AttributeChecker>
MakeIntegerChecker (void)
{
  return internal::MakeIntegerChecker (std::numeric_limits<T>::min (),
                                       std::numeric_limits<T>::max (),
                                       TypeNameGet<T> ());
}

} // namespace ns3

#endif /* INTEGER_H */
