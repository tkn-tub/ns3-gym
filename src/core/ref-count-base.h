/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 Georgia Tech Research Corporation
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
 * Author:  George Riley <riley@ece.gatech.edu>
 * Adapted from original code in object.h by:
 * Authors: Gustavo Carneiro <gjcarneiro@gmail.com>,
 *          Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef __REF_COUNT_BASE_H__
#define __REF_COUNT_BASE_H__

#include <stdint.h>

namespace ns3 {

/**
 * \brief a base class that provides implementations of reference counting
 *    operations.
 *  
 * A base class that provides implementations of reference counting 
 * operations, for classes that wish to use the templatized smart 
 * pointer for memory management but that do not wish to derive from
 * class ns3::Object.
 *
 */
class RefCountBase 
{ 
public:
  RefCountBase();
  RefCountBase (const RefCountBase &o);
  RefCountBase &operator = (const RefCountBase &o);
  virtual ~RefCountBase ();
  /**
   * Increment the reference count. This method should not be called
   * by user code. RefCountBase instances are expected to be used in
   * conjunction with the Ptr template which would make calling Ref
   * unecessary and dangerous.
   */
  inline void Ref () const;
  /**
   * Decrement the reference count. This method should not be called
   * by user code. RefCountBase instances are expected to be used in 
   * conjunction with the Ptr template which would make calling Ref
   * unecessary and dangerous.
   */
  inline void Unref () const;
private:
  // Note we make this mutable so that the const methods can still
  // change it.
  mutable uint32_t m_count;  // Reference count
};

} // namespace ns3

namespace ns3 {

// Implementation of the in-line methods
void
RefCountBase::Ref () const
{
  m_count++;
}

void
RefCountBase::Unref () const
{
  if (--m_count == 0)
    { // All references removed, ok to delete
      delete this;
    }
}

} // namespace ns3

#endif /* __REF_COUNT_BASE_H__*/
