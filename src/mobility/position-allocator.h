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
 * \brief allocate a set of positions. The allocation strategy is implemented
 *        in subclasses.
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
   * \returns the next chosen position.
   *
   * This method _must_ be implement in subclasses.
   */
  virtual Vector GetNext (void) const = 0;
};

/**
 * \brief Allocate positions from a deterministic list specified by the user.
 *
 * The first call to ListPositionAllocator::GetNext  will return the
 * first element of the list, the second call, the second element, and so on.
 */
class ListPositionAllocator : public PositionAllocator
{
public:
  static TypeId GetTypeId (void);
  ListPositionAllocator ();

  /**
   * \param v the position to append at the end of the list of 
   *        positions to return from GetNext.
   */
  void Add (Vector v);
  virtual Vector GetNext (void) const;
private:
  std::vector<Vector> m_positions;
  mutable std::vector<Vector>::const_iterator m_current;
};

/**
 * \brief Allocate positions on a rectangular 2d grid.
 */
class GridPositionAllocator : public PositionAllocator
{
public:
  static TypeId GetTypeId (void);

  /**
   * Determine whether positions are allocated row first or column first.
   */
  enum LayoutType {
    /**
     * In row-first mode, positions are allocated on the first row until
     * N positions have been allocated. Then, the second row located a yMin + yDelta
     * is used to allocate positions.
     */
    ROW_FIRST,
    /**
     * In column-first mode, positions are allocated on the first column until
     * N positions have been allocated. Then, the second column located a xMin + xDelta
     * is used to allocate positions.
     */
    COLUMN_FIRST
  };

  GridPositionAllocator ();

  /**
   * \param xMin the x coordinate where layout will start.
   */
  void SetMinX (double xMin);
  /**
   * \param yMin the y coordinate where layout will start
   */
  void SetMinY (double yMin);
  /**
   * \param deltaX the x interval between two x-consecutive positions.
   */
  void SetDeltaX (double deltaX);
  /**
   * \param deltaY the y interval between two y-consecutive positions.
   */
  void SetDeltaY (double deltaY);
  /**
   * \param n the number of positions allocated on each row (or each column)
   *        before switching to the next column (or row).
   */
  void SetN (uint32_t n);
  /**
   * \param layoutType the type of layout to use (row first or column first).
   */
  void SetLayoutType (enum LayoutType layoutType);

  /**
   * \param returns the x coordinate of the first allocated position.
   */
  double GetMinX (void) const;
  /**
   * \param returns the y coordinate of the first allocated position.
   */
  double GetMinY (void) const;
  /**
   * \returns the x interval between two x-consecutive positions.
   */
  double GetDeltaX (void) const;
  /**
   * \returns the y interval between two y-consecutive positions.
   */
  double GetDeltaY (void) const;
  /**
   * \returns the number of positions to allocate on each row or each column.
   */
  uint32_t GetN (void) const;
  /**
   * \returns the currently-selected layout type.
   */
  enum LayoutType GetLayoutType (void) const;
  

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

  void SetX (RandomVariable x);
  void SetY (RandomVariable y);

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

  void SetTheta (RandomVariable theta);
  void SetRho (RandomVariable rho);
  void SetX (double x);
  void SetY (double y);

  virtual Vector GetNext (void) const;
private:
  RandomVariable m_theta;
  RandomVariable m_rho;
  double m_x;
  double m_y;
};

} // namespace ns3

#endif /* RANDOM_POSITION_H */
