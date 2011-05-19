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
#include "ns3/vector.h"

namespace ns3 {

/**
 * \ingroup mobility
 * \brief Allocate a set of positions. The allocation strategy is implemented in subclasses.
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
   * \return the next chosen position.
   *
   * This method _must_ be implement in subclasses.
   */
  virtual Vector GetNext (void) const = 0;
};

/**
 * \ingroup mobility
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
   * \param v the position to append at the end of the list of positions to return from GetNext.
   */
  void Add (Vector v);
  virtual Vector GetNext (void) const;
private:
  std::vector<Vector> m_positions;
  mutable std::vector<Vector>::const_iterator m_current;
};

/**
 * \ingroup mobility
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
   * \return the x coordinate of the first allocated position.
   */
  double GetMinX (void) const;
  /**
   * \return the y coordinate of the first allocated position.
   */
  double GetMinY (void) const;
  /**
   * \return the x interval between two x-consecutive positions.
   */
  double GetDeltaX (void) const;
  /**
   * \return the y interval between two y-consecutive positions.
   */
  double GetDeltaY (void) const;
  /**
   * \return the number of positions to allocate on each row or each column.
   */
  uint32_t GetN (void) const;
  /**
   * \return the currently-selected layout type.
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
 * \ingroup mobility
 * \brief Allocate random positions within a rectangle according to a pair of random variables.
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
 * \ingroup mobility
 * \brief Allocate random positions within a 3D box according to a set of three random variables.
 */
class RandomBoxPositionAllocator : public PositionAllocator
{
public:
  static TypeId GetTypeId (void);
  RandomBoxPositionAllocator ();
  virtual ~RandomBoxPositionAllocator ();

  void SetX (RandomVariable x);
  void SetY (RandomVariable y);
  void SetZ (RandomVariable z);

  virtual Vector GetNext (void) const;
private:
  RandomVariable m_x;
  RandomVariable m_y;
  RandomVariable m_z;
};

/**
 * \ingroup mobility
 * \brief Allocate random positions within a disc according to a given distribution for the polar coordinates of each node 
          with respect to the provided center of the disc 
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


/**
 * \ingroup mobility
 * \brief Allocate the positions uniformely (with constant density) randomly within a disc.
 *
 * UniformDiscPositionAllocator allocates the positions randomly within a disc \f$ D \f$ lying on the
 * plane \f$ z=0 \f$ and having center at coordinates \f$ (x,y,0) \f$
 * and radius \f$ \rho \f$. The random positions are chosen such that,
 * for any subset \f$ S \subset D \f$, the expected value of the
 * fraction of points which fall into \f$ S \subset D \f$ corresponds
 * to \f$ \frac{|S|}{|D|} \f$, i.e., to the ratio of the area of the
 * subset to the area of the whole disc. 
 *
 * \note using UniformDiscPositionAllocator is not equivalent to using
 * a RandomDiscPositionAllocator with a uniformly-distributed radius,
 * since doing that would results in a point distribution which is
 * more dense towards the center of the disc.
 */
class UniformDiscPositionAllocator : public PositionAllocator
{
public:
  static TypeId GetTypeId (void);
  UniformDiscPositionAllocator ();
  virtual ~UniformDiscPositionAllocator ();

  /** 
   * \param rho the value of the radius of the disc
   */
  void SetRho (double rho);

  /** 
   * \param x  the X coordinate of the center of the disc
   */
  void SetX (double x);

  /** 
   * \param y   the Y coordinate of the center of the disc 
   */
  void SetY (double y);

  virtual Vector GetNext (void) const;
private:
  double m_rho;
  double m_x;
  double m_y;
};

} // namespace ns3

#endif /* RANDOM_POSITION_H */
