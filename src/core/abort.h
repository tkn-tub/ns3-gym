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
 */
#ifndef NS3_ABORT_H
#define NS3_ABORT_H

#include "fatal-error.h"

#define NS_ABORT_MSG(msg)                                       \
  do {								\
    std::cerr << "file=" << __FILE__ <<                         \
      ", line=" << __LINE__ << ", abort, msg=\"" <<             \
      msg << "\"" << std::endl;                                 \
    int *a = 0;                                                 \
    *a = 0;							\
  } while (false)


#define NS_ABORT_IF(cond)					\
  do {								\
    if (cond)							\
      {								\
	std::cerr << "file=" << __FILE__ <<			\
	  ", line=" << __LINE__ << ", abort on=\""#cond <<	\
            "\"" << std::endl;                                  \
	int *a = 0;						\
	*a = 0;							\
      }								\
  } while (false)

#define NS_ABORT_MSG_IF(cond, msg)				\
  do {								\
    if (cond)							\
      {								\
	std::cerr << "file=" << __FILE__ <<			\
	  ", line=" << __LINE__ << ", abort on=\""#cond <<	\
	  "\", msg=\"" << msg << "\"" << std::endl;		\
	int *a = 0;						\
	*a = 0;							\
      }								\
  } while (false)

#define NS_ABORT_UNLESS(cond)				\
  NS_ABORT_IF(!(cond))

#define NS_ABORT_MSG_UNLESS(cond, msg)			\
  NS_ABORT_MSG_IF(!(cond),msg)

#endif /* NS3_ABORT_H */
