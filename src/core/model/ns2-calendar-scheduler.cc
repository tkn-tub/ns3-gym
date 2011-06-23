/* -*- Mode:C++; c-basic-offset:8; tab-width:8; -*- */
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
 */

#include "ns2-calendar-scheduler.h"
#include "event-impl.h"
#include "ns3/assert.h"
#include "ns3/log.h"
#include <cassert>
#include <math.h>
#include <string.h>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Ns2CalendarScheduler");

NS_OBJECT_ENSURE_REGISTERED (Ns2CalendarScheduler);

#define CALENDAR_HASH(key) ((key.m_ts / width_) % nbuckets_)

TypeId
Ns2CalendarScheduler::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Ns2CalendarScheduler")
    .SetParent<Scheduler> ()
    .AddConstructor<Ns2CalendarScheduler> ()
  ;
  return tid;
}

Ns2CalendarScheduler::Ns2CalendarScheduler ()
{
  NS_LOG_FUNCTION (this);

  adjust_new_width_interval_ = 10;
  min_bin_width_ = 1;
  avg_gap_ = -2;
  last_time_ = 0;
  gap_num_ = 0;
  head_search_ = 0;
  insert_search_ = 0;
  round_num_ = 0;
  time_to_newwidth_ = adjust_new_width_interval_;
  cal_clock_ = Scheduler::EventKey ();
  reinit (4, 1, cal_clock_);
}
Ns2CalendarScheduler::~Ns2CalendarScheduler ()
{
  NS_LOG_FUNCTION (this);

  for (int i = 0; i < nbuckets_; i++)
    {
      Bucket *bucket = &buckets_[i];
      if (bucket->list_ == 0)
        {
          continue;
        }
      if (bucket->list_->next_ == bucket->list_)
        {
          delete bucket->list_;
          continue;
        }
      BucketItem *next;
      BucketItem *cur;
      for (cur = bucket->list_;
           cur->next_ != bucket->list_;
           cur = next)
        {
          next = cur->next_;
          delete cur;
        }
      delete cur;
    }
  delete [] buckets_;
  qsize_ = 0;
  stat_qsize_ = 0;
}
void
Ns2CalendarScheduler::Insert (const Event &event)
{
  NS_LOG_FUNCTION (this);

  Scheduler::EventKey newtime = event.key;
  int i = CALENDAR_HASH (newtime);

  Bucket* current = (&buckets_[i]);
  BucketItem *head = current->list_;
  BucketItem *after = 0;
  BucketItem *e = new BucketItem ();
  e->event = event;

  if (!head)
    {
      current->list_ = e;
      e->next_ = e->prev_ = e;
      ++stat_qsize_;
      ++(current->count_);
    }
  else
    {
      insert_search_++;
      if (newtime < head->event.key)
        {
          //  e-> head -> ...
          e->next_ = head;
          e->prev_ = head->prev_;
          e->prev_->next_ = e;
          head->prev_ = e;
          current->list_ = e;
          ++stat_qsize_;
          ++(current->count_);
        }
      else
        {
          for (after = head->prev_; newtime < after->event.key; after = after->prev_)
            {
              insert_search_++;
            }
          // ...-> after -> e -> ...
          e->next_ = after->next_;
          e->prev_ = after;
          e->next_->prev_ = e;
          after->next_ = e;
          if (after->event.key < newtime)
            {
              // unique timing
              ++stat_qsize_;
              ++(current->count_);
            }
        }
    }
  ++qsize_;
  // assert(e == buckets_[i].list_ ||  e->prev_->time_ <= e->time_);
  // assert(e == buckets_[i].list_->prev_ || e->next_->time_ >= e->time_);

  if (stat_qsize_ > top_threshold_)
    {
      resize (nbuckets_ << 1, cal_clock_);
      return;
    }
}
bool
Ns2CalendarScheduler::IsEmpty (void) const
{
  return qsize_ == 0;
}
Scheduler::Event
Ns2CalendarScheduler::PeekNext (void) const
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (!IsEmpty ());

  BucketItem *e = const_cast<Ns2CalendarScheduler *> (this)->head ();
  NS_ASSERT (e != 0);

  return e->event;
}


