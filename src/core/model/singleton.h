/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#ifndef SINGLETON_H
#define SINGLETON_H

/**
 * \file
 * \ingroup core
 * ns3::Singleton declaration and template implementation.
 */

namespace ns3 {

/**
 * \ingroup core
 * \brief A template singleton
 *
 * This template class can be used to implement the singleton pattern.
 * The underlying object will be destroyed automatically when the process
 * exits.
 *
 * For a singleton whose lifetime is bounded by the simulation run,
 * not the process, see SimulationSingleton.
 *
 * \note If you call Get() again after the object has
 * been destroyed, the object will be re-created which will result in a
 * memory leak as reported by most memory leak checkers. It is up to the
 * user to ensure that Get() is never called from a static variable
 * finalizer.
 */
template <typename T>
class Singleton
{
public:
  /**
   * Get a pointer to the singleton instance.
   *
   * The instance will be automatically deleted when
   * the process exits.
   *
   * \return A pointer to the singleton instance.
   */
  static T *Get (void);

private:

  /**
   * \name %Singleton pattern
   * Private constructor, copy and assignment operator.
   *
   *  Note these do not have to be implemented, since they are
   *  never called.
   */
  /**@{*/
  /** Default constructor */
  Singleton<T> (void);
  
  /** Copy constructor. */
  Singleton<T> (const Singleton<T> &);
  /**
   * Assignment.
   * \returns The Singleton.
   */
  Singleton<T> operator = (const Singleton<T> &);
  /**@}*/

};

} // namespace ns3


/********************************************************************
 *  Implementation of the templates declared above.
 ********************************************************************/

namespace ns3 {

template <typename T>
T *
Singleton<T>::Get (void)
{
  static T object;
  return &object;
}


} // namespace ns3

#endif /* SINGLETON_H */
