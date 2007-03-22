/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//

#ifndef __SMART_VECTOR_H__
#define __SMART_VECTOR_H__

#include <vector>

namespace ns3 {
  
// Define a "smart" vector container to be used by any ns3 object
// maintaining a collection of pointers, which must be freed at
// a later time.  The template parameter T must be a pointer, or an
// object supporting the delete operator, and the dereferenced object
// must support the Copy() operation.  The vector implementation
// has in inefficient "Remove" operation, that removes and deletes
// a single element in the container.  If frequent "Remove" operations
// are needed, the SmartSet is likey a better choice.

template <typename T> class SmartVector {
public:
  typedef typename std::vector<T>::const_iterator const_iterator;
  typedef typename std::vector<T>::iterator       iterator;
  typedef typename std::vector<T>::size_type      size_type;
  SmartVector()
  { // Nothing needed for default constructor
  }
  ~SmartVector()
  { // Smart container destructor
    for (const_iterator i = m_elements.begin(); i != m_elements.end(); ++i)
      {
        delete *i;
      }
    // No  need to "clear" the vector, as the vector destructor does this
  }

  SmartVector(const SmartVector& o)
  { // Copy constructor, copy each underlying object
    for (const_iterator i = o.Begin(); i != o.End(); ++i)
      {
        Add((*i)->Copy());
      }
  }
  
  void Add(const T& t)  // Add element, will be deleted on Clear or destructor
  {
    m_elements.push_back(t);
  }

  bool Remove()
  { // Remove the back element
    if (Empty()) return false; // No back element exists
    m_elements.pop_back();
    return true;
  }
      
  bool Remove(const T& t) // Remove and delete specified element
  { // Find the specified element, delete it, and remove from the container.
    // Returns true if found.
    // Note, this implementation is not particularly efficient.  If 
    // explicit individual element removal is a frequent operation for a given
    // smart container, an implementation based on STL "set" will
    // be a better choice.  See "SmartSet" if for this approach.
    // The SmartSet adds extra overhead in that the elementes are sorted,
    // so it should be used with caution.
    for (const_iterator i = m_elements.begin(); i != m_elements.end(); ++i)
        {
        if (*i == t)
          { // Found it
            delete t;               // Delete the object
            m_elements.erase(i);    // Erase the element
            return true;
          }
      }
    return false;
  }

  void Clear()
  { // Delete and remove all elements from the smart container
    for (const_iterator i = m_elements.begin(); i != m_elements.end(); ++i)
      {
        delete *i;
      }
    m_elements.clear();
  }

  // Iterator access
  const_iterator Begin() const
  {
    return m_elements.begin();
  }

  const_iterator End() const
  {
    return m_elements.end();
  }
  
  // Miscellaneous
  size_type Size() const
  {
    return m_elements.size();
  }

  bool Empty() const
  {
    return m_elements.empty();
  }

  T& Back()
  {
    return m_elements.back();
  }

  T operator[](size_type i) const
  { // Indexing operator
    return m_elements[i];
  }

private:
  std::vector<T> m_elements;
};

} // Namespace ns3
#endif
