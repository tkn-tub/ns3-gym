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
#ifndef ASSERT_H
#define ASSERT_H

#ifdef NS3_ASSERT_ENABLE

#include <iostream>

#include "breakpoint.h"

/**
 * \defgroup assert Assert
 * \brief assert functions and macros
 *
 * The assert macros are used to verify
 * at runtime that a certain condition is true. If it is
 * not true, the program halts. These checks are built
 * into the program only in debugging builds. They are
 * removed in optimized builds.
 */

/**
 * \ingroup assert
 * \param condition condition to verifiy.
 *
 * At runtime, in debugging builds, if this condition is not
 * true, the program prints the source file, line number and 
 * unverified condition and halts in the ns3::AssertBreakpoint 
 * function.
 */
#define NS_ASSERT(condition)                                    \
  do                                                            \
    {                                                           \
      if (!(condition))                                         \
        {                                                       \
          std::cerr << "assert failed. file=" << __FILE__ <<    \
            ", line=" << __LINE__ << ", cond=\""#condition <<   \
            "\"" << std::endl;                                  \
          NS_BREAKPOINT ();                                     \
        }                                                       \
    }                                                           \
  while (false)


/**
 * \ingroup assert
 * \param condition condition to verifiy.
 * \param message message to output
 *
 * At runtime, in debugging builds, if this condition is not
 * true, the program prints the message to output and
 * halts in the ns3::AssertBreakpoint function.
 */
#define NS_ASSERT_MSG(condition, message)       \
  do                                            \
    {                                           \
      if (!(condition))                         \
        {                                       \
          std::cerr << message << std::endl;    \
          NS_BREAKPOINT ();                     \
        }                                       \
    }                                           \
  while (false)

#else /* NS3_ASSERT_ENABLE */

#define NS_ASSERT(cond)
#define NS_ASSERT_MSG(cond,msg)

#endif /* NS3_ASSERT_ENABLE */

#endif /* ASSERT_H */
