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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#ifndef APPLICATION_CONTAINER_H
#define APPLICATION_CONTAINER_H

#include <stdint.h>
#include <vector>
#include "ns3/application.h"

namespace ns3 {

/**
 * \brief holds a vector of ns3::Application pointers
 *
 */
class ApplicationContainer
{
public:
  /**
   * Create an empty ApplicationContainer.
   */
  ApplicationContainer ();

  /**
   * Create an ApplicationContainer with exactly one application
   *
   * \param application The application to add to the container
   */
  ApplicationContainer (Ptr<Application> application);

  /**
   * Create an ApplicationContainer with exactly one application
   *
   * \param name The name of the application object to add to the container
   */
  ApplicationContainer (std::string name);

  typedef std::vector<Ptr<Application> >::const_iterator Iterator;

  /**
   * \returns an iterator which points to the start of the array of pointers.
   */
  Iterator Begin (void) const;
  /**
   * \returns an iterator which points to the end of the array of pointers.
   */
  Iterator End (void) const;

  /**
   * \returns the number of application pointers stored in this container.
   */
  uint32_t GetN (void) const;
  /**
   * \param i the index of the requested application pointer.
   * \returns the requested application pointer.
   */
  Ptr<Application> Get (uint32_t i) const;

  /**
   * Append to the end of this container the other input container.
   *
   * \param other another application container
   */
  void Add (ApplicationContainer other);
  /**
   * Append to the end of this container the input application pointer.
   *
   * \param application another netdevice pointer.
   */
  void Add (Ptr<Application> application);
  /**
   * Append to the end of this container the application specified by the name.
   *
   * \param name The name of the application object to add to the container.
   */
  void Add (std::string name);

  void Start (Time start);
  void Stop (Time stop);

private:
  std::vector<Ptr<Application> > m_applications;
};

} // namespace ns3

#endif /* APPLICATION_CONTAINER_H */