Scheduler::Event
Ns2CalendarScheduler::RemoveNext (void)
{
  NS_LOG_FUNCTION (this);
  NS_ASSERT (!IsEmpty ());

  BucketItem *e = head ();
  NS_ASSERT (e != 0);

  if (last_time_ == 0)
    {
      last_time_ = e->event.key.m_ts;
    }
  else
    {
      gap_num_++;
      if (gap_num_ >= qsize_ )
        {
          uint64_t tt_gap_ = e->event.key.m_ts - last_time_;
          avg_gap_ = tt_gap_ / gap_num_;
          gap_num_ = 0;
          last_time_ = e->event.key.m_ts;
          round_num_++;
          if ((round_num_ > 20)
              && (( head_search_ > (insert_search_ << 1))
                  ||( insert_search_ > (head_search_ << 1)) ))
            {
              resize (nbuckets_, cal_clock_);
              round_num_ = 0;
            }
          else
            {
              if (round_num_ > 100)
                {
                  round_num_ = 0;
                  head_search_ = 0;
                  insert_search_ = 0;
                }
            }
        }
    }

  int l = lastbucket_;

  // update stats and unlink
  if (e->next_ == e || e->next_->event.key != e->event.key)
    {
      --stat_qsize_;
      // assert(stat_qsize_ >= 0);
      --buckets_[l].count_;
      // assert(buckets_[l].count_ >= 0);
    }
  --qsize_;

  if (e->next_ == e)
    {
      buckets_[l].list_ = 0;
    }
  else
    {
      e->next_->prev_ = e->prev_;
      e->prev_->next_ = e->next_;
      buckets_[l].list_ = e->next_;
    }

  e->next_ = e->prev_ = NULL;


  // if (buckets_[l].count_ == 0)
  //	assert(buckets_[l].list_ == 0);

  if (stat_qsize_ < bot_threshold_)
    {
      resize (nbuckets_ >> 1, cal_clock_);
    }

  Scheduler::Event event = e->event;
  delete e;
  return event;
}

void
Ns2CalendarScheduler::Remove (const Event &ev)
{
  NS_ASSERT (!IsEmpty ());


  int i = CALENDAR_HASH (ev.key);

  Bucket* current = (&buckets_[i]);
  BucketItem *head = current->list_;
  BucketItem *e = 0;

  if (!head)
    {
      NS_LOG_DEBUG ("event not in scheduler");
      return;
    }
  for (e = head->prev_; ev.key != e->event.key; e = e->prev_)
    {
    }
  --stat_qsize_;
  --buckets_[i].count_;
  if (e->next_ == e)
    {
      assert (buckets_[i].list_ == e);
      buckets_[i].list_ = 0;
    }
  else
    {
      e->next_->prev_ = e->prev_;
      e->prev_->next_ = e->next_;
      if (buckets_[i].list_ == e)
        {
          buckets_[i].list_ = e->next_;
        }
    }

  if (buckets_[i].count_ == 0)
    {
      assert (buckets_[i].list_ == 0);
    }

  e->next_ = e->prev_ = NULL;

  delete e;

  --qsize_;

  return;
}

Ns2CalendarScheduler::BucketItem *
Ns2CalendarScheduler::head (void)
{
  NS_ASSERT (!IsEmpty ());

  int l = -1, i = lastbucket_;
  int lastbucket_dec = (lastbucket_) ? lastbucket_ - 1 : nbuckets_ - 1;
  uint64_t diff;
  BucketItem *e, *min_e = NULL;
#define CAL_DEQUEUE(x)                                          \
  do {                                                    \
      head_search_++;                                         \
      if ((e = buckets_[i].list_) != NULL) {                  \
          diff = e->event.key.m_ts - cal_clock_.m_ts;     \
          if (diff < diff ## x ## _)  {                       \
              l = i;                                  \
              goto found_l;                           \
            }                                               \
          if (min_e == NULL || min_e->event.key > e->event.key) { \
              min_e = e;                                      \
              l = i;                                          \
            }                                                       \
        }                                                               \
      if (++i == nbuckets_) { i = 0; }                                                                         \
    } \
  while (0)

  // CAL_DEQUEUE applied successively will find the event to
  // dequeue (within one year) and keep track of the
  // minimum-priority event seen so far; the argument controls
  // the criteria used to decide whether the event should be
  // considered `within one year'.  Importantly, the number of
  // buckets should not be less than 4.
  CAL_DEQUEUE (0);
  CAL_DEQUEUE (1);
  for (; i != lastbucket_dec; )
    {
      CAL_DEQUEUE (2);
    }
  // one last bucket is left unchecked - take the minimum
  // [could have done CAL_DEQUEUE(3) with diff3_ = bwidth*(nbuck*3/2-1)]
  e = buckets_[i].list_;
  if (min_e != NULL
      && (e == NULL || min_e->event.key < e->event.key))
    {
      e = min_e;
    }
  else
    {
      // assert(e);
      l = i;
    }
found_l:
  // assert(buckets_[l].count_ >= 0);
  // assert(buckets_[l].list_ == e);

  /* l is the index of the bucket to dequeue, e is the event */
  /*
   * still want to advance lastbucket_ and cal_clock_ to save
   * time when deque() follows.
   */
  assert (l != -1);
  lastbucket_ = l;
  cal_clock_  = e->event.key;

  return e;
}

void
Ns2CalendarScheduler::reinit (int nbuck, uint64_t bwidth, Scheduler::EventKey start)
{
  buckets_ = new Bucket[nbuck];

  memset (buckets_, 0, sizeof(Bucket) * nbuck);    // faster than ctor

  width_ = bwidth;
  nbuckets_ = nbuck;
  qsize_ = 0;
  stat_qsize_ = 0;

  lastbucket_ =  CALENDAR_HASH (start);

  diff0_ = bwidth * nbuck / 2;
  diff1_ = diff0_ + bwidth;
  diff2_ = bwidth * nbuck;
  // diff3_ = bwidth*(nbuck*3/2-1);

  bot_threshold_ = (nbuck >> 1) - 2;
  top_threshold_ = (nbuck << 1);
}

