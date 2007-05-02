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
#include <iostream>

namespace ns3 {

#ifdef GATHER_STATISTICS
int HighPrecision::m_nfastadds = 0;
int HighPrecision::m_nfastsubs = 0;
int HighPrecision::m_nfastmuls = 0;
int HighPrecision::m_nfastcmps = 0;
int HighPrecision::m_nfastgets = 0;
int HighPrecision::m_nslowadds = 0;
int HighPrecision::m_nslowsubs = 0;
int HighPrecision::m_nslowmuls = 0;
int HighPrecision::m_nslowcmps = 0;
int HighPrecision::m_nslowgets = 0;
int HighPrecision::m_ndivs = 0;
int HighPrecision::m_nconversions = 0;

void 
HighPrecision::PrintStats (void)
{
  double nadds = m_nfastadds + m_nslowadds;
  double nsubs = m_nfastsubs + m_nslowsubs;
  double ncmps = m_nfastcmps + m_nslowcmps;
  double nmuls = m_nfastmuls + m_nslowmuls;
  double ngets = m_nfastgets + m_nslowgets;
  double fast_add_ratio = m_nfastadds / nadds;
  double fast_sub_ratio = m_nfastsubs / nsubs;
  double fast_cmp_ratio = m_nfastcmps / ncmps;
  double fast_mul_ratio = m_nfastmuls / nmuls;
  double fast_get_ratio = m_nfastgets / ngets;

  std::cout <<
    "add="<<fast_add_ratio<<std::endl<<
    "sub="<<fast_sub_ratio<<std::endl<<
    "cmp="<<fast_cmp_ratio<<std::endl<<
    "mul="<<fast_mul_ratio<<std::endl<<
    "get="<<fast_get_ratio<<std::endl<<
    "nadds="<<nadds<<std::endl<<
    "nsubs="<<nsubs<<std::endl<<
    "ncmps="<<ncmps<<std::endl<<
    "nmuls="<<nmuls<<std::endl<<
    "ngets="<<ngets<<std::endl<<
    "ndivs="<<m_ndivs<<std::endl<<
    "nconversions="<<m_nconversions<<std::endl
    ;
}
#else
void 
HighPrecision::PrintStats (void)
{}
#endif /* GATHER_STATISTICS */


const double HighPrecision::MAX_64 = 18446744073709551615.0;


HighPrecision::HighPrecision (double value)
{
  int64_t hi = (int64_t) floor (value);
  uint64_t lo = (uint64_t) ((value - floor (value)) * MAX_64);
  if (lo == 0)
    {
      m_isFast = true;
      m_fastValue = hi;
      return;
    }
  else
    {
      m_isFast = false;
      m_slowValue = _cairo_int64_to_int128 (hi);
      m_slowValue = _cairo_int128_lsl (m_slowValue, 64);
      cairo_int128_t clo = _cairo_uint128_to_int128 (_cairo_uint64_to_uint128 (lo));
      m_slowValue = _cairo_int128_add (m_slowValue, clo);
    }
}

void
HighPrecision::EnsureSlow (void)
{
  if (m_isFast)
    {
      HP128INC (m_nconversions++);
      m_slowValue = _cairo_int64_to_int128 (m_fastValue);
      m_slowValue = _cairo_int128_lsl (m_slowValue, 64);
      m_isFast = false;
    }
}

int64_t
HighPrecision::SlowGetInteger (void) const
{
  cairo_int128_t value = _cairo_int128_rsa (m_slowValue, 64);
  return _cairo_int128_to_int64 (value);
}

double 
HighPrecision::SlowGetDouble (void) const
{
  bool is_negative = _cairo_int128_negative (m_slowValue);
  cairo_int128_t value = is_negative?_cairo_int128_negate (m_slowValue):m_slowValue;
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
HighPrecision::SlowAdd (HighPrecision const &o)
{
  EnsureSlow ();
  const_cast<HighPrecision &> (o).EnsureSlow ();
  m_slowValue = _cairo_int128_add (m_slowValue, o.m_slowValue);
  return false;
}
bool 
HighPrecision::SlowSub (HighPrecision const &o)
{
  EnsureSlow ();
  const_cast<HighPrecision &> (o).EnsureSlow ();
  m_slowValue = _cairo_int128_sub (m_slowValue, o.m_slowValue);
  return false;
}
bool 
HighPrecision::SlowMul (HighPrecision const &o)
{
  EnsureSlow ();
  const_cast<HighPrecision &> (o).EnsureSlow ();
  cairo_int128_t other = _cairo_int128_rsa (o.m_slowValue, 64);
  m_slowValue = _cairo_int128_mul (m_slowValue, other);
  return false;
}
bool 
HighPrecision::Div (HighPrecision const &o)
{
  HP128INC (m_ndivs++);
  EnsureSlow ();
  const_cast<HighPrecision &> (o).EnsureSlow ();
  cairo_int128_t div = _cairo_int128_rsa (o.m_slowValue, 64);
  cairo_quorem128_t qr;
  qr = _cairo_int128_divrem (m_slowValue, div);
  m_slowValue = qr.quo;
  return false;
}
int 
HighPrecision::SlowCompare (HighPrecision const &o) const
{
  const_cast<HighPrecision *> (this)->EnsureSlow ();
  const_cast<HighPrecision &> (o).EnsureSlow ();
  if (_cairo_int128_lt (m_slowValue, o.m_slowValue))
    {
      return -1;
    }
  else if (_cairo_int128_eq (m_slowValue, o.m_slowValue))
    {
      return 0;
    }
  else
    {
      return 1;
    }
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

static HighPrecision128Tests g_int128Tests;


}; // namespace ns3

#endif /* RUN_SELF_TESTS */
