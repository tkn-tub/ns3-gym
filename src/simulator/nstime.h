/* -*- Mode:NS3; -*- */
/*
 * Copyright (c) 2005,2006 INRIA
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
#ifndef TIME_H
#define TIME_H

#include <stdint.h>

namespace ns3 {

/**
 * \brief simulation time
 *
 * This class is used by the user to specify when a scheduled event
 * is expected to expire (see ns3::Simulator::schedule).
 */
class Time {
 public:
    Time (Time const &o);
    Time &operator = (Time const &o);

    bool IsNegative (void) const;
    bool IsPositive (void) const;
    bool IsStrictlyNegative (void) const;
    bool IsStrictlyPositive (void) const;
    bool IsZero (void) const;

    Time operator += (Time const &o);
    Time operator -= (Time const &o);

    double ApproximateToSeconds (void) const;
    uint64_t ApproximateToMilliSeconds (void) const;  
    uint64_t ApproximateToMicroSeconds (void) const;
    uint64_t ApproximateToNanoSeconds (void) const;
  
    /* semi-public method. */
    uint64_t Get (void) const;
 protected:
    Time (int64_t ns);
 private:
    Time ();
    int64_t m_ns;
};

Time operator + (Time const &lhs, Time const &rhs);
Time operator - (Time const &lhs, Time const &rhs);
bool operator == (Time const &lhs, Time const &rhs);
bool operator != (Time const &lhs, Time const &rhs);
bool operator <  (Time const &lhs, Time const &rhs);
bool operator <= (Time const &lhs, Time const &rhs);
bool operator >  (Time const &lhs, Time const &rhs);
bool operator >= (Time const &lhs, Time const &rhs);


class Now : public Time {
public:
    Now ();
};

class Seconds : public Time 
{
public:
    Seconds (double s);
};
class MilliSeconds : public Time 
{
public:
    MilliSeconds (int32_t ms);
};
class MicroSeconds : public Time 
{
public:
    MicroSeconds (int32_t us);
};
class NanoSeconds : public Time 
{
public:
    NanoSeconds (int64_t ns);
};

}; // namespace ns3

#endif /* TIME_H */