void
Ns2CalendarScheduler::resize (int newsize, Scheduler::EventKey start)
{
  uint64_t bwidth;
  if (newsize == nbuckets_)
    {
      /* we resize for bwidth*/
      if (head_search_)
        {
          bwidth = head_search_;
        }
      else
        {
          bwidth = 1;
        }
      if (insert_search_)
        {
          bwidth = bwidth / insert_search_;
        }
      bwidth = static_cast<uint64_t> (sqrt (bwidth) * width_);
      if (bwidth < min_bin_width_)
        {
          if (time_to_newwidth_ > 0)
            {
              time_to_newwidth_--;
              head_search_ = 0;
              insert_search_ = 0;
              round_num_ = 0;
              return;                   // failed to adjust bwidth
            }
          else
            {
              // We have many (adjust_new_width_interval_) times failure in adjusting bwidth.
              // should do a reshuffle with newwidth
              bwidth = newwidth (newsize);
            }
        }
      // snoopy queue calculation
    }
  else
    {
      /* we resize for size */
      bwidth = newwidth (newsize);
      if (newsize < 4)
        {
          newsize = 4;
        }
    }

  Bucket *oldb = buckets_;
  int oldn = nbuckets_;

  reinit (newsize, bwidth, start);

  // copy events to new buckets
  int i;

  for (i = 0; i < oldn; ++i)
    {
      // we can do inserts faster, if we use insert2, but to
      // preserve FIFO, we have to start from the end of
      // each bucket and use insert2
      if  (oldb[i].list_)
        {
          BucketItem *tail = oldb[i].list_->prev_;
          BucketItem *e = tail;
          do
            {
              BucketItem* ep = e->prev_;
              e->next_ = e->prev_ = 0;
              insert2 (e);
              e = ep;
            }
          while (e != tail);
        }
    }
  head_search_ = 0;
  insert_search_ = 0;
  round_num_ = 0;
  delete [] oldb;
}

void
Ns2CalendarScheduler::insert2 (Ns2CalendarScheduler::BucketItem* e)
{
  // Same as insert, but for inserts e *before* any same-time-events, if
  //   there should be any.  Since it is used only by CalendarScheduler::newwidth(),
  //   some important checks present in insert() need not be performed.

  int i = CALENDAR_HASH (e->event.key);
  BucketItem *head = buckets_[i].list_;
  BucketItem *before = 0;
  if (!head)
    {
      buckets_[i].list_ = e;
      e->next_ = e->prev_ = e;
      ++stat_qsize_;
      ++buckets_[i].count_;
    }
  else
    {
      bool newhead;
      if (e->event.key > head->prev_->event.key)             // strict LIFO, so > and not >=
        {               // insert at the tail
          before = head;
          newhead = false;
        }
      else
        {
          // insert event in time sorted order, LIFO for sim-time events
          for (before = head; e->event.key > before->event.key; before = before->next_)
            {
            }
          newhead = (before == head);
        }

      e->next_ = before;
      e->prev_ = before->prev_;
      before->prev_ = e;
      e->prev_->next_ = e;
      if (newhead)
        {
          buckets_[i].list_ = e;
          // assert(e->time_ <= e->next_->time_);
        }

      if (e != e->next_ && e->next_->event.key != e->event.key)
        {
          // unique timing
          ++stat_qsize_;
          ++buckets_[i].count_;
        }
    }
  // assert(e == buckets_[i].list_ ||  e->prev_->time_ <= e->time_);
  // assert(e == buckets_[i].list_->prev_ || e->next_->time_ >= e->time_);

  ++qsize_;
  // no need to check resizing
}

uint64_t
Ns2CalendarScheduler::newwidth (int newsize)
{
  if (adjust_new_width_interval_)
    {
      time_to_newwidth_ = adjust_new_width_interval_;
      if (avg_gap_ > 0)
        {
          return avg_gap_ * 4;
        }
    }
  int i;
  int max_bucket = 0;       // index of the fullest bucket
  for (i = 1; i < nbuckets_; ++i)
    {
      if (buckets_[i].count_ > buckets_[max_bucket].count_)
        {
          max_bucket = i;
        }
    }
  int nsamples = buckets_[max_bucket].count_;

  if (nsamples <= 4)
    {
      return width_;
    }

  uint64_t nw = (buckets_[max_bucket].list_->prev_->event.key.m_ts
                 - buckets_[max_bucket].list_->event.key.m_ts) * 4;

  nw /= ((newsize < nsamples) ? newsize : nsamples);       // min (newsize, nsamples)

  nw = std::max (nw, min_bin_width_);

  return nw;
}


} // namespace ns3
