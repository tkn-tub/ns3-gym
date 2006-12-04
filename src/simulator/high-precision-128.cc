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
#include "high-precision-128.h"
#include <math.h>

namespace ns3 {

const double HighPrecision::MAX_64 = 18446744073709551615.0;

HighPrecision::HighPrecision ()
{
  m_value = _cairo_int32_to_int128 (0);
  // the following statement is not really needed but it 
  // is here for the sake of symetry. I doubt we will
  // ever see this code in the profiles...
  m_value = _cairo_int128_lsl (m_value, 64);
}

HighPrecision::HighPrecision (int64_t value, bool dummy)
{
  m_value = _cairo_int64_to_int128 (value);
  m_value = _cairo_int128_lsl (m_value, 64);
}

HighPrecision::HighPrecision (double value)
{
  int64_t hi = (int64_t) floor (value);
  uint64_t lo = (uint64_t) ((value - floor (value)) * MAX_64);
  m_value = _cairo_int64_to_int128 (hi);
  m_value = _cairo_int128_lsl (m_value, 64);
  cairo_int128_t clo = _cairo_uint128_to_int128 (_cairo_uint64_to_uint128 (lo));
  m_value = _cairo_int128_add (m_value, clo);
}

int64_t
HighPrecision::GetInteger (void) const
{
  cairo_int128_t value = _cairo_int128_rsa (m_value, 64);
  return _cairo_int128_to_int64 (value);
}

double 
HighPrecision::GetDouble (void) const
{
  bool is_negative = _cairo_int128_negative (m_value);
  cairo_int128_t value = is_negative?_cairo_int128_negate (m_value):m_value;
  cairo_int128_t hi = _cairo_int128_rsa (value, 64);
  cairo_uint128_t lo = _cairo_int128_sub (value, _cairo_uint128_lsl (hi, 64));
  double flo = _cairo_uint128_to_uint64 (lo);
  flo /= MAX_64;
  double retval = _cairo_uint128_to_uint64 (hi);
  retval += flo;
  retval *= is_negative?-1.0:1.0;
  return retval;
}
bool 
HighPrecision::Add (HighPrecision const &o)
{
  m_value = _cairo_int128_add (m_value, o.m_value);
  return false;
}
bool 
HighPrecision::Sub (HighPrecision const &o)
{
  m_value = _cairo_int128_sub (m_value, o.m_value);
  return false;
}
bool 
HighPrecision::Mul (HighPrecision const &o)
{
  cairo_int128_t other = _cairo_int128_rsa (o.m_value, 64);
  m_value = _cairo_int128_mul (m_value, other);
  return false;
}
bool 
HighPrecision::Div (HighPrecision const &o)
{
#if 1
  cairo_int128_t div = _cairo_int128_rsa (o.m_value, 64);
  cairo_quorem128_t qr;
  qr = _cairo_int128_divrem (m_value, div);
  m_value = qr.quo;
#else
  cairo_quorem128_t qr;
  qr = _cairo_int128_divrem (m_value, o.m_value);
  m_value = qr.quo;
  m_value = _cairo_int128_lsl (m_value, 64);
  cairo_int128_t rem = _cairo_int128_rsa (qr.rem, 64);
  m_value = _cairo_int128_add (m_value, rem);
#endif
  return false;
}
int 
HighPrecision::Compare (HighPrecision const &o) const
{
  if (_cairo_int128_lt (m_value, o.m_value))
    {
      return -1;
    }
  else if (_cairo_int128_eq (m_value, o.m_value))
    {
      return 0;
    }
  else
    {
      return 1;
    }
}
HighPrecision 
HighPrecision::Zero (void)
{
  return HighPrecision (0,0);
}


}; // namespace ns3


#ifdef RUN_SELF_TESTS
#include "ns3/test.h"

