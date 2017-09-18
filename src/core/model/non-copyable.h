/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Lawrence Livermore National Laboratory
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
 * Author:  Peter D. Barnes, Jr. <pdbarnes@llnl.gov>
 */

#ifndef NS3_NON_COPYABLE_H
#define NS3_NON_COPYABLE_H

/**
 * \file
 * \ingroup access
 * ns3::NonCopyable declaration.
 */

/**
 * \ingroup core
 * \defgroup access NonCopyable and Singleton
 * Helpers to make classes non-copyable or singleton.
 */

namespace ns3 {

/**
 * \ingroup access
 * A base class for (non-Singleton) objects which shouldn't be copied.
 *
 * To prevent copying of your `class ExampleNC` just inherit from NonCopyable:
 * \code
 *   class ExampleNC : private NonCopyable { ... };
 * \endcode
 *
 * This class prevents each instance of a derived class from being copied.
 * If you want only a single global instance of the derived class,
 * see Singleton.
 *
 * \internal
 * This is based on `boost::noncopyable`.
 */
class NonCopyable
{
protected:
  /** Constructor. */
  NonCopyable () {}
  /** Destructor. */
  ~NonCopyable () {}

private:
  /** Copy constructor.  Private, so not copyable. */
  NonCopyable (const NonCopyable &);
  /**
   * Assignment.  Private, so not copyable.
   * \param [in] other The dummy argument
   * \return The unmodified copy.
   */
  NonCopyable & operator = (const NonCopyable &other);
}; 

} // namespace ns3 

#endif /* NS3_NON_COPYABLE_H */
