/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>

namespace ns3 {

/**
 * \brief Base class that supports reference counting for memory management
 *
 * Many objects in the system derive from this class in order to use its
 * reference counting implementation.
 */
class Object 
{
public:
  /**
   * \brief Constructor
   *
   * Creates an object with a single reference count
   */
  Object ();
  virtual ~Object ();
  /**
   * \brief Increments the reference count of this object
   */
  void Ref (void) const;
  /**
   * \brief Decrements the reference count of this object
   */
  void Unref (void) const;
  /**
   * \return true if there is only a single reference to this object anywhere
   * in the system; false otherwise.
   */
  bool IsSingle (void) const;
  void Dispose (void);
protected:
  virtual void DoDispose (void);
private:
  mutable uint32_t m_count;
  bool m_disposed;
};

}//namespace ns3

#endif /* OBJECT_H */
