/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 1997 David Wetherall
 * Copyright (c) 2005 David Wei
 * Copyright (c) 2009 INRIA
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
 * Authors: 
 *  David Wetherall <djw@juniper.lcs.mit.edu>: originally, in ns-2, back in 1997
 *  David X. Wei: optimizations in ns-2.28
 *  Mathieu Lacage <mathieu.lacage@sophia.inria.fr>: port to ns-3
 */

#ifndef NS2_CALENDAR_SCHEDULER_H
#define NS2_CALENDAR_SCHEDULER_H

#include "scheduler.h"
#include <stdint.h>
#include <list>

namespace ns3 {

class EventImpl;

/**
 * \ingroup scheduler
 * \brief a calendar queue event scheduler
 *
 * This event scheduler is a copy/paste of the ns2.29 calendar scheduler.
 */
class Ns2CalendarScheduler : public Scheduler 
{
public:
  static TypeId GetTypeId (void);

  Ns2CalendarScheduler ();
  virtual ~Ns2CalendarScheduler ();

  virtual void Insert (const Event &ev);
  virtual bool IsEmpty (void) const;
  virtual Event PeekNext (void) const;
  virtual Event RemoveNext (void);
  virtual void Remove (const Event &ev);

private:
  struct BucketItem {
    ns3::Scheduler::Event event;
    struct BucketItem *next_;
    struct BucketItem *prev_;
  };
  struct Bucket {
    struct BucketItem *list_;
    int count_;
  };

  void reinit(int nbuck, uint64_t bwidth, Scheduler::EventKey start);
  void resize(int newsize, Scheduler::EventKey start);
  uint64_t newwidth(int newsize);
  void insert2 (Ns2CalendarScheduler::BucketItem *e);
  Ns2CalendarScheduler::BucketItem *head (void);

  
  uint64_t min_bin_width_;		// minimum bin width for Calendar Queue
  unsigned int adjust_new_width_interval_; // The interval (in unit of resize time) for adjustment of bin width. A zero value disables automatic bin width adjustment
  unsigned time_to_newwidth_;	// how many time we failed to adjust the width based on snoopy-queue
  long unsigned head_search_;
  long unsigned insert_search_;
  int round_num_;
  long int gap_num_;		//the number of gap samples in this window (in process of calculation)
  uint64_t last_time_;		//the departure time of first event in this window
  int64_t avg_gap_;		//the average gap in last window (finished calculation)

  uint64_t width_;
  uint64_t diff0_, diff1_, diff2_; /* wrap-around checks */

  int stat_qsize_;		/* # of distinct priorities in queue*/
  int nbuckets_;
  int lastbucket_;
  int top_threshold_;
  int bot_threshold_;
  int qsize_;
  struct Bucket *buckets_;
  Scheduler::EventKey cal_clock_;
};

} // namespace ns3

#endif /* NS2_CALENDAR_SCHEDULER_H */
