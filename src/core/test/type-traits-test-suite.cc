/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#include "ns3/type-traits.h"
#include "ns3/test.h"

using namespace ns3;

class TypeTraitsTestCase : public TestCase
{
public:
  TypeTraitsTestCase ();
  virtual ~TypeTraitsTestCase () {}

private:
  virtual void DoRun (void);
};

TypeTraitsTestCase::TypeTraitsTestCase (void)
  : TestCase ("Check type traits")
{
}

void
TypeTraitsTestCase::DoRun (void)
{
  NS_TEST_ASSERT_MSG_EQ (TypeTraits<void (TypeTraitsTestCase::*) (void)>::IsPointerToMember, 1, "Check");
  NS_TEST_ASSERT_MSG_EQ (TypeTraits<void (TypeTraitsTestCase::*) (void) const>::IsPointerToMember, 1, "Check");
  NS_TEST_ASSERT_MSG_EQ (TypeTraits<void (TypeTraitsTestCase::*) (int)>::IsPointerToMember, 1, "Check");
  NS_TEST_ASSERT_MSG_EQ (TypeTraits<void (TypeTraitsTestCase::*) (int) const>::IsPointerToMember, 1, "Check");
  NS_TEST_ASSERT_MSG_EQ (TypeTraits<void (TypeTraitsTestCase::*) (void) const>::PointerToMemberTraits::nArgs, 0, "Check");
  NS_TEST_ASSERT_MSG_EQ (TypeTraits<void (TypeTraitsTestCase::*) (int) const>::PointerToMemberTraits::nArgs, 1, "Check");
}

class TypeTraitsTestSuite : public TestSuite
{
public:
  TypeTraitsTestSuite ();
};

TypeTraitsTestSuite::TypeTraitsTestSuite ()
  : TestSuite ("type-traits", UNIT)
{
  AddTestCase (new TypeTraitsTestCase, TestCase::QUICK);
}

static TypeTraitsTestSuite typeTraitsTestSuite;
