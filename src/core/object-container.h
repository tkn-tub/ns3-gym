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
#ifndef OBJECT_CONTAINER_H
#define OBJECT_CONTAINER_H

#include <list>
#include <vector>
#include <stdint.h>
#include "fatal-error.h"

namespace ns3 {


/**
 * \brief Store objects for delayed deletion.
 *
 * This class can store any type of object provided it can be deleted.
 * There is no requirement that the destructor of these objects be
 * virtual. When the ObjectContainer::Cleanup method is invoked,
 * all the objects still left in the container will be deleted.
 * Alternatively, Cleanup is called automatically. when this object is
 * destroyed.
 */
class ObjectContainer 
{
public:
  /**
   * Invoke ObjectContainer::Cleanup if the user has not invoked it
   * himself.
   */
  ~ObjectContainer ();
  /**
   * Delete all objects stored in this container.
   */
  void Cleanup (void);

  /**
   * \param object object to store in the container.
   *
   * Take ownership of the input pointer: the object
   * will be deleted when ObjectContainer::Cleanup is invoked.
   */
  template <typename T>
  void Acquire (T *object);

  /**
   * \param object template of the object to store in the 
   *        container.
   *
   * Invoke the Copy method of the input object. This method is
   * expected to return a pointer of similar type and that
   * pointer will be returned. Furthermore, the pointer will
   * be stored internally to delete it when ObjectContainer::Cleanup
   * is invoked.
   */
  template <typename T>
  T *Add (T const &object);

  /**
   * \param object object to remove from container.
   *
   * Remove the object from the container: it will not
   * be deleted when the ObjectContainer::Cleanup method
   * is invoked.
   */
  template <typename T>
  void Remove (T *object);

private:
  typedef void (*ObjectDeleter) (void *);
  typedef std::list<std::pair<uint32_t,std::vector<void *> *> > List;
  typedef std::list<std::pair<uint32_t,ObjectDeleter> > DeleterList;
  template <typename T>
  static void DeleteObject (void *ptr);
  template <typename T>
  uint32_t GetUid (void) const;

  template <typename T>
  std::vector<T *> *GetVector (void) const;

  uint32_t GetGlobalUid (void) const;
  uint32_t RegisterUid (uint32_t uid, ObjectDeleter deleter) const;
  ObjectContainer::ObjectDeleter LookupObjectDeleter (uint32_t uid) const;
  List m_list;
  static DeleterList m_deleterList;
};

}; // namespace ns3

namespace ns3 {

template <typename T>
void
ObjectContainer::Acquire (T *object)
{
  uint32_t uid = GetUid<T> ();
  for (List::iterator i = m_list.begin (); i != m_list.end (); i++)
    {
      if (i->first == uid)
	{
	  i->second->push_back (object);
	  return;
	}
    }
  std::vector<void *> * vec = new std::vector<void *> ();
  vec->push_back (object);
  m_list.push_back (std::make_pair (uid, vec));
}

template <typename T>
T *
ObjectContainer::Add (T const &object)
{
  T *copy = object.Copy ();
  Acquire (copy);
  return copy;
}


template <typename T>
void
ObjectContainer::Remove (T *object)
{
  uint32_t uid = GetUid<T> ();
  for (List::iterator i = m_list.begin (); i != m_list.end (); i++)
    {
      if (i->first == uid)
	{	  
	  for (std::vector<void *>::iterator j = i->second->begin ();
	       j != i->second->end (); j++)
	    {
	      if ((*j) == object)
		{
		  i->second->erase (j);
		  return;
		}
	    }
	  goto error;
	}
    }
 error:
  NS_FATAL_ERROR ("tried to remove non-existant object from object container");
}

template <typename T>
std::vector<T *> *
ObjectContainer::GetVector (void) const
{
  uint32_t uid = GetUid<T> ();
  for (List::const_iterator i = m_list.begin (); i != m_list.end (); i++)
    {
      if (i->first == uid)
	{
	  std::vector<void *> *vec = i->second;
	  std::vector<T *> *retval = (std::vector<T *> *)vec;
	  return retval;
	}
    }
  NS_FATAL_ERROR ("no object registered for requested type.");
  // quiet compiler
  return 0;
}




template <typename T>
void
ObjectContainer::DeleteObject (void *ptr)
{
  T *object = (T*) ptr;
  delete object;
}

template <typename T>
uint32_t
ObjectContainer::GetUid (void) const
{
  static uint32_t uid = RegisterUid (GetGlobalUid (), 
				     &ObjectContainer::DeleteObject<T>);
  return uid;
}

}//namespace ns3


#endif /* OBJECT_CONTAINER_H */
