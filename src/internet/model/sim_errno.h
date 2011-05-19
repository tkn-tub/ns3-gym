#ifndef NETWORK_SIMULATION_CRADLE_ERR_H_
#define NETWORK_SIMULATION_CRADLE_ERR_H_

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

#endif
