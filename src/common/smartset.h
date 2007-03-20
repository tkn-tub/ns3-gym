/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2006 Georgia Tech Research Corporation
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

#ifndef __SMART_SET_H__
#define __SMART_SET_H__

#include <set>

namespace ns3 {

// Define a "smart" set container to be used by any ns3 object
// maintaining a collection of pointers, which must be freed at
// a later time.  The template parameter T must be a pointer, or an
// object supporting the delete operator, and the dereferenced object
// must support a Copy() function.  The set implementation
// has a O(1) "Remove" operation, that removes and deletes
// a single element in the container.  However, additions to the
// set are O(n) due to the sorted nature of the underlying STL set.

// Define a "smart" set container
template <typename T> class SmartSet {
public:
  typedef typename std::set<T>::const_iterator const_iterator;
  typedef typename std::set<T>::iterator       iterator;
  typedef typename std::set<T>::size_type      size_type;
  SmartSet()
  { // Nothing needed for default constructor
  }

  ~SmartSet()
  { // Smart container destructor
    for (const_iterator i = m_elements.begin(); i != m_elements.end(); ++i)
      {
        delete *i;
      }
    // No need to "clear" the set, as the set destructor does this
  }

  SmartSet(const SmartSet& o)
  { // Copy constructor, copy all underlying objects
    for (iterator i = o.Begin(); i != o.End(); ++i)
      {
        Add((*i)->Copy());
      }
  }

  void Add(const T& t)  // Add element, will be deleted on Clear or destructor
  {
    m_elements.insert(t);
  }

  bool Remove(T& t) // Remove and delete specified element
  { // Find the specified element, delete it, and remove from the container.
    // Returns true if found
    iterator i = m_elements.find(t);
    if (i != m_elements.end())
      { // Found it, delete it
        delete t;               // Delete the object
        m_elements.erase(i);    // Erase the element
        return true;
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
  iterator Begin() const
  {
    return m_elements.begin();
  }

  iterator End() const
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

private:
  std::set<T> m_elements;
};

}  // namespace ns3
#endif
