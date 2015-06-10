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
 * Author: Mathieu Lacage <mathieu.lacage@cutebugs.net>
 */

#ifndef NSC_SIM_ERRNO_H
#define NSC_SIM_ERRNO_H

// list of network stack errors that may happen in a simulation,
// and can be handled by the simulator in a sane way.
// Note that NSC handles several errors internally though
// nsc_assert, BUG() and friends, because they (should) never
// happen in a simulation (e.g. ESOCKTNOSUPPORT).
//
// These values are returned by the various methods provided by nsc.
// They must always be < 0, as values >= 0 are a success indicator;
// e.g. send_data() will return the number of bytes sent or one of
// the nsc_errno numbers below, accept() will return 0 on success or
// one of the nsc_errno numbers below, etc.
enum nsc_errno {
  NSC_EUNKNOWN = -1,
  NSC_EADDRINUSE = -10,
  NSC_EADDRNOTAVAIL = -11,
  NSC_EAGAIN = -12,
  NSC_EALREADY = -25,
  NSC_ECONNREFUSED = -32,
  NSC_ECONNRESET = -33,
  NSC_EHOSTDOWN = -50,
  NSC_EHOSTUNREACH = -51,
  NSC_EINPROGRESS = -60,
  NSC_EISCONN = -61,
  NSC_EMSGSIZE = -70,
  NSC_ENETUNREACH = -82,
  NSC_ENOTCONN = -86,
  NSC_ENOTDIR = -87,        // used by sysctl(2)
  NSC_ESHUTDOWN = -130,
  NSC_ETIMEDOUT = -140,
};

#endif /* NSC_SIM_ERRNO_H */
