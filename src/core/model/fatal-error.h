/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA, 2010 NICTA
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
 *         Quincy Tse <quincy.tse@nicta.com.au>
 */
#ifndef NS3_FATAL_ERROR_H
#define NS3_FATAL_ERROR_H

#include <iostream>
#include <exception>
#include <cstdlib>

#include "fatal-impl.h"

/**
 * \ingroup core
 * \defgroup fatal Fatal Error Handlers
 *
 * \brief Functions to help clean up when a fatal error
 * is encountered.
 *
 * The functions in this group are used to perform
 * limited clean up, like flushing active streams, when
 * fatal errors are encountered (through assertion fail,
 * calls to NS_ABORT_* or calls to NS_FATAL_ERROR).
 *
 * Currently, other than flushing active ostreams, these
 * functions does not interfere with outside memory.  There
 * is still a residual risk that invalid ostream
 * pointers may be present, and may corrupt the memory
 * on the attempt to execute the flush() function.
 */


/**
 * \ingroup fatal
 * \private
 *
 * \brief fatal error handling
 *
 * When this macro is hit at runtime, details of filename
 * and line number is printed to stderr, and the program
 * is halted by calling std::terminate(). This will
 * trigger any clean up code registered by
 * std::set_terminate (NS3 default is a stream-flushing
 * code), but may be overridden.
 *
 * This macro is enabled unconditionally in all builds,
 * including debug and optimized builds.
 */
#define NS_FATAL_ERROR_NO_MSG()                           \
  do                                                      \
    {                                                     \
      std::cerr << "file=" << __FILE__ << ", line=" <<    \
      __LINE__ << std::endl;                            \
      ::ns3::FatalImpl::FlushStreams ();                  \
      std::terminate ();                                  \
    }                                                     \
  while (false)

/**
 * \ingroup fatal
 *
 * \brief fatal error handling
 *
 * \param msg message to output when this macro is hit.
 *
 * When this macro is hit at runtime, the user-specified
 * error message is printed to stderr, followed by a call
 * to the NS_FATAL_ERROR_NO_MSG() macro which prints the
 * details of filename and line number to stderr. The
 * program will be halted by calling std::terminate(),
 * triggering any clean up code registered by
 * std::set_terminate (NS3 default is a stream-flushing
 * code, but may be overridden).
 *
 * This macro is enabled unconditionally in all builds,
 * including debug and optimized builds.
 */
#define NS_FATAL_ERROR(msg)                             \
  do                                                    \
    {                                                   \
      std::cerr << "msg=\"" << msg << "\", ";           \
      NS_FATAL_ERROR_NO_MSG ();                          \
    }                                                   \
  while (false)

#endif /* FATAL_ERROR_H */
