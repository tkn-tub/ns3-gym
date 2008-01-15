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
#ifndef MOBILITY_MODEL_NOTIFIER_H
#define MOBILITY_MODEL_NOTIFIER_H

#include "ns3/object.h"
#include "ns3/callback.h"
#include "ns3/callback-trace-source.h"
#include "mobility-model.h"

namespace ns3 {

/**
 * \brief notify listeners of position changes.
 */
class MobilityModelNotifier : public Object
{
public:
  static TypeId iid (void);

  /**
   * Create a new position notifier
   */
  MobilityModelNotifier ();

  /**
   * \param position the position which just changed.
   */
  void Notify (Ptr<const MobilityModel> position) const;
protected:
  virtual Ptr<TraceResolver> GetTraceResolver (void) const;
private:
  CallbackTraceSource<Ptr<const MobilityModel> > m_trace;
};

} // namespace ns3

#endif /* MOBILITY_MODEL_NOTIFIER_H */
