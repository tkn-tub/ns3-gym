/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c)
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
 * Author:  Juliana Freitag Borin, Flavio Kubota and Nelson L.
 * S. da Fonseca - wimaxgroup@lrc.ic.unicamp.br
 */

#ifndef UL_JOB_H
#define UL_JOB_H

#include <stdint.h>
#include "ns3/header.h"
#include "ss-record.h"
#include "service-flow.h"
#include "service-flow-record.h"

namespace ns3 {

class SSRecord;
class ServiceFlow;

enum ReqType
{
  DATA, UNICAST_POLLING
};

/**
 * \ingroup wimax
 * \brief this class implements a structure to compute the priority of service flows
 */
class UlJob : public Object
{
public:
  enum JobPriority
  {
    LOW, INTERMEDIATE, HIGH
  };
  UlJob (void);
  virtual ~UlJob (void);
  SSRecord *
  GetSsRecord (void);
  void SetSsRecord (SSRecord* ssRecord);
  enum ServiceFlow::SchedulingType GetSchedulingType (void);
  void SetSchedulingType (ServiceFlow::SchedulingType schedulingType);
  ServiceFlow *
  GetServiceFlow (void);
  void SetServiceFlow (ServiceFlow *serviceFlow);

  ReqType GetType (void);
  void SetType (ReqType type);

  Time GetReleaseTime (void);
  void SetReleaseTime (Time releaseTime);

  Time GetPeriod (void);
  void SetPeriod (Time period);

  Time GetDeadline (void);
  void SetDeadline (Time deadline);

  uint32_t GetSize (void);
  void SetSize (uint32_t size);

private:
  friend bool operator == (const UlJob &a, const UlJob &b);

  Time m_releaseTime; /* The time after which the job can be processed*/
  Time m_period; /* For periodic jobs*/
  Time m_deadline; /* Request should be satisfied by this time */
  uint32_t m_size; /* Number of minislots requested */
  enum ServiceFlow::SchedulingType m_schedulingType; /* Scheduling type of flow */

  uint8_t m_flag; /* To delete or not..*/
  uint8_t m_retryCount;
  double m_ugsJitter; /* The jitter in the grant, valid only for UGS flows */
  int m_jitterSamples;
  double m_last_jitterCalTime; /* Last time avg jitter was calculated */

  SSRecord *m_ssRecord; /* Pointer to SSRecord */

  ReqType m_type; /* Type of request, DATA or Unicast req slots */
  ServiceFlow *m_serviceFlow;

};


class PriorityUlJob : public Object
{

  /**
   * \brief this class implements an auxiliar struct to compute the priority of the rtPS and nrtPS in
   * the intermediate queue
   */
public:
  PriorityUlJob ();
  int GetPriority (void);
  void SetPriority (int priority);

  Ptr<UlJob>
  GetUlJob (void);
  void SetUlJob (Ptr<UlJob> job);

private:
  int m_priority;
  Ptr<UlJob> m_job;
};

struct SortProcess : public std::binary_function<PriorityUlJob*, PriorityUlJob*, bool>
{
  bool operator () (PriorityUlJob& left, PriorityUlJob& right) const
  { // return true if left is logically less then right for given comparison
    if (left.GetPriority () < right.GetPriority ())
      {
        return true;
      }
    else if (left.GetPriority () == right.GetPriority ())
      {
        int32_t leftBacklogged = left.GetUlJob ()->GetServiceFlow ()->GetRecord ()->GetBacklogged ();
        int32_t rightBacklogged = left.GetUlJob ()->GetServiceFlow ()->GetRecord ()->GetBacklogged ();
        if (leftBacklogged <= rightBacklogged)
          {
            return true;
          }
        else
          {
            return false;
          }
      }
    else
      {
        return false;
      }
  }
};

struct SortProcessPtr: public std::binary_function< Ptr<PriorityUlJob>, Ptr<PriorityUlJob>, bool>
{
  bool operator () (Ptr<PriorityUlJob>& left, Ptr<PriorityUlJob>& right) const
  { //return true if left is logically less then right for given comparison
    if (left->GetPriority () < right->GetPriority ())
      {
        return true;
      }
    else if (left->GetPriority () == right->GetPriority ())
      {
        int32_t leftBacklogged = left->GetUlJob ()->GetServiceFlow ()->GetRecord ()->GetBacklogged ();
        int32_t rightBacklogged = left->GetUlJob ()->GetServiceFlow ()->GetRecord ()->GetBacklogged ();
        if (leftBacklogged <= rightBacklogged)
          {
            return true;
          }
        else
          {
            return false;
          }
      }
    else
      {
        return false;
      }
  }
};


} // namespace ns3

#endif /* UL_JOB_H */
