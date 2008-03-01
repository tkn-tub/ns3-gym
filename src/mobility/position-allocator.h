/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#ifndef POSITION_ALLOCATOR_H
#define POSITION_ALLOCATOR_H

#include "ns3/object.h"
#include "ns3/random-variable.h"
#include "vector.h"

namespace ns3 {

/**
 * \brief choose a position at random.
 *
 * This is a pure abstract base class.
 */
class PositionAllocator : public Object
{
public:
  static TypeId GetTypeId (void);
  PositionAllocator ();
  virtual ~PositionAllocator ();
  /**
   * \returns the next randomly-choosen position.
   */
  virtual Vector GetNext (void) const = 0;
};

class ListPositionAllocator : public PositionAllocator
{
public:
  static TypeId GetTypeId (void);
  ListPositionAllocator ();
  void Add (Vector v);
  virtual Vector GetNext (void) const;
private:
  std::vector<Vector> m_positions;
  mutable std::vector<Vector>::const_iterator m_current;
};

class GridPositionAllocator : public PositionAllocator
{
public:
  static TypeId GetTypeId (void);

  enum LayoutType {
    ROW_FIRST,
    COLUMN_FIRST
  };

  GridPositionAllocator ();

  virtual Vector GetNext (void) const;
private:
  mutable uint32_t m_current;
  enum LayoutType m_layoutType;
  double m_xMin;
  double m_yMin;
  uint32_t m_n;
  double m_deltaX;
  double m_deltaY;
};

/**
 * \brief allocate random positions within a rectangle
 * according to a pair of random variables.
 */
class RandomRectanglePositionAllocator : public PositionAllocator
{
public:
  static TypeId GetTypeId (void);
  RandomRectanglePositionAllocator ();
  virtual ~RandomRectanglePositionAllocator ();
  virtual Vector GetNext (void) const;
private:
  RandomVariable m_x;
  RandomVariable m_y;
};

/**
 * \brief allocate random positions within a disc
 * according to a pair of random variables.
 */
class RandomDiscPositionAllocator : public PositionAllocator
{
public:
  static TypeId GetTypeId (void);
  RandomDiscPositionAllocator ();
  virtual ~RandomDiscPositionAllocator ();
  virtual Vector GetNext (void) const;
private:
  RandomVariable m_theta;
  RandomVariable m_rho;
  double m_x;
  double m_y;
};

} // namespace ns3

#endif /* RANDOM_POSITION_H */
