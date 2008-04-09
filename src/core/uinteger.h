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
#ifndef UINTEGER_H
#define UINTEGER_H

#include "attribute.h"
#include "attribute-helper.h"
#include <stdint.h>

namespace ns3 {

/**
 * \brief Hold an unsigned integer type
 *
 * \anchor uint8_t
 * \anchor uint16_t
 * \anchor uint32_t
 * \anchor uint64_t
 *
 * This class can be used to hold variables of unsigned integer
 * type such as uint8_t, uint16_t, uint32_t, uint64_t, or,
 * unsigned int, etc.
 */
class Uinteger
{
public:
  Uinteger (uint64_t value);
  Uinteger ();

  void Set (uint64_t value);
  uint64_t Get (void) const;

  operator uint64_t () const;

  ATTRIBUTE_CONVERTER_DEFINE (Uinteger);
private:
  uint64_t m_value;
};

std::ostream & operator << (std::ostream &os, const Uinteger &uinteger);
std::istream & operator >> (std::istream &is, Uinteger &uinteger);

ATTRIBUTE_VALUE_DEFINE (Uinteger);
ATTRIBUTE_ACCESSOR_DEFINE (Uinteger);

template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker (void);

template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker (uint64_t min);

template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker (uint64_t min, uint64_t max);

} // namespace ns3

#include "type-name.h"

namespace ns3 {

namespace internal {

Ptr<const AttributeChecker> MakeUintegerChecker (uint64_t min, uint64_t max, std::string name);

} // namespace internal


template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker (void)
{
  return internal::MakeUintegerChecker (std::numeric_limits<T>::min (),
                                        std::numeric_limits<T>::max (),
                                        TypeNameGet<T> ());
}

template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker (uint64_t min)
{
  return internal::MakeUintegerChecker (min,
                                        std::numeric_limits<T>::max (),
                                        TypeNameGet<T> ());
}

template <typename T>
Ptr<const AttributeChecker> MakeUintegerChecker (uint64_t min, uint64_t max)
{
  return internal::MakeUintegerChecker (min,
                                        max, 
                                        TypeNameGet<T> ());
}

} // namespace ns3

#endif /* UINTEGER_H */