namespace ns3 {

class HighPrecision128Tests : public Test
{
public:
  HighPrecision128Tests ();
  virtual ~HighPrecision128Tests ();
  virtual bool RunTests (void);
};

HighPrecision128Tests::HighPrecision128Tests ()
  : Test ("Int128")
{}
HighPrecision128Tests::~HighPrecision128Tests ()
{}

#define CHECK_EXPECTED(v,expected) \
{ \
  if (v.GetInteger () != expected) \
    { \
      Failure () << "file="<<__FILE__<<", line="<<__LINE__<<", expected: "<<expected<<", got: "<< v.GetInteger ()<<std::endl; \
      ok = false; \
    } \
}

#define V(v) \
  HighPrecision (v, false)

bool
HighPrecision128Tests::RunTests (void)
{
  bool ok = true;

  HighPrecision a, b;
  a = HighPrecision (1, false);
  b = HighPrecision (1, false);

  a.Sub (b);
  CHECK_EXPECTED (a, 0);

  a = V (1);
  a.Sub (V(2));
  CHECK_EXPECTED (a, -1);

  a = V (1);
  a.Sub (V(3));
  CHECK_EXPECTED (a, -2);

  a = V (1);
  a.Sub (V(-1));
  CHECK_EXPECTED (a, 2);

  a = V (1);
  a.Sub (V(-2));
  CHECK_EXPECTED (a, 3);

  a = V (-3);
  a.Sub (V(-4));
  CHECK_EXPECTED (a, 1);

  a = V (-2);
  a.Sub (V(3));
  CHECK_EXPECTED (a, -5);

  a = V (1);
  a.Add (V(2));
  CHECK_EXPECTED (a, 3);

  a = V (1);
  a.Add (V(-3));
  CHECK_EXPECTED (a, -2);

  a = V (0);
  a.Add (V(0));
  CHECK_EXPECTED (a, 0);

  a = V (0);
  a.Mul (V(0));
  CHECK_EXPECTED (a, 0);
  a = V (0);
  a.Mul (V(1));
  CHECK_EXPECTED (a, 0);
  a = V (0);
  a.Mul (V(-1));
  CHECK_EXPECTED (a, 0);
  a = V (1);
  a.Mul (V(0));
  CHECK_EXPECTED (a, 0);
  a = V (1);
  a.Mul (V(1));
  CHECK_EXPECTED (a, 1);
  a = V (1);
  a.Mul (V(-1));
  CHECK_EXPECTED (a, -1);
  a = V (-1);
  a.Mul (V(-1));
  CHECK_EXPECTED (a, 1);

  a = V (0);
  a.Mul (V(1));
  CHECK_EXPECTED (a, 0);
  a = V (0);
  a.Mul (V(-1));
  CHECK_EXPECTED (a, 0);
  a = V (1);
  a.Mul (V(1));
  CHECK_EXPECTED (a, 1);
  a = V (1);
  a.Mul (V(-1));
  CHECK_EXPECTED (a, -1);
  a = V (-1);
  a.Mul (V(1));
  CHECK_EXPECTED (a, -1);
  a = V (-1);
  a.Mul (V(-1));
  CHECK_EXPECTED (a, 1);



  a = V (2);
  a.Mul (V(3));
  a.Div (V(3));
  CHECK_EXPECTED (a, 2);

  // Below, the division loses precision because 2/3 is not
  // representable exactly in 64.64 integers. So, we got
  // something super close but the final rounding kills us.
  a = V (2);
  a.Div (V(3));
  a.Mul (V(3));
  CHECK_EXPECTED (a, 1);

  // The example below shows that we really do not lose
  // much precision internally: it is almost always the
  // final conversion which loses precision.
  a = V (2000000000);
  a.Div (V(3));
  a.Mul (V(3));
  CHECK_EXPECTED (a, 1999999999);
  



  return ok;
}

static HighPrecision128Tests g_int128_tests;


}; // namespace ns3

#endif /* RUN_SELF_TESTS */
