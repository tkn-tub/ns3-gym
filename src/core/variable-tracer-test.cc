/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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

#include "uv-trace-source.h"
#include "sv-trace-source.h"
#include "trace-context.h"
#include "test.h"
#include "callback.h"


namespace ns3 {

class Foo {
public:
  void Notify (TraceContext const &contex, uint64_t oldVal, uint64_t newVal) {}
};

class VariableTracerTest: public Test {
public:
  VariableTracerTest ();
  void RunUnsignedTests (void);
  void RunSignedUnsignedTests (void);
  virtual bool RunTests (void);
};
void
VariableTracerTest::RunUnsignedTests (void)
{
  UVTraceSource<uint32_t> var, ovar, tmp;
  uint32_t utmp;
  Foo *foo = new Foo ();
  
  var.AddCallback (MakeCallback (&Foo::Notify, foo), TraceContext ());

  var = 10;
  ovar = var;

  if (var == ovar) 
    {
    }
  if (var != ovar) 
    {
    }
  if (var > ovar) 
    {
    }
  if (var >= ovar) 
    {
    }
  if (var < ovar)
    {
    }
  
  if (var <= ovar)

  if (var == 1)
    {
    }
  if (var != 1)
    {
    }
  if (var > 1)
    {
    }
  if (var >= 1)
    {
    }
  if (var < 1)
    {
    }
  if (var <= 1)
    {
    }

  if (1 == ovar)
    {
    }
  if (1 != ovar)
    {
    }
  if (1 > ovar)
    {
    }
  if (1 >= ovar)
    {
    }
  if (1 < ovar)
    {
    }
  if (1 <= ovar)
    {
    }

  var++;
  ++var;
  var--;
  --var;

  tmp = var + ovar;
  tmp = var - ovar;
  tmp = var / ovar;
  tmp = var * ovar;
  tmp = var << ovar;
  tmp = var >> ovar;
  tmp = var & ovar;
  tmp = var | ovar;
  tmp = var ^ ovar;

  tmp = var + 1;
  tmp = var - 1;
  tmp = var / 1;
  tmp = var * 1;
  tmp = var << 1;
  tmp = var >> 1;
  tmp = var & 1;
  tmp = var | 1;
  tmp = var ^ 1;

  tmp = 1 + ovar;
  tmp = 1 - ovar;
  tmp = 1 / ovar;
  tmp = 1 * ovar;
  tmp = 1 << ovar;
  tmp = 1 >> ovar;
  tmp = 1 & ovar;
  tmp = 1 | ovar;
  tmp = 1 ^ ovar;

  tmp += var;
  tmp -= var;
  tmp /= var;
  tmp *= var;
  tmp <<= var;
  tmp >>= var;
  tmp &= var;
  tmp |= var;
  tmp ^= var;

  tmp += 1;
  tmp -= 1;
  tmp /= 1;
  tmp *= 1;
  tmp <<= 1;
  tmp >>= 1;
  tmp &= 1;
  tmp |= 1;
  tmp ^= 1;


  utmp = var + ovar;
  utmp = var - ovar;
  utmp = var / ovar;
  utmp = var * ovar;
  utmp = var << ovar;
  utmp = var >> ovar;
  utmp = var & ovar;
  utmp = var | ovar;
  utmp = var ^ ovar;

  utmp = var + 1;
  utmp = var - 1;
  utmp = var / 1;
  utmp = var * 1;
  utmp = var << 1;
  utmp = var >> 1;
  utmp = var & 1;
  utmp = var | 1;
  utmp = var ^ 1;

  utmp = 1 + ovar;
  utmp = 1 - ovar;
  utmp = 1 / ovar;
  utmp = 1 * ovar;
  utmp = 1 << ovar;
  utmp = 1 >> ovar;
  utmp = 1 & ovar;
  utmp = 1 | ovar;
  utmp = 1 ^ ovar;

  utmp += var;
  utmp -= var;
  utmp /= var;
  utmp *= var;
  utmp <<= var;
  utmp >>= var;
  utmp &= var;
  utmp |= var;
  utmp ^= var;

  utmp += 1;
  utmp -= 1;
  utmp /= 1;
  utmp *= 1;
  utmp <<= 1;
  utmp >>= 1;
  utmp &= 1;
  utmp |= 1;
  utmp ^= 1;

  delete foo;
}

void
VariableTracerTest::RunSignedUnsignedTests (void)
{
  unsigned short utmp = 10;
  unsigned int uitmp = 7;
  short stmp = 5;
  utmp = stmp;
  utmp += stmp;
  uitmp = utmp;
  utmp = uitmp;

  UVTraceSource<unsigned short> uvar = 10;
  UVTraceSource<unsigned int> uivar = 5;
  SVTraceSource<short> svar = 5;
  SVTraceSource<int> sivar = 5;
  uvar = svar;
  svar = uvar;
  uvar += svar;
  svar += uvar;

  uvar = sivar;
  sivar = uvar;
  uvar += sivar;
  sivar += uvar;

  uivar = uvar;
  uvar = uivar;
  uivar += uvar;
  uvar += uivar;

  sivar = svar;
  svar = sivar;
  sivar += svar;
  svar += sivar;
}

bool 
VariableTracerTest::RunTests (void)
{
  RunUnsignedTests ();
  RunSignedUnsignedTests ();

  return true;
}

VariableTracerTest::VariableTracerTest ()
  : Test ("VariableTracer") {}

static VariableTracerTest gVariableTracerTest;

}; // namespace ns3


