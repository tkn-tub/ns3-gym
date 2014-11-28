/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 INRIA
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

#ifndef DEFAULT_DELETER_H
#define DEFAULT_DELETER_H

namespace ns3 {

/**
 * \brief a template used to delete objects
 *        by the *RefCount<> templates when the
 *        last reference to an object they manage
 *        disappears.
 *
 * \sa ns3::SimpleRefCount
 */
template <typename T>
struct DefaultDeleter
{
  inline static void Delete (T *object) {
    delete object;
  }
};

} // namespace ns3

#endif /* DEFAULT_DELETER_H */
